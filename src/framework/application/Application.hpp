#ifndef CPP_SERVER_LINUX_APPLICATION_HPP
#define CPP_SERVER_LINUX_APPLICATION_HPP

#include <utility>
#include "../http/HttpRequest.hpp"
#include "Router.hpp"

namespace framework{
    namespace application {
        class Controller;
        abstract class Application {
            virtual http::HttpResponse& dispatchRequest(const http::HttpRequest& req) = 0;
        };

        typedef Controller* (*ControllerCreator)(Application*);
        template <class... controllers>
        ControllerCreator* ControllerCreatorArray = {[](Application* app){return new controllers(app);}...}
        /*
         * We want to generate a router giving it an initializer list {{"pathstr", int}}
         * Then the router will return the int if the a match is found.
         * our handleUrl will convert that int through template to a constructor call for the correct class.
         */


        template<class... Controller>
        class RouterApplication: Application {
        public:
            /*
             * creates a router using list initialization.
             * Assuming we pass Controller1, Controller2 as controller classes to the template we get:
             * _router{{Controller1::path, 0}, {Controller2::path, 1}}
             */
            RouterApplication():
                _router{{Controller::path, std::make_index_sequence<sizeof...(Controller)>}, ...},
                _controllersSize(sizeof...(Controller)),
                _controllerCreators(ControllerCreatorArray<Controller...>)
                {}

            http::HttpResponse& dispatchRequest(const http::HttpRequest& req) override {
                 Router::match m = _router.match(req.getUrl());
                 const int controllerIndex = m.code;
                 const bool matched = m.match;
                 const http_method method = req.getMethod();
                 if(matched && controllerIndex < _controllersSize) {
                     _controller = unique_ptr<Controller>(_controllerCreators[controllerIndex](this));
                     switch(method){
                         case http_method::GET:
                             return _controller -> get(req);
                         case http_method::POST:
                             return _controller -> post(req);
                         case http_method::DELETE:
                             return _controller -> del(req);
                         case http_method::PUT:
                             return _controller -> put(req);
                         default:
                             return NULL;
                             //TODO handle 404
                     }
                 } else {
                     //TODO: handle 404
                     return NULL;
                 }
            };
        private:
            Router _router;
            const size_t _controllersSize;
            const ControllerCreator * _controllerCreators{std::move(ControllerCreatorArray<Controller...>);
            unique_ptr<Controller> _controller;
        };
    }
}



#endif //CPP_SERVER_LINUX_APPLICATION_HPP
