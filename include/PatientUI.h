#ifndef PATIENTUI_H
#define PATIENTUI_H

#include "UIWidgets.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Prescription.h"
#include "Bill.h"

class HospitalSystem;

class PatientUI {
private:
    int patientId;

    Doctor doctorList[100];
    int doctorCount;
    UIField bookDrIdField;
    UIField bookDateField;
    UIField bookSlotField;
    int bookActiveField;
    int bookScroll;

    Appointment apptList[100];
    int apptCount;
    UIField cancelIdField;
    int apptScroll;

    int viewApptScroll;

    Prescription rxList[100];
    int rxCount;
    int rxScroll;

    Bill billList[100];
    int billCount;
    int billScroll;

    UIField payBillIdField;

    UIField topupAmtField;

    char msgBuf[128];
    bool msgOk;
    int blink;

    UIButton btnBack;
    UIButton btnConfirm;

public:
    PatientUI();
    void setPatientId(int id);
    void loadForScreen(int screen, HospitalSystem& system);
    void handleKey(sf::Keyboard::Key key, int& screen, HospitalSystem& system);
    void handleText(unsigned int ch, int screen);
    void handleClick(float bx, float by, int& screen, HospitalSystem& system);
    void handleScroll(float delta, int screen);
    void update();
    void draw(sf::RenderWindow& win, const sf::Font& font,
              int screen, int mx, int my, HospitalSystem& system);

private:
    void drawMenu(sf::RenderWindow& win, const sf::Font& font, int mx, int my,
                  HospitalSystem& system);
    void drawBook(sf::RenderWindow& win, const sf::Font& font, int mx, int my);
    void drawCancel(sf::RenderWindow& win, const sf::Font& font, int mx, int my,
                    HospitalSystem& system);
    void drawAppointments(sf::RenderWindow& win, const sf::Font& font, int mx, int my,
                          HospitalSystem& system);
    void drawRecords(sf::RenderWindow& win, const sf::Font& font, int mx, int my,
                     HospitalSystem& system);
    void drawBills(sf::RenderWindow& win, const sf::Font& font, int mx, int my);
    void drawPayBill(sf::RenderWindow& win, const sf::Font& font, int mx, int my);
    void drawTopUp(sf::RenderWindow& win, const sf::Font& font, int mx, int my,
                   HospitalSystem& system);
};

#endif
