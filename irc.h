#ifndef IRC_H
#define IRC_H

#ifndef _WIN64
    #include <unistd.h>
    #include <netdb.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <stdexcept>
    #include <fcntl.h>
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
#include <map>
#include <list>

#define PORT "6667" // the port client will be connecting to 

#define MAXDATASIZE 512 // max number of bytes we can get at once , via IRC RFQ

namespace irc {

enum cmd 
{
    CAP = 1,
    AUTHENTICATE,
    PASS,
    NICK,
    USER,
    PING,
    PONG,
    OPER,
    QUIT,
    ERROR,
    JOIN,
    PART,
    TOPIC,
    NAMES,
    LIST,
    INVITE,
    KICK,
    MOTD,
    VERSION,
    ADMIN,
    CONNECT,
    LUSERS,
    TIME,
    STATS,
    HELP,
    INFO,
    MODE,
    PRIVMSG, // handled
    NOTICE,
    WHO,
    WHOIS,
    WHOWAS,
    KILL,
    REHASH,
    RESTART,
    SQUIT,
    AWAY,
    LINKS,
    USERHOST,
    WALLOPS
};

class message;
class channel;

class connection {
private:
    std::string name_s;
    std::string uname_s;
    std::string nick_s;
    int insz;
    char in_buf[MAXDATASIZE+1];
    std::unique_ptr<char[]> inbuf;
//    int sockfd;
    std::list<std::unique_ptr<message> > msg_list;
    std::list<std::unique_ptr<channel> > chan_list;
    std::unique_ptr<std::string > read_socket();

public:
    int sockfd;
    connection(std::string const &serverName, std::string const &nick, std::string const &portString);
    connection(std::string const &serverName, std::string const &nick, int port); // change to unsigned

    std::string name() { return this->name_s; };
    void name(std::string const &name);
    std::string nick() { return this->nick_s; };
    void nick(std::string const &nick);

    void handshake(std::string const&);
    void send_str(std::string const &);
    std::string get_str();
    void pong(std::string const &);
    void join(std::string const &channel);
    std::unique_ptr<message> next_msg();

};

class channel {
private:
    std::string name;
//    connection & c_ref;
public:
    channel(std::string const &chanName);
    std::string get_name();
//    std::unique_ptr<message> next_msg();

 //   void join();
//    void part();
//    void say(std::string msg);
//    void say_to(std::string msg, std::string user);

};

class message {
private:
    std::string tags;
    std::string source;
    std::string command;
    std::string parameters;
    int cmd;
    std::string all;
public:
//    void set_tags(std::string const &);
//    void set_source(std::string const &);
//    void set_command(std::string const &);
//    void set_parameters(std::string const &);
    void set_all(std::string const &);

//    std::string get_tags();
    std::string get_source();
    std::string get_command();
    std::string get_parameters();
    int get_cmd();
    std::string get_channel();
    std::string & toString();

    bool has_source();
    bool has_parameters();
};

std::tuple<int, std::string> parse(std::string const &);

} // namespace irc

#endif // IRC_H
