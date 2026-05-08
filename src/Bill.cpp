#include "Bill.h"
#include "Validator.h"

namespace sf {}
using namespace std;
using namespace sf;


Bill::Bill() : billId(0), patientId(0), appointmentId(0), amount(0.0) {
    status[0] = '\0';
    date[0] = '\0';
}

Bill::Bill(int bId, int pId, int aId, double bAmount, const char* bStatus, const char* bDate)
    : billId(bId), patientId(pId), appointmentId(aId), amount(bAmount) {
    Validator::strCopy(status, bStatus, 20);
    Validator::strCopy(date, bDate, 11);
}

int Bill::getBillId() const { return billId; }
int Bill::getPatientId() const { return patientId; }
int Bill::getAppointmentId() const { return appointmentId; }
double Bill::getAmount() const { return amount; }
const char* Bill::getStatus() const { return status; }
const char* Bill::getDate() const { return date; }

void Bill::setStatus(const char* value) { Validator::strCopy(status, value, 20); }
