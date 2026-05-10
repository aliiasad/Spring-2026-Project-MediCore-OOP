#ifndef HOSPITALSYSTEM_H
#define HOSPITALSYSTEM_H

#include "Storage.h"
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "FileHandler.h"

class HospitalSystem {
private:
    Storage<Patient> patients;
    Storage<Doctor> doctors;
    Storage<Admin> admins;
    Storage<Appointment> appointments;
    Storage<Bill> bills;
    Storage<Prescription> prescriptions;
    FileHandler fileHandler;

    int loginFlow(const char* role);
    void patientMenu(int patientId);
    void doctorMenu(int doctorId);
    void adminMenu(int adminId);

    void patientBookAppointment(Patient& patient);
    void patientCancelAppointment(Patient& patient);
    void patientViewAppointments(const Patient& patient) const;
    void patientViewMedicalRecords(const Patient& patient) const;
    void patientViewBills(const Patient& patient) const;
    void patientPayBill(Patient& patient);
    void patientTopUp(Patient& patient);

    void doctorViewTodayAppointments(const Doctor& doctor) const;
    void doctorMarkAppointmentComplete(const Doctor& doctor);
    void doctorMarkAppointmentNoShow(const Doctor& doctor);
    void doctorWritePrescription(const Doctor& doctor);
    void doctorViewPatientHistory(const Doctor& doctor) const;

    void adminAddDoctor();
    void adminRemoveDoctor();
    void adminViewAllPatients() const;
    void adminViewAllDoctors() const;
    void adminViewAllAppointments() const;
    void adminViewUnpaidBills() const;
    void adminDischargePatient();
    void adminViewSecurityLog() const;
    void adminGenerateDailyReport() const;

    int findDoctorFee(int doctorId) const;
    const char* findDoctorName(int doctorId) const;
    const char* findDoctorSpec(int doctorId) const;
    const char* findPatientName(int patientId) const;

    bool hasPendingAppointmentsForDoctor(int doctorId) const;
    bool hasPendingAppointmentsForPatient(int patientId) const;
    bool hasUnpaidBillsForPatient(int patientId) const;

    void sortAppointmentsByDate(Appointment* list, int count, bool asc) const;
    void sortPrescriptionsByDate(Prescription* list, int count, bool desc) const;

public:
    HospitalSystem();

    bool initialize();
    bool loadData();

    void run();
    void runConsole();

    // Called by SFMLUI: validate one login attempt (no retry loop)
    bool verifyLogin(const char* role, int id, const char* password);

    // Called by SFMLUI: run the interactive console menu for a logged-in user
    void runPatientMenu(int patientId);
    void runDoctorMenu(int doctorId);
    void runAdminMenu(int adminId);

    // SFML UI action methods
    bool bookAppointment(int patientId, int doctorId, const char* date,
                         const char* slot, char* outMsg);
    bool cancelAppointment(int patientId, int appointmentId, char* outMsg);
    bool payBill(int patientId, int billId, char* outMsg);
    bool topUpBalance(int patientId, double amount, char* outMsg);
    bool updateAppointmentStatus(int doctorId, int appointmentId,
                                 const char* status, char* outMsg);
    bool writePrescription(int doctorId, int appointmentId,
                           const char* medicines, const char* notes,
                           char* outMsg);
    bool addDoctor(const char* name, const char* spec, const char* contact,
                   const char* password, double fee, char* outMsg);
    bool addPatient(const char* name, int age, const char* gender,
                    const char* contact, const char* password,
                    double initialBalance, char* outMsg);
    bool removeDoctor(int doctorId, char* outMsg);
    bool dischargePatient(int patientId, char* outMsg);

    // SFML UI query methods
    void getPatientAppointments(int patientId,
                                Appointment* outArr, int& outCount) const;
    void getPatientBills(int patientId,
                         Bill* outArr, int& outCount) const;
    void getPatientPrescriptions(int patientId,
                                 Prescription* outArr, int& outCount) const;
    void getDoctorTodayAppointments(int doctorId,
                                    Appointment* outArr, int& outCount) const;
    void getDoctorPatientHistory(int doctorId, int patientId,
                                 Prescription* outArr, int& outCount) const;
    void getAllPatients(Patient* outArr, int& outCount) const;
    void getAllDoctors(Doctor* outArr, int& outCount) const;
    void getAllAppointments(Appointment* outArr, int& outCount) const;
    void getUnpaidBills(Bill* outArr, int& outCount) const;
    void getSecurityLog(char lines[][256], int maxLines, int& outCount) const;
    double getTotalRevenue() const;

    const char* getPatientName(int patientId) const;
    const char* getDoctorName(int doctorId)   const;
    double getPatientBalance(int patientId) const;
};

#endif
