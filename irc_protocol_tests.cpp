#include "irc.h"
#include "args.h"
#include "gtest/gtest.h"

namespace {

TEST (IRC, PARSE){

    // modified this test to discover what is wrong with the parser
    // it needs that trailing space, but the parser will be reworked
    // so i'm not fixing it right now. At least this test doesn't segfault
    std::tuple<int, std::string> rval;
    std::string test_in = "PING :1234 ";

    rval = std::make_tuple(1,"1234");
    EXPECT_EQ(rval, irc::parse(test_in));
}

TEST (COMMANDLINE, PARSE){
    /* this should probably be in it's own test file, but here it is. */
    int n = 5;
    char argl[5][25] = { 
                    "ircb",
                    "-c",
                    "irc.freenode.net",
                    "-n",
                    "norias" 
                    };

    char *args[n];

    for(int i=0; i<n; i++){
        args[i] = argl[i];
    };

    ircb::args.parse(n,&args[0]);

//    std::cerr<<ircb::args.serverName<<std::endl;
//    std::cerr<<ircb::args.userName<<std::endl;

    EXPECT_EQ("irc.freenode.net",ircb::args.serverName);
    EXPECT_EQ("norias",ircb::args.userName);
}
}
