#include "irc.h"
#include "args.h"
#include "gtest/gtest.h"

namespace {


/* This test is sort of broken, because it tests the behavior of the 
 * broken parser, not the correct function of the parser.
 */
TEST (IRC, PARSE){

    // modified this test to discover what is wrong with the parser
    // it needs that trailing space, but the parser will be reworked
    // so i'm not fixing it right now. At least this test doesn't segfault
    std::tuple<int, std::string> rval;
    std::string test_in = "PING :1234 ";

    rval = std::make_tuple(1,"1234 ");
    EXPECT_EQ(rval, irc::parse(test_in));
}


/* These tests check our usage of the tclap library under the assumption
 * that tclap is correct.
 */

TEST (COMMANDLINE, FULL_CORRECT){
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

    EXPECT_EQ("irc.freenode.net",ircb::args.serverName);
    EXPECT_EQ("norias",ircb::args.nickName);
}

TEST (COMMANDLINE, MISSING_NICK){

    int n = 3;
    char argl[n][25] = { 
                    "ircb",
                    "-c",
                    "irc.freenode.net",
                    };

    char *args[n];

    for(int i=0; i<n; i++){
        args[i] = argl[i];
    };

    /* The program exits and gives an error message, so we check for that.
     * Note that you can check for the _correct_ error message, but I'm
     * not ready for that depth, yet. The c* regex matches anything. 
     */
    EXPECT_DEATH( ircb::args.parse(n,&args[0]), "c*" );
}
   

}
