#include <tclap/CmdLine.h>

namespace ircb {

    struct {
        std::string serverName;
        std::string nickName;
        int port;

        void parse(int argc, char **argv){
            
            try {
                TCLAP::CmdLine cmd("Irc Bot", ' ', "0.1");
                // flag, one word name, description, required?, default value, human readable description, assignment
                TCLAP::ValueArg<std::string> sNameArg("c", "serverName", "irc server name", true, "",  
                    "Fully qualified domain name of the server.", cmd);
                TCLAP::ValueArg<std::string> nNameArg("n", "nickName", "nick for irc", true, "", 
                    "The nick to use on this irc server.", cmd);
                TCLAP::ValueArg<int> portArg("p", "port", "port for irc", false, 6667, 
                    "The port to use on this irc server.", cmd);
   
                cmd.ignoreUnmatched(false); 
                cmd.parse(argc,argv);

                this->serverName = sNameArg.getValue();
                this->nickName = nNameArg.getValue();
                this->port = portArg.getValue();
            } // try
            catch (std::exception &e){
 //               std::cerr<<"error: "<<e.error() << "for arg "<<e.argId() << std::endl;
                throw 20;
            }
        } // parse
    } args;
} // namespace ircb
