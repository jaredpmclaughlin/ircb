#include <iostream>
#include "irc.h"

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

