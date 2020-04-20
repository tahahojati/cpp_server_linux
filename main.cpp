#include <sys/socket.h> //#AF_INET is ipv4
#include <netinet/in.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>

int main() {
    /*
     * int socket(int domain, int type, int protocol);
     * domain is the namespace of the socket (or protocol family) for IPv4 we choose PF_INET, PF_INET6 denotes IPv6
     * type determines communication style, or how you can use the socket after creation.  STREAM type let's us write
     * to the socket like a file
     * protocol is the protocol used (e.g. tcp).  Each namespace has a default protocol and for PF_INET it is
     * IPPROTO_TCP which denotes tcp.  Passing 0 chooses this default protocol.
     *
     * The function returns the socket descriptor which will be -1 in case of failure
     */
    int socket_desc = socket(PF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        if (errno == EACCES) {
            //in libc, system errors are stored in errno.
            std::cerr << "You do not have privileges to open a socket \n";
        }
        exit(EXIT_FAILURE);
    }
    /*
     * You need to specify an address for the socket to bind to.
     * For an internet server socket this is specified in struct sockaddr_in
     * htonl converts an unsigned integer to the "network byte order" .  I.e it may change the endian-ness of the int.
     * sin_addr.s_addr is used to specify the ip of the server we'll be connecting to.  Since this is a server,
     * we need to set it to INADDR_ANY meaning that we'll connect to any host.
     */
    struct sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(3000u);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(socket_desc, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        if(errno == EACCES) {
            std::cerr << "You do not have privileges to bind to port 3000 \n";
        } else if (errno == EADDRINUSE) {
            std::cerr << "Port 3000 is already in use \n";
        }
        close(socket_desc);
        exit(EXIT_FAILURE);
    }
    //listen to the port and keep a backlog of 20 connection requests.
    if(listen(socket_desc, 20) < 0) {
        std::cerr << "listen failed. Here's the error code "<<errno<<"\n";
        close(socket_desc);
        exit(EXIT_FAILURE);
    }

    sockaddr_storage client_info{};
    socklen_t client_info_size = sizeof(client_info);
    int new_socket = accept(socket_desc, (struct sockaddr *) &client_info, &client_info_size);
    std::cerr << "connection accepted \n";
    if(new_socket < 0) {
        std::cerr << "failed to accept connection. Here's the error code: "<<errno<<std::endl;
        close(socket_desc);
        exit(EXIT_FAILURE);
    }
    int num_bytes_read;
    char buffer[2];
    std::cout << "Here's what I got from the client: \n";
    fcntl(new_socket, F_SETFL, O_NONBLOCK);
    while(true){
        num_bytes_read = recv(new_socket,
            static_cast<void *>(buffer),
            static_cast<size_t>(sizeof(buffer)-1), O_NONBLOCK);
//        if(num_bytes_read >= 0) {
            std::cout << buffer;
            std::cerr << "getting this many bytes!! "<< num_bytes_read<<"\n";
//        }
        if(num_bytes_read < (int)sizeof(buffer) -1) {
            if(num_bytes_read < 0) {
                std::cerr <<"failed to receive data from the socket, Here's the error code: "<<errno<<std::endl;
                close(new_socket);
                close(socket_desc);
                exit(EXIT_FAILURE);
            } else {
                std::cerr<<"we got this many bytes: "<< num_bytes_read <<std::endl;
                break;
            }
        }
    }

    close(new_socket);
    close(socket_desc);
    return 0;
}