#ifndef CPP_SERVER_LINUX_HTTPRESPONSE_HPP
#define CPP_SERVER_LINUX_HTTPRESPONSE_HPP
#include <http-parser/http-parser.h> //only included for the enums
#include <string>
#define StrMap std::unordered_map<std::string, std::string>
namespace framework {
    class HttpResponse {
    public:
        std::string&& toString();

        http_status status;
        StrMap headers;
        std::string body;
    };
}

#endif //CPP_SERVER_LINUX_HTTPRESPONSE_HPP
