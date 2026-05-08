#include "HospitalException.h"
#include "Validator.h"

namespace sf {}
using namespace std;
using namespace sf;


HospitalException::HospitalException() {
    Validator::strCopy(message, "Hospital exception", 200);
}

HospitalException::HospitalException(const char* msg) {
    Validator::strCopy(message, msg ? msg : "Hospital exception", 200);
}

const char* HospitalException::what() const {
    return message;
}
