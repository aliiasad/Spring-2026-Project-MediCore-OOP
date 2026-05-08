#include "FileNotFoundException.h"

namespace sf {}
using namespace std;
using namespace sf;


FileNotFoundException::FileNotFoundException(const char* msg) : HospitalException(msg) {}
