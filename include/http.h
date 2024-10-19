#ifndef HTTP_H
#define HTTP_H

#include "wlib.h"
#include <map>
#include <mutex>
#include <functional> // C++11 or higher


#include "httplib.h"

namespace wlib
{

	class client
	{
		using progress = std::function<bool(uint64_t current, uint64_t total)>;
	public:
		explicit client();
	public:
		void setKeepAlive(bool keepAlive);
		void setRequestHeader(std::string param, std::string value);
		void setRequestHeaders(std::multimap<std::string, std::string> headers);
	public:
		bool get(std::string url, std::string path, progress callback = nullptr);
		bool post(std::string url, std::string path, std::string body, std::string contentType);
	public:
		std::multimap<std::string, std::string> responseHeaders();
		std::string body();
		std::string error();
	private:
		bool m_keepAlive;
		std::multimap<std::string, std::string> m_requestHeaders;
		std::multimap<std::string, std::string> m_responseHeaders;
		std::string m_error;
		std::string m_body;
		std::mutex m_mutex;
	};

	class server
	{

		public:
			server();
			~server();
		public:
				bool listen(std::string ip, int port);
				void get(std::string path, httplib::Server::Handler callback);
		private:
			httplib::Server m_server;
	};
}

#endif // !_HTTP_H
