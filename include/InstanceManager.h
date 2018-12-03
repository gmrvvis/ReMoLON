#ifndef __RRFRONTEND_INSTANCEMANAGER_H__
#define __RRFRONTEND_INSTANCEMANAGER_H__

#include <unordered_map>
#include <string>
#include <cstdint>
#include <mutex>
#include <thread>
#include <memory>

namespace rrfrontend
{
	typedef struct StreamInstance
	{
		std::string name;
		std::string address;
		uint16_t port;
		std::string application;

		std::thread instanceThread;

		const std::string & getAddress() const { return address; }
		const uint16_t & getPort() const { return port; }
		const std::string & getRunningApp() const { return application; }

		const std::string & getAddress() { return address; }
		const uint16_t & getPort() { return port; }
		const std::string & getRunningApp() { return application; }

		StreamInstance()
			: address("")
			, port(0)
			, application("")
		{
		}

	} StreamInstance;

	typedef std::unique_ptr<StreamInstance> StreamInstancePtr;
	typedef std::unordered_map<std::string, StreamInstancePtr> UserSessionList;

	class InstanceManager
	{
		private:
			static InstanceManager INSTANCE;
			static UserSessionList EMPTY_SESSION_LIST;
		public:
			static const int CREATION_SUCCESSFUL = 0;
			static const int CREATION_FAILED_DUPLICATE_NAME = 1;
			static const int CREATION_FAILED_NO_RESOURCES_AVAILABLE = 2;
		public:
			static InstanceManager & getInstance();
		private:
			std::unordered_map<std::string, UserSessionList> instancesTable;
			std::mutex mtx;
		private:
			InstanceManager();
		public:
			~InstanceManager();
	
			const StreamInstance * getInstanceForUser(const std::string & unixUser, const std::string & sessionName);
			const UserSessionList & getUserInstances(const std::string & unixUser);

			const StreamInstance * createInstance(
				const std::string & sessionName,
				const std::string & unixUser,
				const std::string & serverAddress,
				const uint16_t & port,
				const std::string & app,
				int & result);

			bool removeInstance(const std::string & unixUser, const std::string & sessionName);
			bool removeAllInstances(const std::string & unixUser);

			bool handleSessionFailure(const std::string & unixUser, const std::string & sessionName);
	};
}

#endif
