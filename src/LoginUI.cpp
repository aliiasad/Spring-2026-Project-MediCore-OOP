#include "LoginUI.h"
#include "HospitalSystem.h"
#include "Validator.h"

namespace sf {}
using namespace std;
using namespace sf;

LoginUI::LoginUI() : activeField(0), msgOk(false), role(0), blink(0), attempts(0) {
    msgBuf[0] = '\0';
    fieldId.set(250.f, 240.f, 400.f, 46.f, "User ID", false);
    fieldPass.set(250.f, 325.f, 400.f, 46.f, "Password", true);
    btnLogin.set(250.f, 395.f, 400.f, 46.f, "Login  (Enter)", 0);
    btnBack.set(250.f, 452.f, 400.f, 38.f, "Back  (Esc)", 2);
}

void LoginUI::setRole(int r) {
    role = r;
    reset();
}

void LoginUI::reset() {
    fieldId.clear();
    fieldPass.clear();
    activeField = 0;
    msgBuf[0] = '\0';
    msgOk = false;
    blink = 0;
    attempts = 0;
}

void LoginUI::handleKey(Keyboard::Key key, int& screen, int& loggedId,
                        HospitalSystem& system)
{
    if (key == Keyboard::Key::Escape) {
        screen = SCREEN_MAIN;
        reset();
        return;
    }

    if (key == Keyboard::Key::Tab) {
        activeField = (activeField + 1) % 2;
        return;
    }

    if (key == Keyboard::Key::Backspace) {
        if (activeField == 0) fieldId.backspace();
        else fieldPass.backspace();
        msgBuf[0] = '\0';
        return;
    }

    if (key != Keyboard::Key::Enter) return;

    const char* roleLabel = "";
    if (role == ROLE_PATIENT) roleLabel = "Patient";
    else if (role == ROLE_DOCTOR) roleLabel = "Doctor";
    else if (role == ROLE_ADMIN) roleLabel = "Admin";

    int id = Validator::toInt(fieldId.getValue());
    if (Validator::strLength(fieldId.getValue()) == 0 ||
        Validator::strLength(fieldPass.getValue()) == 0) {
        Validator::strCopy(msgBuf, "Please fill in both fields.", 128);
        msgOk = false;
        return;
    }

    if (attempts >= 3) {
        Validator::strCopy(msgBuf, "Account locked. Contact admin.", 128);
        msgOk = false;
        return;
    }

    if (system.verifyLogin(roleLabel, id, fieldPass.getValue())) {
        msgBuf[0] = '\0';
        loggedId = id;
        screen = getRoleScreenOnSuccess();
        fieldId.clear();
        fieldPass.clear();
        attempts = 0;
        return;
    }

    ++attempts;
    if (attempts < 3) {
        char nb[4];
        nb[0] = (char)('0' + (3 - attempts)); nb[1] = '\0';
        Validator::strCopy(msgBuf, "Invalid credentials. Attempts left: ", 128);
        int ml = Validator::strLength(msgBuf);
        Validator::strCopy(msgBuf + ml, nb, 128 - ml);
    } else {
        Validator::strCopy(msgBuf, "Account locked. Contact admin.", 128);
    }
    msgOk = false;
    fieldPass.clear();
}

void LoginUI::handleText(unsigned int ch)
{
    if (activeField == 0) fieldId.append(ch);
    else fieldPass.append(ch);
    msgBuf[0] = '\0';
}

void LoginUI::handleClick(float bx, float by, int& screen)
{
    if (fieldId.isHit((int)bx, (int)by)) activeField = 0;
    if (fieldPass.isHit((int)bx, (int)by)) activeField = 1;
    if (btnBack.isHit(bx, by)) {
        screen = SCREEN_MAIN;
        reset();
    }
}

void LoginUI::handleMouseMove(int mx, int my) {
    (void)mx; (void)my;
}

void LoginUI::update() { ++blink; }

void LoginUI::draw(RenderWindow& win, int mx, int my)
{
    const Font& font = g_bodyFontLoaded ? g_bodyFont : g_displayFont;
    const Font& df = g_displayFontLoaded ? g_displayFont : font;

    char hdr[64];
    Validator::strCopy(hdr, role == ROLE_PATIENT ? "Patient" :
                             role == ROLE_DOCTOR ? "Doctor" : "Admin", 64);
    int hl = Validator::strLength(hdr);
    Validator::strCopy(hdr + hl, " Login", 64 - hl);

    float CPX=200.f, CPY=110.f, CPW=500.f, CPH=360.f;
    win.draw(makeRect(CPX, CPY, CPW, CPH, COL_SURFACE));
    win.draw(makeRect(CPX, CPY, CPW, 3.f, COL_GOLD));
    RectangleShape cardOl = makeRect(CPX, CPY, CPW, CPH, Color::Transparent);
    cardOl.setOutlineThickness(1.f);
    cardOl.setOutlineColor(COL_BORDER);
    win.draw(cardOl);
    drawCornerAccents(win, CPX, CPY, CPW, CPH, COL_GOLD_DIM);

    float icX=450.f, icY=145.f, icS=10.f;
    RectangleShape ic = makeRect(icX-icS*0.5f, icY-icS*0.5f, icS, icS, COL_GOLD);
    ic.setRotation(degrees(45.f));
    win.draw(ic);

    Text wb(font, "Welcome back", 13);
    wb.setFillColor(COL_MUTED);
    centreTextX(wb, CPX, CPW);
    wb.setPosition({wb.getPosition().x, 162.f});
    win.draw(wb);
    Text rn(df, hdr, 22);
    rn.setFillColor(COL_WHITE);
    centreTextX(rn, CPX, CPW);
    rn.setPosition({rn.getPosition().x, 184.f});
    win.draw(rn);

    bool blinkOn = (blink / 30) % 2 == 0;
    fieldId.setActive(activeField == 0);
    fieldPass.setActive(activeField == 1);
    fieldId.draw(win, font, blinkOn);
    fieldPass.draw(win, font, blinkOn);
    btnLogin.draw(win, font, mx, my);
    btnBack.draw(win, font, mx, my);

    if (msgBuf[0]) {
        Text msg(font, msgBuf, 14);
        msg.setFillColor(msgOk ? COL_OK : COL_ERROR);
        centreTextX(msg, 0.f, 900.f);
        msg.setPosition({msg.getPosition().x, 506.f});
        win.draw(msg);
    }

    drawStatusBar(win, font, "", "");
    Text hint(font, "Tab = switch field  |  Enter = submit  |  Esc = back", 11);
    hint.setFillColor(COL_DIM);
    centreTextX(hint, 0.f, 900.f);
    hint.setPosition({hint.getPosition().x, 583.f});
    win.draw(hint);
}

int LoginUI::getRoleScreenOnSuccess() const
{
    if (role == ROLE_PATIENT) return SCREEN_PATIENT_MENU;
    if (role == ROLE_DOCTOR)  return SCREEN_DOCTOR_MENU;
    return SCREEN_ADMIN_MENU;
}
