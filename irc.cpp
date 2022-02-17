#include "irc.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

irc::connection::connection(std::string const & host, std::string const & port) {

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

void irc::connection::send_str(std::string const & msg) {
    send(this->sockfd, msg.c_str(), msg.length(), 0);
}

std::tuple<int,std::string> irc::parse(std::string const & msg) {

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
