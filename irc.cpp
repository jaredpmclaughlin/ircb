#ifndef CALLGRAPH
#include "irc.h"
#endif
#include <iostream>

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

irc::connection::connection(std::string const & host, std::string const &nick, int port) : connection{host, nick, std::to_string(port) } {};

irc::connection::connection(std::string const & host, std::string const &nick, std::string const & port) {

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
#ifdef _WIN64
            closesocket(this->sockfd);
#else
            close(this->sockfd);
#endif

            continue;
        }

        break;
    }

    if (p==NULL) {
        throw std::runtime_error("Connection error.");
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), addressbuff, sizeof addressbuff);

    socklen_t optsz = sizeof(insz); 
    getsockopt(this->sockfd, SOL_SOCKET, SO_RCVBUF, (void *)&insz, &optsz); // let's make our buffers match
    this->inbuf = std::make_unique<char[]>(this->insz); 
/*
    int val;
    if((val=fcntl(this->sockfd,F_GETFL,0))<0) 
        throw std::runtime_error("Connection error getting file descriptor flags.");

    val |= O_NONBLOCK;

    if( fcntl(this->sockfd,F_SETFL,val)<0) 
        throw std::runtime_error("Connection error setting file descriptor flags."); 
*/
    this->name_s = p->ai_canonname;
    freeaddrinfo(servinfo); // all done with this structure
    // now let's handshake, or else abort and clean up
    // this will have to throw something, but I'm not sure how to check
    // if handshaking worked
    this->nick_s = nick;
    this->handshake(this->nick_s); 

};

// a raw interface for testing / debugging
void irc::connection::send_str(std::string const & msg) {
    send(this->sockfd, msg.c_str(), msg.length(), 0);
    send(this->sockfd, "\r\n", 2, 0);
}

// a raw interface for testing / debugging
std::string irc::connection::get_str(){
    char buf[512];
    recv(this->sockfd, buf, 511, 0);

    return std::string(buf);
}

void irc::connection::handshake(std::string const &nick){
    send(this->sockfd, "CAP LS 302\r\n", 12, 0);
    // missing PASS ?
    send(this->sockfd, "NICK ", 5,0);
    send(this->sockfd, nick.c_str(), nick.length(), 0);
    send(this->sockfd, "\r\n",2,0);
    send(this->sockfd, "USER d * 0 : A Name\r\n", 21, 0);
}

void irc::connection::pong(std::string const & token){
    this->send_str("PONG "+token);
}

void irc::connection::join(std::string const & chan){
    this->send_str("JOIN "+chan);
    // check for success?
    std::unique_ptr<channel> tmp = std::make_unique<channel>(chan);
    this->chan_list.push_back(std::move(tmp));
    
}

std::unique_ptr<std::string > irc::connection::read_socket(){
    std::unique_ptr<std::string > tmp = std::make_unique<std::string>();
    int i=0;
    do {
        if( (recv(this->sockfd, (void *)&(this->in_buf[i]),1,0))==-1 ){
            perror("recv");
            exit(1);
        }
        i++;
    } while(this->in_buf[i-1]!='\n');
    this->in_buf[i+1]='\0'; //guard
    tmp->assign(this->in_buf);
    return std::move(tmp);
}

std::unique_ptr<irc::message> irc::connection::next_msg(){
    std::unique_ptr<std::string > input = this->read_socket();
    std::unique_ptr<message> tmp = std::make_unique<message>();
    tmp->set_all(*input);
  
    if(chan_list.size()>0 && tmp->get_cmd() == irc::PRIVMSG ) {
        for(std::list<std::unique_ptr<irc::channel> >::iterator it=this->chan_list.begin(); it!=this->chan_list.end(); ++it) {
            if((tmp->get_parameters().find((*it)->get_name())) != std::string::npos ) {
                std::cout<<tmp->get_parameters()<<std::endl; 
            }
        }
    }
    else this->msg_list.push_back(std::move(tmp));
    tmp=std::move(this->msg_list.front()); 
    this->msg_list.pop_front();
    return std::move(tmp);
};

std::string irc::channel::get_name() {
    return this->name;
}

irc::channel::channel(std::string const &chanName)
{
    this->name=chanName;
}


std::string & irc::message::toString(){
    this->all.assign(this->source);
    this->all.append(this->command);
    this->all.append(this->parameters);
    return (this->all);
}

std::map<std::string,int> tokens {
  
    {"INVITE",irc::INVITE}, 
    {"CAP",irc::CAP}, 
    {"PING",irc::PING},
    {"PRIVMSG",irc::PRIVMSG},
    {"NONE",0}
    };

std::tuple<int,std::string> irc::parse(std::string const & msg) {
    char token[512];
    std::map<std::string,int>::iterator it;

    int i=0;
    for( ; i<msg.length() && i<512 && msg[i]!=' '; i++ ) ; 
    it = tokens.find(msg.substr(0,i));
    if (it == tokens.end() ) return {0,""};
    return {it->second,msg.substr((i+1),msg.length())};
}

void irc::message::set_all(std::string const & msg){
    //skip tags for right now
    // search for the optional source
    std::string::size_type source_start;
    std::string::size_type source_end;
    std::string::size_type cmd_start;
    std::string::size_type cmd_end;

    std::map<std::string,int>::iterator it;

    //source_start = msg.find(':');
    if(msg[0]==':') {
    //if(source_start != std::string::npos) {
        source_end = msg.find(' ',0);
        this->source = msg.substr(0,source_end);
        cmd_start = source_end+1;
    } else {
        cmd_start = 0;
    }

    cmd_end = msg.find(' ',cmd_start);
    if ( cmd_end == std::string::npos){
        this->command = msg.substr(cmd_start,msg.size()-2);
    }else{
        this->command = msg.substr(cmd_start,(cmd_end-cmd_start));
        this->parameters = msg.substr(cmd_end+1,(msg.size()-2-cmd_end+1));
    }
    it = tokens.find(this->command);
    if (it==tokens.end()) this->cmd=0;
    else this->cmd = it->second;
}

std::string irc::message::get_source(){
    return this->source;
}

std::string irc::message::get_command(){
    return this->command;
}

std::string irc::message::get_parameters(){
    return this->parameters;
}

std::string irc::message::get_channel(){
    std::string::size_type start;
    std::string::size_type end;

    start=this->parameters.find('#');
    end  =this->parameters.size();

    return this->parameters.substr(start,(end-start));
}

int irc::message::get_cmd(){
    return this->cmd;
}

bool irc::message::has_source(){
    return ! this->source.empty();
}

bool irc::message::has_parameters(){
    return ! this->source.empty();
}


