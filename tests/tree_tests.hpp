#ifndef STC_TESTS_TREE_TESTS_HPP_INCLUDED
#define STC_TESTS_TREE_TESTS_HPP_INCLUDED

#include <gtest/gtest.h>

#include "../src/tree.hpp"

static STC::Containers::RBTree SUT_SAMPLE;

TEST(TestTreeSize, ReturnsRightSize) {
    STC::Containers::RBTree sut(SUT_SAMPLE);
    EXPECT_EQ(sut.size(), 0);
    sut.insert(0);
    sut.insert(1);
    EXPECT_EQ(sut.size(), 2);
    sut.insert(-1);
    EXPECT_EQ(sut.size(), 3);
}

TEST(TestTreeInsert, HandlesValidInput) {
    STC::Containers::RBTree sut{};
    sut.insert(1);
    EXPECT_EQ(sut.size(), 1);
    sut.insert(2);
    sut.insert(60);
    EXPECT_EQ(sut.size(), 3);
}

TEST(TreeInsert, HandlesRepeatingKeyInput) {
    STC::Containers::RBTree sut{};
    sut.insert(1);
    EXPECT_ANY_THROW(sut.insert(1));
    EXPECT_NO_THROW(sut.insert(-1));
}

TEST(TreeFindFromBegin, HandlesValidInput) {
    STC::Containers::RBTree sut{};
    sut.insert(1);
    sut.insert(5);
    sut.insert(10);
    EXPECT_EQ(sut.find_from_begin(2), 5);
    sut.insert(-10);
    sut.insert(0);
    sut.insert(20);
    EXPECT_EQ(sut.find_from_begin(5), 10);

}

TEST(TreeFindFromBegin, HandlesInvalidInput) {
    STC::Containers::RBTree sut{};
    sut.insert(1);
    sut.insert(-1);
    sut.insert(0);
    EXPECT_ANY_THROW(sut.find_from_begin(99));
    EXPECT_ANY_THROW(sut.find_from_begin(4));
}

TEST(TreeCountNodesLess, HandlesValidInput) {
    STC::Containers::RBTree sut{};
    EXPECT_EQ(sut.count_nodes_less(99), 0);
    sut.insert(10);
    sut.insert(40);
    sut.insert(-1);
    EXPECT_EQ(sut.count_nodes_less(0), 1);
    EXPECT_EQ(sut.count_nodes_less(41), 3);
}

#endif