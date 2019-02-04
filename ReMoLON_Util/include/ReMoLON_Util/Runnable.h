#ifndef __REMOLON_UTIL_RUNNABLE_H__
#define __REMOLON_UTIL_RUNNABLE_H__

#include <memory>

namespace remolonUtil
{
  class Runnable
  {
    public:
      virtual void run ( ) { }
  };

  typedef std::unique_ptr < Runnable > runnablePtr;
}

#endif