#include "Validator.h"
#include <cctype>
#include <cstdio>
#include <cstdlib>

namespace sf {}
using namespace std;
using namespace sf;


int Validator::strLength(const char* text) {
    if (!text) return 0;
    int len = 0;
    while (text[len] != '\0') ++len;
    return len;
}

void Validator::strCopy(char* dest, const char* src, int maxLen) {
    if (!dest || maxLen <= 0) return;
    if (!src) {
        dest[0] = '\0';
        return;
    }
    int i = 0;
    for (; i < maxLen - 1 && src[i] != '\0'; ++i) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

bool Validator::strEqual(const char* a, const char* b) {
    if (!a || !b) return false;
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return false;
        ++i;
    }
    return a[i] == '\0' && b[i] == '\0';
}

bool Validator::strEqualIgnoreCase(const char* a, const char* b) {
    if (!a || !b) return false;
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (std::tolower(static_cast<unsigned char>(a[i])) != std::tolower(static_cast<unsigned char>(b[i]))) {
            return false;
        }
        ++i;
    }
    return a[i] == '\0' && b[i] == '\0';
}

bool Validator::startsWith(const char* a, const char* prefix) {
    if (!a || !prefix) return false;
    int i = 0;
    while (prefix[i] != '\0') {
        if (a[i] != prefix[i]) return false;
        ++i;
    }
    return true;
}

bool Validator::isPositiveFloat(const char* text) {
    if (!text || text[0] == '\0') return false;
    bool dotSeen = false;
    bool digitSeen = false;
    for (int i = 0; text[i] != '\0'; ++i) {
        if (text[i] == '.') {
            if (dotSeen) return false;
            dotSeen = true;
        } else if (text[i] >= '0' && text[i] <= '9') {
            digitSeen = true;
        } else {
            return false;
        }
    }
    if (!digitSeen) return false;
    return std::atof(text) > 0.0;
}

bool Validator::isPositiveInt(const char* text) {
    if (!text || text[0] == '\0') return false;
    for (int i = 0; text[i] != '\0'; ++i) {
        if (text[i] < '0' || text[i] > '9') return false;
    }
    return std::atoi(text) > 0;
}

bool Validator::isValidId(int id) {
    return id > 0;
}

bool Validator::isValidDate(const char* date) {
    if (!date || strLength(date) != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (date[i] < '0' || date[i] > '9') return false;
    }

    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    if (day < 1 || day > 31 || month < 1 || month > 12) return false;

    std::time_t now = std::time(0);
    std::tm* current = std::localtime(&now);
    int currentYear = current ? current->tm_year + 1900 : 1900;
    return year >= currentYear;
}

bool Validator::isValidTimeSlot(const char* slot) {
    static const char valid[8][6] = {"09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"};
    if (!slot) return false;
    for (int i = 0; i < 8; ++i) {
        if (strEqual(slot, valid[i])) return true;
    }
    return false;
}

bool Validator::isValidContact(const char* contact) {
    if (!contact || strLength(contact) != 11) return false;
    for (int i = 0; i < 11; ++i) {
        if (contact[i] < '0' || contact[i] > '9') return false;
    }
    return true;
}

bool Validator::isValidPassword(const char* password) {
    return password && strLength(password) >= 6;
}

bool Validator::isValidMenuChoice(int choice, int min, int max) {
    return choice >= min && choice <= max;
}

int Validator::toInt(const char* text) {
    if (!text) return 0;
    return std::atoi(text);
}

double Validator::toDouble(const char* text) {
    if (!text) return 0.0;
    return std::atof(text);
}

void Validator::formatDouble(double value, char* out, int outLen) {
    if (!out || outLen <= 0) return;
    std::snprintf(out, outLen, "%.2f", value);
}

void Validator::getTodayDate(char* outDate) {
    if (!outDate) return;
    std::time_t now = std::time(0);
    std::tm* t = std::localtime(&now);
    if (!t) {
        outDate[0] = '\0';
        return;
    }
    std::strftime(outDate, 11, "%d-%m-%Y", t);
}

void Validator::getCurrentDateTime(char* outDateTime) {
    if (!outDateTime) return;
    std::time_t now = std::time(0);
    std::tm* t = std::localtime(&now);
    if (!t) {
        outDateTime[0] = '\0';
        return;
    }
    std::strftime(outDateTime, 20, "%d-%m-%Y %H:%M:%S", t);
}

int Validator::dateToNumber(const char* date) {
    if (!isValidDate(date)) return 0;
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');
    return year * 10000 + month * 100 + day;
}

int Validator::compareDate(const char* a, const char* b) {
    int na = dateToNumber(a);
    int nb = dateToNumber(b);
    if (na < nb) return -1;
    if (na > nb) return 1;
    return 0;
}

time_t Validator::parseDate(const char* date) {
    if (!isValidDate(date)) return 0;
    std::tm t = {};
    t.tm_mday = (date[0] - '0') * 10 + (date[1] - '0');
    t.tm_mon = ((date[3] - '0') * 10 + (date[4] - '0')) - 1;
    t.tm_year = ((date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0')) - 1900;
    t.tm_isdst = -1;
    return std::mktime(&t);
}
