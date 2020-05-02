#ifndef CPP_SERVER_LINUX_CONTROLLER_HPP
#define CPP_SERVER_LINUX_CONTROLLER_HPP

#include "../http/HttpRequest.hpp"
#include "../http/HttpResponse.cpp"

namespace framework{
    namespace application{
        class Controller {
            virtual http::HttpResponse& put(http::HttpRequest& req) = 0;
            virtual http::HttpResponse& post(http::HttpRequest& req) = 0;
            virtual http::HttpResponse& get(http::HttpRequest& req) = 0;
            virtual http::HttpResponse& del(http::HttpRequest& req) = 0;
        };
    }
}
#endif //CPP_SERVER_LINUX_CONTROLLER_HPP
