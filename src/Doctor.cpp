#include "Doctor.h"
#include "Validator.h"

namespace sf {}
using namespace std;
using namespace sf;


Doctor::Doctor() : Person(), fee(0.0) {
    specialization[0] = '\0';
    contact[0] = '\0';
}

Doctor::Doctor(int dId, const char* dName, const char* dSpec, const char* dContact, const char* dPassword, double dFee)
    : Person(dId, dName, dPassword), fee(dFee) {
    Validator::strCopy(specialization, dSpec, 51);
    Validator::strCopy(contact, dContact, 20);
}

const char* Doctor::getSpecialization() const { return specialization; }
const char* Doctor::getContact() const { return contact; }
double Doctor::getFee() const { return fee; }

void Doctor::setSpecialization(const char* value) { Validator::strCopy(specialization, value, 51); }
void Doctor::setContact(const char* value) { Validator::strCopy(contact, value, 20); }
void Doctor::setFee(double value) { fee = value; }

bool Doctor::operator==(const Doctor& other) const { return id == other.id; }

const char* Doctor::getRole() const { return "Doctor"; }
void Doctor::display() const { std::cout << *this << '\n'; }

std::ostream& operator<<(std::ostream& os, const Doctor& doctor) {
    char feeBuf[32];
    Validator::formatDouble(doctor.fee, feeBuf, 32);
    os << doctor.id << " | " << doctor.name << " | " << doctor.specialization << " | "
       << doctor.contact << " | " << feeBuf;
    return os;
}
