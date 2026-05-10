#ifndef DOCTORUI_H
#define DOCTORUI_H

#include "UIWidgets.h"
#include "Appointment.h"
#include "Prescription.h"

class HospitalSystem;

class DoctorUI {
private:
    int doctorId;

    Appointment todayList[100];
    int todayCount;
    int todayScroll;

    UIField apptIdField;
    UIField prescApptField;
    UIField prescMedField;
    UIField prescNotesField;
    int prescActiveField;

    UIField histPidField;
    Prescription histList[100];
    int histCount;
    int histScroll;
    bool histLoaded;

    char msgBuf[128];
    bool msgOk;
    int blink;

    UIButton btnBack;
    UIButton btnConfirm;

public:
    DoctorUI();
    void setDoctorId(int id);
    void loadForScreen(int screen, HospitalSystem& system);
    void handleKey(sf::Keyboard::Key key, int& screen, HospitalSystem& system);
    void handleText(unsigned int ch, int screen);
    void handleClick(float bx, float by, int& screen, HospitalSystem& system);
    void handleScroll(float delta, int screen);
    void update();
    void draw(sf::RenderWindow& win, const sf::Font& font,
              int screen, int mx, int my, HospitalSystem& system);

private:
    void drawMenu(sf::RenderWindow& win, const sf::Font& font,
                  int mx, int my, HospitalSystem& system);
    void drawToday(sf::RenderWindow& win, const sf::Font& font, int mx, int my,
                   HospitalSystem& system);
    void drawCompleteNoShow(sf::RenderWindow& win, const sf::Font& font,
                            int screen, int mx, int my, HospitalSystem& system);
    void drawPrescribe(sf::RenderWindow& win, const sf::Font& font, int mx, int my);
    void drawHistory(sf::RenderWindow& win, const sf::Font& font,
                     int mx, int my, HospitalSystem& system);
};

#endif
