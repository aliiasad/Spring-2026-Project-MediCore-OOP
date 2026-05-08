#ifndef BILL_H
#define BILL_H

class Bill {
private:
    int billId;
    int patientId;
    int appointmentId;
    double amount;
    char status[20];
    char date[11];
public:
    Bill();
    Bill(int bId, int pId, int aId, double bAmount, const char* bStatus, const char* bDate);

    int getBillId() const;
    int getPatientId() const;
    int getAppointmentId() const;
    double getAmount() const;
    const char* getStatus() const;
    const char* getDate() const;

    void setStatus(const char* value);
};

#endif
