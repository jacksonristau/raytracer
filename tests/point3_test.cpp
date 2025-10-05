#include "../src/math/point3.h"
#include "../src/math/vector3.h"
#include <gtest/gtest.h>

class Point3Test : public testing::Test {
    protected:
        Point3Test() {
            p1_ = Point3(0.0f, 0.0f, 0.0f);
            p2_ = Point3(1.0f, 1.0f, 1.0f);
            p3_ = Point3(2.0f, 3.0f, 4.0f);
            v1_ = Vector3(1.0f, 0.0f, 0.0f);
        }

        Point3 p1_;
        Point3 p2_;
        Point3 p3_;
        Vector3 v1_;
    };

TEST_F(Point3Test, TestDefaultConstructor) {
    Point3 p;
    EXPECT_EQ(p, Point3(0.0f, 0.0f, 0.0f));
}

TEST_F(Point3Test, TestParameterizedConstructor) {
    Point3 p(1.0f, 2.0f, 3.0f);
    EXPECT_EQ(p, Point3(1.0f, 2.0f, 3.0f));
}

TEST_F(Point3Test, TestCopyConstructor) {
    Point3 p(p2_);
    EXPECT_EQ(p, p2_);
}

TEST_F(Point3Test, TestEqualityOperator) {
    EXPECT_TRUE(p1_ == Point3(0.0f, 0.0f, 0.0f));
    EXPECT_FALSE(p1_ == p2_);
}

TEST_F(Point3Test, TestNegationOperator) {
    EXPECT_EQ(-p2_, Point3(-1.0f, -1.0f, -1.0f));
}

TEST_F(Point3Test, TestAssignmentOperator) {
    Point3 p;
    p = p3_;
    EXPECT_EQ(p, p3_);
}

TEST_F(Point3Test, TestSubtractionPoint) {
    EXPECT_EQ(p2_ - p1_, Vector3(1.0f, 1.0f, 1.0f));
}

TEST_F(Point3Test, TestSubtractionVector) {
    EXPECT_EQ(p3_ - v1_, Point3(1.0f, 3.0f, 4.0f));
}

TEST_F(Point3Test, TestAdditionVector) {
    EXPECT_EQ(p1_ + v1_, Point3(1.0f, 0.0f, 0.0f));
}

TEST_F(Point3Test, TestScalarMultiplication) {
    EXPECT_EQ(2.0f * p2_, Point3(2.0f, 2.0f, 2.0f));
    EXPECT_EQ(p2_ * 3.0f, Point3(3.0f, 3.0f, 3.0f));
}

TEST_F(Point3Test, TestDistance) {
    EXPECT_NEAR(distance(p1_, p2_), sqrt(3.0f), 1e-5);
}