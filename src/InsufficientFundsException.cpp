#include "InsufficientFundsException.h"

namespace sf {}
using namespace std;
using namespace sf;


InsufficientFundsException::InsufficientFundsException(const char* msg) : HospitalException(msg) {}
