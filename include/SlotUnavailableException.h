#ifndef SLOTUNAVAILABLEEXCEPTION_H
#define SLOTUNAVAILABLEEXCEPTION_H

#include "HospitalException.h"

class SlotUnavailableException : public HospitalException {
public:
    explicit SlotUnavailableException(const char* msg);
};

#endif
