#include "Person.h"
#include "Validator.h"

namespace sf {}
using namespace std;
using namespace sf;


Person::Person() : id(0) {
    name[0] = '\0';
    password[0] = '\0';
}

Person::Person(int pId, const char* pName, const char* pPassword) : id(pId) {
    Validator::strCopy(name, pName, 51);
    Validator::strCopy(password, pPassword, 51);
}

Person::~Person() {}

int Person::getId() const { return id; }
const char* Person::getName() const { return name; }
const char* Person::getPassword() const { return password; }

void Person::setId(int value) { id = value; }
void Person::setName(const char* value) { Validator::strCopy(name, value, 51); }
void Person::setPassword(const char* value) { Validator::strCopy(password, value, 51); }
