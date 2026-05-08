#ifndef ADMINUI_H
#define ADMINUI_H

#include "UIWidgets.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"

class HospitalSystem;

class AdminUI {
private:
    UIField addNameField;
    UIField addSpecField;
    UIField addContactField;
    UIField addPassField;
    UIField addFeeField;
    int     addActiveField;

    UIField remDrIdField;

    UIField addPatNameField;
    UIField addPatAgeField;
    UIField addPatGenderField;
    UIField addPatContactField;
    UIField addPatPassField;
    UIField addPatBalField;
    int     addPatActiveField;

    UIField disPatIdField;

    Patient     patList[100];
    int         patCount;
    int         patScroll;

    Doctor      drList[100];
    int         drCount;
    int         drScroll;

    Appointment apptList[100];
    int         apptCount;
    int         apptScroll;

    Bill        unpaidList[100];
    int         unpaidCount;
    int         unpaidScroll;

    char        logLines[200][256];
    int         logCount;
    int         logScroll;

    char    msgBuf[128];
    bool    msgOk;
    int     blink;

    UIButton btnBack;
    UIButton btnConfirm;

public:
    AdminUI();
    void loadForScreen(int screen, HospitalSystem& system);
    void handleKey(sf::Keyboard::Key key, int& screen, HospitalSystem& system);
    void handleText(unsigned int ch, int screen);
    void handleClick(float bx, float by, int& screen, HospitalSystem& system);
    void handleScroll(float delta, int screen);
    void update();
    void draw(sf::RenderWindow& win, const sf::Font& font,
              int screen, int mx, int my, HospitalSystem& system);

private:
    void drawMenu(sf::RenderWindow& win, const sf::Font& font, int mx, int my);
    void drawAddDoctor(sf::RenderWindow& win, const sf::Font& font, int mx, int my);
    void drawRemoveDoctor(sf::RenderWindow& win, const sf::Font& font, int mx, int my);
    void drawAddPatient(sf::RenderWindow& win, const sf::Font& font, int mx, int my);
    void drawAllPatients(sf::RenderWindow& win, const sf::Font& font, int mx, int my,
                         HospitalSystem& system);
    void drawAllDoctors(sf::RenderWindow& win, const sf::Font& font, int mx, int my,
                        HospitalSystem& system);
    void drawAllAppointments(sf::RenderWindow& win, const sf::Font& font,
                             int mx, int my, HospitalSystem& system);
    void drawUnpaidBills(sf::RenderWindow& win, const sf::Font& font,
                         int mx, int my, HospitalSystem& system);
    void drawDischarge(sf::RenderWindow& win, const sf::Font& font, int mx, int my);
    void drawSecurityLog(sf::RenderWindow& win, const sf::Font& font, int mx, int my);
    void drawReport(sf::RenderWindow& win, const sf::Font& font,
                    int mx, int my, HospitalSystem& system);
};

#endif
