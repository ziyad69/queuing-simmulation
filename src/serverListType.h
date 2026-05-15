#ifndef SERVER_LIST_TYPE_H
#define SERVER_LIST_TYPE_H

#include <ostream>
#include "serverType.h"
#include "customerType.h"

class serverListType {
private:
    int         numOfServers;
    serverType* servers;          // dynamic array of serverType

public:
    explicit serverListType(int = 1);
    ~serverListType();

    // Returns index (0-based) of first free server, -1 if none
    int getFreeServerID() const;

    // Returns how many servers are currently busy
    int getNumberOfBusyServers() const;

    // Assign customer to server[id], with explicit transaction time
    void setServerBusy(int serverID, customerType customer, int transactionTime);

    // Assign customer to server[id], transaction time taken from customer
    void setServerBusy(int serverID, customerType customer);

    // Tick all busy servers one clock unit; print departures to out
    void updateServers(std::ostream& out);
};

#endif
