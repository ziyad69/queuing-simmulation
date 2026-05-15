#ifndef TEST_CUSTOMER_TYPE_H
#define TEST_CUSTOMER_TYPE_H

#include "../tests/testRunner.h"
#include "../src/customerType.h"

void test_customerType() {
    TEST_SUITE("customerType — constructor")

    customerType c1;
    EXPECT_EQ("default customerNumber is 0",  c1.getCustomerNumber(),   0)
    EXPECT_EQ("default arrivalTime is 0",     c1.getArrivalTime(),      0)
    EXPECT_EQ("default waitingTime is 0",     c1.getWaitingTime(),      0)
    EXPECT_EQ("default transactionTime is 0", c1.getTransactionTime(),  0)

    TEST_SUITE("customerType — parameterized constructor")

    customerType c2(1, 5, 2, 10);
    EXPECT_EQ("customerNumber set correctly",   c2.getCustomerNumber(),  1)
    EXPECT_EQ("arrivalTime set correctly",      c2.getArrivalTime(),     5)
    EXPECT_EQ("waitingTime set correctly",      c2.getWaitingTime(),     2)
    EXPECT_EQ("transactionTime set correctly",  c2.getTransactionTime(), 10)

    TEST_SUITE("customerType — setCustomerInfo()")

    customerType c3;
    c3.setCustomerInfo(3, 8, 1, 4);
    EXPECT_EQ("customerNumber updated",   c3.getCustomerNumber(),  3)
    EXPECT_EQ("arrivalTime updated",      c3.getArrivalTime(),     8)
    EXPECT_EQ("waitingTime updated",      c3.getWaitingTime(),     1)
    EXPECT_EQ("transactionTime updated",  c3.getTransactionTime(), 4)

    TEST_SUITE("customerType — setWaitingTime()")

    customerType c4;
    c4.setWaitingTime(7);
    EXPECT_EQ("waitingTime set to 7", c4.getWaitingTime(), 7)

    TEST_SUITE("customerType — incrementWaitingTime()")

    customerType c5;
    c5.setWaitingTime(3);
    c5.incrementWaitingTime();
    EXPECT_EQ("waitingTime increments from 3 to 4", c5.getWaitingTime(), 4)

    c5.incrementWaitingTime();
    c5.incrementWaitingTime();
    EXPECT_EQ("waitingTime increments correctly after multiple calls", c5.getWaitingTime(), 6)
}

#endif