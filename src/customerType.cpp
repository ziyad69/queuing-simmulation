#include "customerType.h"

customerType::customerType(int cn, int at, int wt, int tt)
    : customerNumber(cn), arrivalTime(at), waitingTime(wt), transactionTime(tt) {}

void customerType::setCustomerInfo(int cn, int at, int wt, int tt) {
    customerNumber  = cn;
    arrivalTime     = at;
    waitingTime     = wt;
    transactionTime = tt;
}

int  customerType::getWaitingTime()    const { return waitingTime; }
void customerType::setWaitingTime(int t)     { waitingTime = t; }
void customerType::incrementWaitingTime()    { waitingTime++; }

int  customerType::getArrivalTime()      const { return arrivalTime; }
int  customerType::getTransactionTime()  const { return transactionTime; }
int  customerType::getCustomerNumber()   const { return customerNumber; }
