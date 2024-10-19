#include "http.h"

namespace wlib
{
	client::client()
	{
	}


	void client::setKeepAlive(bool keepAlive)
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_keepAlive = keepAlive;
	}

	void client::setRequestHeader(std::string param, std::string value)
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_requestHeaders.insert(std::make_pair(param, value));
	}

	void client::setRequestHeaders(std::multimap<std::string, std::string> headers)
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		m_requestHeaders = headers;
	}

	bool client::get(std::string url, std::string path, progress callback)
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		httplib::Client cli(url);
		//cli.set_decompress(false);
		m_requestHeaders.clear();
		m_responseHeaders.clear();
		httplib::Headers headers;
		for (std::map<std::string, std::string>::iterator iter = m_requestHeaders.begin(); iter != m_requestHeaders.end(); iter++)
		{
			headers.insert(std::make_pair(iter->first, iter->second));
		}
		//cli.set_connection_timeout(1,0);
		//cli.set_read_timeout(3, 0);
		//cli.set_write_timeout(3, 0);
		if (httplib::Result result = cli.Get(path, headers, callback))
		{
			if (result && result->status == 200)
			{
				std::string contentEncoding;
				if (result->has_header("Content-Encoding"))
				{
					contentEncoding = result->get_header_value("Content-Encoding");
				}
				if (contentEncoding == "gzip")
				{

					//std::cout << "Response is gzip encoded." << std::endl;

				}
				else
				{

					//std::cout << "Response is not gzip encoded." << std::endl;
				}
			}

			for (auto iter = result->headers.begin(); iter != result->headers.end(); iter++)
			{
				m_responseHeaders.insert(std::make_pair(iter->first, iter->second));
				//std::cout << "[Response]" << iter->first << ":" << iter->second << std::endl;
				// m_responseHeaders.insert(iter->first, iter->second);
			}

			m_body = result->body;//Codec::utf8ToAnsi(result->body);//UTF8_To_string
			return true;
		}
		else
		{
			httplib::Error error = result.error();
			m_error = httplib::to_string(error);
			return false;
		}

	}

	bool client::post(std::string url, std::string path, std::string body, std::string contentType)
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		httplib::Client cli(url);
		//cli.set_decompress(false);
		m_requestHeaders.clear();
		m_responseHeaders.clear();
		httplib::Headers headers;
		for (std::map<std::string, std::string>::iterator iter = m_requestHeaders.begin(); iter != m_requestHeaders.end(); iter++)
		{
			headers.insert(std::make_pair(iter->first, iter->second));
		}
		cli.set_connection_timeout(1, 0);
		cli.set_read_timeout(3, 0);
		cli.set_write_timeout(3, 0);
		if (httplib::Result result = cli.Post(path, headers, body, contentType))
		{
			if (result && result->status == 200)
			{
				std::string contentEncoding;
				if (result->has_header("Content-Encoding"))
				{
					contentEncoding = result->get_header_value("Content-Encoding");
				}
				if (contentEncoding == "gzip")
				{

					//std::cout << "Response is gzip encoded." << std::endl;

				}
				else
				{

					//std::cout << "Response is not gzip encoded." << std::endl;
				}
			}

			for (auto iter = result->headers.begin(); iter != result->headers.end(); iter++)
			{
				m_responseHeaders.insert(std::make_pair(iter->first, iter->second));
				//std::cout << "[Response]" << iter->first << ":" << iter->second << std::endl;
				// m_responseHeaders.insert(iter->first, iter->second);
			}

			m_body = result->body;//Codec::utf8ToAnsi(result->body);//UTF8_To_string
			return true;
		}
		else
		{
			httplib::Error error = result.error();
			m_error = httplib::to_string(error);
			return false;
		}
	}



	std::multimap<std::string, std::string> client::responseHeaders()
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_responseHeaders;
	}

	std::string client::body()
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_body;
	}

	std::string client::error()
	{
		std::lock_guard<std::mutex> guard(m_mutex);
		return m_error;
	}

    server::server()
    {
    }

    server::~server()
    {
    }

    bool server::listen(std::string ip, int port)
    {
		m_server.listen(ip.c_str(), port);

        return false;
    }

	void server::get(std::string path, httplib::Server::Handler callback)
	{
		m_server.Get(path.c_str(), callback);
	}

}
