#include "HttpRequest.hpp"
#include <string>
#include <unordered_map>
#include <http-parser/http-parser.h>
#define BOUND(pointer, method) std::bind(&HttpRequest::method, pointer);
#define INFO_LEVEL 5
using std::unordered_map;
using std::string;
using std::move;

namespace framework {
    template <logger = nullptr> {
        static HttpRequest&& HttpRequest::fromStringBuffer(const std::string &buffer){
            HttpRequest req{};
            HttpRequest *req_p = &req;
            http_parser_settings settings;
            //settings object..
            settings.on_message_begin = BOUND(req_p, on_message_begin);
            settings.on_url = BOUND(req_p, on_url);
            settings.on_header_field = BOUND(req_p, on_header_field);
            settings.on_header_value = BOUND(req_p, on_header_value);
            settings.on_headers_complete = BOUND(req_p, on_headers_complete);
            settings.on_body = BOUND(req_p, on_body);
            settings.on_message_complete = BOUND(req_p, on_message_complete);
            settings.on_chunk_header = BOUND(req_p, on_chunk_header);
            settings.on_chunk_complete = BOUND(req_p, on_chunk_complete);

            http_parser *parser = malloc(sizeof(http_parser));
            if(parser) {
                http_parser_init(parser, HTTP_REQUEST);
                int nparsed = http_parser_execute(parser, &settings, buffer.c_str(), buffer.length() + 1);
                free(parser);
                if (nparsed != recved) {
                    //Error occurred in parsing.
                    throw std::runtime_error("Parsing failed");
                }
                return move(req);
            } else {
                //malloc failed
                throw std::bad_alloc("Allocation for parser failed");
            }
        }

        const StrMap &HttpRequest::getHeaders() const{
            return _headers;
        }

        const StrMap &HttpRequest::getData() const{
            return _data;
        }

        http_method HttpRequest::getMethod() const{
            return _method;
        }

        const string &HttpRequest::getBody() const{
            return _body;
        }

        const string &HttpRequest::getUrl() const{
            return _url;
        }

        //these methods deal with the parsing library
        DEFINE_HTTP_CB(HttpRequest::on_message_begin){
            if(logger != nullptr) {
                logger(INFO_LEVEL, "on_message_begin was called");
            }
        }

        DEFINE_DATA_CB(HttpRequest::on_url){
            if(logger != nullptr) {
                logger(INFO_LEVEL, string("on_url was called with: ") + string(at, length));
            }
            _url += string(at, length);
        }

        DEFINE_DATA_CB(HttpRequest::on_header_field){
            if(logger != nullptr) {
                logger(INFO_LEVEL, string("on_header_field was called with: ") + string(at, length));
            }
            switch (_header_parse_state) {
                case: header_parse_state::value:
                    _headers[_headerBuf] = _valueBuf;
                    _valueBuf = "";
                    _headerBuf = "";
                case: header_parse_state::initial:
                    _header_parse_state = header_parse_state::field;
                case: header_parse_state::field:
                    _headerBuf += string(at, length);
                    break;
            }
        }

        DEFINE_DATA_CB(HttpRequest::on_header_value){
            if(logger != nullptr) {
                logger(INFO_LEVEL, string("on_header_value was called with: ") + string(at, length));
            }
            _header_parse_state = header_parse_state::value;
            _valueBuf += string(at, length);
        }

        DEFINE_HTTP_CB(HttpRequest::on_headers_complete){
            if(logger != nullptr) {
                logger(INFO_LEVEL, "on_headers_complete was called");
            }
            //according to documentation, here we can get the method
            _method = parser -> http_method;
            _error = parser -> http_errno;
        }

        DEFINE_DATA_CB(HttpRequest::on_body){
            if(logger != nullptr) {
                logger(INFO_LEVEL, string("on_body was called with: ") + string(at, length));
            }
            _body += string(at, length);
        }

        DEFINE_HTTP_CB(HttpRequest::on_message_complete){
            if(logger != nullptr) {
                logger(INFO_LEVEL, "on_message_complete was called");
            }
        }
        /* When on_chunk_header is called, the current chunk length is stored
         * in parser->content_length.
         */
        DEFINE_HTTP_CB(HttpRequest::on_chunk_header){
            if(logger != nullptr) {
                logger(INFO_LEVEL, "on_chunk_header was called");
            }
        }

        DEFINE_HTTP_CB(HttpRequest::on_chunk_complete){
            if(logger != nullptr) {
                logger(INFO_LEVEL, "on_chunk_complete was called");
            }
        }
    }
}

#endif //CPP_SERVER_LINUX_HTTPREQUEST_HPP
