#include "Appointment.h"
#include "Validator.h"

namespace sf {}
using namespace std;
using namespace sf;


Appointment::Appointment() : appointmentId(0), patientId(0), doctorId(0) {
    date[0] = '\0';
    timeSlot[0] = '\0';
    status[0] = '\0';
}

Appointment::Appointment(int aId, int pId, int dId, const char* aDate, const char* aTimeSlot, const char* aStatus)
    : appointmentId(aId), patientId(pId), doctorId(dId) {
    Validator::strCopy(date, aDate, 11);
    Validator::strCopy(timeSlot, aTimeSlot, 6);
    Validator::strCopy(status, aStatus, 20);
}

int Appointment::getAppointmentId() const { 
    return appointmentId; 
}
int Appointment::getPatientId() const { 
    return patientId; 
}
int Appointment::getDoctorId() const { 
    return doctorId; 
}
const char* Appointment::getDate() const { 
    return date; 
}
const char* Appointment::getTimeSlot() const { 
    return timeSlot; 
}
const char* Appointment::getStatus() const { 
    return status; 
}

void Appointment::setStatus(const char* value) { 
    Validator::strCopy(status, value, 20); 
}

bool Appointment::operator==(const Appointment& other) const {
    bool neitherCancelled = !Validator::strEqual(status, "cancelled") && !Validator::strEqual(other.status, "cancelled");
    return doctorId == other.doctorId
        && Validator::strEqual(date, other.date)
        && Validator::strEqual(timeSlot, other.timeSlot)
        && neitherCancelled;
}

std::ostream& operator<<(std::ostream& os, const Appointment& appointment) {
    os << appointment.appointmentId << " | " << appointment.patientId << " | " << appointment.doctorId
       << " | " << appointment.date << " | " << appointment.timeSlot << " | " << appointment.status;
    return os;
}
