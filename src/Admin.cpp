#include "Admin.h"
#include <iostream>

namespace sf {}
using namespace std;
using namespace sf;


Admin::Admin() : Person() {}
Admin::Admin(int aId, const char* aName, const char* aPassword) : Person(aId, aName, aPassword) {}

const char* Admin::getRole() const { 
    return "Admin"; 
}
void Admin::display() const { 
    std::cout << id << " | " << name << '\n';
}
