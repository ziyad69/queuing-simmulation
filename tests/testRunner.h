#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include <iostream>
#include <string>
#include "../src/errorHandler.h"

// ─── Counters ─────────────────────────────────────────────────────────────────
static int _passed = 0;
static int _failed = 0;

// ─── Colors ───────────────────────────────────────────────────────────────────
#define GREEN  "\033[32m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"
#define RESET  "\033[0m"

// ─── Suite header ─────────────────────────────────────────────────────────────
#define TEST_SUITE(name) \
    std::cout << "\n" << YELLOW << "[ " << name << " ]" << RESET << "\n";

// ─── toStr helper: converts any value to string for printing ─────────────────
template<typename T>
std::string toStr(const T& val) {
    return std::to_string(val);
}
// specialization for ErrorCode (uses ErrorHandler::toString)
inline std::string toStr(const ErrorCode& code) {
    return ErrorHandler::toString(code);
}
// specialization for bool
inline std::string toStr(const bool& val) {
    return val ? "true" : "false";
}
// specialization for std::string
inline std::string toStr(const std::string& val) {
    return val;
}

// ─── EXPECT_EQ ────────────────────────────────────────────────────────────────
#define EXPECT_EQ(testName, actual, expected)                                   \
    if ((actual) == (expected)) {                                               \
        std::cout << GREEN << "  PASS" << RESET << "  " << testName << "\n";   \
        _passed++;                                                              \
    } else {                                                                    \
        std::cout << RED << "  FAIL" << RESET << "  " << testName << "\n";     \
        std::cout << "        expected: " << toStr(expected) << "\n";          \
        std::cout << "        actual:   " << toStr(actual)   << "\n";          \
        _failed++;                                                              \
    }

// ─── EXPECT_TRUE ──────────────────────────────────────────────────────────────
#define EXPECT_TRUE(testName, expr)                                             \
    if ((expr)) {                                                               \
        std::cout << GREEN << "  PASS" << RESET << "  " << testName << "\n";   \
        _passed++;                                                              \
    } else {                                                                    \
        std::cout << RED << "  FAIL" << RESET << "  " << testName << "\n";     \
        std::cout << "        expected: true\n";                               \
        std::cout << "        actual:   false\n";                              \
        _failed++;                                                              \
    }

// ─── EXPECT_FALSE ─────────────────────────────────────────────────────────────
#define EXPECT_FALSE(testName, expr)                                            \
    if (!(expr)) {                                                              \
        std::cout << GREEN << "  PASS" << RESET << "  " << testName << "\n";   \
        _passed++;                                                              \
    } else {                                                                    \
        std::cout << RED << "  FAIL" << RESET << "  " << testName << "\n";     \
        std::cout << "        expected: false\n";                              \
        std::cout << "        actual:   true\n";                               \
        _failed++;                                                              \
    }

// ─── Final summary ────────────────────────────────────────────────────────────
#define TEST_SUMMARY()                                                          \
    std::cout << "\n============================\n";                           \
    std::cout << "  " << GREEN << _passed << " passed" << RESET;              \
    std::cout << "  " << (_failed > 0 ? RED : GREEN)                          \
              << _failed << " failed" << RESET << "\n";                        \
    std::cout << "============================\n";                             \
    return _failed > 0 ? 1 : 0;

#endif