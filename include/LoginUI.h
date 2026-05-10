#ifndef LOGINUI_H
#define LOGINUI_H

#include "UIWidgets.h"

class HospitalSystem;

class LoginUI {
private:
    UIField fieldId;
    UIField fieldPass;
    UIButton btnLogin;
    UIButton btnBack;
    int activeField;
    char msgBuf[128];
    bool msgOk;
    int role;
    int blink;
    int attempts;

public:
    LoginUI();
    void setRole(int role);
    void reset();
    void handleKey(sf::Keyboard::Key key, int& screen, int& loggedId,
                   HospitalSystem& system);
    void handleText(unsigned int ch);
    void handleClick(float bx, float by, int& screen);
    void handleMouseMove(int mx, int my);
    void update();
    void draw(sf::RenderWindow& win, int mx, int my);

    int  getRoleScreenOnSuccess() const;
};

#endif
