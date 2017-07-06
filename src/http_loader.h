#pragma once

#include <curl/curl.h>
#include <vector>
class http_loader
{
    CURL*       m_curl;
    std::string m_url;
public:
    http_loader();
    ~http_loader();

	std::string load_file(const litehtml::tstring& url);
    const char* get_url() const;

private:
    static size_t curlWriteFunction( void *ptr, size_t size, size_t nmemb, void *stream );
};

inline const char* http_loader::get_url() const
{
    return m_url.c_str();
}
