#ifndef TEST_SERVER_TYPE_H
#define TEST_SERVER_TYPE_H

#include "../tests/testRunner.h"
#include "../src/serverType.h"
#include "../src/customerType.h"

void test_serverType() {
    TEST_SUITE("serverType — constructor")

    serverType s1;
    EXPECT_TRUE("server starts as free",              s1.isFree())
    EXPECT_EQ("transaction time starts at 0",
        s1.getRemainingTransactionTime(), 0)

    TEST_SUITE("serverType — setBusy() / setFree()")

    serverType s2;
    s2.setBusy();
    EXPECT_FALSE("setBusy makes isFree false",        s2.isFree())

    s2.setFree();
    EXPECT_TRUE("setFree makes isFree true again",    s2.isFree())

    TEST_SUITE("serverType — setTransactionTime(int)")

    serverType s3;
    s3.setTransactionTime(5);
    EXPECT_EQ("transaction time set to 5",
        s3.getRemainingTransactionTime(), 5)

    TEST_SUITE("serverType — decreaseTransactionTime()")

    serverType s4;
    s4.setTransactionTime(5);
    s4.decreaseTransactionTime();
    EXPECT_EQ("decreases from 5 to 4",
        s4.getRemainingTransactionTime(), 4)

    s4.decreaseTransactionTime();
    s4.decreaseTransactionTime();
    EXPECT_EQ("decreases correctly after multiple calls",
        s4.getRemainingTransactionTime(), 2)

    TEST_SUITE("serverType — setCurrentCustomer()")

    serverType s5;
    customerType c(7, 3, 1, 6);
    s5.setCurrentCustomer(c);
    s5.setTransactionTime();   // takes transactionTime from customer

    EXPECT_EQ("getCurrentCustomerNumber()",
        s5.getCurrentCustomerNumber(), 7)

    EXPECT_EQ("getCurrentCustomerArrivalTime()",
        s5.getCurrentCustomerArrivalTime(), 3)

    EXPECT_EQ("getCurrentCustomerWaitingTime()",
        s5.getCurrentCustomerWaitingTime(), 1)

    EXPECT_EQ("getCurrentCustomerTransactionTime()",
        s5.getCurrentCustomerTransactionTime(), 6)

    EXPECT_EQ("setTransactionTime() takes time from customer",
        s5.getRemainingTransactionTime(), 6)
}

#endif