#include <catch2/catch_test_macros.hpp>
#include "../include/math/floatutil.h"
#include <iostream>

TEST_CASE("floatutil::equalf") {
    // exact equality
    REQUIRE(equalf(0.0f, 0.0f));

    // within epsilon relative tolerance
    REQUIRE(equalf(1.0f, 1.0f + (Eps * 0.5f)));

    // outside epsilon
    REQUIRE_FALSE(equalf(1.0f, 1.0f + (Eps * 4.0f)));
}

TEST_CASE("floatutil::is_near_zero") {
    REQUIRE(is_near_zero(0.0f));
    REQUIRE(is_near_zero(Eps * 0.5f));
    REQUIRE_FALSE(is_near_zero(Eps * 2.0f));
}

TEST_CASE("floatutil::is_negative_and_is_greater_than_zero") {
    // slightly less than -Eps should be considered negative
    REQUIRE(is_negative(-Eps - 1e-8f));
    // values inside the epsilon band around zero are not negative
    REQUIRE_FALSE(is_negative(-Eps * 0.5f));

    // slightly greater than Eps should be considered greater-than-zero
    REQUIRE(is_greater_than_zero(Eps + 1e-8f));
    // values inside the epsilon band around zero are not greater-than-zero
    REQUIRE_FALSE(is_greater_than_zero(Eps * 0.5f));
}