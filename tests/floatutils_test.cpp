#include <catch2/catch_test_macros.hpp>
#include "../include/math/floatutil.h"

TEST_CASE("floatutils") {
    SECTION ("equalf") {
        REQUIRE(equalf(1.0f, 1.0f)); // identical
        REQUIRE(equalf(1000000.0f, 1000001.0f, 1e-6f)); // relative difference within tolerance
        REQUIRE(!equalf(1.0f, 1.2f, 1e-6f)); // clearly different
        REQUIRE(equalf(-1.0f, -1.0000005f, 1e-6f)); // negative values, within tolerance
    }
    SECTION("is_negative") {
        REQUIRE(is_negative(-1.0f));
        REQUIRE(!is_negative(-1e-9f)); // not less than default -eps
        REQUIRE(is_negative(-1e-6f, 1e-8f)); // explicitly smaller than -eps
        REQUIRE(!is_negative(0.0f));
    }
    SECTION("is_near_zero") {
        REQUIRE(is_near_zero(1e-9f)); // within default eps
        REQUIRE(is_near_zero(-5e-9f));
        REQUIRE(!is_near_zero(1e-7f)); // outside default eps
        REQUIRE(is_near_zero(1e-6f, 1e-5f)); // custom eps larger than value
    }
}