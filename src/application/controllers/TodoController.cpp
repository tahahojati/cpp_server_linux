#include "TodoController.hpp"
#include "../../framework/application/Controller.hpp"
namespace application{
    namespace controllers {
        string TodoController::path = string("/id/{id:\\d+}");
        int TodoController::get(http::Request& req, http::Response& res) {

        }
        int TodoController::post(http::Request& req, http::Response& res) {

        }
    }
}

