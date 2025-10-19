#include "../src/math/point2.h"
#include <gtest/gtest.h>

class Point2Test : public testing::Test {
protected:
    Point2Test() {
        p1_ = Point2(0.0f, 0.0f);
        p2_ = Point2(1.0f, 1.0f);
        p3_ = Point2(2.0f, 3.0f);
    }

    Point2 p1_;
    Point2 p2_;
    Point2 p3_;
};

// Test default constructor
TEST_F(Point2Test, TestDefaultConstructor) {
    Point2 p;
    EXPECT_EQ(p, Point2(0.0f, 0.0f));
}

// Test parameterized constructor
TEST_F(Point2Test, TestParameterizedConstructor) {
    Point2 p(1.0f, 2.0f);
    EXPECT_EQ(p, Point2(1.0f, 2.0f));
}

// Test copy constructor
TEST_F(Point2Test, TestCopyConstructor) {
    Point2 p(p2_);
    EXPECT_EQ(p, p2_);
}

// Test equality operator
TEST_F(Point2Test, TestEqualityOperator) {
    EXPECT_TRUE(p1_ == Point2(0.0f, 0.0f));
    EXPECT_FALSE(p1_ == p2_);
}

// Test negation operator
TEST_F(Point2Test, TestNegationOperator) {
    EXPECT_EQ(-p2_, Point2(-1.0f, -1.0f));
}

// Test assignment operator
TEST_F(Point2Test, TestAssignmentOperator) {
    Point2 p;
    p = p3_;
    EXPECT_EQ(p, p3_);
}

// Test scalar multiplication (s * Point2)
TEST_F(Point2Test, TestScalarMultiplicationLeft) {
    EXPECT_EQ(2.0f * p2_, Point2(2.0f, 2.0f));
}

// Test scalar multiplication (Point2 * s)
TEST_F(Point2Test, TestScalarMultiplicationRight) {
    EXPECT_EQ(p2_ * 3.0f, Point2(3.0f, 3.0f));
}

// Test distance function
TEST_F(Point2Test, TestDistance) {
    EXPECT_NEAR(distance(p1_, p2_), sqrt(2.0f), 1e-5);
}