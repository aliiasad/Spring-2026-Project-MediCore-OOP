#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

class Prescription {
private:
    int prescriptionId;
    int appointmentId;
    int patientId;
    int doctorId;
    char date[11];
    char medicines[500];
    char notes[300];
public:
    Prescription();
    Prescription(int prId, int aId, int pId, int dId, const char* pDate, const char* pMedicines, const char* pNotes);

    int getPrescriptionId() const;
    int getAppointmentId() const;
    int getPatientId() const;
    int getDoctorId() const;
    const char* getDate() const;
    const char* getMedicines() const;
    const char* getNotes() const;
};

#endif
