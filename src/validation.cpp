#include "validation.h"
#include <cctype>
#include <string>

ErrorCode isValidFormat(const std::string& input) {

    // Rule 1: empty string
    if (input.empty()) return ErrorCode::EMPTY_INPUT;

    // Rule 2: negative number
    if (input[0] == '-') return ErrorCode::NEGATIVE_NUMBER;

    // Rule 3: decimal number
    for (int i = 0; i < (int)input.length(); i++)
        if (input[i] == '.') return ErrorCode::DECIMAL_NUMBER;

    // Rule 4: letters or symbols
    for (int i = 0; i < (int)input.length(); i++)
        if (!isdigit(input[i])) return ErrorCode::INVALID_FORMAT;

    return ErrorCode::SUCCESS;
}

bool isInRange(int number, int lo, int hi) {
    return number >= lo && number <= hi;
}

ValidationResult validate(const std::string& input, int lo, int hi) {

    // Step 1: check format
    ErrorCode formatResult = isValidFormat(input);
    if (formatResult != ErrorCode::SUCCESS)
        return { formatResult, 0 };

    // Step 2: convert to int
    int number = stoi(input);

    // Step 3: check range
    if (!isInRange(number, lo, hi))
        return { ErrorCode::OUT_OF_RANGE, 0 };

    // All good
    return { ErrorCode::SUCCESS, number };
}
