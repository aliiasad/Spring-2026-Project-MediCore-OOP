#ifndef INSUFFICIENTFUNDSEXCEPTION_H
#define INSUFFICIENTFUNDSEXCEPTION_H

#include "HospitalException.h"

class InsufficientFundsException : public HospitalException {
public:
    explicit InsufficientFundsException(const char* msg);
};

#endif
