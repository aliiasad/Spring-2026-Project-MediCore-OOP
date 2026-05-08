#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <iostream>

class Appointment {
private:
    int appointmentId;
    int patientId;
    int doctorId;
    char date[11];
    char timeSlot[6];
    char status[20];
public:
    Appointment();
    Appointment(int aId, int pId, int dId, const char* aDate, const char* aTimeSlot, const char* aStatus);

    int getAppointmentId() const;
    int getPatientId() const;
    int getDoctorId() const;
    const char* getDate() const;
    const char* getTimeSlot() const;
    const char* getStatus() const;

    void setStatus(const char* value);

    bool operator==(const Appointment& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Appointment& appointment);
};

#endif
