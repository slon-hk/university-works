#include <gtest/gtest.h>
#include <stdexcept>

// Класс CircularDeque вынесен в modules/circular_deque.h
#include "modules/circular_deque.h"

TEST(CircularDeque, CreateEmptyDeque) {
    CircularDeque dq(5, false);
    EXPECT_EQ(dq.size(), 0);
    EXPECT_TRUE(dq.empty());
    EXPECT_FALSE(dq.full());
}

TEST(CircularDeque, InvalidCapacityRaises) {
    EXPECT_THROW(CircularDeque(0, false), std::invalid_argument);
}

TEST(CircularDeque, FillDequeFromBothEnds) {
    CircularDeque dq(5, false);
    dq.push_front(1);
    dq.push_front(0);
    dq.push_back(2);
    dq.push_back(3);
    dq.push_back(4);

    EXPECT_EQ(dq.front(), 0);
    EXPECT_EQ(dq.back(), 4);
    EXPECT_TRUE(dq.full());
    EXPECT_EQ(dq.size(), 5);
}

TEST(CircularDeque, PushOnFullWithoutForceRaises) {
    CircularDeque dq(5, false);
    dq.push_front(1);
    dq.push_front(0);
    dq.push_back(2);
    dq.push_back(3);
    dq.push_back(4);

    EXPECT_THROW(dq.push_back(5), std::overflow_error);
    EXPECT_THROW(dq.push_front(5), std::overflow_error);
    EXPECT_EQ(dq.size(), 5);
    EXPECT_EQ(dq.front(), 0);
    EXPECT_EQ(dq.back(), 4);
}

TEST(CircularDeque, PopFromBothEnds) {
    CircularDeque dq(5, false);
    dq.push_front(1);
    dq.push_front(0);
    dq.push_back(2);
    dq.push_back(3);
    dq.push_back(4);

    EXPECT_EQ(dq.pop_back(), 4);
    EXPECT_EQ(dq.pop_back(), 3);
    EXPECT_EQ(dq.pop_back(), 2);
    EXPECT_EQ(dq.pop_front(), 0);
    EXPECT_EQ(dq.pop_back(), 1);
    EXPECT_TRUE(dq.empty());
}

TEST(CircularDeque, PopFromEmptyRaises) {
    CircularDeque dq(5, false);
    EXPECT_THROW(dq.pop_back(), std::underflow_error);
    EXPECT_THROW(dq.pop_front(), std::underflow_error);
}

TEST(CircularDeque, FrontBackOnEmptyRaise) {
    CircularDeque dq(1, false);
    EXPECT_THROW(dq.front(), std::underflow_error);
    EXPECT_THROW(dq.back(), std::underflow_error);
}

TEST(CircularDeque, PushForceEvictsOppositeEnd) {
    CircularDeque dq(3, true);
    dq.push_back(0);
    dq.push_back(1);
    dq.push_back(2);
    dq.push_front(3);
    dq.push_back(4);

    EXPECT_EQ(dq.front(), 0);
    EXPECT_EQ(dq.back(), 4);
    EXPECT_EQ(dq.size(), 3);
}

TEST(CircularDeque, PushForceExampleFromTask) {
    CircularDeque dq(3, true);
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);

    dq.push_back(4);

    EXPECT_EQ(dq.front(), 2);
    EXPECT_EQ(dq.back(), 4);
    EXPECT_EQ(dq.size(), 3);
}

TEST(CircularDeque, PushFrontForceEvictsBack) {
    CircularDeque dq(3, true);
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);

    dq.push_front(0);

    EXPECT_EQ(dq.front(), 0);
    EXPECT_EQ(dq.back(), 2);
    EXPECT_EQ(dq.size(), 3);
}

TEST(CircularDeque, ResizeGrowPreservesData) {
    CircularDeque dq(2, false);
    dq.push_back(1);
    dq.push_back(2);
    dq.resize(4);

    EXPECT_EQ(dq.size(), 2);
    EXPECT_EQ(dq.front(), 1);
    EXPECT_EQ(dq.back(), 2);
    EXPECT_FALSE(dq.full());
    dq.push_back(3);
    dq.push_front(0);
    EXPECT_EQ(dq.front(), 0);
    EXPECT_EQ(dq.back(), 3);
    EXPECT_TRUE(dq.full());
}

TEST(CircularDeque, ResizeShrinkTruncatesData) {
    CircularDeque dq(5, false);
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);
    dq.push_back(4);
    dq.resize(2);

    EXPECT_EQ(dq.size(), 2);
    EXPECT_TRUE(dq.full());
    EXPECT_EQ(dq.front(), 1);
    EXPECT_EQ(dq.back(), 2);
}

TEST(CircularDeque, ResizePreservesWrappedData) {
    CircularDeque dq(3, false);
    dq.push_back(1);
    dq.push_back(2);
    dq.push_back(3);
    dq.pop_front();
    dq.push_back(4);
    dq.resize(5);

    EXPECT_EQ(dq.size(), 3);
    EXPECT_EQ(dq.pop_front(), 2);
    EXPECT_EQ(dq.pop_front(), 3);
    EXPECT_EQ(dq.pop_front(), 4);
    EXPECT_TRUE(dq.empty());
}

TEST(CircularDeque, ResizeInvalidCapacityRaises) {
    CircularDeque dq(3, false);
    EXPECT_THROW(dq.resize(0), std::invalid_argument);
}

TEST(CircularDeque, PipelineFromReport) {
    CircularDeque dq(4, true);

    dq.push_back(0);
    dq.push_back(1);
    dq.push_front(2);
    EXPECT_EQ(dq.pop_back(), 1);
    dq.push_back(3);
    dq.push_back(4);
    EXPECT_TRUE(dq.full());

    dq.push_back(5);
    EXPECT_EQ(dq.pop_front(), 0);

    EXPECT_EQ(dq.size(), 3);
    EXPECT_EQ(dq.front(), 3);
    EXPECT_EQ(dq.back(), 5);
}

TEST(CircularDeque, SingleElementEdgeCases) {
    CircularDeque dq(1, false);
    dq.push_back(42);

    EXPECT_EQ(dq.front(), 42);
    EXPECT_EQ(dq.back(), 42);
    EXPECT_TRUE(dq.full());

    EXPECT_EQ(dq.pop_front(), 42);
    EXPECT_TRUE(dq.empty());
}

TEST(CircularDeque, SingleElementForceOverwrite) {
    CircularDeque dq(1, true);
    dq.push_back(1);
    dq.push_back(2);
    EXPECT_EQ(dq.front(), 2);
    EXPECT_EQ(dq.back(), 2);
    EXPECT_EQ(dq.size(), 1);

    dq.push_front(3);
    EXPECT_EQ(dq.front(), 3);
    EXPECT_EQ(dq.back(), 3);
    EXPECT_EQ(dq.size(), 1);
}

TEST(CircularDeque, FrontBackReturnMutableReference) {
    CircularDeque dq(3, false);
    dq.push_back(1);
    dq.push_back(2);

    dq.front() = 10;
    dq.back() = 20;

    EXPECT_EQ(dq.front(), 10);
    EXPECT_EQ(dq.back(), 20);
}

TEST(CircularDeque, WrapAroundManyOperations) {
    CircularDeque dq(3, false);
    for (int i = 0; i < 100; ++i) {
        dq.push_back(i);
        EXPECT_EQ(dq.back(), i);
        EXPECT_EQ(dq.pop_front(), i);
        EXPECT_TRUE(dq.empty());
    }
}