#ifndef __REMOLON_UTIL_ThreadPool_H__
#define __REMOLON_UTIL_ThreadPool_H__

#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>
#include <list>
#include <typeinfo>

#include "ReMoLON_Util/Runnable.h"

namespace remolonUtil
{
  typedef struct WorkerThread
  {
    std::thread _t;
    long _lastActiveTime;
    bool _active;
    bool _terminated;
  } TWorkerThread;

  template< class T >
  class ThreadPool
  {
    public:
      ThreadPool ( )
        : _minPoolSize ( 8 )
        , _maxPoolSize ( 16 )
        , _maxIdleTimeMilis ( 60000 )
        , _maxQueueSize ( 256 )
        , _active ( true )

      {
        initializeThreadPool ( );
      }

      ThreadPool ( int poolSize_, 
                   int maxPoolSize_, 
                   long maxIdleTimeMilis_, 
                   long maxQueueSize_ )
        : _minPoolSize ( poolSize_ )
        , _maxPoolSize ( maxPoolSize_ )
        , _maxIdleTimeMilis ( maxIdleTimeMilis_ )
        , _maxQueueSize ( maxQueueSize_ )
        , _active ( true )
      {
        initializeThreadPool ( );
      }

      ~ThreadPool ( )
      {
        shutDown ( );
      }

      void executeTask ( std::unique_ptr< T > task_ )
      {
        std::unique_lock < std::mutex > lock ( _mtx );

        if ( _tasks.size ( ) >= _maxQueueSize )
        {
          throw std::runtime_error ( "ThreadPool: cannot accept new tasks - reached tasks queue max capacity ");
        }

        if ( _tasks.size ( ) > _currentPoolSize ) // Tasks are added faster than what threads can complete
        {
          createWorker ( ); // "Attempts" to create a worker
        }
        else if ( _currentPoolSize > _minPoolSize) // More threads than tasks, clean unused ones
        {
          purgeIdleThreads ( );
        }

        _tasks.push ( std::move( task_ ) );

        _monitor.notify_all ( );
      }

      void shutDown ( )
      {
        // Stop flag
        _active = false;

        // Wake up workers
        _monitor.notify_all ( );

        // Wait for threads to finish current tasks
        auto it = _pool.begin ( );
        for ( ; it != _pool.end ( ); it++ )
        {
          if ( (*it).get ( )->_t.joinable ( ) )
          {
            (*it).get ( )->_t.join ( );
          }
        }

        // Drop remaining tasks
        while( !_tasks.empty ( ) ) _tasks.pop ( );

        // Release thread resources
        _pool.clear ( );
      }

    private:
      void initializeThreadPool ( )
      {
        if ( !std::is_base_of< Runnable, T>::value )
        {
          const std::type_info & inf = typeid( T );
          std::string msg = "ThreadPool: The template argument is not a sublcass of runnable: " + std::string( inf.name ( ) );
          throw std::runtime_error ( msg.c_str( ) );
          return;
        }

        for ( int i = 0; i < _minPoolSize; i++ )
        {
          createWorker ( );
        }
      }

      void createWorker ( )
      {
        std::unique_lock < std::mutex > lock ( _poolMtx );
        std::unique_ptr < TWorkerThread > wt = std::make_unique < TWorkerThread > ( );
        TWorkerThread * wtPtr = wt.get ( );
        wtPtr->_lastActiveTime = currentTimeMillis ( );
        wtPtr->_active = true;
        wtPtr->_terminated = false;
        wtPtr->_t = std::thread ( &ThreadPool::threadLoop, this, wtPtr );
        _pool.push_back ( std::move ( wt ) );
        _currentPoolSize++;
      }

      void purgeIdleThreads ( )
      {
        long currentTime = currentTimeMillis ( );
        std::unique_lock < std::mutex > lock ( _poolMtx );
        auto it = _pool.begin ( );
        for ( ; it != _pool.end ( ) && _currentPoolSize > _minPoolSize; )
        {
          TWorkerThread * wt = (*it).get ( );

          if ( wt->_terminated )
          {
            _pool.erase ( it );
            continue;
          }
          
          long diff = currentTime - wt->_lastActiveTime;
          if ( diff > _maxIdleTimeMilis )
          {
            // sayonara
            wt->_active = false; // set thread to finish
            _currentPoolSize--;
          }

          it++;
        }
      }

      inline long currentTimeMillis ( )
      {
        auto now = std::chrono::high_resolution_clock::now ( );
        std::chrono::duration < long, std::milli > d = std::chrono::duration_cast < std::chrono::milliseconds > ( now.time_since_epoch ( ) );
        return d.count ( );
      }

      void threadLoop ( TWorkerThread * wt_ )
      {
        while ( _active && wt_->_active )
        {
          // Wait for tasks to become available
          std::unique_lock < std::mutex > taskLock ( _mtx );
          while ( _active && wt_->_active && _tasks.empty ( ) )
          {
            _monitor.wait ( taskLock );
          }

          // Retrieve next task
          std::unique_ptr<T> task = nullptr;
          
          if( !_tasks.empty ( ))
          {
            task = std::move( _tasks.front ( ) );
            _tasks.pop ( );
          }

          // Allow next threads to keep monitoring the queue
          taskLock.unlock ( );

          if ( task )
          {
            // No lock advantages outperforms locking advantages
            // Plus there is no real race condition
            // Update thread activity
            //std::unique_lock < std::mutex > poolLock ( _poolMtx );
            wt_->_lastActiveTime = currentTimeMillis ( );
            //poolLock.unlock ( );

            // Execute task
            task.get ( )->run ( );
          }
        }

        std::unique_lock < std::mutex > poolLock ( _poolMtx );
        wt_->_terminated = true;
        poolLock.unlock ( );
      }

    private:
          std::list < std::unique_ptr < TWorkerThread > > _pool;
          std::mutex _mtx;
          std::condition_variable _monitor;
          std::mutex _poolMtx;
          std::queue < std::unique_ptr<T> > _tasks;
          int _minPoolSize;
          int _maxPoolSize;
          int _currentPoolSize;
          long _maxIdleTimeMilis;
          long _maxQueueSize;
          bool _active;
  };
}

#endif