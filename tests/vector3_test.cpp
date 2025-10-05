#include "../src/math/vector3.h"
#include <gtest/gtest.h>

class Vector3Test : public testing::Test {
    protected:
        Vector3Test() {
            v1_ = Vector3(0.0f, 0.0f, 0.0f);
            v2_ = Vector3(0.0f, 0.0f, 1.0f);
            v3_ = Vector3(1.0f, 1.0f, 0.0f);
        }
    
    Vector3 v1_;
    Vector3 v2_;
    Vector3 v3_;
};

TEST_F(Vector3Test, TestDefaultConstructor) {
    Vector3 v;
    EXPECT_EQ(v, Vector3(0.0f, 0.0f, 0.0f));
}

TEST_F(Vector3Test, TestParameterizedConstructor) {
    Vector3 v(1.0f, 2.0f, 3.0f);
    EXPECT_EQ(v, Vector3(1.0f, 2.0f, 3.0f));
}

TEST_F(Vector3Test, TestCopyConstructor) {
    Vector3 v(v3_);
    EXPECT_EQ(v, v3_);
}

TEST_F(Vector3Test, TestEqualityOperator) {
    EXPECT_TRUE(v1_ == Vector3(0.0f, 0.0f, 0.0f));
    EXPECT_FALSE(v1_ == v2_);
}

TEST_F(Vector3Test, TestNegationOperator) {
    EXPECT_EQ(-v3_, Vector3(-1.0f, -1.0f, 0.0f));
}

TEST_F(Vector3Test, TestAssignmentOperator) {
    Vector3 v;
    v = v2_;
    EXPECT_EQ(v, v2_);
}

TEST_F(Vector3Test, TestNormalize) {
    Vector3 v(3.0f, 0.0f, 4.0f);
    v.normalize();
    EXPECT_NEAR(v.x, 0.6f, 1e-5);
    EXPECT_NEAR(v.y, 0.0f, 1e-5);
    EXPECT_NEAR(v.z, 0.8f, 1e-5);
}

TEST_F(Vector3Test, TestCrossProduct) {
    EXPECT_EQ(v2_.cross(v3_), Vector3(-1.0f, 1.0f, 0.0f));
}

TEST_F(Vector3Test, TestDistance) {
    EXPECT_NEAR(v1_.distance(v2_), 1.0f, 1e-5);
}

TEST_F(Vector3Test, TestDotProduct) {
    EXPECT_EQ(v2_.dot(v3_), 0.0f);
    EXPECT_EQ(v3_.dot(v3_), 2.0f);
}

TEST_F(Vector3Test, TestAdditionOperator) {
    EXPECT_EQ(v1_ + v2_, Vector3(0.0f, 0.0f, 1.0f));
    EXPECT_EQ(v2_ + v3_, Vector3(1.0f, 1.0f, 1.0f));
    EXPECT_EQ(v1_ + v3_, Vector3(1.0f, 1.0f, 0.0f));
}

TEST_F(Vector3Test, TestSubtractionOperator) {
    EXPECT_EQ(v3_ - v2_, Vector3(1.0f, 1.0f, -1.0f));
}

TEST_F(Vector3Test, TestScalarMultiplication) {
    EXPECT_EQ(2.0f * v3_, Vector3(2.0f, 2.0f, 0.0f));
}