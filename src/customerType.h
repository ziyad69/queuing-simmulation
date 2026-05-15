#ifndef CUSTOMER_TYPE_H
#define CUSTOMER_TYPE_H

class customerType {
private:
    int customerNumber;
    int arrivalTime;
    int waitingTime;
    int transactionTime;

public:
    customerType(int = 0, int = 0, int = 0, int = 0);

    void setCustomerInfo(int = 0, int = 0, int = 0, int = 0);

    int  getWaitingTime() const;
    void setWaitingTime(int);
    void incrementWaitingTime();

    int  getArrivalTime() const;
    int  getTransactionTime() const;
    int  getCustomerNumber() const;
};

#endif
