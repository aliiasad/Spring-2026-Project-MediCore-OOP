#include "Prescription.h"
#include "Validator.h"

namespace sf {}
using namespace std;
using namespace sf;


Prescription::Prescription() : prescriptionId(0), appointmentId(0), patientId(0), doctorId(0) {
    date[0] = '\0';
    medicines[0] = '\0';
    notes[0] = '\0';
}

Prescription::Prescription(int prId, int aId, int pId, int dId, const char* pDate, const char* pMedicines, const char* pNotes)
    : prescriptionId(prId), appointmentId(aId), patientId(pId), doctorId(dId) {
    Validator::strCopy(date, pDate, 11);
    Validator::strCopy(medicines, pMedicines, 500);
    Validator::strCopy(notes, pNotes, 300);
}

int Prescription::getPrescriptionId() const { return prescriptionId; }
int Prescription::getAppointmentId() const { return appointmentId; }
int Prescription::getPatientId() const { return patientId; }
int Prescription::getDoctorId() const { return doctorId; }
const char* Prescription::getDate() const { return date; }
const char* Prescription::getMedicines() const { return medicines; }
const char* Prescription::getNotes() const { return notes; }
