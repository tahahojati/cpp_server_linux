#include "HttpResponse.hpp"
#include <http-parser/http-parser.h> //only included for the enums
#include <string>
#include <ostringstream>
#include <unordered_map>
#include <ctime>
#define StrMap std::unordered_map<std::string, std::string>

using std::unordered_map;
using std:ostringstream;
using std:string;
using std::end;
namespace framework {
    /*
     * We want to convert the http status to a text.
     * So we define a map for it.
     */
#define XX(num, name, string) {num, "string"},
    unordered_map<http_status, string> statusMap {HTTP_STATUS_MAP(XX)};
#undef XX
    string getCurrentDateTime() {
        time_t rawtime;
        struct tm *timeinfo;
        char buffer[200];
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %Z", timeinfo);
        return string(buffer);
    }

    std::string&& HttpResponse::toString(){
        ostringstream stream;
        stream<<"HTTP/1.1 "<<_status<<" "<<statusMap[_status]<<endl;
        stream<<"Date: "<<getCurrentDateTime()<<endl;
        stream<<"Content-Length: "<< body.length() <<endl;
        for(const auto& i: headers){
            stream<<i.first<<": "<<i.second<<endl;
        }
        stream<<endl<<body; 
        return stream.str();
    }
}

/*
 * HTTP/1.1 200 OK
Date: Mon, 27 Jul 2009 12:28:53 GMT
Server: Apache/2.2.14 (Win32)
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
Content-Length: 88
Content-Type: text/html
Connection: Closed
<html>
<body>
<h1>Hello, World!</h1>
</body>
</html>
 */