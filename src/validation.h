#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include "errorHandler.h"

// Result of a validation attempt
// If code == SUCCESS, value holds the valid int
// Otherwise value is 0 and code tells you exactly what went wrong
struct ValidationResult {
    ErrorCode code;
    int       value;
};

// Checks format only — returns the specific ErrorCode for what failed
ErrorCode isValidFormat(const std::string& input);

// Checks if number is within [lo, hi]
bool isInRange(int number, int lo, int hi);

// Full validation — combines format + range check
// Returns ValidationResult with SUCCESS + value, or an ErrorCode explaining failure
ValidationResult validate(const std::string& input, int lo, int hi);

#endif
