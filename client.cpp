/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include <iostream>
#include <memory>
#include <string>
#include <exception>
#include <tuple>

#define PORT "6667" // the port client will be connecting to 

#define MAXDATASIZE 512 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

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

connection::connection(std::string const & host, std::string const & port) {

    struct addrinfo *servinfo, *p;

    struct addrinfo hints = {
        .ai_flags = AI_CANONNAME,
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM
    };

    char addressbuff[INET6_ADDRSTRLEN];

    int rv = 0;
    if ((rv = getaddrinfo(host.c_str(), port.c_str(), &hints, &servinfo)) != 0) {
        throw std::runtime_error(gai_strerror(rv));
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {

        if ((this->sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1 ) {
            continue;
        }

        if (connect(this->sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(this->sockfd);
            continue;
        }

        break;
    }

    if (p==NULL) {
        throw std::runtime_error("Connection error.");
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), addressbuff, sizeof addressbuff);

    this->name_s = p->ai_canonname;
    freeaddrinfo(servinfo); // all done with this structure

};

void connection::send_str(std::string const & msg) {
    send(this->sockfd, msg.c_str(), msg.length(), 0);
}

std::tuple<int,std::string> parse(std::string const & msg) {

    enum states {OPEN,
                 P, PI, PIN, PING,
                 C, CA, CAP,
                 M, MO, MOD, MODE
                };
    enum states state = OPEN;

    int j = 0;
    char token[20];
    std::string token_s;
    enum tokens {NONE_T, PING_T, CAP_T, MODE_T};
    enum tokens tok_r = NONE_T;

    for( int i = 0; i < msg.length() ; i++) {

        switch(state) {

        case OPEN:

            switch ( msg[i] ) {
            case 'P':
                state = P;
                break;
            case 'C':
                state = C;
                break;
            case 'M':
                state = M;
                break;
            };
            break;

        case P:
            if ( msg[i] == 'I') {
                state = PI;
            }
            else state = OPEN;
            break;

        case PI:
            if ( msg[i] == 'N') {
                state = PIN;
            }
            else state = OPEN;
            break;

        case PIN:
            if ( msg[i] == 'G') {
                state = PING;
            }
            else state = OPEN;
            break;

        case PING:
            i++;
            while( msg[i] != ' ') {
                i++;
                token[j] = msg[i];
                j++;
            }
            token[j] = '\0';
            j=0;
            token_s = token;
            return {PING_T, token_s};
            break;

        case C:
            if ( msg[i] == 'A' ) {
                state = CA;
            } else state = OPEN;
            break;

        case CA:
            if ( msg[i] == 'P' ) {
                state = CAP;
            } else state = OPEN;
            break;

        case CAP:
            std::cout<<std::endl<<" CAP "<<std::endl;
            state = OPEN;
            return {CAP_T, " "};
            break;

        case M:
            if(msg[i] == 'O') state = MO;
            else state = OPEN;
            break;

        case MO:
            if(msg[i] == 'D') state = MOD;
            else state = OPEN;
            break;

        case MOD:
            if(msg[i] == 'E') return {MODE_T, " "};
            else state = OPEN;
            break;

        }
    }
    return {NONE_T, " "};
}

} // namespace irc

int main(int argc, char *argv[])
{
    int numbytes;
    char buf[MAXDATASIZE];
    std::string bufs;

    std::string server = std::string(argv[1]);
    std::string port = std::string("6667");

    std::shared_ptr<irc::connection> to = std::make_shared<irc::connection>(server,port);

    std::cout<<"Connecting to "<<to->name()<<" ... ";
    std::cout<<std::endl<<std::endl;

    to->send_str("CAP LS 302\r\n");
    to->send_str("NICK booboo\r\n");
    to->send_str("USER d * 0 : a good name\r\n");

    std::string tok_val ;

    int joined = 0;

    do {
        if ((numbytes = recv(to->sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';
        bufs = buf;
        auto [tok, val] = irc::parse(bufs);

        switch(tok) {
        case 1:
            to->send_str("PONG :");
            to->send_str(val);
            to->send_str("\r\n");
            break;
        case 2:
            to->send_str("CAP END");
            to->send_str("\r\n");
            break;
        case 3:
            if( !joined ) {
                to->send_str("JOIN #antisocial\r\n");
                joined++;
            };
            break;
        };
        printf("%s",buf);
    } while (numbytes != 0);

    close(to->sockfd);

    return 0;
}

