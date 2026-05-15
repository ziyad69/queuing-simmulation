#ifndef TEST_VALIDATION_H
#define TEST_VALIDATION_H

#include "../tests/testRunner.h"
#include "../src/validation.h"
#include "../src/errorHandler.h"

void test_validation() {
    TEST_SUITE("validation — isValidFormat")

    EXPECT_EQ("empty string returns EMPTY_INPUT",
        isValidFormat(""),
        ErrorCode::EMPTY_INPUT)

    EXPECT_EQ("letters returns INVALID_FORMAT",
        isValidFormat("abc"),
        ErrorCode::INVALID_FORMAT)

    EXPECT_EQ("negative returns NEGATIVE_NUMBER",
        isValidFormat("-5"),
        ErrorCode::NEGATIVE_NUMBER)

    EXPECT_EQ("decimal returns DECIMAL_NUMBER",
        isValidFormat("3.5"),
        ErrorCode::DECIMAL_NUMBER)

    EXPECT_EQ("symbols returns INVALID_FORMAT",
        isValidFormat("@#!"),
        ErrorCode::INVALID_FORMAT)

    EXPECT_EQ("mixed letters and numbers returns INVALID_FORMAT",
        isValidFormat("12abc"),
        ErrorCode::INVALID_FORMAT)

    EXPECT_EQ("valid number returns SUCCESS",
        isValidFormat("42"),
        ErrorCode::SUCCESS)

    EXPECT_EQ("single digit returns SUCCESS",
        isValidFormat("1"),
        ErrorCode::SUCCESS)

    // ── validate() full flow ──────────────────────────────────────────────────
    TEST_SUITE("validation — validate()")

    EXPECT_EQ("valid input in range returns SUCCESS",
        validate("5", 1, 10).code,
        ErrorCode::SUCCESS)

    EXPECT_EQ("valid input in range returns correct value",
        validate("5", 1, 10).value,
        5)

    EXPECT_EQ("number above range returns OUT_OF_RANGE",
        validate("99", 1, 10).code,
        ErrorCode::OUT_OF_RANGE)

    EXPECT_EQ("number below range returns OUT_OF_RANGE",
        validate("0", 1, 10).code,
        ErrorCode::OUT_OF_RANGE)

    EXPECT_EQ("letters return INVALID_FORMAT",
        validate("abc", 1, 10).code,
        ErrorCode::INVALID_FORMAT)

    EXPECT_EQ("negative returns NEGATIVE_NUMBER",
        validate("-3", 1, 10).code,
        ErrorCode::NEGATIVE_NUMBER)

    EXPECT_EQ("decimal returns DECIMAL_NUMBER",
        validate("2.5", 1, 10).code,
        ErrorCode::DECIMAL_NUMBER)

    EXPECT_EQ("boundary lo value is valid",
        validate("1", 1, 10).code,
        ErrorCode::SUCCESS)

    EXPECT_EQ("boundary hi value is valid",
        validate("10", 1, 10).code,
        ErrorCode::SUCCESS)

    EXPECT_EQ("boundary lo value is correct",
        validate("1", 1, 10).value,
        1)

    EXPECT_EQ("boundary hi value is correct",
        validate("10", 1, 10).value,
        10)
}

#endif