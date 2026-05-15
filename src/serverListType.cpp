#include "serverListType.h"
#include <iostream>

serverListType::serverListType(int n)
    : numOfServers(n), servers(new serverType[n]) {}

serverListType::~serverListType() {
    delete[] servers;
}

int serverListType::getFreeServerID() const {
    for (int i = 0; i < numOfServers; i++)
        if (servers[i].isFree()) return i;
    return -1;
}

int serverListType::getNumberOfBusyServers() const {
    int count = 0;
    for (int i = 0; i < numOfServers; i++)
        if (!servers[i].isFree()) count++;
    return count;
}

void serverListType::setServerBusy(int id, customerType customer, int transactionTime) {
    servers[id].setCurrentCustomer(customer);
    servers[id].setTransactionTime(transactionTime);
    servers[id].setBusy();
}

void serverListType::setServerBusy(int id, customerType customer) {
    servers[id].setCurrentCustomer(customer);
    servers[id].setTransactionTime();   // uses customer's own transactionTime
    servers[id].setBusy();
}

void serverListType::updateServers(std::ostream& out) {
    for (int i = 0; i < numOfServers; i++) {
        if (!servers[i].isFree()) {
            servers[i].decreaseTransactionTime();

            if (servers[i].getRemainingTransactionTime() == 0) {
                out << "  Server " << (i + 1)
                    << " -> Customer " << servers[i].getCurrentCustomerNumber()
                    << " departed"
                    << " | Arrival: "   << servers[i].getCurrentCustomerArrivalTime()
                    << " | Wait: "      << servers[i].getCurrentCustomerWaitingTime()
                    << " | Service: "   << servers[i].getCurrentCustomerTransactionTime()
                    << "\n";
                servers[i].setFree();
            }
        }
    }
}
