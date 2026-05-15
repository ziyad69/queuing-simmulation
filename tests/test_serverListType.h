#ifndef TEST_SERVER_LIST_TYPE_H
#define TEST_SERVER_LIST_TYPE_H

#include "../tests/testRunner.h"
#include "../src/serverListType.h"
#include "../src/customerType.h"
#include <sstream>

void test_serverListType() {
    TEST_SUITE("serverListType — constructor")

    serverListType list1(3);
    EXPECT_EQ("all 3 servers start free — no busy servers",
        list1.getNumberOfBusyServers(), 0)

    EXPECT_EQ("getFreeServerID returns 0 when all free",
        list1.getFreeServerID(), 0)

    TEST_SUITE("serverListType — setServerBusy(id, customer)")

    serverListType list2(3);
    customerType c1(1, 1, 0, 4);
    list2.setServerBusy(0, c1);

    EXPECT_EQ("1 server busy after setServerBusy",
        list2.getNumberOfBusyServers(), 1)

    EXPECT_EQ("getFreeServerID skips busy server 0, returns 1",
        list2.getFreeServerID(), 1)

    TEST_SUITE("serverListType — setServerBusy(id, customer, transactionTime)")

    serverListType list3(2);
    customerType c2(2, 1, 0, 3);
    list3.setServerBusy(0, c2, 10);   // override transaction time to 10

    EXPECT_EQ("1 server busy",
        list3.getNumberOfBusyServers(), 1)

    TEST_SUITE("serverListType — all servers busy")

    serverListType list4(2);
    customerType ca(1, 1, 0, 5);
    customerType cb(2, 2, 0, 5);
    list4.setServerBusy(0, ca);
    list4.setServerBusy(1, cb);

    EXPECT_EQ("both servers busy",
        list4.getNumberOfBusyServers(), 2)

    EXPECT_EQ("getFreeServerID returns -1 when all busy",
        list4.getFreeServerID(), -1)

    TEST_SUITE("serverListType — updateServers()")

    serverListType list5(1);
    customerType c3(3, 1, 0, 2);   // transaction time = 2
    list5.setServerBusy(0, c3);

    std::ostringstream log;

    list5.updateServers(log);   // tick 1 — remaining = 1, still busy
    EXPECT_EQ("server still busy after 1 tick",
        list5.getNumberOfBusyServers(), 1)

    list5.updateServers(log);   // tick 2 — remaining = 0, server freed
    EXPECT_EQ("server free after transaction time expires",
        list5.getNumberOfBusyServers(), 0)

    EXPECT_EQ("getFreeServerID returns 0 after server freed",
        list5.getFreeServerID(), 0)
}

#endif