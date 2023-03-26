#include <catch2/catch_all.hpp>
#include <iostream>
#include "../utils/flag.hpp"


TEST_CASE("OR", "FlagTest")
{
    using namespace fl;

    uint8_t flag1 = 0b1;
    uint8_t flag2 = 0b10;
    uint8_t flag3 = 0b100;
    CreationFlag cflag_1(flag1);
    CreationFlag cflag_2(flag2);
    CreationFlag cflag_3(flag3);

    auto flag = cflag_1 | cflag_2 | cflag_3;
    
    REQUIRE(static_cast<uint8_t>(flag) == 0b111);
}

TEST_CASE("AND", "FlagTest")
{
    using namespace fl;

    uint8_t flag1 = 0b11101;
    uint8_t flag2 = 0b10111;
    uint8_t flag3 = 0b11011;
    CreationFlag cflag_1(flag1);
    CreationFlag cflag_2(flag2);
    CreationFlag cflag_3(flag3);

    auto flag = cflag_1 & cflag_2 & cflag_3;
    
    REQUIRE(static_cast<uint8_t>(flag) == 0b10001);
}

TEST_CASE("AND & OR", "FlagTest")
{
    using namespace fl;

    uint8_t flag1 = 0b11101;
    uint8_t flag2 = 0b10111; //10101
    uint8_t flag3 = 0b01100; //11101
    uint8_t flag4 = 0b11111;
    uint8_t flag5 = 0b00001;
    CreationFlag cflag_1(flag1);
    CreationFlag cflag_2(flag2);
    CreationFlag cflag_3(flag3);
    CreationFlag cflag_4(flag4);
    CreationFlag cflag_5(flag5);

    auto flag = cflag_1 & cflag_2 | cflag_3 & cflag_4 | cflag_5;
     
    REQUIRE(static_cast<uint8_t>(flag) == 0b11101);
}