#include "InvalidInputException.h"

namespace sf {}
using namespace std;
using namespace sf;


InvalidInputException::InvalidInputException(const char* msg) : HospitalException(msg) {}
