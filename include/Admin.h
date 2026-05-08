#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"

class Admin : public Person {
public:
    Admin();
    Admin(int aId, const char* aName, const char* aPassword);
    const char* getRole() const;
    void display() const;
};

#endif
