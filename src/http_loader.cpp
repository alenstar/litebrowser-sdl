#include <iostream>
#include <regex>
#include <string>

#include "globals.h"
#include "http_loader.h"
#include "logdef.h"

http_loader::http_loader() {
    m_curl = curl_easy_init();
    curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPIDLE, 120L);
    curl_easy_setopt(m_curl, CURLOPT_TCP_KEEPINTVL, 60L);
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION,
                     http_loader::curlWriteFunction);
}

http_loader::~http_loader() { curl_easy_cleanup(m_curl); }

size_t http_loader::curlWriteFunction(void *ptr, size_t size, size_t nmemb,
                                      void *stream) {
    std::string *s = (std::string *)stream;
    s->append((const char *)ptr, size * nmemb);
    return size * nmemb;
}

std::string http_loader::load_file(const litehtml::tstring &url) {
    m_url = url;

    std::string stream;
    LOGD("load_file: %s", url.c_str());
    std::regex url_regex(
        "^(([^:/?#]+):)?(//([^/?#]*))?([^?#]*)(\\?([^#]*))?(#(.*))?",
        std::regex::extended);
    std::smatch url_match_result;
    if (std::regex_match(url, url_match_result, url_regex)) {

        if (url_match_result[2] == "file" || url_match_result[2] == "") {
            return loadfile(url.c_str());
        } else {
            curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &stream);
            curl_easy_perform(m_curl);
            char *new_url = NULL;
            if (curl_easy_getinfo(m_curl, CURLINFO_EFFECTIVE_URL, &new_url) ==
                CURLE_OK) {
                if (new_url) {
                    m_url = new_url;
                }
            }
        }
    }

    // LOGD("stream: %s", stream.c_str());
    return stream;
}
