#ifndef SERVER_TYPE_H
#define SERVER_TYPE_H

#include <string>
#include "customerType.h"

class serverType {
private:
    customerType currentCustomer;
    std::string  status;          // "free" or "busy"
    int          transactionTime;

public:
    serverType();

    bool isFree() const;
    void setBusy();
    void setFree();

    void setTransactionTime(int);
    void setTransactionTime();           // sets from currentCustomer

    int  getRemainingTransactionTime() const;
    void decreaseTransactionTime();

    void setCurrentCustomer(customerType);
    int  getCurrentCustomerNumber()         const;
    int  getCurrentCustomerArrivalTime()    const;
    int  getCurrentCustomerWaitingTime()    const;
    int  getCurrentCustomerTransactionTime()const;
};

#endif
