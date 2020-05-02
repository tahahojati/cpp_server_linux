#ifndef CPP_SERVER_LINUX_TODOCONTROLLER_HPP
#define CPP_SERVER_LINUX_TODOCONTROLLER_HPP
#include "../../framework/application/Controller.hpp"
namespace application{
namespace controllers {
        class TodoController: public framework::application::Controller {
            int get(http::Request& req, http::Response& res) override ;
            int post(http::Request& req, http::Response& res) override ;
        };
    }
}



#endif //CPP_SERVER_LINUX_TODOCONTROLLER_HPP
