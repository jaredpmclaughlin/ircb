#ifndef IRC_H
#define IRC_H

#ifndef _WIN64
    #include <unistd.h>
    #include <netdb.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
#else
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
#pragma comment(lib, "Ws2_32.lib")
// winsock headers here
    #include <winsock2.h>
    #include <ws2tcpip.h> // IPv6
    #include<stdexcept>

#endif


#include <memory>
#include <string>
#include <exception>
#include <tuple>

#define PORT "6667" // the port client will be connecting to 

#define MAXDATASIZE 512 // max number of bytes we can get at once 

namespace irc {
class connection {
private:
    std::string name_s;
    std::string uname_s;
    std::string nick_s;
    int insz;
    //char in_buf[insz];
    std::unique_ptr<char[]> inbuf;
    //int sockfd;

public:
    int sockfd;
    connection(std::string const &, std::string const &);
    connection(std::string const &, int);
    std::string name() {
        return this->name_s;
    };
    void send_str(std::string const &);
    std::string get_str();

};

std::tuple<int, std::string> parse(std::string const &);

} // namespace irc

#endif // IRC_H
