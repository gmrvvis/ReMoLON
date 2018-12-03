#include "InstanceManager.h"

#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

#define LOCK std::unique_lock<std::mutex>

namespace rrfrontend
{
	InstanceManager InstanceManager::INSTANCE;

	UserSessionList InstanceManager::EMPTY_SESSION_LIST;

	InstanceManager & InstanceManager::getInstance()
	{
		return INSTANCE;
	}

	InstanceManager::InstanceManager()
	{
	}

	InstanceManager::~InstanceManager()
	{
	}

	const StreamInstance * InstanceManager::getInstanceForUser(const std::string & unixUser, const std::string & sessionName)
	{
		LOCK lock(mtx);

		auto it = instancesTable.find(unixUser);
		if(it != instancesTable.end())
		{
			auto innerIt = it->second.find(sessionName);
			if(innerIt != it->second.end())
			{
				StreamInstance * inst = innerIt->second.get();
				lock.unlock();

				return inst;
			}
		}

		lock.unlock();

		return nullptr;
	}

	const UserSessionList & InstanceManager::getUserInstances(const std::string & unixUser)
	{
		LOCK lock(mtx);
		
		auto it = instancesTable.find(unixUser);

		if(it != instancesTable.end())
		{
			lock.unlock();
	
			return it->second;
		}

		lock.unlock();

		return EMPTY_SESSION_LIST;
	}

	const StreamInstance * InstanceManager::createInstance(
		const std::string & sessionName,
		const std::string & unixUser,
		const std::string & serverAddress,
		const uint16_t & port,
		const std::string & app,
		int & result)
	{
		
		LOCK lock(mtx);

		UserSessionList & userSessions = instancesTable[unixUser];
		auto it = userSessions.find(sessionName);
		
		if(it != userSessions.end())
		{
			result = CREATION_FAILED_DUPLICATE_NAME;
			lock.unlock();
			return nullptr;
		}

		StreamInstancePtr newInstance = std::make_unique<StreamInstance>();
		StreamInstance * si = newInstance.get();
		userSessions[sessionName] = std::move(newInstance);

		si->name = sessionName;
		si->address = serverAddress;
		si->port = port;
		si->application = app;

		
		si->instanceThread = std::thread([unixUser, sessionName]
		{
				int child = fork();
				if(child == 0)
				{
					struct passwd * userInfo = getpwnam(unixUser.c_str());
					char buf[0xff];
					sprintf(buf, "HOME=%s", userInfo->pw_dir);
					char * env[] = {buf, NULL};
					execle("/usr/bin/sudo", "sudo", "-u", unixUser.c_str(), "xinit", "--", ":4", (char*)0, env);
				}
				else if(child > 0)
				{
					int status;
					waitpid(child, &status, 0);
					if(status != 0)
					{
						InstanceManager::getInstance().handleSessionFailure(unixUser, sessionName);
					}
				}
		});

		result = CREATION_SUCCESSFUL;
		lock.unlock();

		return si;
	}

	bool InstanceManager::removeInstance(const std::string & unixUser, const std::string & sessionName)
	{
		LOCK lock(mtx);
		auto it = instancesTable.find(unixUser);
		if(it != instancesTable.end())
		{
			auto innerIt = it->second.find(sessionName);
			if(innerIt != it->second.end())
			{
				// TODO: Do clean up stuff
				
				it->second.erase(innerIt);
				lock.unlock();
				return true;
			}
		}

		lock.unlock();

		return false;
	}

	bool InstanceManager::removeAllInstances(const std::string & unixUser)
	{
		LOCK lock(mtx);

		auto it = instancesTable.find(unixUser);
		auto innerIt = it->second.begin();
		for(; innerIt != it->second.end(); innerIt++)
		{
			// TODO: Do clean up stuff
		}	

		instancesTable.erase(it);

		lock.unlock();
	}

	bool InstanceManager::handleSessionFailure(const std::string & unixUser, const std::string & sessionName)
	{
		return removeInstance(unixUser, sessionName);
	}
}
