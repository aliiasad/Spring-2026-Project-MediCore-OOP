#include "AdminUI.h"
#include "HospitalSystem.h"
#include "Validator.h"
#include "AudioManager.h"
#include <cstdio>

namespace sf {}
using namespace std;
using namespace sf;

#ifdef HMS_ENABLE_SFML
extern void playClick();
extern void playError();
#else
inline void playClick() {}
inline void playError() {}
#endif

static bool isNonNegativeFloat(const char* text)
{
    if (!text || text[0] == '\0') return false;
    bool dotSeen = false;
    bool digitSeen = false;
    for (int i = 0; text[i] != '\0'; ++i) {
        if (text[i] == '.') {
            if (dotSeen) return false;
            dotSeen = true;
        } else if (text[i] >= '0' && text[i] <= '9') {
            digitSeen = true;
        } else {
            return false;
        }
    }
    if (!digitSeen) return false;
    return Validator::toDouble(text) >= 0.0;
}

AdminUI::AdminUI()
    : addActiveField(0), addPatActiveField(0),
      patCount(0), patScroll(0), drCount(0), drScroll(0),
      apptCount(0), apptScroll(0), unpaidCount(0), unpaidScroll(0),
      logCount(0), logScroll(0), msgOk(false), blink(0)
{
    msgBuf[0] = '\0';

    addNameField.set(100.f, 110.f, 700.f, 38.f, "Full Name", false);
    addSpecField.set(100.f, 185.f, 700.f, 38.f, "Specialization", false);
    addContactField.set(100.f, 260.f, 700.f, 38.f, "Contact Number (11 digits)", false);
    addPassField.set(100.f, 335.f, 700.f, 38.f, "Password", true);
    addFeeField.set(100.f, 410.f, 700.f, 38.f, "Consultation Fee (PKR)", false);

    remDrIdField.set(300.f, 355.f, 300.f, 38.f, "Doctor ID to Remove", false);

    addPatNameField.set(100.f, 110.f, 700.f, 38.f, "Patient Name", false);
    addPatAgeField.set(100.f, 170.f, 700.f, 38.f, "Age (1-120)", false);
    addPatGenderField.set(100.f, 230.f, 700.f, 38.f, "Gender (Male/Female/Other)", false);
    addPatContactField.set(100.f, 290.f, 700.f, 38.f, "Contact Number (11 digits)", false);
    addPatPassField.set(100.f, 350.f, 700.f, 38.f, "Password", true);
    addPatBalField.set(100.f, 410.f, 700.f, 38.f, "Initial Balance (PKR)", false);

    disPatIdField.set(300.f, 345.f, 300.f, 38.f, "Patient ID to Discharge", false);

    btnBack.set(20.f, 540.f, 130.f, 34.f, "< Back (Esc)", 2);
}

void AdminUI::loadForScreen(int screen, HospitalSystem& system)
{
    msgBuf[0] = '\0';
    msgOk = false;
    if (screen == SCREEN_ADMIN_PATIENTS) {
        system.getAllPatients(patList, patCount);
        patScroll = 0;
    } else if (screen == SCREEN_ADMIN_DOCTORS) {
        system.getAllDoctors(drList, drCount);
        drScroll = 0;
    } else if (screen == SCREEN_ADMIN_APPOINTMENTS) {
        system.getAllAppointments(apptList, apptCount);
        apptScroll = 0;
    } else if (screen == SCREEN_ADMIN_UNPAID) {
        system.getUnpaidBills(unpaidList, unpaidCount);
        unpaidScroll = 0;
    } else if (screen == SCREEN_ADMIN_SECURITY_LOG) {
        system.getSecurityLog(logLines, 200, logCount);
        logScroll = 0;
    } else if (screen == SCREEN_ADMIN_ADD_DOCTOR) {
        addNameField.clear();
        addSpecField.clear();
        addContactField.clear();
        addPassField.clear();
        addFeeField.clear();
        addActiveField = 0;
    } else if (screen == SCREEN_ADMIN_REMOVE_DOCTOR) {
        system.getAllDoctors(drList, drCount);
        drScroll = 0;
        remDrIdField.clear();
    } else if (screen == SCREEN_ADMIN_DISCHARGE) {
        system.getAllPatients(patList, patCount);
        patScroll = 0;
        disPatIdField.clear();
    } else if (screen == SCREEN_ADMIN_ADD_PATIENT) {
        addPatNameField.clear();
        addPatAgeField.clear();
        addPatGenderField.clear();
        addPatContactField.clear();
        addPatPassField.clear();
        addPatBalField.clear();
        addPatActiveField = 0;
    }
}

void AdminUI::handleKey(Keyboard::Key key, int& screen, HospitalSystem& system)
{
    if (key == Keyboard::Key::Escape) {
        if (screen == SCREEN_ADMIN_MENU) screen = SCREEN_MAIN;
        else screen = SCREEN_ADMIN_MENU;
        return;
    }

    if (key == Keyboard::Key::Up) {
        if ((screen == SCREEN_ADMIN_PATIENTS || screen == SCREEN_ADMIN_DISCHARGE) && patScroll > 0)
            --patScroll;
        else if ((screen == SCREEN_ADMIN_DOCTORS || screen == SCREEN_ADMIN_REMOVE_DOCTOR) && drScroll > 0)
            --drScroll;
        else if (screen == SCREEN_ADMIN_APPOINTMENTS && apptScroll > 0) --apptScroll;
        else if (screen == SCREEN_ADMIN_UNPAID && unpaidScroll > 0) --unpaidScroll;
        else if (screen == SCREEN_ADMIN_SECURITY_LOG && logScroll > 0) --logScroll;
    }
    if (key == Keyboard::Key::Down) {
        if (screen == SCREEN_ADMIN_PATIENTS || screen == SCREEN_ADMIN_DISCHARGE) ++patScroll;
        else if (screen == SCREEN_ADMIN_DOCTORS || screen == SCREEN_ADMIN_REMOVE_DOCTOR) ++drScroll;
        else if (screen == SCREEN_ADMIN_APPOINTMENTS) ++apptScroll;
        else if (screen == SCREEN_ADMIN_UNPAID) ++unpaidScroll;
        else if (screen == SCREEN_ADMIN_SECURITY_LOG) ++logScroll;
    }

    if (key == Keyboard::Key::Tab) {
        if (screen == SCREEN_ADMIN_ADD_DOCTOR) {
            addActiveField = (addActiveField + 1) % 5;
            return;
        }
        if (screen == SCREEN_ADMIN_ADD_PATIENT) {
            addPatActiveField = (addPatActiveField + 1) % 6;
            return;
        }
    }

    if (key == Keyboard::Key::Backspace) {
        if (screen == SCREEN_ADMIN_ADD_DOCTOR) {
            if (addActiveField == 0) addNameField.backspace();
            else if (addActiveField == 1) addSpecField.backspace();
            else if (addActiveField == 2) addContactField.backspace();
            else if (addActiveField == 3) addPassField.backspace();
            else addFeeField.backspace();
        } else if (screen == SCREEN_ADMIN_REMOVE_DOCTOR) {
            remDrIdField.backspace();
        } else if (screen == SCREEN_ADMIN_DISCHARGE) {
            disPatIdField.backspace();
        } else if (screen == SCREEN_ADMIN_ADD_PATIENT) {
            if (addPatActiveField == 0) addPatNameField.backspace();
            else if (addPatActiveField == 1) addPatAgeField.backspace();
            else if (addPatActiveField == 2) addPatGenderField.backspace();
            else if (addPatActiveField == 3) addPatContactField.backspace();
            else if (addPatActiveField == 4) addPatPassField.backspace();
            else addPatBalField.backspace();
        }
        msgBuf[0] = '\0';
        return;
    }

    if (key != Keyboard::Key::Enter) return;

    if (screen == SCREEN_ADMIN_ADD_DOCTOR) {
        double fee = Validator::toDouble(addFeeField.getValue());
        bool ok = system.addDoctor(addNameField.getValue(), addSpecField.getValue(),
                                   addContactField.getValue(), addPassField.getValue(),
                                   fee, msgBuf);
        msgOk = ok;
        if (!ok) playError(); else playClick();
        if (ok) {
            addNameField.clear();
            addSpecField.clear();
            addContactField.clear();
            addPassField.clear();
            addFeeField.clear();
            addActiveField = 0;
        }
    } else if (screen == SCREEN_ADMIN_REMOVE_DOCTOR) {
        int did = Validator::toInt(remDrIdField.getValue());
        bool ok = system.removeDoctor(did, msgBuf);
        msgOk = ok;
        if (!ok) playError(); else playClick();
        if (ok) remDrIdField.clear();
    } else if (screen == SCREEN_ADMIN_DISCHARGE) {
        int pid = Validator::toInt(disPatIdField.getValue());
        bool ok = system.dischargePatient(pid, msgBuf);
        msgOk = ok;
        if (!ok) playError(); else playClick();
        if (ok) disPatIdField.clear();
    } else if (screen == SCREEN_ADMIN_ADD_PATIENT) {
        const char* gender = addPatGenderField.getValue();
        if (!Validator::strEqualIgnoreCase(gender, "Male") &&
            !Validator::strEqualIgnoreCase(gender, "Female") &&
            !Validator::strEqualIgnoreCase(gender, "Other")) {
            Validator::strCopy(msgBuf, "Gender must be Male, Female, or Other.", 128);
            msgOk = false;
            playError();
            return;
        }
        if (!isNonNegativeFloat(addPatBalField.getValue())) {
            Validator::strCopy(msgBuf, "Balance must be 0 or more.", 128);
            msgOk = false;
            playError();
            return;
        }
        int age = Validator::toInt(addPatAgeField.getValue());
        double bal = Validator::toDouble(addPatBalField.getValue());
        bool ok = system.addPatient(addPatNameField.getValue(), age,
                                    addPatGenderField.getValue(),
                                    addPatContactField.getValue(),
                                    addPatPassField.getValue(),
                                    bal, msgBuf);
        msgOk = ok;
        if (!ok) playError(); else playClick();
        if (ok) {
            addPatNameField.clear();
            addPatAgeField.clear();
            addPatGenderField.clear();
            addPatContactField.clear();
            addPatPassField.clear();
            addPatBalField.clear();
            addPatActiveField = 0;
        }
    }
}

void AdminUI::handleText(unsigned int ch, int screen)
{
    if (screen == SCREEN_ADMIN_ADD_DOCTOR) {
        if (addActiveField == 0) addNameField.append(ch);
        else if (addActiveField == 1) addSpecField.append(ch);
        else if (addActiveField == 2) addContactField.append(ch);
        else if (addActiveField == 3) addPassField.append(ch);
        else addFeeField.append(ch);
    } else if (screen == SCREEN_ADMIN_REMOVE_DOCTOR) {
        remDrIdField.append(ch);
    } else if (screen == SCREEN_ADMIN_DISCHARGE) {
        disPatIdField.append(ch);
    } else if (screen == SCREEN_ADMIN_ADD_PATIENT) {
        if (addPatActiveField == 0) addPatNameField.append(ch);
        else if (addPatActiveField == 1) addPatAgeField.append(ch);
        else if (addPatActiveField == 2) addPatGenderField.append(ch);
        else if (addPatActiveField == 3) addPatContactField.append(ch);
        else if (addPatActiveField == 4) addPatPassField.append(ch);
        else addPatBalField.append(ch);
    }
    msgBuf[0] = '\0';
}

void AdminUI::handleClick(float bx, float by, int& screen, HospitalSystem& system)
{
    if (screen == SCREEN_ADMIN_MENU) {
        const float BW=260.f,BH=45.f,YS=100.f,YG=7.f;
        const float BX[3]={20.f, 320.f, 620.f};
        const int DEST[11]={SCREEN_ADMIN_ADD_DOCTOR,SCREEN_ADMIN_REMOVE_DOCTOR,
            SCREEN_ADMIN_PATIENTS,SCREEN_ADMIN_DOCTORS,
            SCREEN_ADMIN_APPOINTMENTS,SCREEN_ADMIN_UNPAID,
            SCREEN_ADMIN_DISCHARGE,SCREEN_ADMIN_SECURITY_LOG,
            SCREEN_ADMIN_REPORT,SCREEN_ADMIN_ADD_PATIENT,-1};
        for (int i=0;i<11;++i) {
            float bxPos=BX[i%3];
            float byPos=YS+(i/3)*(BH+YG);
            if (rectHit(makeRect(bxPos,byPos,BW,BH,COL_ACCENT),(int)bx,(int)by)) {
                if (DEST[i]==-1) screen=SCREEN_MAIN;
                else screen=DEST[i];
            }
        }
        return;
    }

    if (btnBack.isHit(bx, by)) {
        screen = SCREEN_ADMIN_MENU;
        return;
    }

    if (screen == SCREEN_ADMIN_ADD_DOCTOR) {
        if (addNameField.isHit((int)bx,(int)by)) addActiveField = 0;
        else if (addSpecField.isHit((int)bx,(int)by)) addActiveField = 1;
        else if (addContactField.isHit((int)bx,(int)by)) addActiveField = 2;
        else if (addPassField.isHit((int)bx,(int)by)) addActiveField = 3;
        else if (addFeeField.isHit((int)bx,(int)by)) addActiveField = 4;
        if (rectHit(makeRect(100.f,467.f,700.f,42.f,COL_ACCENT),(int)bx,(int)by)) {
            double fee = Validator::toDouble(addFeeField.getValue());
            bool ok = system.addDoctor(addNameField.getValue(), addSpecField.getValue(),
                                       addContactField.getValue(), addPassField.getValue(),
                                       fee, msgBuf);
            msgOk = ok;
        if (!ok) playError(); else playClick();
            if (ok) {
                addNameField.clear();
                addSpecField.clear();
                addContactField.clear();
                addPassField.clear();
                addFeeField.clear();
                addActiveField = 0;
            }
        }
    } else if (screen == SCREEN_ADMIN_REMOVE_DOCTOR) {
        if (rectHit(makeRect(300.f,408.f,300.f,42.f,COL_ERROR),(int)bx,(int)by)) {
            int did = Validator::toInt(remDrIdField.getValue());
            bool ok = system.removeDoctor(did, msgBuf);
            msgOk = ok;
        if (!ok) playError(); else playClick();
            if (ok) remDrIdField.clear();
        }
    } else if (screen == SCREEN_ADMIN_DISCHARGE) {
        if (rectHit(makeRect(300.f,403.f,300.f,42.f,COL_ERROR),(int)bx,(int)by)) {
            int pid = Validator::toInt(disPatIdField.getValue());
            bool ok = system.dischargePatient(pid, msgBuf);
            msgOk = ok;
        if (!ok) playError(); else playClick();
            if (ok) disPatIdField.clear();
        }
    } else if (screen == SCREEN_ADMIN_ADD_PATIENT) {
        if (addPatNameField.isHit((int)bx,(int)by)) addPatActiveField = 0;
        else if (addPatAgeField.isHit((int)bx,(int)by)) addPatActiveField = 1;
        else if (addPatGenderField.isHit((int)bx,(int)by)) addPatActiveField = 2;
        else if (addPatContactField.isHit((int)bx,(int)by)) addPatActiveField = 3;
        else if (addPatPassField.isHit((int)bx,(int)by)) addPatActiveField = 4;
        else if (addPatBalField.isHit((int)bx,(int)by)) addPatActiveField = 5;
        if (rectHit(makeRect(100.f,467.f,700.f,42.f,COL_ACCENT),(int)bx,(int)by)) {
            const char* gender = addPatGenderField.getValue();
            if (!Validator::strEqualIgnoreCase(gender, "Male") &&
                !Validator::strEqualIgnoreCase(gender, "Female") &&
                !Validator::strEqualIgnoreCase(gender, "Other")) {
                Validator::strCopy(msgBuf, "Gender must be Male, Female, or Other.", 128);
                msgOk = false;
            playError();
                return;
            }
            if (!isNonNegativeFloat(addPatBalField.getValue())) {
                Validator::strCopy(msgBuf, "Balance must be 0 or more.", 128);
                msgOk = false;
            playError();
                return;
            }
            int age = Validator::toInt(addPatAgeField.getValue());
            double bal = Validator::toDouble(addPatBalField.getValue());
            bool ok = system.addPatient(addPatNameField.getValue(), age,
                                        addPatGenderField.getValue(),
                                        addPatContactField.getValue(),
                                        addPatPassField.getValue(),
                                        bal, msgBuf);
            msgOk = ok;
        if (!ok) playError(); else playClick();
            if (ok) {
                addPatNameField.clear();
                addPatAgeField.clear();
                addPatGenderField.clear();
                addPatContactField.clear();
                addPatPassField.clear();
                addPatBalField.clear();
                addPatActiveField = 0;
            }
        }
    }
}

void AdminUI::handleScroll(float delta, int screen)
{
    if (delta > 0.f) {
        if ((screen == SCREEN_ADMIN_PATIENTS || screen == SCREEN_ADMIN_DISCHARGE) && patScroll > 0)
            --patScroll;
        else if ((screen == SCREEN_ADMIN_DOCTORS || screen == SCREEN_ADMIN_REMOVE_DOCTOR) && drScroll > 0)
            --drScroll;
        else if (screen == SCREEN_ADMIN_APPOINTMENTS && apptScroll > 0) --apptScroll;
        else if (screen == SCREEN_ADMIN_UNPAID && unpaidScroll > 0) --unpaidScroll;
        else if (screen == SCREEN_ADMIN_SECURITY_LOG && logScroll > 0) --logScroll;
    } else if (delta < 0.f) {
        if (screen == SCREEN_ADMIN_PATIENTS || screen == SCREEN_ADMIN_DISCHARGE) ++patScroll;
        else if (screen == SCREEN_ADMIN_DOCTORS || screen == SCREEN_ADMIN_REMOVE_DOCTOR) ++drScroll;
        else if (screen == SCREEN_ADMIN_APPOINTMENTS) ++apptScroll;
        else if (screen == SCREEN_ADMIN_UNPAID) ++unpaidScroll;
        else if (screen == SCREEN_ADMIN_SECURITY_LOG) ++logScroll;
    }
}

void AdminUI::update() { ++blink; }

void AdminUI::draw(RenderWindow& win, const Font& font,
                   int screen, int mx, int my, HospitalSystem& system)
{
    if (screen == SCREEN_ADMIN_MENU) drawMenu(win, font, mx, my);
    else if (screen == SCREEN_ADMIN_ADD_DOCTOR) drawAddDoctor(win, font, mx, my);
    else if (screen == SCREEN_ADMIN_REMOVE_DOCTOR) drawRemoveDoctor(win, font, mx, my);
    else if (screen == SCREEN_ADMIN_ADD_PATIENT) drawAddPatient(win, font, mx, my);
    else if (screen == SCREEN_ADMIN_PATIENTS) drawAllPatients(win, font, mx, my, system);
    else if (screen == SCREEN_ADMIN_DOCTORS) drawAllDoctors(win, font, mx, my, system);
    else if (screen == SCREEN_ADMIN_APPOINTMENTS) drawAllAppointments(win, font, mx, my, system);
    else if (screen == SCREEN_ADMIN_UNPAID) drawUnpaidBills(win, font, mx, my, system);
    else if (screen == SCREEN_ADMIN_DISCHARGE) drawDischarge(win, font, mx, my);
    else if (screen == SCREEN_ADMIN_SECURITY_LOG) drawSecurityLog(win, font, mx, my);
    else if (screen == SCREEN_ADMIN_REPORT) drawReport(win, font, mx, my, system);
}

void AdminUI::drawMenu(RenderWindow& win, const Font& font, int mx, int my)
{
    drawHeader(win, font, "Admin Panel", "MediCore Hospital Management");

    const float BW=260.f,BH=45.f,YS=100.f,YG=7.f;
    const float BX[3]={20.f, 320.f, 620.f};
    const char* BLBL[11]={
        "1. Add Doctor","2. Remove Doctor",
        "3. View All Patients","4. View All Doctors",
        "5. View Appointments","6. Unpaid Bills",
        "7. Discharge Patient","8. Security Log",
        "9. Daily Report","11. Add Patient","12. Logout"};
    for (int i=0;i<11;++i) {
        float bxP=BX[i%3];
        float byP=YS+(i/3)*(BH+YG);
        UIButton btn;
        btn.set(bxP, byP, BW, BH, BLBL[i], i==10 ? 2 : 0);
        btn.draw(win, font, mx, my);
    }
    drawStatusBar(win, font, "Admin Panel", "Press 12 to logout");
}

void AdminUI::drawAddDoctor(RenderWindow& win, const Font& font, int mx, int my)
{
    drawHeader(win, font, "Add New Doctor", "Fill all fields and confirm");
    bool blinkOn = (blink / 30) % 2 == 0;
    addNameField.setActive(addActiveField == 0);
    addSpecField.setActive(addActiveField == 1);
    addContactField.setActive(addActiveField == 2);
    addPassField.setActive(addActiveField == 3);
    addFeeField.setActive(addActiveField == 4);
    addNameField.draw(win, font, blinkOn);
    addSpecField.draw(win, font, blinkOn);
    addContactField.draw(win, font, blinkOn);
    addPassField.draw(win, font, blinkOn);
    addFeeField.draw(win, font, blinkOn);

    btnConfirm.set(100.f, 467.f, 700.f, 42.f, "Add Doctor  (Enter)", 0);
    btnConfirm.draw(win, font, mx, my);
    btnBack.draw(win, font, mx, my);

    if (msgBuf[0]) {
        Text t(font, msgBuf, 14);
        t.setFillColor(msgOk ? COL_OK : COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,526.f});
        win.draw(t);
    }
    Text hint(font, "Tab=next field  Enter=submit", 12);
    hint.setFillColor(Color(185, 178, 155)); hint.setPosition({100.f,560.f}); win.draw(hint);
}

void AdminUI::drawRemoveDoctor(RenderWindow& win, const Font& font, int mx, int my)
{
    drawHeader(win, font, "Remove Doctor",
               "Doctors with pending appointments cannot be removed");

    const int tmpCnt = drCount;
    const int vvis=8;
    int mxOf=tmpCnt-vvis;
    if(drScroll>mxOf&&mxOf>=0) drScroll=mxOf;
    if(drScroll<0) drScroll=0;

    static const char* drhdr[]={"ID","Name","Specialization","Fee"};
    static const float drxp[]={10.f,70.f,270.f,570.f};
    static const float drwi[]={58.f,198.f,298.f,200.f};
    drawTableHeader(win,font,95.f,22.f,drhdr,4,drxp);
    for(int i=0;i<vvis;++i){
        int idx=drScroll+i;
        if(idx>=tmpCnt)break;
        const Doctor& d=drList[idx];
        char idS[8],fS[20];
        std::snprintf(idS,8,"%d",d.getId());
        Validator::formatDouble(d.getFee(),fS,20);
        const char* cols[4]={idS,d.getName(),d.getSpecialization(),fS};
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*28.f,26.f,cols,4,drxp,drwi,rc);
    }
    drawScrollBar(win,tmpCnt,vvis,drScroll,886.f,118.f,vvis*28.f);

    bool blinkOn = (blink / 30) % 2 == 0;
    remDrIdField.setActive(true);
    remDrIdField.draw(win, font, blinkOn);
    btnConfirm.set(300.f, 408.f, 300.f, 42.f, "Remove Doctor  (Enter)", 0);
    btnConfirm.draw(win, font, mx, my);
    btnBack.draw(win, font, mx, my);

    if (msgBuf[0]) {
        Text t(font, msgBuf, 14);
        t.setFillColor(msgOk ? COL_OK : COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,526.f});
        win.draw(t);
    }
}

void AdminUI::drawAddPatient(RenderWindow& win, const Font& font, int mx, int my)
{
    drawHeader(win, font, "Add New Patient", "Fill all fields and confirm");

    bool blinkOn = (blink / 30) % 2 == 0;
    addPatNameField.setActive(addPatActiveField == 0);
    addPatAgeField.setActive(addPatActiveField == 1);
    addPatGenderField.setActive(addPatActiveField == 2);
    addPatContactField.setActive(addPatActiveField == 3);
    addPatPassField.setActive(addPatActiveField == 4);
    addPatBalField.setActive(addPatActiveField == 5);

    addPatNameField.draw(win, font, blinkOn);
    addPatAgeField.draw(win, font, blinkOn);
    addPatGenderField.draw(win, font, blinkOn);
    addPatContactField.draw(win, font, blinkOn);
    addPatPassField.draw(win, font, blinkOn);
    addPatBalField.draw(win, font, blinkOn);

    btnConfirm.set(100.f, 467.f, 700.f, 42.f, "Add Patient  (Enter)", 0);
    btnConfirm.draw(win, font, mx, my);
    btnBack.draw(win, font, mx, my);

    if (msgBuf[0]) {
        Text t(font, msgBuf, 14);
        t.setFillColor(msgOk ? COL_OK : COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,526.f});
        win.draw(t);
    }
    Text hint(font, "Tab=next field  Enter=submit", 12);
    hint.setFillColor(Color(185, 178, 155)); hint.setPosition({100.f,560.f}); win.draw(hint);
}

void AdminUI::drawAllPatients(RenderWindow& win, const Font& font, int mx, int my,
                              HospitalSystem& system)
{
    (void)system;
    drawHeader(win, font, "All Patients", "Registered patients");
    const int VIS=12;
    int maxOff=patCount-VIS;
    if(patScroll>maxOff&&maxOff>=0)patScroll=maxOff;
    if(patScroll<0)patScroll=0;

    static const char* phdr[]={"ID","Name","Age","Gender","Contact","Balance"};
    static const float pxp[]={10.f,65.f,245.f,303.f,373.f,590.f};
    static const float pwi[]={53.f,178.f,56.f,68.f,215.f,200.f};
    drawTableHeader(win,font,95.f,22.f,phdr,6,pxp);
    for(int i=0;i<VIS;++i){
        int idx=patScroll+i;
        if(idx>=patCount)break;
        const Patient& p=patList[idx];
        char idS[8],ageS[8],balS[20];
        std::snprintf(idS,8,"%d",p.getId());
        std::snprintf(ageS,8,"%d",p.getAge());
        Validator::formatDouble(p.getBalance(),balS,20);
        const char* cols[6]={idS,p.getName(),ageS,p.getGender(),p.getContact(),balS};
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*32.f,30.f,cols,6,pxp,pwi,rc);
    }
    drawScrollBar(win, patCount, VIS, patScroll, 886.f, 118.f, VIS*32.f);
    btnBack.draw(win, font, mx, my);
    if(patCount==0) {
        Text t(font, "No patients registered.", 14);
        t.setFillColor(COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,300.f});
        win.draw(t);
    }
}

void AdminUI::drawAllDoctors(RenderWindow& win, const Font& font, int mx, int my,
                             HospitalSystem& system)
{
    (void)system;
    drawHeader(win, font, "All Doctors", "Registered doctors");
    const int VIS=13;
    int maxOff=drCount-VIS;
    if(drScroll>maxOff&&maxOff>=0)drScroll=maxOff;
    if(drScroll<0)drScroll=0;

    static const char* adhdr[]={"ID","Name","Specialization","Contact","Fee"};
    static const float adxp[]={10.f,65.f,270.f,570.f,730.f};
    static const float adwi[]={53.f,203.f,298.f,158.f,150.f};
    drawTableHeader(win,font,95.f,22.f,adhdr,5,adxp);
    for(int i=0;i<VIS;++i){
        int idx=drScroll+i;
        if(idx>=drCount)break;
        const Doctor& d=drList[idx];
        char idS[8],fS[20];
        std::snprintf(idS,8,"%d",d.getId());
        Validator::formatDouble(d.getFee(),fS,20);
        const char* cols[5]={idS,d.getName(),d.getSpecialization(),d.getContact(),fS};
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*31.f,29.f,cols,5,adxp,adwi,rc);
    }
    drawScrollBar(win, drCount, VIS, drScroll, 886.f, 118.f, VIS*31.f);
    btnBack.draw(win, font, mx, my);
    if(drCount==0) {
        Text t(font, "No doctors registered.", 14);
        t.setFillColor(COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,300.f});
        win.draw(t);
    }
}

void AdminUI::drawAllAppointments(RenderWindow& win, const Font& font,
                                  int mx, int my, HospitalSystem& system)
{
    drawHeader(win, font, "All Appointments", "System-wide appointments");
    const int VIS=12;
    int maxOff=apptCount-VIS;
    if(apptScroll>maxOff&&maxOff>=0)apptScroll=maxOff;
    if(apptScroll<0)apptScroll=0;

    static const char* aahdr[]={"ID","Patient","Doctor","Date","Time","Status"};
    static const float aaxp[]={10.f,65.f,248.f,430.f,540.f,628.f};
    static const float aawi[]={53.f,181.f,180.f,108.f,86.f,200.f};
    drawTableHeader(win,font,95.f,22.f,aahdr,6,aaxp);
    for(int i=0;i<VIS;++i){
        int idx=apptScroll+i;
        if(idx>=apptCount)break;
        const Appointment& a=apptList[idx];
        char idS[8]; std::snprintf(idS,8,"%d",a.getAppointmentId());
        const char* cols[6]={idS,
            system.getPatientName(a.getPatientId()),
            system.getDoctorName(a.getDoctorId()),
            a.getDate(), a.getTimeSlot(), a.getStatus()};
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*32.f,30.f,cols,6,aaxp,aawi,rc);
    }
    drawScrollBar(win, apptCount, VIS, apptScroll, 886.f, 118.f, VIS*32.f);
    btnBack.draw(win, font, mx, my);
    if(apptCount==0) {
        Text t(font, "No appointments found.", 14);
        t.setFillColor(COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,300.f});
        win.draw(t);
    }
}

void AdminUI::drawUnpaidBills(RenderWindow& win, const Font& font,
                              int mx, int my, HospitalSystem& system)
{
    drawHeader(win, font, "Unpaid Bills", "All outstanding unpaid bills");
    const int VIS=12;
    int maxOff=unpaidCount-VIS;
    if(unpaidScroll>maxOff&&maxOff>=0)unpaidScroll=maxOff;
    if(unpaidScroll<0)unpaidScroll=0;

    static const char* ubhdr[]={"Bill ID","Patient","Amount","Date"};
    static const float ubxp[]={10.f,80.f,428.f,586.f};
    static const float ubwi[]={68.f,346.f,156.f,280.f};
    drawTableHeader(win,font,95.f,22.f,ubhdr,4,ubxp);
    for(int i=0;i<VIS;++i){
        int idx=unpaidScroll+i;
        if(idx>=unpaidCount)break;
        const Bill& b=unpaidList[idx];
        char bidS[8],amtS[24];
        std::snprintf(bidS,8,"%d",b.getBillId());
        Validator::formatDouble(b.getAmount(),amtS,24);
        const char* cols[4]={bidS, system.getPatientName(b.getPatientId()), amtS, b.getDate()};
        Color rc=(i%2==0)?Color(50,15,15):Color(35,10,10);
        drawTableRow(win,font,118.f+i*32.f,30.f,cols,4,ubxp,ubwi,rc);
    }
    drawScrollBar(win, unpaidCount, VIS, unpaidScroll, 886.f, 118.f, VIS*32.f);
    btnBack.draw(win, font, mx, my);
    if(unpaidCount==0) {
        Text t(font, "No unpaid bills — all clear!", 14);
        t.setFillColor(COL_OK);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,300.f});
        win.draw(t);
    }
}

void AdminUI::drawDischarge(RenderWindow& win, const Font& font, int mx, int my)
{
    drawHeader(win, font, "Discharge Patient",
               "Patient must have no unpaid bills or pending appointments");

    const int tmpCnt = patCount;
    const int vvis=8;
    int mxOf=tmpCnt-vvis;
    if(patScroll>mxOf&&mxOf>=0)patScroll=mxOf;
    if(patScroll<0)patScroll=0;

    static const char* dphdr[]={"ID","Name","Age","Gender","Balance"};
    static const float dpxp[]={10.f,70.f,265.f,323.f,430.f};
    static const float dpwi[]={58.f,193.f,56.f,105.f,200.f};
    drawTableHeader(win,font,95.f,22.f,dphdr,5,dpxp);
    for(int i=0;i<vvis;++i){
        int idx=patScroll+i;
        if(idx>=tmpCnt)break;
        const Patient& p=patList[idx];
        char idS[8],ageS[8],balS[20];
        std::snprintf(idS,8,"%d",p.getId());
        std::snprintf(ageS,8,"%d",p.getAge());
        Validator::formatDouble(p.getBalance(),balS,20);
        const char* cols[5]={idS,p.getName(),ageS,p.getGender(),balS};
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*28.f,26.f,cols,5,dpxp,dpwi,rc);
    }
    drawScrollBar(win,tmpCnt,vvis,patScroll,886.f,118.f,vvis*28.f);

    bool blinkOn = (blink / 30) % 2 == 0;
    disPatIdField.setActive(true);
    disPatIdField.draw(win, font, blinkOn);
    btnConfirm.set(300.f, 398.f, 300.f, 42.f, "Discharge Patient  (Enter)", 0);
    btnConfirm.draw(win, font, mx, my);
    btnBack.draw(win, font, mx, my);

    if (msgBuf[0]) {
        Text t(font, msgBuf, 14);
        t.setFillColor(msgOk ? COL_OK : COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,526.f});
        win.draw(t);
    }
}

void AdminUI::drawSecurityLog(RenderWindow& win, const Font& font, int mx, int my)
{
    drawHeader(win, font, "Security Log", "System login events");
    const int VIS=13;
    int maxOff=logCount-VIS;
    if(logScroll>maxOff&&maxOff>=0)logScroll=maxOff;
    if(logScroll<0)logScroll=0;

    for(int i=0;i<VIS;++i){
        int idx=logScroll+i;
        if(idx>=logCount)break;
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        win.draw(makeRect(0.f,96.f+i*32.f,892.f,30.f,rc));
        Text t(font, logLines[idx], 13);
        t.setFillColor(Color(255, 248, 220));
        t.setPosition({8.f,99.f+i*32.f});
        win.draw(t);
    }
    drawScrollBar(win, logCount, VIS, logScroll, 886.f, 96.f, VIS*32.f);
    btnBack.draw(win, font, mx, my);
    if(logCount==0) {
        Text t(font, "No security events logged.", 14);
        t.setFillColor(COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,300.f});
        win.draw(t);
    }
}

void AdminUI::drawReport(RenderWindow& win, const Font& font,
                         int mx, int my, HospitalSystem& system)
{
    drawHeader(win, font, "Daily Report", "System summary");

    Patient tmpPats[100]; int nPats=0;
    Doctor  tmpDocs[100]; int nDocs=0;
    Appointment tmpApps[100]; int nApps=0;
    Bill    tmpUnpd[100]; int nUnpd=0;
    system.getAllPatients(tmpPats,nPats);
    system.getAllDoctors(tmpDocs,nDocs);
    system.getAllAppointments(tmpApps,nApps);
    system.getUnpaidBills(tmpUnpd,nUnpd);
    double revenue=system.getTotalRevenue();

    char today[11]; Validator::getTodayDate(today);
    int todayAppts=0;
    for(int i=0;i<nApps;++i)
        if(Validator::strEqual(tmpApps[i].getDate(),today)) ++todayAppts;

    double unpaidTotal=0.0;
    for(int i=0;i<nUnpd;++i) unpaidTotal+=tmpUnpd[i].getAmount();

    char revBuf[32],upBuf[32];
    Validator::formatDouble(revenue,revBuf,32);
    Validator::formatDouble(unpaidTotal,upBuf,32);

    const float LX=100.f,RX=520.f,YS=115.f,YG=52.f;
    const char* rowLabels[6]={
        "Total Patients","Total Doctors","Appointments Today",
        "Unpaid Bills","Outstanding Amount (PKR)","Total Revenue Collected (PKR)"
    };
    char rowVals[6][32];
    std::snprintf(rowVals[0],32,"%d",nPats);
    std::snprintf(rowVals[1],32,"%d",nDocs);
    std::snprintf(rowVals[2],32,"%d",todayAppts);
    std::snprintf(rowVals[3],32,"%d",nUnpd);
    Validator::strCopy(rowVals[4],upBuf,32);
    Validator::strCopy(rowVals[5],revBuf,32);
    Color rowColors[6]={
        COL_WHITE, COL_WHITE,
        COL_GOLD,
        nUnpd>0?COL_ERROR:COL_OK,
        unpaidTotal>0?COL_ERROR:COL_OK,
        COL_OK
    };

    for(int r=0;r<6;++r){
        float y=YS+r*YG;
        win.draw(makeRect(LX-12.f,y-6.f,784.f,46.f,COL_SURFACE));
        win.draw(makeRect(LX-12.f,y-6.f,3.f,46.f,COL_GOLD_DIM));
        Text lbl(font,rowLabels[r],15);
        lbl.setFillColor(Color(190, 182, 158)); lbl.setPosition({LX,y+7.f}); win.draw(lbl);
        const Font& df2=g_displayFontLoaded?g_displayFont:font;
        Text vt(df2,rowVals[r],18);
        vt.setFillColor(rowColors[r]); vt.setPosition({RX,y+5.f}); win.draw(vt);
    }
    btnBack.draw(win, font, mx, my);
}
