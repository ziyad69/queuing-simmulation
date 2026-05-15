#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <string>

// All possible input error types
enum class ErrorCode {
    SUCCESS,
    EMPTY_INPUT,
    NEGATIVE_NUMBER,
    DECIMAL_NUMBER,
    INVALID_FORMAT,
    OUT_OF_RANGE
};

class ErrorHandler {
public:
    // Prints the right error message for the given code
    // lo and hi are used only for OUT_OF_RANGE message
    static void handle(ErrorCode code, int lo = 0, int hi = 0);

    // Converts ErrorCode to a readable string (useful for logging)
    static std::string toString(ErrorCode code);
};

#endif
