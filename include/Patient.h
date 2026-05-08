#ifndef PATIENT_H
#define PATIENT_H

#include "Person.h"
#include <iostream>

class Patient : public Person {
private:
    int age;
    char gender[10];
    char contact[20];
    double balance;
public:
    Patient();
    Patient(int pId, const char* pName, int pAge, const char* pGender, const char* pContact, const char* pPassword, double pBalance);

    int getAge() const;
    const char* getGender() const;
    const char* getContact() const;
    double getBalance() const;

    void setAge(int value);
    void setGender(const char* value);
    void setContact(const char* value);
    void setBalance(double value);

    Patient& operator+=(double amount);
    Patient& operator-=(double amount);
    bool operator==(const Patient& other) const;

    const char* getRole() const;
    void display() const;

    friend std::ostream& operator<<(std::ostream& os, const Patient& patient);
};

#endif
