#include "Patient.h"
#include "Validator.h"

namespace sf {}
using namespace std;
using namespace sf;


Patient::Patient() : Person(), age(0), balance(0.0) {
    gender[0] = '\0';
    contact[0] = '\0';
}

Patient::Patient(int pId, const char* pName, int pAge, const char* pGender, const char* pContact, const char* pPassword, double pBalance)
    : Person(pId, pName, pPassword), age(pAge), balance(pBalance) {
    Validator::strCopy(gender, pGender, 10);
    Validator::strCopy(contact, pContact, 20);
}

int Patient::getAge() const { 
    return age; 
}
const char* Patient::getGender() const { 
    return gender; 
}
const char* Patient::getContact() const { 
    return contact; 
}
double Patient::getBalance() const { 
    return balance; 
}

void Patient::setAge(int value) { 
    age = value;
}
void Patient::setGender(const char* value) { 
    Validator::strCopy(gender, value, 10); 
}
void Patient::setContact(const char* value) { 
    Validator::strCopy(contact, value, 20); 
}
void Patient::setBalance(double value) { 
    balance = value; 
}

Patient& Patient::operator+=(double amount) {
    balance += amount;
    return *this;
}

Patient& Patient::operator-=(double amount) {
    balance -= amount;
    return *this;
}

bool Patient::operator==(const Patient& other) const {
    return id == other.id;
}

const char* Patient::getRole() const { 
    return "Patient"; 
}
void Patient::display() const { 
    std::cout << *this << '\n'; 
}

std::ostream& operator<<(std::ostream& os, const Patient& patient) {
    char b[32];
    Validator::formatDouble(patient.balance, b, 32);
    os << patient.id << " | " << patient.name << " | " << patient.age << " | " << patient.gender
       << " | " << patient.contact << " | " << b;
    return os;
}
