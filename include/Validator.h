#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <ctime>

class Validator {
public:
    static int strLength(const char* text);
    static void strCopy(char* dest, const char* src, int maxLen);
    static bool strEqual(const char* a, const char* b);
    static bool strEqualIgnoreCase(const char* a, const char* b);
    static bool startsWith(const char* a, const char* prefix);

    static bool isPositiveFloat(const char* text);
    static bool isPositiveInt(const char* text);
    static bool isValidId(int id);
    static bool isValidDate(const char* date);
    static bool isValidTimeSlot(const char* slot);
    static bool isValidContact(const char* contact);
    static bool isValidPassword(const char* password);
    static bool isValidMenuChoice(int choice, int min, int max);

    static int toInt(const char* text);
    static double toDouble(const char* text);
    static void formatDouble(double value, char* out, int outLen);

    static void getTodayDate(char* outDate);
    static void getCurrentDateTime(char* outDateTime);
    static int dateToNumber(const char* date);
    static int compareDate(const char* a, const char* b);
    static time_t parseDate(const char* date);
};

#endif
