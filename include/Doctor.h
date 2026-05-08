#ifndef DOCTOR_H
#define DOCTOR_H

#include "Person.h"
#include <iostream>

class Doctor : public Person {
private:
    char specialization[51];
    char contact[20];
    double fee;
public:
    Doctor();
    Doctor(int dId, const char* dName, const char* dSpec, const char* dContact, const char* dPassword, double dFee);

    const char* getSpecialization() const;
    const char* getContact() const;
    double getFee() const;

    void setSpecialization(const char* value);
    void setContact(const char* value);
    void setFee(double value);

    bool operator==(const Doctor& other) const;

    const char* getRole() const;
    void display() const;

    friend std::ostream& operator<<(std::ostream& os, const Doctor& doctor);
};

#endif
