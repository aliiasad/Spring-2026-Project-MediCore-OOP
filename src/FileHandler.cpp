#include "FileHandler.h"
#include "Validator.h"
#include <cstdio>

namespace sf {}
using namespace std;
using namespace sf;


FileHandler::FileHandler() {
    Validator::strCopy(basePath, "data", 200);
}

FileHandler::FileHandler(const char* dataFolder) {
    Validator::strCopy(basePath, dataFolder ? dataFolder : "data", 200);
}

void FileHandler::safeFieldCopy(char* dest, const char* src, int maxLen) const {
    Validator::strCopy(dest, src, maxLen);
}

bool FileHandler::getFilePath(const char* fileName, char* outPath, int outLen) const {
    if (!fileName || !outPath || outLen <= 0) return false;
    int bl = Validator::strLength(basePath);
    int fl = Validator::strLength(fileName);
    if (bl + fl + 2 > outLen) return false;
    int idx = 0;
    for (int i = 0; i < bl; ++i) outPath[idx++] = basePath[i];
    outPath[idx++] = '/';
    for (int i = 0; i < fl; ++i) outPath[idx++] = fileName[i];
    outPath[idx] = '\0';
    return true;
}

bool FileHandler::parseCSV(const char* line, char fields[][600], int maxFields, int& outCount) const {
    outCount = 0;
    if (!line || maxFields <= 0) return false;
    int i = 0;
    int fieldIdx = 0;
    int charIdx = 0;
    while (line[i] != '\0' && line[i] != '\n' && line[i] != '\r') {
        if (line[i] == ',') {
            fields[fieldIdx][charIdx] = '\0';
            ++fieldIdx;
            charIdx = 0;
            if (fieldIdx >= maxFields) break;
        } else {
            if (charIdx < 599) {
                fields[fieldIdx][charIdx++] = line[i];
            }
        }
        ++i;
    }
    if (fieldIdx < maxFields) {
        fields[fieldIdx][charIdx] = '\0';
        ++fieldIdx;
    }
    outCount = fieldIdx;
    return true;
}

bool FileHandler::ensureDataFilesExist() {
    const char* files[8] = {
        "patients.txt", "doctors.txt", "admin.txt", "appointments.txt",
        "prescriptions.txt", "bills.txt", "security_log.txt", "discharged.txt"
    };
    for (int i = 0; i < 8; ++i) {
        char path[300];
        if (!getFilePath(files[i], path, 300)) return false;
        FILE* f = std::fopen(path, "a+");
        if (!f) return false;
        std::fclose(f);
    }
    return true;
}

bool FileHandler::loadPatients(Storage<Patient>& patients) const {
    patients.clear();
    char path[300];
    if (!getFilePath("patients.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "r");
    if (!f) return false;

    char line[1024];
    while (std::fgets(line, sizeof(line), f)) {
        char fields[10][600];
        int fc = 0;
        parseCSV(line, fields, 10, fc);
        if (fc < 7 || !Validator::isPositiveInt(fields[0])) continue;
        Patient p(Validator::toInt(fields[0]), fields[1], Validator::toInt(fields[2]), fields[3], fields[4], fields[5], Validator::toDouble(fields[6]));
        patients.add(p);
    }
    std::fclose(f);
    return true;
}

bool FileHandler::loadDoctors(Storage<Doctor>& doctors) const {
    doctors.clear();
    char path[300];
    if (!getFilePath("doctors.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "r");
    if (!f) return false;

    char line[1024];
    while (std::fgets(line, sizeof(line), f)) {
        char fields[10][600];
        int fc = 0;
        parseCSV(line, fields, 10, fc);
        if (fc < 6 || !Validator::isPositiveInt(fields[0])) continue;
        Doctor d(Validator::toInt(fields[0]), fields[1], fields[2], fields[3], fields[4], Validator::toDouble(fields[5]));
        doctors.add(d);
    }
    std::fclose(f);
    return true;
}

bool FileHandler::loadAdmins(Storage<Admin>& admins) const {
    admins.clear();
    char path[300];
    if (!getFilePath("admin.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "r");
    if (!f) return false;

    char line[1024];
    while (std::fgets(line, sizeof(line), f)) {
        char fields[6][600];
        int fc = 0;
        parseCSV(line, fields, 6, fc);
        if (fc < 3 || !Validator::isPositiveInt(fields[0])) continue;
        Admin a(Validator::toInt(fields[0]), fields[1], fields[2]);
        admins.add(a);
    }
    std::fclose(f);
    return true;
}

bool FileHandler::loadAppointments(Storage<Appointment>& appointments) const {
    appointments.clear();
    char path[300];
    if (!getFilePath("appointments.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "r");
    if (!f) return false;

    char line[1024];
    while (std::fgets(line, sizeof(line), f)) {
        char fields[10][600];
        int fc = 0;
        parseCSV(line, fields, 10, fc);
        if (fc < 6 || !Validator::isPositiveInt(fields[0])) continue;
        Appointment a(Validator::toInt(fields[0]), Validator::toInt(fields[1]), Validator::toInt(fields[2]), fields[3], fields[4], fields[5]);
        appointments.add(a);
    }
    std::fclose(f);
    return true;
}

bool FileHandler::loadPrescriptions(Storage<Prescription>& prescriptions) const {
    prescriptions.clear();
    char path[300];
    if (!getFilePath("prescriptions.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "r");
    if (!f) return false;

    char line[1200];
    while (std::fgets(line, sizeof(line), f)) {
        char fields[10][600];
        int fc = 0;
        parseCSV(line, fields, 10, fc);
        if (fc < 7 || !Validator::isPositiveInt(fields[0])) continue;
        Prescription p(Validator::toInt(fields[0]), Validator::toInt(fields[1]), Validator::toInt(fields[2]), Validator::toInt(fields[3]), fields[4], fields[5], fields[6]);
        prescriptions.add(p);
    }
    std::fclose(f);
    return true;
}

bool FileHandler::loadBills(Storage<Bill>& bills) const {
    bills.clear();
    char path[300];
    if (!getFilePath("bills.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "r");
    if (!f) return false;

    char line[1024];
    while (std::fgets(line, sizeof(line), f)) {
        char fields[10][600];
        int fc = 0;
        parseCSV(line, fields, 10, fc);
        if (fc < 6 || !Validator::isPositiveInt(fields[0])) continue;
        Bill b(Validator::toInt(fields[0]), Validator::toInt(fields[1]), Validator::toInt(fields[2]), Validator::toDouble(fields[3]), fields[4], fields[5]);
        bills.add(b);
    }
    std::fclose(f);
    return true;
}

bool FileHandler::savePatients(const Storage<Patient>& patients) const {
    char path[300];
    if (!getFilePath("patients.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "w");
    if (!f) return false;
    for (int i = 0; i < patients.size(); ++i) {
        const Patient& p = patients.getAll()[i];
        std::fprintf(f, "%d,%s,%d,%s,%s,%s,%.2f\n", p.getId(), p.getName(), p.getAge(), p.getGender(), p.getContact(), p.getPassword(), p.getBalance());
    }
    std::fclose(f);
    return true;
}

bool FileHandler::saveDoctors(const Storage<Doctor>& doctors) const {
    char path[300];
    if (!getFilePath("doctors.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "w");
    if (!f) return false;
    for (int i = 0; i < doctors.size(); ++i) {
        const Doctor& d = doctors.getAll()[i];
        std::fprintf(f, "%d,%s,%s,%s,%s,%.2f\n", d.getId(), d.getName(), d.getSpecialization(), d.getContact(), d.getPassword(), d.getFee());
    }
    std::fclose(f);
    return true;
}

bool FileHandler::saveAppointments(const Storage<Appointment>& appointments) const {
    char path[300];
    if (!getFilePath("appointments.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "w");
    if (!f) return false;
    for (int i = 0; i < appointments.size(); ++i) {
        const Appointment& a = appointments.getAll()[i];
        std::fprintf(f, "%d,%d,%d,%s,%s,%s\n", a.getAppointmentId(), a.getPatientId(), a.getDoctorId(), a.getDate(), a.getTimeSlot(), a.getStatus());
    }
    std::fclose(f);
    return true;
}

bool FileHandler::savePrescriptions(const Storage<Prescription>& prescriptions) const {
    char path[300];
    if (!getFilePath("prescriptions.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "w");
    if (!f) return false;
    for (int i = 0; i < prescriptions.size(); ++i) {
        const Prescription& p = prescriptions.getAll()[i];
        std::fprintf(f, "%d,%d,%d,%d,%s,%s,%s\n", p.getPrescriptionId(), p.getAppointmentId(), p.getPatientId(), p.getDoctorId(), p.getDate(), p.getMedicines(), p.getNotes());
    }
    std::fclose(f);
    return true;
}

bool FileHandler::saveBills(const Storage<Bill>& bills) const {
    char path[300];
    if (!getFilePath("bills.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "w");
    if (!f) return false;
    for (int i = 0; i < bills.size(); ++i) {
        const Bill& b = bills.getAll()[i];
        std::fprintf(f, "%d,%d,%d,%.2f,%s,%s\n", b.getBillId(), b.getPatientId(), b.getAppointmentId(), b.getAmount(), b.getStatus(), b.getDate());
    }
    std::fclose(f);
    return true;
}

bool FileHandler::appendSecurityLog(const char* role, int enteredId, const char* result) const {
    char path[300];
    if (!getFilePath("security_log.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "a");
    if (!f) return false;
    char dt[20];
    Validator::getCurrentDateTime(dt);
    std::fprintf(f, "%s,%s,%d,%s\n", dt, role, enteredId, result);
    std::fclose(f);
    return true;
}

bool FileHandler::readSecurityLog(char lines[][256], int maxLines, int& outCount) const {
    outCount = 0;
    char path[300];
    if (!getFilePath("security_log.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "r");
    if (!f) return false;
    char line[300];
    while (outCount < maxLines && std::fgets(line, sizeof(line), f)) {
        Validator::strCopy(lines[outCount], line, 256);
        ++outCount;
    }
    std::fclose(f);
    return true;
}

bool FileHandler::appendDischargedLine(const char* line) const {
    char path[300];
    if (!getFilePath("discharged.txt", path, 300)) return false;
    FILE* f = std::fopen(path, "a");
    if (!f) return false;
    std::fprintf(f, "%s\n", line);
    std::fclose(f);
    return true;
}

int FileHandler::nextAppointmentId() const {
    Storage<Appointment> tmp;
    if (!loadAppointments(tmp)) return 1;
    int mx = 0;
    for (int i = 0; i < tmp.size(); ++i) if (tmp.getAll()[i].getAppointmentId() > mx) mx = tmp.getAll()[i].getAppointmentId();
    return mx + 1;
}

int FileHandler::nextBillId() const {
    Storage<Bill> tmp;
    if (!loadBills(tmp)) return 1;
    int mx = 0;
    for (int i = 0; i < tmp.size(); ++i) if (tmp.getAll()[i].getBillId() > mx) mx = tmp.getAll()[i].getBillId();
    return mx + 1;
}

int FileHandler::nextPrescriptionId() const {
    Storage<Prescription> tmp;
    if (!loadPrescriptions(tmp)) return 1;
    int mx = 0;
    for (int i = 0; i < tmp.size(); ++i) if (tmp.getAll()[i].getPrescriptionId() > mx) mx = tmp.getAll()[i].getPrescriptionId();
    return mx + 1;
}

int FileHandler::nextDoctorId() const {
    Storage<Doctor> tmp;
    if (!loadDoctors(tmp)) return 1;
    int mx = 0;
    for (int i = 0; i < tmp.size(); ++i) if (tmp.getAll()[i].getId() > mx) mx = tmp.getAll()[i].getId();
    return mx + 1;
}
