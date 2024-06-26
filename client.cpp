#ifndef CALLGRAPH
#include <iostream>
#include <cassert>
#include "irc.h"
#include "args.h"
#endif

int main(int argc, char *argv[])
{

    int numbytes;
    char buf[MAXDATASIZE+1];
    char line[MAXDATASIZE+1];

    std::string port = std::string("6667");
    ircb::args.parse(argc, argv);

    std::shared_ptr<irc::connection> to = nullptr;
    std::cout<<"Connecting to "<<ircb::args.serverName;
    std::cout<<" on port "<<ircb::args.port<<" ... ";
    std::cout<<std::endl<<std::endl;

    try {
        to = std::make_shared<irc::connection>(ircb::args.serverName,ircb::args.nickName,ircb::args.port);
    }

    catch(std::runtime_error &e){
        std::cerr<<e.what()<<std::endl;
        return -1;
    }

    std::cout<<"Handshake complete"<<std::endl;
    std::string val;

    int tok;
    int joined = 0;
    int pos=0;

    do {
        std::unique_ptr<irc::message> msg = to->next_msg(); 
    //    std::cout<<msg->toString();
/*
        std::cout<<" SOURCE "<<msg->get_source();
        std::cout<<" ";
        std::cout<<" COMMAND "<<msg->get_command();
        std::cout<<" ";
        std::cout<<" PARAMS "<<msg->get_parameters();
        std::cout<<std::endl;

        std::string tmp_msg = msg->get_command();
        tmp_msg.append(" ");
        tmp_msg.append(msg->get_parameters());
*/        
        switch(msg->get_cmd()) {
        default:
        case 0:
            //std::cout<<" +++ "<<msg->get_command()<<std::endl;
            break;
        case irc::PING:
            to->pong(msg->get_parameters());
            std::cout<<"PONG "<<msg->get_parameters()<<std::endl;
            break;
        case irc::CAP:
            to->send_str("CAP END");
            break;
        case irc::INVITE:
            to->join(msg->get_channel());
            std::cout<<"Joined "<<std::endl;
            break;
        };
    } while (1);

#ifdef _WIN64
    closesocket(to->sockfd);
#else
    close(to->sockfd);
#endif // _WIN64
    return 0;
}

