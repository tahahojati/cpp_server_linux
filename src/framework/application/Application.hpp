#ifndef CPP_SERVER_LINUX_APPLICATION_HPP
#define CPP_SERVER_LINUX_APPLICATION_HPP

#include <utility>
#include "../http/HttpRequest.hpp"
#include "Router.hpp"

namespace framework{
    namespace application {
        abstract class Application {
            virtual http::HttpResponse& dispatchRequest(const http::HttpRequest& req) = 0;
        };

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
            constexpr RouterApplication():
                _router{{Controller::path, std::make_index_sequence<sizeof...(Controller)>}, ...} {}

            constexpr http::HttpResponse& dispatchRequest(const http::HttpRequest& req) override {
                 Router::match m = _router.match(req.getUrl());
                 return _handleRequestImpl<m.routeIndex>(req, m.params);
            };
        private:
            /**
             * The router will return an int specifying the route that was matched to the request.
             * We need to turn this int into an actual call.
             * @tparam index
             */
            template <int index>
            constexpr HttpResponse& _handleRequestImpl(const http::HttpRequest& req, Router::match m){
                std::get<index>(Controller) _controller;
                return req.getMethod() == http_method::GET ? _controller.get(req, m.params) :
                (
                    req.getMethod() == http_method::POST ? _controller.post(req, m.params) :
                    (
                        req.getMethod() == http_method::DELETE ? _controller.del(req, m.params) :
                        (
                            req.getMethod() == http_method::PUT ? _controller.put(req, m.params) : NULL
                        )
                    )
                );
            }
            constexpr Router _router;
        };


        /*
         * Correct implementation bellow
         * #include <iostream>
#include <string>
#include <utility>
using namespace std;
struct Router {
  void add(string str, size_t i){}
};

struct Base {
  static const string d;
};
const string Base::d = string ("");
template <size_t i, class first, class... cls>
void _addToRouter (Router &r) {
  r.add(first::d, i);
  _addToRouter<i+1, cls...> (r);
}
template <size_t i>
void _addToRouter(){}

template<class... cls>
void addToRouter(){
  _addToRouter<0, cls...>();
}

template<class... cls>
Base* (* factory[sizeof...(cls)])() = {[](){return dynamic_cast<Base*>(new cls);}...};

template<class... cls>
struct RouterApp {
  Router r;
  static const Base* (*controllerFactory[sizeof...(cls)])() = factory<cls...>;
//  RouterApp(){
//    addToRouter<cls...>(r);
//  }
  RouterApp(): r{{cls::d, std::make_index_sequence<sizeof...(cls)>()}...} {}
};


         */
    }
}



#endif //CPP_SERVER_LINUX_APPLICATION_HPP
