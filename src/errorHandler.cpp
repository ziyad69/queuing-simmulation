#include "errorHandler.h"
#include <iostream>

void ErrorHandler::handle(ErrorCode code, int lo, int hi) {
    std::cout << "  [!] ";
    switch (code) {
        case ErrorCode::EMPTY_INPUT:
            std::cout << "Input cannot be empty.\n";
            break;
        case ErrorCode::NEGATIVE_NUMBER:
            std::cout << "Negative numbers are not allowed.\n";
            break;
        case ErrorCode::DECIMAL_NUMBER:
            std::cout << "Decimal numbers are not allowed — enter a whole number.\n";
            break;
        case ErrorCode::INVALID_FORMAT:
            std::cout << "Invalid input — enter a whole positive number (no letters or symbols).\n";
            break;
        case ErrorCode::OUT_OF_RANGE:
            std::cout << "Out of range — enter a value between " << lo << " and " << hi << ".\n";
            break;
        case ErrorCode::SUCCESS:
            break;
    }
}

std::string ErrorHandler::toString(ErrorCode code) {
    switch (code) {
        case ErrorCode::EMPTY_INPUT:     return "EMPTY_INPUT";
        case ErrorCode::NEGATIVE_NUMBER: return "NEGATIVE_NUMBER";
        case ErrorCode::DECIMAL_NUMBER:  return "DECIMAL_NUMBER";
        case ErrorCode::INVALID_FORMAT:  return "INVALID_FORMAT";
        case ErrorCode::OUT_OF_RANGE:    return "OUT_OF_RANGE";
        case ErrorCode::SUCCESS:         return "SUCCESS";
        default:                         return "UNKNOWN";
    }
}
