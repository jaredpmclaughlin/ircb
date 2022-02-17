#include "irc.h"
#include "gtest/gtest.h"

namespace {

TEST (IRC, PARSE){

    std::tuple<int, std::string> rval;

    rval = std::make_tuple(1,"1234");
    EXPECT_EQ(rval, irc::parse("PING :1234"));
}
}
