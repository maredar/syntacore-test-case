#ifndef STC_TESTS_APP_TESTS_HPP_INCLUDED
#define STC_TESTS_APP_TESTS_HPP_INCLUDED

#include <gtest/gtest.h>
#include <iostream>

#include "../src/app.hpp"

TEST(AppRun, HandlesValidInput) {
    STC::Application app{};
    std::string expected = "-1 2 2 \n";
    std::istringstream is("k 8 k 2 k -1 m 1 m 2 n 3");
    testing::internal::CaptureStdout();
    app.run(is);
    std::string actual = testing::internal::GetCapturedStdout();
    EXPECT_EQ(actual, expected);
}

#endif