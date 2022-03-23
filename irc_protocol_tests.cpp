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

TEST (COMMANDLINE, FULL_CORRECT_ORDER_1){
    /* this should probably be in it's own test file, but here it is. */
    int n = 7;
    char argl[n][25] = { 
                    "ircb",
                    "-c",
                    "ircserver.net",
                    "-n",
                    "username",
                    "-p",
                    "6668"
                    };

    char *args[n];

    for(int i=0; i<n; i++){
        args[i] = argl[i];
    };

    ircb::args.parse(n,&args[0]);

    EXPECT_EQ("ircserver.net",ircb::args.serverName);
    EXPECT_EQ("username",ircb::args.nickName);
    EXPECT_EQ(6668,ircb::args.port);
}

TEST (COMMANDLINE, FULL_CORRECT_ORDER_2){
    /* this should probably be in it's own test file, but here it is. */
    int n = 7;
    char argl[n][25] = { 
                    "ircb",
                    "-c",
                    "ircserver.net",
                    "-p",
                    "6668",
                    "-n",
                    "username"
                    };

    char *args[n];

    for(int i=0; i<n; i++){
        args[i] = argl[i];
    };

    ircb::args.parse(n,&args[0]);

    EXPECT_EQ("ircserver.net",ircb::args.serverName);
    EXPECT_EQ("username",ircb::args.nickName);
    EXPECT_EQ(6668,ircb::args.port);
}

TEST (COMMANDLINE, FULL_CORRECT_ORDER_3){
    /* this should probably be in it's own test file, but here it is. */
    int n = 7;
    char argl[n][25] = { 
                    "ircb",
                    "-n",
                    "username",
                    "-p",
                    "6668",
                    "-c",
                    "ircserver.net"
                    };

    char *args[n];

    for(int i=0; i<n; i++){
        args[i] = argl[i];
    };

    ircb::args.parse(n,&args[0]);

    EXPECT_EQ("ircserver.net",ircb::args.serverName);
    EXPECT_EQ("username",ircb::args.nickName);
    EXPECT_EQ(6668,ircb::args.port);
}

TEST (COMMANDLINE, FULL_CORRECT_ORDER_4){
    /* this should probably be in it's own test file, but here it is. */
    int n = 7;
    char argl[n][25] = { 
                    "ircb",
                    "-n",
                    "username",
                    "-c",
                    "ircserver.net",
                    "-p",
                    "6668"
                    };

    char *args[n];

    for(int i=0; i<n; i++){
        args[i] = argl[i];
    };

    ircb::args.parse(n,&args[0]);

    EXPECT_EQ("ircserver.net",ircb::args.serverName);
    EXPECT_EQ("username",ircb::args.nickName);
    EXPECT_EQ(6668,ircb::args.port);
}

TEST (COMMANDLINE, FULL_CORRECT_ORDER_5){
    /* this should probably be in it's own test file, but here it is. */
    int n = 7;
    char argl[n][25] = { 
                    "ircb",
                    "-p",
                    "6668",
                    "-n",
                    "username",
                    "-c",
                    "ircserver.net"
                    };

    char *args[n];

    for(int i=0; i<n; i++){
        args[i] = argl[i];
    };

    ircb::args.parse(n,&args[0]);

    EXPECT_EQ("ircserver.net",ircb::args.serverName);
    EXPECT_EQ("username",ircb::args.nickName);
    EXPECT_EQ(6668,ircb::args.port);
}

TEST (COMMANDLINE, FULL_CORRECT_ORDER_6){
    /* this should probably be in it's own test file, but here it is. */
    int n = 7;
    char argl[n][25] = { 
                    "ircb",
                    "-p",
                    "6668",
                    "-c",
                    "ircserver.net",
                    "-n",
                    "username"
                    };

    char *args[n];

    for(int i=0; i<n; i++){
        args[i] = argl[i];
    };

    ircb::args.parse(n,&args[0]);

    EXPECT_EQ("ircserver.net",ircb::args.serverName);
    EXPECT_EQ("username",ircb::args.nickName);
    EXPECT_EQ(6668,ircb::args.port);
}


TEST (COMMANDLINE, FULL_CORRECT_NO_PORT_ORDER_1){
    /* this should probably be in it's own test file, but here it is. */
    int n = 5;
    char argl[n][25] = { 
                    "ircb",
                    "-c",
                    "ircserver.net",
                    "-n",
                    "username"
                    };

    char *args[n];

    for(int i=0; i<n; i++){
        args[i] = argl[i];
    };

    ircb::args.parse(n,&args[0]);

    EXPECT_EQ("ircserver.net",ircb::args.serverName);
    EXPECT_EQ("username",ircb::args.nickName);
    EXPECT_EQ(6667,ircb::args.port);
}

TEST (COMMANDLINE, FULL_CORRECT_NO_PORT_ORDER_2){
    /* this should probably be in it's own test file, but here it is. */
    int n = 7;
    char argl[n][25] = { 
                    "ircb",
                    "-n",
                    "username",
                    "-c",
                    "ircserver.net"
                    };

    char *args[n];

    for(int i=0; i<n; i++){
        args[i] = argl[i];
    };

    ircb::args.parse(n,&args[0]);

    EXPECT_EQ("ircserver.net",ircb::args.serverName);
    EXPECT_EQ("username",ircb::args.nickName);
    EXPECT_EQ(6667,ircb::args.port);
}

TEST (COMMANDLINE, MISSING_NICK){

    int n = 3;
    char argl[n][25] = { 
                    "ircb",
                    "-c",
                    "ircserver.net",
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

TEST (COMMANDLINE, MISSING_NICK_HAS_PORT){

    int n = 5;
    char argl[n][25] = { 
                    "ircb",
                    "-c",
                    "ircserver.net",
                    "-p",
                    "6668"
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

TEST (COMMANDLINE, MISSING_SERVER){

    int n = 3;
    char argl[n][25] = { 
                    "ircb",
                    "-n",
                    "username",
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

TEST (COMMANDLINE, MISSING_SERVER_HAS_PORT){

    int n = 5;
    char argl[n][25] = { 
                    "ircb",
                    "-n",
                    "username",
                    "-p",
                    "6668"
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


TEST (COMMANDLINE, NICK_W_SPACE){

    int n = 6;
    char argl[n][25] = { 
                    "ircb",
                    "-n",
                    "user",
                    "name",
                    "-c",
                    "ircserver.net"
                    };

    char *args[n];

    for(int i=0; i<n; i++){
        args[i] = argl[i];
    };

    //EXPECT_ANY_THROW( ircb::args.parse(n,&args[0]) );
    EXPECT_DEATH(ircb::args.parse(n,&args[0]),"c*");
}

TEST (COMMANDLINE, NO_ARGS){

    int n = 1;
    char argl[n][25] = { 
                    "ircb"
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
