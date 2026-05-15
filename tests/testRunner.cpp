#include <iostream>
#include "testRunner.h"
#include "test_validation.h"
#include "test_customerType.h"
#include "test_serverType.h"
#include "test_serverListType.h"

int main() {
    std::cout << "============================\n";
    std::cout << "   QUEUING SIM TEST RUNNER  \n";
    std::cout << "============================\n";

    test_validation();
    test_customerType();
    test_serverType();
    test_serverListType();

    TEST_SUMMARY()
}