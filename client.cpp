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

    try {
        to = std::make_shared<irc::connection>(ircb::args.serverName,ircb::args.port);
    }

    catch(std::runtime_error &e){
        std::cerr<<e.what()<<std::endl;
        return -1;
    }

    std::cout<<"Connecting to "<<to->name()<<" ... ";
    std::cout<<std::endl<<std::endl;

    to->handshake(ircb::args.nickName);    

    std::cout<<"Handshake complete"<<std::endl;
    std::string val;
    int tok;
    int joined = 0;
    int pos=0;

    do {
/*
        if ((numbytes = recv(to->sockfd, buf, MAXDATASIZE, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        irc::message msg;
        std::string bufs;
        
        int i=0;        
        for(;i<MAXDATASIZE-2 && buf[i]!='\r' && buf[i+1]!='\n';i++);

        buf[i]='\0';
        bufs.append(buf);
        msg.set_all(bufs);

        std::cout<<"* "<<bufs<<std::endl;
        bufs=buf[i+1];
*/
        std::unique_ptr<irc::message> msg = to->next_msg(); 
        //std::cout<<"*+*"<<msg->get_command()<<" / "<<msg->get_parameters();
        std::cout<<msg->get_source()<<" "<<msg->get_command()<<" "<<msg->get_parameters();
        std::string tmp_msg = msg->get_command();
        tmp_msg.append(" ");
        tmp_msg.append(msg->get_parameters());
        
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
            std::cout<<"CAP END"<<std::endl;
            to->send_str("CAP END");
            break;
        case irc::INVITE:
            //start=msg.get_parameters().find('#');
            //end=msg.get_parameters().size();
            //to->join(msg.get_parameters().substr(start,(end-start)));
            to->join(msg->get_channel());
            break;
        };
       /* 
        if ( tail!=(bufs.size()-2) ){
            int length=0;
            length=bufs.size()-tail;
            bufs=bufs.substr(tail+2,length);
            bufs.resize(length);
        } else bufs.clear();
        */ 
        //bufs.clear();
        
    //} while (numbytes != 0);
    } while (1);

#ifdef _WIN64
    closesocket(to->sockfd);
#else
    close(to->sockfd);
#endif // _WIN64
    return 0;
}

