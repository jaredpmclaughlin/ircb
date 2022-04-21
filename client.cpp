#include <iostream>
#include "irc.h"
#include "args.h"

int main(int argc, char *argv[])
{

    int numbytes;
    char buf[MAXDATASIZE];
    std::string bufs;

    std::string port = std::string("6667");
    ircb::args.parse(argc, argv);

    std::shared_ptr<irc::connection> to = nullptr;

    try {
        to = std::make_shared<irc::connection>(ircb::args.serverName,ircb::args.port);
    }

    catch(std::runtime_error &e){
        std::cerr<<e.what()<<std::endl;
        return -1;
    }

    std::cout<<"Connecting to "<<to->name()<<" ... ";
    std::cout<<std::endl<<std::endl;
/*
    to->send_str("CAP LS 302n");
    to->send_str("NICK ");
    to->send_str(ircb::args.nickName);
    to->send_str("");
    to->send_str("USER d * 0 : a good name");
*/

    to->handshake(ircb::args.nickName);    

    std::string val;
    int tok;

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

#ifdef _WIN64
    closesocket(to->sockfd);
#else
    close(to->sockfd);
#endif // _WIN64
    return 0;
}

