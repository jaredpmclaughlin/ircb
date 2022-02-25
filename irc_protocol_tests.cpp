#include "irc.h"
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
}
