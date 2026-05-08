#include "HospitalSystem.h"
#include "Validator.h"
#include "InsufficientFundsException.h"
#include "InvalidInputException.h"
#include "SlotUnavailableException.h"
#include <iostream>
#include <cstdio>

namespace sf {}
using namespace std;
using namespace sf;


HospitalSystem::HospitalSystem() : fileHandler("data") {}

bool HospitalSystem::initialize() {
    return fileHandler.ensureDataFilesExist();
}

bool HospitalSystem::loadData() {
    bool ok = true;
    ok = ok && fileHandler.loadPatients(patients);
    ok = ok && fileHandler.loadDoctors(doctors);
    ok = ok && fileHandler.loadAdmins(admins);
    ok = ok && fileHandler.loadAppointments(appointments);
    ok = ok && fileHandler.loadPrescriptions(prescriptions);
    ok = ok && fileHandler.loadBills(bills);
    return ok;
}

int HospitalSystem::loginFlow(const char* role) {
    int attempts = 0;
    while (attempts < 3) {
        int enteredId;
        char enteredPassword[80];
        std::cout << "Enter ID: ";
        std::cin >> enteredId;
        std::cin.ignore(1000, '\n');
        std::cout << "Enter password: ";
        std::cin.getline(enteredPassword, 80);

        if (Validator::strEqual(role, "Patient")) {
            Patient* p = patients.findById(enteredId);
            if (p && Validator::strEqual(p->getPassword(), enteredPassword)) {
                fileHandler.appendSecurityLog(role, enteredId, "SUCCESS");
                return enteredId;
            }
        } else if (Validator::strEqual(role, "Doctor")) {
            Doctor* d = doctors.findById(enteredId);
            if (d && Validator::strEqual(d->getPassword(), enteredPassword)) {
                fileHandler.appendSecurityLog(role, enteredId, "SUCCESS");
                return enteredId;
            }
        } else {
            Admin* a = admins.findById(enteredId);
            if (a && Validator::strEqual(a->getPassword(), enteredPassword)) {
                fileHandler.appendSecurityLog(role, enteredId, "SUCCESS");
                return enteredId;
            }
        }

        ++attempts;
        fileHandler.appendSecurityLog(role, enteredId, "FAILED");
        std::cout << "Invalid credentials. Attempts left: " << (3 - attempts) << "\n";
    }

    std::cout << "Account locked. Contact admin.\n";
    return -1;
}

void HospitalSystem::run() { runConsole(); }

void HospitalSystem::runConsole() {
    int choice;
    do {
        std::cout << "\nWelcome to MediCore Hospital Management System\n";
        std::cout << "===============================================\n";
        std::cout << "1. Patient\n2. Doctor\n3. Admin\n4. Exit\nChoice: ";
        std::cin >> choice;
        std::cin.ignore(1000, '\n');

        if (choice == 1) {
            int id = loginFlow("Patient");
            if (id != -1) patientMenu(id);
        } else if (choice == 2) {
            int id = loginFlow("Doctor");
            if (id != -1) doctorMenu(id);
        } else if (choice == 3) {
            int id = loginFlow("Admin");
            if (id != -1) adminMenu(id);
        }
    } while (choice != 4);
}

bool HospitalSystem::verifyLogin(const char* role, int id, const char* password) {
    bool ok = false;
    if (Validator::strEqual(role, "Patient")) {
        Patient* p = patients.findById(id);
        ok = (p && Validator::strEqual(p->getPassword(), password));
    } else if (Validator::strEqual(role, "Doctor")) {
        Doctor* d = doctors.findById(id);
        ok = (d && Validator::strEqual(d->getPassword(), password));
    } else {
        Admin* a = admins.findById(id);
        ok = (a && Validator::strEqual(a->getPassword(), password));
    }
    fileHandler.appendSecurityLog(role, id, ok ? "SUCCESS" : "FAILED");
    return ok;
}

void HospitalSystem::runPatientMenu(int patientId) { patientMenu(patientId); }
void HospitalSystem::runDoctorMenu(int doctorId)   { doctorMenu(doctorId);   }
void HospitalSystem::runAdminMenu(int adminId)     { adminMenu(adminId);     }

void HospitalSystem::patientMenu(int patientId) {
    Patient* p = patients.findById(patientId);
    if (!p) return;

    int choice;
    do {
        char bal[32];
        Validator::formatDouble(p->getBalance(), bal, 32);
        std::cout << "\nWelcome, " << p->getName() << "\nBalance: PKR " << bal << "\n";
        std::cout << "1.Book Appointment\n2.Cancel Appointment\n3.View My Appointments\n4.View My Medical Records\n5.View My Bills\n6.Pay Bill\n7.Top Up Balance\n8.Logout\nChoice: ";
        std::cin >> choice;
        std::cin.ignore(1000, '\n');
        if (choice == 1) patientBookAppointment(*p);
        else if (choice == 2) patientCancelAppointment(*p);
        else if (choice == 3) patientViewAppointments(*p);
        else if (choice == 4) patientViewMedicalRecords(*p);
        else if (choice == 5) patientViewBills(*p);
        else if (choice == 6) patientPayBill(*p);
        else if (choice == 7) patientTopUp(*p);
    } while (choice != 8);
}

void HospitalSystem::patientBookAppointment(Patient& patient) {
    char specialization[60];
    std::cout << "Enter specialization to search: ";
    std::cin.getline(specialization, 60);

    int mCount = 0;
    for (int i = 0; i < doctors.size(); ++i) {
        if (Validator::strEqualIgnoreCase(doctors.getAll()[i].getSpecialization(), specialization)) {
            ++mCount;
            std::cout << doctors.getAll()[i] << '\n';
        }
    }
    if (mCount == 0) {
        std::cout << "No doctors available for that specialization.\n";
        return;
    }

    int doctorId;
    std::cout << "Enter Doctor ID: ";
    std::cin >> doctorId;
    std::cin.ignore(1000, '\n');
    Doctor* doc = doctors.findById(doctorId);
    if (!doc) {
        std::cout << "Doctor not found.\n";
        return;
    }

    char date[20];
    int dateTry = 0;
    while (dateTry < 3) {
        std::cout << "Enter date (DD-MM-YYYY): ";
        std::cin.getline(date, 20);
        if (Validator::isValidDate(date)) break;
        std::cout << "Invalid date. Use format DD-MM-YYYY.\n";
        ++dateTry;
    }
    if (dateTry == 3 && !Validator::isValidDate(date)) return;

    std::cout << "Available slots: ";
    const char slots[8][6] = {"09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00"};
    for (int s = 0; s < 8; ++s) {
        bool busy = false;
        for (int i = 0; i < appointments.size(); ++i) {
            const Appointment& a = appointments.getAll()[i];
            if (a.getDoctorId() == doctorId && Validator::strEqual(a.getDate(), date)
                && Validator::strEqual(a.getTimeSlot(), slots[s])
                && !Validator::strEqual(a.getStatus(), "cancelled")) {
                busy = true;
                break;
            }
        }
        if (!busy) std::cout << slots[s] << ' ';
    }
    std::cout << '\n';

    char selectedSlot[10];
    while (true) {
        try {
            std::cout << "Enter time slot: ";
            std::cin.getline(selectedSlot, 10);
            if (!Validator::isValidTimeSlot(selectedSlot)) {
                throw SlotUnavailableException("Invalid time slot selected.");
            }

            bool taken = false;
            for (int i = 0; i < appointments.size(); ++i) {
                Appointment probe(0, patient.getId(), doctorId, date, selectedSlot, "pending");
                if (appointments.getAll()[i] == probe) { taken = true; break; }
            }
            if (taken) {
                throw SlotUnavailableException("Slot already unavailable. Choose another.");
            }
            break;
        } catch (const HospitalException& ex) {
            std::cout << ex.what() << '\n';
        }
    }

    try {
        if (patient.getBalance() < doc->getFee()) {
            throw InsufficientFundsException("Insufficient balance for booking.");
        }
    } catch (const HospitalException& ex) {
        std::cout << ex.what() << '\n';
        return;
    }

    patient -= doc->getFee();

    Appointment appointment(fileHandler.nextAppointmentId(), patient.getId(), doctorId, date, selectedSlot, "pending");
    appointments.add(appointment);
    Bill bill(fileHandler.nextBillId(), patient.getId(), appointment.getAppointmentId(), doc->getFee(), "unpaid", date);
    bills.add(bill);

    fileHandler.savePatients(patients);
    fileHandler.saveAppointments(appointments);
    fileHandler.saveBills(bills);

    std::cout << "Appointment booked successfully. Appointment ID: " << appointment.getAppointmentId() << "\n";
}

void HospitalSystem::patientCancelAppointment(Patient& patient) {
    int pCount = 0;
    for (int i = 0; i < appointments.size(); ++i) {
        Appointment& a = appointments.getAll()[i];
        if (a.getPatientId() == patient.getId() && Validator::strEqual(a.getStatus(), "pending")) {
            ++pCount;
            std::cout << a.getAppointmentId() << " | " << findDoctorName(a.getDoctorId()) << " | " << a.getDate() << " | " << a.getTimeSlot() << '\n';
        }
    }
    if (pCount == 0) {
        std::cout << "You have no pending appointments.\n";
        return;
    }

    int aid;
    std::cout << "Enter Appointment ID to cancel: ";
    std::cin >> aid;
    std::cin.ignore(1000, '\n');

    Appointment* target = 0;
    for (int i = 0; i < appointments.size(); ++i) {
        if (appointments.getAll()[i].getAppointmentId() == aid
            && appointments.getAll()[i].getPatientId() == patient.getId()
            && Validator::strEqual(appointments.getAll()[i].getStatus(), "pending")) {
            target = &appointments.getAll()[i];
            break;
        }
    }
    if (!target) {
        std::cout << "Invalid appointment ID.\n";
        return;
    }

    int fee = findDoctorFee(target->getDoctorId());
    target->setStatus("cancelled");
    patient += fee;

    for (int i = 0; i < bills.size(); ++i) {
        if (bills.getAll()[i].getAppointmentId() == aid) {
            bills.getAll()[i].setStatus("cancelled");
        }
    }

    fileHandler.saveAppointments(appointments);
    fileHandler.saveBills(bills);
    fileHandler.savePatients(patients);

    std::cout << "Appointment cancelled. PKR " << fee << " refunded to your balance.\n";
}

void HospitalSystem::sortAppointmentsByDate(Appointment* list, int count, bool asc) const {
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            int cmp = Validator::compareDate(list[j].getDate(), list[j + 1].getDate());
            bool swap = asc ? (cmp > 0) : (cmp < 0);
            if (swap) {
                Appointment t = list[j];
                list[j] = list[j + 1];
                list[j + 1] = t;
            }
        }
    }
}

void HospitalSystem::patientViewAppointments(const Patient& patient) const {
    Appointment local[100];
    int count = 0;
    for (int i = 0; i < appointments.size(); ++i) {
        if (appointments.getAll()[i].getPatientId() == patient.getId()) {
            local[count++] = appointments.getAll()[i];
        }
    }
    if (count == 0) {
        std::cout << "No appointments found.\n";
        return;
    }
    sortAppointmentsByDate(local, count, true);
    for (int i = 0; i < count; ++i) {
        std::cout << local[i].getAppointmentId() << " | " << findDoctorName(local[i].getDoctorId()) << " | "
                  << findDoctorSpec(local[i].getDoctorId()) << " | " << local[i].getDate() << " | "
                  << local[i].getTimeSlot() << " | " << local[i].getStatus() << '\n';
    }
}

void HospitalSystem::sortPrescriptionsByDate(Prescription* list, int count, bool desc) const {
    for (int i = 0; i < count - 1; ++i) {
        for (int j = 0; j < count - i - 1; ++j) {
            int cmp = Validator::compareDate(list[j].getDate(), list[j + 1].getDate());
            bool swap = desc ? (cmp < 0) : (cmp > 0);
            if (swap) {
                Prescription t = list[j];
                list[j] = list[j + 1];
                list[j + 1] = t;
            }
        }
    }
}

void HospitalSystem::patientViewMedicalRecords(const Patient& patient) const {
    Prescription local[100];
    int count = 0;
    for (int i = 0; i < prescriptions.size(); ++i) {
        if (prescriptions.getAll()[i].getPatientId() == patient.getId()) {
            local[count++] = prescriptions.getAll()[i];
        }
    }
    if (count == 0) {
        std::cout << "No medical records found.\n";
        return;
    }
    sortPrescriptionsByDate(local, count, true);
    for (int i = 0; i < count; ++i) {
        std::cout << local[i].getDate() << " | " << findDoctorName(local[i].getDoctorId()) << " | "
                  << local[i].getMedicines() << " | " << local[i].getNotes() << '\n';
    }
}

void HospitalSystem::patientViewBills(const Patient& patient) const {
    double total = 0.0;
    int found = 0;
    for (int i = 0; i < bills.size(); ++i) {
        const Bill& b = bills.getAll()[i];
        if (b.getPatientId() == patient.getId()) {
            char a[32];
            Validator::formatDouble(b.getAmount(), a, 32);
            std::cout << b.getBillId() << " | " << b.getAppointmentId() << " | " << a << " | " << b.getStatus() << " | " << b.getDate() << '\n';
            if (Validator::strEqual(b.getStatus(), "unpaid")) total += b.getAmount();
            found = 1;
        }
    }
    if (!found) {
        std::cout << "No bills found.\n";
        return;
    }
    char out[32];
    Validator::formatDouble(total, out, 32);
    std::cout << "Outstanding unpaid total: PKR " << out << '\n';
}

void HospitalSystem::patientPayBill(Patient& patient) {
    int unpaidCount = 0;
    for (int i = 0; i < bills.size(); ++i) {
        Bill& b = bills.getAll()[i];
        if (b.getPatientId() == patient.getId() && Validator::strEqual(b.getStatus(), "unpaid")) {
            ++unpaidCount;
            std::cout << b.getBillId() << " | Appointment " << b.getAppointmentId() << " | " << b.getAmount() << " | " << b.getDate() << '\n';
        }
    }
    if (unpaidCount == 0) {
        std::cout << "No unpaid bills.\n";
        return;
    }

    int billId;
    std::cout << "Enter Bill ID to pay: ";
    std::cin >> billId;
    std::cin.ignore(1000, '\n');

    Bill* target = 0;
    for (int i = 0; i < bills.size(); ++i) {
        if (bills.getAll()[i].getBillId() == billId && bills.getAll()[i].getPatientId() == patient.getId()
            && Validator::strEqual(bills.getAll()[i].getStatus(), "unpaid")) {
            target = &bills.getAll()[i];
        }
    }
    if (!target) {
        std::cout << "Invalid bill selection.\n";
        return;
    }

    try {
        if (patient.getBalance() < target->getAmount()) {
            throw InsufficientFundsException("Insufficient balance to pay bill.");
        }
    } catch (const HospitalException& ex) {
        std::cout << ex.what() << '\n';
        return;
    }

    patient -= target->getAmount();
    target->setStatus("paid");
    fileHandler.savePatients(patients);
    fileHandler.saveBills(bills);
    std::cout << "Bill paid successfully. Remaining balance: PKR " << patient.getBalance() << "\n";
}

void HospitalSystem::patientTopUp(Patient& patient) {
    int tries = 0;
    while (tries < 3) {
        char amountText[40];
        std::cout << "Enter amount to add (PKR): ";
        std::cin.getline(amountText, 40);
        try {
            if (!Validator::isPositiveFloat(amountText)) {
                throw InvalidInputException("Invalid amount. Please enter a positive number.");
            }
            double amount = Validator::toDouble(amountText);
            patient += amount;
            fileHandler.savePatients(patients);
            std::cout << "Balance updated. New balance: PKR " << patient.getBalance() << "\n";
            return;
        } catch (const HospitalException& ex) {
            std::cout << ex.what() << '\n';
        }
        ++tries;
    }
}

void HospitalSystem::doctorMenu(int doctorId) {
    Doctor* d = doctors.findById(doctorId);
    if (!d) return;
    int choice;
    do {
        std::cout << "\nWelcome, Dr. " << d->getName() << " | Specialization: " << d->getSpecialization() << "\n";
        std::cout << "1.View Today's Appointments\n2.Mark Appointment Complete\n3.Mark Appointment No-Show\n4.Write Prescription\n5.View Patient Medical History\n6.Logout\nChoice: ";
        std::cin >> choice;
        std::cin.ignore(1000, '\n');

        if (choice == 1) doctorViewTodayAppointments(*d);
        else if (choice == 2) doctorMarkAppointmentComplete(*d);
        else if (choice == 3) doctorMarkAppointmentNoShow(*d);
        else if (choice == 4) doctorWritePrescription(*d);
        else if (choice == 5) doctorViewPatientHistory(*d);
    } while (choice != 6);
}

void HospitalSystem::doctorViewTodayAppointments(const Doctor& doctor) const {
    char today[11];
    Validator::getTodayDate(today);
    int found = 0;
    for (int i = 0; i < appointments.size(); ++i) {
        const Appointment& a = appointments.getAll()[i];
        if (a.getDoctorId() == doctor.getId() && Validator::strEqual(a.getDate(), today)) {
            std::cout << a.getAppointmentId() << " | " << findPatientName(a.getPatientId()) << " | " << a.getTimeSlot() << " | " << a.getStatus() << '\n';
            found = 1;
        }
    }
    if (!found) std::cout << "No appointments scheduled for today.\n";
}

void HospitalSystem::doctorMarkAppointmentComplete(const Doctor& doctor) {
    char today[11];
    Validator::getTodayDate(today);
    for (int i = 0; i < appointments.size(); ++i) {
        const Appointment& a = appointments.getAll()[i];
        if (a.getDoctorId() == doctor.getId() && Validator::strEqual(a.getDate(), today) && Validator::strEqual(a.getStatus(), "pending")) {
            std::cout << a.getAppointmentId() << " | " << findPatientName(a.getPatientId()) << " | " << a.getTimeSlot() << '\n';
        }
    }
    int id;
    std::cout << "Enter Appointment ID: ";
    std::cin >> id;
    std::cin.ignore(1000, '\n');
    for (int i = 0; i < appointments.size(); ++i) {
        Appointment& a = appointments.getAll()[i];
        if (a.getAppointmentId() == id && a.getDoctorId() == doctor.getId() && Validator::strEqual(a.getDate(), today)
            && Validator::strEqual(a.getStatus(), "pending")) {
            a.setStatus("completed");
            fileHandler.saveAppointments(appointments);
            std::cout << "Appointment marked as completed.\n";
            return;
        }
    }
    std::cout << "Invalid appointment ID.\n";
}

void HospitalSystem::doctorMarkAppointmentNoShow(const Doctor& doctor) {
    char today[11];
    Validator::getTodayDate(today);
    int id;
    std::cout << "Enter Appointment ID to mark no-show: ";
    std::cin >> id;
    std::cin.ignore(1000, '\n');
    for (int i = 0; i < appointments.size(); ++i) {
        Appointment& a = appointments.getAll()[i];
        if (a.getAppointmentId() == id && a.getDoctorId() == doctor.getId() && Validator::strEqual(a.getDate(), today)
            && Validator::strEqual(a.getStatus(), "pending")) {
            a.setStatus("no-show");
            for (int b = 0; b < bills.size(); ++b) if (bills.getAll()[b].getAppointmentId() == id) bills.getAll()[b].setStatus("cancelled");
            fileHandler.saveAppointments(appointments);
            fileHandler.saveBills(bills);
            std::cout << "Appointment marked as no-show.\n";
            return;
        }
    }
    std::cout << "Invalid appointment ID.\n";
}

void HospitalSystem::doctorWritePrescription(const Doctor& doctor) {
    int aid;
    std::cout << "Enter Appointment ID: ";
    std::cin >> aid;
    std::cin.ignore(1000, '\n');

    Appointment* ap = 0;
    for (int i = 0; i < appointments.size(); ++i) {
        if (appointments.getAll()[i].getAppointmentId() == aid && appointments.getAll()[i].getDoctorId() == doctor.getId()
            && Validator::strEqual(appointments.getAll()[i].getStatus(), "completed")) {
            ap = &appointments.getAll()[i];
        }
    }
    if (!ap) {
        std::cout << "Invalid appointment for prescription.\n";
        return;
    }

    for (int i = 0; i < prescriptions.size(); ++i) {
        if (prescriptions.getAll()[i].getAppointmentId() == aid) {
            std::cout << "Prescription already written for this appointment.\n";
            return;
        }
    }

    char medicines[500];
    char notes[300];
    std::cout << "Enter medicines: ";
    std::cin.getline(medicines, 500);
    std::cout << "Enter notes: ";
    std::cin.getline(notes, 300);

    prescriptions.add(Prescription(fileHandler.nextPrescriptionId(), ap->getAppointmentId(), ap->getPatientId(), doctor.getId(), ap->getDate(), medicines, notes));
    fileHandler.savePrescriptions(prescriptions);
    std::cout << "Prescription saved.\n";
}

void HospitalSystem::doctorViewPatientHistory(const Doctor& doctor) const {
    int patientId;
    std::cout << "Enter Patient ID: ";
    std::cin >> patientId;
    std::cin.ignore(1000, '\n');

    bool allowed = false;
    for (int i = 0; i < appointments.size(); ++i) {
        const Appointment& a = appointments.getAll()[i];
        if (a.getPatientId() == patientId && a.getDoctorId() == doctor.getId() && Validator::strEqual(a.getStatus(), "completed")) {
            allowed = true;
            break;
        }
    }
    if (!allowed || !patients.findById(patientId)) {
        std::cout << "Access denied. You can only view records of your own patients.\n";
        return;
    }

    Prescription local[100];
    int count = 0;
    for (int i = 0; i < prescriptions.size(); ++i) {
        if (prescriptions.getAll()[i].getPatientId() == patientId && prescriptions.getAll()[i].getDoctorId() == doctor.getId()) {
            local[count++] = prescriptions.getAll()[i];
        }
    }
    sortPrescriptionsByDate(local, count, true);
    for (int i = 0; i < count; ++i) {
        std::cout << local[i].getDate() << " | " << local[i].getMedicines() << " | " << local[i].getNotes() << '\n';
    }
}

void HospitalSystem::adminMenu(int adminId) {
    (void)adminId;
    int choice;
    do {
        std::cout << "\nAdmin Panel - MediCore\n";
        std::cout << "1.Add Doctor\n2.Remove Doctor\n3.View All Patients\n4.View All Doctors\n5.View All Appointments\n6.View Unpaid Bills\n7.Discharge Patient\n8.View Security Log\n9.Generate Daily Report\n10.Logout\nChoice: ";
        std::cin >> choice;
        std::cin.ignore(1000, '\n');

        if (choice == 1) adminAddDoctor();
        else if (choice == 2) adminRemoveDoctor();
        else if (choice == 3) adminViewAllPatients();
        else if (choice == 4) adminViewAllDoctors();
        else if (choice == 5) adminViewAllAppointments();
        else if (choice == 6) adminViewUnpaidBills();
        else if (choice == 7) adminDischargePatient();
        else if (choice == 8) adminViewSecurityLog();
        else if (choice == 9) adminGenerateDailyReport();
    } while (choice != 10);
}

void HospitalSystem::adminAddDoctor() {
    char name[51], spec[51], contact[20], password[60], feeTxt[40];
    std::cout << "Enter name: "; std::cin.getline(name, 51);
    std::cout << "Enter specialization: "; std::cin.getline(spec, 51);
    std::cout << "Enter contact (11 digits): "; std::cin.getline(contact, 20);
    std::cout << "Enter password: "; std::cin.getline(password, 60);
    std::cout << "Enter consultation fee: "; std::cin.getline(feeTxt, 40);

    if (!Validator::isValidContact(contact) || !Validator::isValidPassword(password) || !Validator::isPositiveFloat(feeTxt)) {
        std::cout << "Invalid doctor input.\n";
        return;
    }

    Doctor d(fileHandler.nextDoctorId(), name, spec, contact, password, Validator::toDouble(feeTxt));
    doctors.add(d);
    fileHandler.saveDoctors(doctors);
    std::cout << "Doctor added successfully. ID: " << d.getId() << '\n';
}

void HospitalSystem::adminRemoveDoctor() {
    for (int i = 0; i < doctors.size(); ++i) std::cout << doctors.getAll()[i] << '\n';
    int id;
    std::cout << "Enter Doctor ID to remove: ";
    std::cin >> id;
    std::cin.ignore(1000, '\n');

    if (hasPendingAppointmentsForDoctor(id)) {
        std::cout << "Cannot remove doctor with pending appointments. Cancel or reassign them first.\n";
        return;
    }

    if (doctors.removeById(id)) {
        fileHandler.saveDoctors(doctors);
        std::cout << "Doctor removed.\n";
    } else {
        std::cout << "Doctor not found.\n";
    }
}

void HospitalSystem::adminViewAllPatients() const {
    for (int i = 0; i < patients.size(); ++i) {
        int unpaid = 0;
        for (int b = 0; b < bills.size(); ++b) {
            if (bills.getAll()[b].getPatientId() == patients.getAll()[i].getId() && Validator::strEqual(bills.getAll()[b].getStatus(), "unpaid")) {
                ++unpaid;
            }
        }
        std::cout << patients.getAll()[i] << " | unpaid bills: " << unpaid << '\n';
    }
}

void HospitalSystem::adminViewAllDoctors() const {
    for (int i = 0; i < doctors.size(); ++i) std::cout << doctors.getAll()[i] << '\n';
}

void HospitalSystem::adminViewAllAppointments() const {
    Appointment local[100];
    int count = appointments.size();
    for (int i = 0; i < count; ++i) local[i] = appointments.getAll()[i];
    sortAppointmentsByDate(local, count, false);
    for (int i = 0; i < count; ++i) {
        std::cout << local[i].getAppointmentId() << " | " << findPatientName(local[i].getPatientId()) << " | "
                  << findDoctorName(local[i].getDoctorId()) << " | " << local[i].getDate() << " | "
                  << local[i].getTimeSlot() << " | " << local[i].getStatus() << '\n';
    }
}

void HospitalSystem::adminViewUnpaidBills() const {
    char today[11];
    Validator::getTodayDate(today);
    time_t now = Validator::parseDate(today);

    for (int i = 0; i < bills.size(); ++i) {
        const Bill& b = bills.getAll()[i];
        if (Validator::strEqual(b.getStatus(), "unpaid")) {
            bool overdue = false;
            time_t d = Validator::parseDate(b.getDate());
            if (d > 0) {
                double days = std::difftime(now, d) / (60.0 * 60.0 * 24.0);
                overdue = days > 7.0;
            }
            std::cout << b.getBillId() << " | " << findPatientName(b.getPatientId()) << " | " << b.getAmount()
                      << " | " << b.getDate() << (overdue ? " [OVERDUE]" : "") << '\n';
        }
    }
}

void HospitalSystem::adminDischargePatient() {
    int pid;
    std::cout << "Enter Patient ID: ";
    std::cin >> pid;
    std::cin.ignore(1000, '\n');

    Patient* p = patients.findById(pid);
    if (!p) {
        std::cout << "Patient not found.\n";
        return;
    }
    if (hasUnpaidBillsForPatient(pid)) {
        std::cout << "Cannot discharge patient with unpaid bills.\n";
        return;
    }
    if (hasPendingAppointmentsForPatient(pid)) {
        std::cout << "Cannot discharge patient with pending appointments.\n";
        return;
    }

    char line[512];
    std::snprintf(line, sizeof(line), "%d,%s,%d,%s,%s,%s,%.2f", p->getId(), p->getName(), p->getAge(), p->getGender(), p->getContact(), p->getPassword(), p->getBalance());
    fileHandler.appendDischargedLine(line);

    Storage<Appointment> newA;
    for (int i = 0; i < appointments.size(); ++i) if (appointments.getAll()[i].getPatientId() != pid) newA.add(appointments.getAll()[i]);
    appointments = newA;

    Storage<Bill> newB;
    for (int i = 0; i < bills.size(); ++i) if (bills.getAll()[i].getPatientId() != pid) newB.add(bills.getAll()[i]);
    bills = newB;

    Storage<Prescription> newP;
    for (int i = 0; i < prescriptions.size(); ++i) if (prescriptions.getAll()[i].getPatientId() != pid) newP.add(prescriptions.getAll()[i]);
    prescriptions = newP;

    patients.removeById(pid);

    fileHandler.savePatients(patients);
    fileHandler.saveAppointments(appointments);
    fileHandler.saveBills(bills);
    fileHandler.savePrescriptions(prescriptions);

    std::cout << "Patient discharged and archived successfully.\n";
}

void HospitalSystem::adminViewSecurityLog() const {
    char lines[200][256];
    int count = 0;
    if (!fileHandler.readSecurityLog(lines, 200, count) || count == 0) {
        std::cout << "No security events logged.\n";
        return;
    }
    for (int i = 0; i < count; ++i) std::cout << lines[i];
}

void HospitalSystem::adminGenerateDailyReport() const {
    char today[11];
    Validator::getTodayDate(today);
    int total = 0, pending = 0, completed = 0, noShow = 0, cancelled = 0;
    for (int i = 0; i < appointments.size(); ++i) {
        const Appointment& a = appointments.getAll()[i];
        if (Validator::strEqual(a.getDate(), today)) {
            ++total;
            if (Validator::strEqual(a.getStatus(), "pending")) ++pending;
            else if (Validator::strEqual(a.getStatus(), "completed")) ++completed;
            else if (Validator::strEqual(a.getStatus(), "no-show")) ++noShow;
            else if (Validator::strEqual(a.getStatus(), "cancelled")) ++cancelled;
        }
    }

    double revenue = 0.0;
    for (int i = 0; i < bills.size(); ++i) {
        if (Validator::strEqual(bills.getAll()[i].getStatus(), "paid") && Validator::strEqual(bills.getAll()[i].getDate(), today)) {
            revenue += bills.getAll()[i].getAmount();
        }
    }

    std::cout << "Total appointments today: " << total << " (Pending: " << pending << " Completed: " << completed
              << " No-show: " << noShow << " Cancelled: " << cancelled << ")\n";
    std::cout << "Revenue collected today (paid bills): PKR " << revenue << '\n';

    std::cout << "Patients with outstanding unpaid bills:\n";
    for (int p = 0; p < patients.size(); ++p) {
        double owed = 0.0;
        for (int b = 0; b < bills.size(); ++b) {
            if (bills.getAll()[b].getPatientId() == patients.getAll()[p].getId() && Validator::strEqual(bills.getAll()[b].getStatus(), "unpaid")) {
                owed += bills.getAll()[b].getAmount();
            }
        }
        if (owed > 0.0) std::cout << patients.getAll()[p].getName() << " | " << owed << '\n';
    }

    std::cout << "Doctor-wise summary for today:\n";
    for (int d = 0; d < doctors.size(); ++d) {
        int c = 0, p = 0, n = 0;
        for (int i = 0; i < appointments.size(); ++i) {
            const Appointment& a = appointments.getAll()[i];
            if (a.getDoctorId() == doctors.getAll()[d].getId() && Validator::strEqual(a.getDate(), today)) {
                if (Validator::strEqual(a.getStatus(), "completed")) ++c;
                else if (Validator::strEqual(a.getStatus(), "pending")) ++p;
                else if (Validator::strEqual(a.getStatus(), "no-show")) ++n;
            }
        }
        std::cout << doctors.getAll()[d].getName() << " | " << c << " | " << p << " | " << n << '\n';
    }
}

int HospitalSystem::findDoctorFee(int doctorId) const {
    const Doctor* d = doctors.findById(doctorId);
    return d ? static_cast<int>(d->getFee()) : 0;
}

const char* HospitalSystem::findDoctorName(int doctorId) const {
    const Doctor* d = doctors.findById(doctorId);
    return d ? d->getName() : "Unknown";
}

const char* HospitalSystem::findDoctorSpec(int doctorId) const {
    const Doctor* d = doctors.findById(doctorId);
    return d ? d->getSpecialization() : "Unknown";
}

const char* HospitalSystem::findPatientName(int patientId) const {
    const Patient* p = patients.findById(patientId);
    return p ? p->getName() : "Unknown";
}

bool HospitalSystem::hasPendingAppointmentsForDoctor(int doctorId) const {
    for (int i = 0; i < appointments.size(); ++i) {
        if (appointments.getAll()[i].getDoctorId() == doctorId && Validator::strEqual(appointments.getAll()[i].getStatus(), "pending")) return true;
    }
    return false;
}

bool HospitalSystem::hasPendingAppointmentsForPatient(int patientId) const {
    for (int i = 0; i < appointments.size(); ++i) {
        if (appointments.getAll()[i].getPatientId() == patientId && Validator::strEqual(appointments.getAll()[i].getStatus(), "pending")) return true;
    }
    return false;
}

bool HospitalSystem::hasUnpaidBillsForPatient(int patientId) const {
    for (int i = 0; i < bills.size(); ++i) {
        if (bills.getAll()[i].getPatientId() == patientId && Validator::strEqual(bills.getAll()[i].getStatus(), "unpaid")) return true;
    }
    return false;
}

// =============================================================================
// SFML UI action methods
// =============================================================================

bool HospitalSystem::bookAppointment(int patientId, int doctorId,
                                     const char* date, const char* slot,
                                     char* outMsg) {
    Patient* p = patients.findById(patientId);
    if (!p) { Validator::strCopy(outMsg, "Patient not found.", 256); return false; }

    Doctor* doc = doctors.findById(doctorId);
    if (!doc) { Validator::strCopy(outMsg, "Doctor not found.", 256); return false; }

    if (!Validator::isValidDate(date)) {
        Validator::strCopy(outMsg, "Invalid date. Use DD-MM-YYYY.", 256); return false;
    }
    if (!Validator::isValidTimeSlot(slot)) {
        Validator::strCopy(outMsg, "Invalid time slot.", 256); return false;
    }

    for (int i = 0; i < appointments.size(); ++i) {
        const Appointment& a = appointments.getAll()[i];
        if (a.getDoctorId() == doctorId &&
            Validator::strEqual(a.getDate(), date) &&
            Validator::strEqual(a.getTimeSlot(), slot) &&
            !Validator::strEqual(a.getStatus(), "cancelled")) {
            Validator::strCopy(outMsg, "Slot already taken. Choose another.", 256);
            return false;
        }
    }

    if (p->getBalance() < doc->getFee()) {
        Validator::strCopy(outMsg, "Insufficient balance.", 256); return false;
    }

    *p -= doc->getFee();
    Appointment appt(fileHandler.nextAppointmentId(), patientId, doctorId,
                     date, slot, "pending");
    appointments.add(appt);
    Bill bill(fileHandler.nextBillId(), patientId,
              appt.getAppointmentId(), doc->getFee(), "unpaid", date);
    bills.add(bill);

    fileHandler.savePatients(patients);
    fileHandler.saveAppointments(appointments);
    fileHandler.saveBills(bills);

    std::snprintf(outMsg, 256, "Appointment booked! ID: %d",
                  appt.getAppointmentId());
    return true;
}

bool HospitalSystem::cancelAppointment(int patientId, int appointmentId,
                                       char* outMsg) {
    Patient* p = patients.findById(patientId);
    if (!p) { Validator::strCopy(outMsg, "Patient not found.", 256); return false; }

    Appointment* target = 0;
    for (int i = 0; i < appointments.size(); ++i) {
        Appointment& a = appointments.getAll()[i];
        if (a.getAppointmentId() == appointmentId &&
            a.getPatientId() == patientId &&
            Validator::strEqual(a.getStatus(), "pending")) {
            target = &a; break;
        }
    }
    if (!target) {
        Validator::strCopy(outMsg, "Appointment not found or not pending.", 256);
        return false;
    }

    const Doctor* doc = doctors.findById(target->getDoctorId());
    double fee = doc ? doc->getFee() : 0.0;
    target->setStatus("cancelled");
    *p += fee;

    for (int i = 0; i < bills.size(); ++i) {
        if (bills.getAll()[i].getAppointmentId() == appointmentId)
            bills.getAll()[i].setStatus("cancelled");
    }

    fileHandler.saveAppointments(appointments);
    fileHandler.saveBills(bills);
    fileHandler.savePatients(patients);

    std::snprintf(outMsg, 256, "Appointment cancelled. PKR %.2f refunded.", fee);
    return true;
}

bool HospitalSystem::payBill(int patientId, int billId, char* outMsg) {
    Patient* p = patients.findById(patientId);
    if (!p) { Validator::strCopy(outMsg, "Patient not found.", 256); return false; }

    Bill* b = 0;
    for (int i = 0; i < bills.size(); ++i) {
        Bill& bRef = bills.getAll()[i];
        if (bRef.getBillId() == billId &&
            bRef.getPatientId() == patientId &&
            Validator::strEqual(bRef.getStatus(), "unpaid")) {
            b = &bRef; break;
        }
    }
    if (!b) {
        Validator::strCopy(outMsg, "Bill not found or already paid.", 256);
        return false;
    }

    if (p->getBalance() < b->getAmount()) {
        Validator::strCopy(outMsg, "Insufficient balance.", 256); return false;
    }

    *p -= b->getAmount();
    b->setStatus("paid");
    fileHandler.savePatients(patients);
    fileHandler.saveBills(bills);

    std::snprintf(outMsg, 256, "Bill paid. Remaining balance: PKR %.2f",
                  p->getBalance());
    return true;
}

bool HospitalSystem::topUpBalance(int patientId, double amount, char* outMsg) {
    Patient* p = patients.findById(patientId);
    if (!p) { Validator::strCopy(outMsg, "Patient not found.", 256); return false; }
    if (amount <= 0.0) {
        Validator::strCopy(outMsg, "Amount must be positive.", 256); return false;
    }
    *p += amount;
    fileHandler.savePatients(patients);
    std::snprintf(outMsg, 256, "Balance updated. New balance: PKR %.2f",
                  p->getBalance());
    return true;
}

bool HospitalSystem::updateAppointmentStatus(int doctorId, int appointmentId,
                                             const char* status, char* outMsg) {
    for (int i = 0; i < appointments.size(); ++i) {
        Appointment& a = appointments.getAll()[i];
        if (a.getAppointmentId() == appointmentId &&
            a.getDoctorId() == doctorId &&
            Validator::strEqual(a.getStatus(), "pending")) {
            a.setStatus(status);
            if (Validator::strEqual(status, "no-show")) {
                for (int b = 0; b < bills.size(); ++b)
                    if (bills.getAll()[b].getAppointmentId() == appointmentId)
                        bills.getAll()[b].setStatus("cancelled");
                fileHandler.saveBills(bills);
            }
            fileHandler.saveAppointments(appointments);
            std::snprintf(outMsg, 256, "Appointment %d marked as %s.",
                          appointmentId, status);
            return true;
        }
    }
    Validator::strCopy(outMsg, "Appointment not found or not pending.", 256);
    return false;
}

bool HospitalSystem::writePrescription(int doctorId, int appointmentId,
                                       const char* medicines,
                                       const char* notes, char* outMsg) {
    Appointment* ap = 0;
    for (int i = 0; i < appointments.size(); ++i) {
        Appointment& a = appointments.getAll()[i];
        if (a.getAppointmentId() == appointmentId &&
            a.getDoctorId() == doctorId &&
            Validator::strEqual(a.getStatus(), "completed")) {
            ap = &a; break;
        }
    }
    if (!ap) {
        Validator::strCopy(outMsg,
            "Appointment not found or not completed.", 256);
        return false;
    }
    for (int i = 0; i < prescriptions.size(); ++i) {
        if (prescriptions.getAll()[i].getAppointmentId() == appointmentId) {
            Validator::strCopy(outMsg,
                "Prescription already written for this appointment.", 256);
            return false;
        }
    }

    prescriptions.add(Prescription(
        fileHandler.nextPrescriptionId(),
        ap->getAppointmentId(), ap->getPatientId(), doctorId,
        ap->getDate(), medicines, notes));
    fileHandler.savePrescriptions(prescriptions);
    Validator::strCopy(outMsg, "Prescription saved successfully.", 256);
    return true;
}

bool HospitalSystem::addDoctor(const char* name, const char* spec,
                               const char* contact, const char* password,
                               double fee, char* outMsg) {
    if (!name || name[0] == '\0') {
        Validator::strCopy(outMsg, "Name cannot be empty.", 256); return false;
    }
    if (!Validator::isValidContact(contact)) {
        Validator::strCopy(outMsg, "Invalid contact (11 digits required).", 256);
        return false;
    }
    if (!Validator::isValidPassword(password)) {
        Validator::strCopy(outMsg, "Invalid password.", 256); return false;
    }
    if (fee <= 0.0) {
        Validator::strCopy(outMsg, "Fee must be positive.", 256); return false;
    }
    Doctor d(fileHandler.nextDoctorId(), name, spec, contact, password, fee);
    doctors.add(d);
    fileHandler.saveDoctors(doctors);
    std::snprintf(outMsg, 256, "Doctor added successfully. ID: %d", d.getId());
    return true;
}

bool HospitalSystem::addPatient(const char* name, int age,
                                const char* gender, const char* contact,
                                const char* password, double initialBalance,
                                char* outMsg)
{
    if (!name || Validator::strLength(name) < 1) {
        Validator::strCopy(outMsg, "Name cannot be empty.", 128); return false;
    }
    if (age < 1 || age > 120) {
        Validator::strCopy(outMsg, "Age must be between 1 and 120.", 128); return false;
    }
    if (!Validator::isValidContact(contact)) {
        Validator::strCopy(outMsg, "Contact must be 11 digits.", 128); return false;
    }
    if (!Validator::isValidPassword(password)) {
        Validator::strCopy(outMsg, "Password must be at least 6 characters.", 128); return false;
    }
    if (initialBalance < 0.0) {
        Validator::strCopy(outMsg, "Balance cannot be negative.", 128); return false;
    }

    int newId = 1;
    for (int i = 0; i < patients.size(); ++i)
        if (patients.getAll()[i].getId() >= newId)
            newId = patients.getAll()[i].getId() + 1;

    Patient p(newId, name, age, gender, contact, password, initialBalance);
    if (!patients.add(p)) {
        Validator::strCopy(outMsg, "Patient storage is full.", 128); return false;
    }
    fileHandler.savePatients(patients);
    Validator::strCopy(outMsg, "Patient added successfully.", 128);
    return true;
}

bool HospitalSystem::removeDoctor(int doctorId, char* outMsg) {
    if (hasPendingAppointmentsForDoctor(doctorId)) {
        Validator::strCopy(outMsg,
            "Cannot remove: doctor has pending appointments.", 256);
        return false;
    }
    if (doctors.removeById(doctorId)) {
        fileHandler.saveDoctors(doctors);
        Validator::strCopy(outMsg, "Doctor removed successfully.", 256);
        return true;
    }
    Validator::strCopy(outMsg, "Doctor not found.", 256);
    return false;
}

bool HospitalSystem::dischargePatient(int patientId, char* outMsg) {
    Patient* p = patients.findById(patientId);
    if (!p) {
        Validator::strCopy(outMsg, "Patient not found.", 256); return false;
    }
    if (hasUnpaidBillsForPatient(patientId)) {
        Validator::strCopy(outMsg,
            "Cannot discharge: patient has unpaid bills.", 256);
        return false;
    }
    if (hasPendingAppointmentsForPatient(patientId)) {
        Validator::strCopy(outMsg,
            "Cannot discharge: patient has pending appointments.", 256);
        return false;
    }

    char line[512];
    std::snprintf(line, sizeof(line), "%d,%s,%d,%s,%s,%s,%.2f",
                  p->getId(), p->getName(), p->getAge(), p->getGender(),
                  p->getContact(), p->getPassword(), p->getBalance());
    fileHandler.appendDischargedLine(line);

    Storage<Appointment> newA;
    for (int i = 0; i < appointments.size(); ++i)
        if (appointments.getAll()[i].getPatientId() != patientId)
            newA.add(appointments.getAll()[i]);
    appointments = newA;

    Storage<Bill> newB;
    for (int i = 0; i < bills.size(); ++i)
        if (bills.getAll()[i].getPatientId() != patientId)
            newB.add(bills.getAll()[i]);
    bills = newB;

    Storage<Prescription> newP;
    for (int i = 0; i < prescriptions.size(); ++i)
        if (prescriptions.getAll()[i].getPatientId() != patientId)
            newP.add(prescriptions.getAll()[i]);
    prescriptions = newP;

    patients.removeById(patientId);
    fileHandler.savePatients(patients);
    fileHandler.saveAppointments(appointments);
    fileHandler.saveBills(bills);
    fileHandler.savePrescriptions(prescriptions);

    Validator::strCopy(outMsg, "Patient discharged and archived.", 256);
    return true;
}

// =============================================================================
// SFML UI query methods
// =============================================================================

void HospitalSystem::getPatientAppointments(int patientId,
                                            Appointment* outArr,
                                            int& outCount) const {
    outCount = 0;
    for (int i = 0; i < appointments.size() && outCount < 100; ++i) {
        if (appointments.getAll()[i].getPatientId() == patientId)
            outArr[outCount++] = appointments.getAll()[i];
    }
}

void HospitalSystem::getPatientBills(int patientId,
                                     Bill* outArr, int& outCount) const {
    outCount = 0;
    for (int i = 0; i < bills.size() && outCount < 100; ++i) {
        if (bills.getAll()[i].getPatientId() == patientId)
            outArr[outCount++] = bills.getAll()[i];
    }
}

void HospitalSystem::getPatientPrescriptions(int patientId,
                                             Prescription* outArr,
                                             int& outCount) const {
    outCount = 0;
    for (int i = 0; i < prescriptions.size() && outCount < 100; ++i) {
        if (prescriptions.getAll()[i].getPatientId() == patientId)
            outArr[outCount++] = prescriptions.getAll()[i];
    }
}

void HospitalSystem::getDoctorTodayAppointments(int doctorId,
                                                Appointment* outArr,
                                                int& outCount) const {
    outCount = 0;
    char today[11];
    Validator::getTodayDate(today);
    for (int i = 0; i < appointments.size() && outCount < 100; ++i) {
        const Appointment& a = appointments.getAll()[i];
        if (a.getDoctorId() == doctorId && Validator::strEqual(a.getDate(), today))
            outArr[outCount++] = a;
    }
}

void HospitalSystem::getDoctorPatientHistory(int doctorId, int patientId,
                                             Prescription* outArr,
                                             int& outCount) const {
    outCount = 0;
    for (int i = 0; i < prescriptions.size() && outCount < 100; ++i) {
        const Prescription& pr = prescriptions.getAll()[i];
        if (pr.getDoctorId() == doctorId && pr.getPatientId() == patientId)
            outArr[outCount++] = pr;
    }
}

void HospitalSystem::getAllPatients(Patient* outArr, int& outCount) const {
    outCount = patients.size();
    for (int i = 0; i < outCount; ++i) outArr[i] = patients.getAll()[i];
}

void HospitalSystem::getAllDoctors(Doctor* outArr, int& outCount) const {
    outCount = doctors.size();
    for (int i = 0; i < outCount; ++i) outArr[i] = doctors.getAll()[i];
}

void HospitalSystem::getAllAppointments(Appointment* outArr,
                                        int& outCount) const {
    outCount = appointments.size();
    for (int i = 0; i < outCount; ++i) outArr[i] = appointments.getAll()[i];
}

void HospitalSystem::getUnpaidBills(Bill* outArr, int& outCount) const {
    outCount = 0;
    for (int i = 0; i < bills.size() && outCount < 100; ++i) {
        if (Validator::strEqual(bills.getAll()[i].getStatus(), "unpaid"))
            outArr[outCount++] = bills.getAll()[i];
    }
}

void HospitalSystem::getSecurityLog(char lines[][256], int maxLines,
                                    int& outCount) const {
    fileHandler.readSecurityLog(lines, maxLines, outCount);
}

double HospitalSystem::getTotalRevenue() const {
    double total = 0.0;
    for (int i = 0; i < bills.size(); ++i) {
        if (Validator::strEqual(bills.getAll()[i].getStatus(), "paid"))
            total += bills.getAll()[i].getAmount();
    }
    return total;
}

const char* HospitalSystem::getPatientName(int patientId) const {
    return findPatientName(patientId);
}

const char* HospitalSystem::getDoctorName(int doctorId) const {
    return findDoctorName(doctorId);
}

double HospitalSystem::getPatientBalance(int patientId) const {
    const Patient* p = patients.findById(patientId);
    return p ? p->getBalance() : 0.0;
}
