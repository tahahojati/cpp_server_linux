#ifndef CPP_SERVER_LINUX_HTTPRESPONSE_HPP
#define CPP_SERVER_LINUX_HTTPRESPONSE_HPP
#include <http-parser/http-parser.h> //only included for the enums
#include <string>
#include <unique_ptr>
#define StrMap std::unordered_map<std::string, std::string>
//TODO how to have a data map that also stores type information about the data? But can hold any type?
//TODO how to do runtime type stuff with c++
namespace framework {
    namespace http {
        class HttpResponse {
        public:
            std::string &&toString();
            http_status status;
            StrMap headers;
            std::string body;
        };
    }
}

#endif //CPP_SERVER_LINUX_HTTPRESPONSE_HPP
