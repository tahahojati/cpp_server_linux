#ifndef CPP_SERVER_LINUX_HTTPREQUEST_HPP
#define CPP_SERVER_LINUX_HTTPREQUEST_HPP
#include <string>
#include <unordered_map>
#define DEFINE_DATA_CB(name) int name(http_parser* parser, const char *at, size_t length)
#define DEFINE_HTTP_CB(name) int name(http_parser* parser)
#define StrMap std::unordered_map<std::string, std::string>
namespace framework {
    namespace http {
        typedef void (*logger_p)(int, std::string);
        template<logger_p logger = nullptr>
        class HttpRequest {
        public:
            static HttpRequest &&fromStringBuffer(const std::string &buffer);

            const StrMap &getHeaders() const;

            const StrMap &getData() const;

            http_method getMethod() const;

            const string &getBody() const;

            const string &getUrl() const;

        private:
            HttpRequest() = default; //use parseString to create requests.

            StrMap _headers{}, _data{};
            std::string _body{""}, _url{""};
            http_method _method;
            http_errno _error;
            std::string _valueBuf{""}, _headerBuf; //buffer for bellow
            enum class header_parse_state {
                initial = 1000,
                field,
                value,
            } _header_parse_state = header_parse_state::initial;
            /*
             * ABOVE: header parsing with http-parser library requires us to keep our own state.
             * We will use numbers
             * Documentation from README bellow:
             * (on_header_field and on_header_value shortened to on_h_*)
                 ------------------------ ------------ --------------------------------------------
                | State (prev. callback) | Callback   | Description/action                         |
                 ------------------------ ------------ --------------------------------------------
                | nothing (first call)   | on_h_field | Allocate new buffer and copy callback data |
                |                        |            | into it                                    |
                 ------------------------ ------------ --------------------------------------------
                | value                  | on_h_field | New header started.                        |
                |                        |            | Copy current name,value buffers to headers |
                |                        |            | list and allocate new buffer for new name  |
                 ------------------------ ------------ --------------------------------------------
                | field                  | on_h_field | Previous name continues. Reallocate name   |
                |                        |            | buffer and append callback data to it      |
                 ------------------------ ------------ --------------------------------------------
                | field                  | on_h_value | Value for current header started. Allocate |
                |                        |            | new buffer and copy callback data to it    |
                 ------------------------ ------------ --------------------------------------------
                | value                  | on_h_value | Value continues. Reallocate value buffer   |
                |                        |            | and append callback data to it             |
                 ------------------------ ------------ --------------------------------------------
             */

            //these methods deal with the parsing library
            DEFINE_HTTP_CB(on_message_begin);

            DEFINE_DATA_CB(on_url);

            DEFINE_DATA_CB(on_header_field);

            DEFINE_DATA_CB(on_header_value);

            DEFINE_HTTP_CB(on_headers_complete);

            DEFINE_DATA_CB(on_body);

            DEFINE_HTTP_CB(on_message_complete);
            /* When on_chunk_header is called, the current chunk length is stored
             * in parser->content_length.
             */
            DEFINE_HTTP_CB(on_chunk_header);

            DEFINE_HTTP_CB(on_chunk_complete);
        };
    }
}

#endif //CPP_SERVER_LINUX_HTTPREQUEST_HPP
