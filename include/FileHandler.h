#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"

class FileHandler {
private:
    char basePath[200];

    bool parseCSV(const char* line, char fields[][600], int maxFields, int& outCount) const;
    void safeFieldCopy(char* dest, const char* src, int maxLen) const;

public:
    FileHandler();
    explicit FileHandler(const char* dataFolder);

    bool ensureDataFilesExist();

    bool loadPatients(Storage<Patient>& patients) const;
    bool loadDoctors(Storage<Doctor>& doctors) const;
    bool loadAdmins(Storage<Admin>& admins) const;
    bool loadAppointments(Storage<Appointment>& appointments) const;
    bool loadPrescriptions(Storage<Prescription>& prescriptions) const;
    bool loadBills(Storage<Bill>& bills) const;

    bool savePatients(const Storage<Patient>& patients) const;
    bool saveDoctors(const Storage<Doctor>& doctors) const;
    bool saveAppointments(const Storage<Appointment>& appointments) const;
    bool savePrescriptions(const Storage<Prescription>& prescriptions) const;
    bool saveBills(const Storage<Bill>& bills) const;

    bool appendSecurityLog(const char* role, int enteredId, const char* result) const;
    bool readSecurityLog(char lines[][256], int maxLines, int& outCount) const;
    bool appendDischargedLine(const char* line) const;

    int nextAppointmentId() const;
    int nextBillId() const;
    int nextPrescriptionId() const;
    int nextDoctorId() const;

    bool getFilePath(const char* fileName, char* outPath, int outLen) const;
};

#endif
