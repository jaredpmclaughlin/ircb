#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

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
    //int sockfd;

public:
    int sockfd;
    connection(std::string const &, std::string const &);
    std::string name() {
        return this->name_s;
    };
    void send_str(std::string const &);

};

std::tuple<int, std::string> parse(std::string const &);

} // namespace irc
