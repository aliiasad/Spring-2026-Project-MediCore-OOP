#include "DoctorUI.h"
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

DoctorUI::DoctorUI()
    : doctorId(0), todayCount(0), todayScroll(0),
      prescActiveField(0), histCount(0), histScroll(0), histLoaded(false),
      msgOk(false), blink(0)
{
    msgBuf[0] = '\0';
    apptIdField.set(300.f, 320.f, 300.f, 38.f, "Appointment ID", false);

    prescApptField.set(100.f, 130.f, 700.f, 38.f,
                       "Appointment ID (completed)", false);
    prescMedField.set(100.f, 220.f, 700.f, 80.f,
                      "Medicines (comma separated)", false);
    prescNotesField.set(100.f, 340.f, 700.f, 70.f,
                        "Notes / Instructions", false);

    histPidField.set(100.f, 130.f, 480.f, 38.f, "Patient ID", false);

    btnBack.set(20.f, 540.f, 130.f, 34.f, "< Back (Esc)", 2);
}

void DoctorUI::setDoctorId(int id) { doctorId = id; }

void DoctorUI::loadForScreen(int screen, HospitalSystem& system)
{
    msgBuf[0] = '\0';
    msgOk = false;
    if (screen == SCREEN_DOCTOR_TODAY) {
        system.getDoctorTodayAppointments(doctorId, todayList, todayCount);
        todayScroll = 0;
    } else if (screen == SCREEN_DOCTOR_COMPLETE || screen == SCREEN_DOCTOR_NOSHOW) {
        system.getDoctorTodayAppointments(doctorId, todayList, todayCount);
        apptIdField.clear();
        todayScroll = 0;
    } else if (screen == SCREEN_DOCTOR_PRESCRIBE) {
        prescApptField.clear();
        prescMedField.clear();
        prescNotesField.clear();
        prescActiveField = 0;
    } else if (screen == SCREEN_DOCTOR_HISTORY) {
        histPidField.clear();
        histLoaded = false;
        histCount = 0;
        histScroll = 0;
    }
}

void DoctorUI::handleKey(Keyboard::Key key, int& screen, HospitalSystem& system)
{
    if (key == Keyboard::Key::Escape) {
        if (screen == SCREEN_DOCTOR_MENU) {
            screen = SCREEN_MAIN;
            doctorId = 0;
        } else {
            screen = SCREEN_DOCTOR_MENU;
        }
        return;
    }

    if (key == Keyboard::Key::Up) {
        if ((screen == SCREEN_DOCTOR_TODAY || screen == SCREEN_DOCTOR_COMPLETE ||
             screen == SCREEN_DOCTOR_NOSHOW) && todayScroll > 0) --todayScroll;
        else if (screen == SCREEN_DOCTOR_HISTORY && histScroll > 0) --histScroll;
    }
    if (key == Keyboard::Key::Down) {
        if (screen == SCREEN_DOCTOR_TODAY || screen == SCREEN_DOCTOR_COMPLETE ||
            screen == SCREEN_DOCTOR_NOSHOW) ++todayScroll;
        else if (screen == SCREEN_DOCTOR_HISTORY) ++histScroll;
    }

    if (key == Keyboard::Key::Tab && screen == SCREEN_DOCTOR_PRESCRIBE) {
        prescActiveField = (prescActiveField + 1) % 3;
        return;
    }

    if (key == Keyboard::Key::Backspace) {
        if (screen == SCREEN_DOCTOR_COMPLETE || screen == SCREEN_DOCTOR_NOSHOW) {
            apptIdField.backspace();
        } else if (screen == SCREEN_DOCTOR_PRESCRIBE) {
            if (prescActiveField == 0) prescApptField.backspace();
            else if (prescActiveField == 1) prescMedField.backspace();
            else prescNotesField.backspace();
        } else if (screen == SCREEN_DOCTOR_HISTORY) {
            histPidField.backspace();
        }
        msgBuf[0] = '\0';
        return;
    }

    if (key != Keyboard::Key::Enter) return;

    if (screen == SCREEN_DOCTOR_COMPLETE) {
        int aid = Validator::toInt(apptIdField.getValue());
        bool ok = system.updateAppointmentStatus(doctorId, aid, "completed", msgBuf);
        msgOk = ok;
        if (!ok) playError(); else playClick();
        if (ok) {
            apptIdField.clear();
            system.getDoctorTodayAppointments(doctorId, todayList, todayCount);
        }
    } else if (screen == SCREEN_DOCTOR_NOSHOW) {
        int aid = Validator::toInt(apptIdField.getValue());
        bool ok = system.updateAppointmentStatus(doctorId, aid, "no-show", msgBuf);
        msgOk = ok;
        if (!ok) playError(); else playClick();
        if (ok) {
            apptIdField.clear();
            system.getDoctorTodayAppointments(doctorId, todayList, todayCount);
        }
    } else if (screen == SCREEN_DOCTOR_PRESCRIBE) {
        int aid = Validator::toInt(prescApptField.getValue());
        bool ok = system.writePrescription(doctorId, aid,
                                           prescMedField.getValue(),
                                           prescNotesField.getValue(), msgBuf);
        msgOk = ok;
        if (!ok) playError(); else playClick();
        if (ok) {
            prescApptField.clear();
            prescMedField.clear();
            prescNotesField.clear();
        }
    } else if (screen == SCREEN_DOCTOR_HISTORY) {
        int pid = Validator::toInt(histPidField.getValue());
        system.getDoctorPatientHistory(doctorId, pid, histList, histCount);
        histLoaded = true;
        histScroll = 0;
        if (histCount == 0) {
            Validator::strCopy(msgBuf, "No prescription history found.", 128);
            msgOk = false;
            playError();
        } else {
            msgBuf[0] = '\0';
        }
    }
}

void DoctorUI::handleText(unsigned int ch, int screen)
{
    if (screen == SCREEN_DOCTOR_COMPLETE || screen == SCREEN_DOCTOR_NOSHOW) {
        apptIdField.append(ch);
    } else if (screen == SCREEN_DOCTOR_PRESCRIBE) {
        if (prescActiveField == 0) prescApptField.append(ch);
        else if (prescActiveField == 1) prescMedField.append(ch);
        else prescNotesField.append(ch);
    } else if (screen == SCREEN_DOCTOR_HISTORY) {
        histPidField.append(ch);
    }
    msgBuf[0] = '\0';
}

void DoctorUI::handleClick(float bx, float by, int& screen, HospitalSystem& system)
{
    if (screen == SCREEN_DOCTOR_MENU) {
        const float BW=400.f,BH=46.f,BX=(900.f-BW)*0.5f,YS=120.f,YG=8.f;
        const int DEST[6]={SCREEN_DOCTOR_TODAY,SCREEN_DOCTOR_COMPLETE,
            SCREEN_DOCTOR_NOSHOW,SCREEN_DOCTOR_PRESCRIBE,
            SCREEN_DOCTOR_HISTORY,-1};
        for (int i=0;i<6;++i) {
            if (rectHit(makeRect(BX,YS+i*(BH+YG),BW,BH,COL_ACCENT),(int)bx,(int)by)) {
                if (DEST[i]==-1){screen=SCREEN_MAIN;doctorId=0;}
                else screen=DEST[i];
            }
        }
        return;
    }

    if (btnBack.isHit(bx, by)) {
        screen = SCREEN_DOCTOR_MENU;
        return;
    }

    if (screen == SCREEN_DOCTOR_COMPLETE) {
        if (rectHit(makeRect(300.f,362.f,300.f,42.f,COL_OK),(int)bx,(int)by)) {
            int aid = Validator::toInt(apptIdField.getValue());
            bool ok = system.updateAppointmentStatus(doctorId, aid, "completed", msgBuf);
            msgOk = ok;
        if (!ok) playError(); else playClick();
            if (ok) {
                apptIdField.clear();
                system.getDoctorTodayAppointments(doctorId, todayList, todayCount);
            }
        }
    } else if (screen == SCREEN_DOCTOR_NOSHOW) {
        if (rectHit(makeRect(300.f,362.f,300.f,42.f,COL_ERROR),(int)bx,(int)by)) {
            int aid = Validator::toInt(apptIdField.getValue());
            bool ok = system.updateAppointmentStatus(doctorId, aid, "no-show", msgBuf);
            msgOk = ok;
        if (!ok) playError(); else playClick();
            if (ok) {
                apptIdField.clear();
                system.getDoctorTodayAppointments(doctorId, todayList, todayCount);
            }
        }
    } else if (screen == SCREEN_DOCTOR_PRESCRIBE) {
        if (prescApptField.isHit((int)bx,(int)by)) prescActiveField = 0;
        else if (prescMedField.isHit((int)bx,(int)by)) prescActiveField = 1;
        else if (prescNotesField.isHit((int)bx,(int)by)) prescActiveField = 2;
        if (rectHit(makeRect(100.f,430.f,700.f,42.f,COL_ACCENT),(int)bx,(int)by)) {
            int aid = Validator::toInt(prescApptField.getValue());
            bool ok = system.writePrescription(doctorId, aid,
                                               prescMedField.getValue(),
                                               prescNotesField.getValue(), msgBuf);
            msgOk = ok;
        if (!ok) playError(); else playClick();
            if (ok) {
                prescApptField.clear();
                prescMedField.clear();
                prescNotesField.clear();
            }
        }
    } else if (screen == SCREEN_DOCTOR_HISTORY) {
        if (rectHit(makeRect(600.f,130.f,180.f,38.f,COL_ACCENT),(int)bx,(int)by)) {
            int pid = Validator::toInt(histPidField.getValue());
            system.getDoctorPatientHistory(doctorId, pid, histList, histCount);
            histLoaded = true;
            histScroll = 0;
            if (histCount == 0) {
                Validator::strCopy(msgBuf, "No prescription history found.", 128);
                msgOk = false;
            playError();
            } else {
                msgBuf[0] = '\0';
            }
        }
    }
}

void DoctorUI::handleScroll(float delta, int screen)
{
    if (delta > 0.f) {
        if ((screen == SCREEN_DOCTOR_TODAY || screen == SCREEN_DOCTOR_COMPLETE ||
             screen == SCREEN_DOCTOR_NOSHOW) && todayScroll > 0) --todayScroll;
        else if (screen == SCREEN_DOCTOR_HISTORY && histScroll > 0) --histScroll;
    } else if (delta < 0.f) {
        if (screen == SCREEN_DOCTOR_TODAY || screen == SCREEN_DOCTOR_COMPLETE ||
            screen == SCREEN_DOCTOR_NOSHOW) ++todayScroll;
        else if (screen == SCREEN_DOCTOR_HISTORY) ++histScroll;
    }
}

void DoctorUI::update() { ++blink; }

void DoctorUI::draw(RenderWindow& win, const Font& font,
                    int screen, int mx, int my, HospitalSystem& system)
{
    if (screen == SCREEN_DOCTOR_MENU) drawMenu(win, font, mx, my, system);
    else if (screen == SCREEN_DOCTOR_TODAY) drawToday(win, font, mx, my, system);
    else if (screen == SCREEN_DOCTOR_COMPLETE || screen == SCREEN_DOCTOR_NOSHOW)
        drawCompleteNoShow(win, font, screen, mx, my, system);
    else if (screen == SCREEN_DOCTOR_PRESCRIBE) drawPrescribe(win, font, mx, my);
    else if (screen == SCREEN_DOCTOR_HISTORY) drawHistory(win, font, mx, my, system);
}

void DoctorUI::drawMenu(RenderWindow& win, const Font& font,
                        int mx, int my, HospitalSystem& system)
{
    char sub[64];
    std::snprintf(sub,64,"Dr. %s", system.getDoctorName(doctorId));
    drawHeader(win, font, "Doctor Dashboard", sub);

    const float BW=400.f,BH=46.f,BX=(900.f-BW)*0.5f,YS=120.f,YG=8.f;
    const char* BLBL[6]={
        "1. View Today's Appointments",
        "2. Mark Appointment Complete",
        "3. Mark Appointment No-Show",
        "4. Write Prescription",
        "5. View Patient History",
        "6. Logout"};
    for (int i=0;i<6;++i) {
        UIButton btn;
        btn.set(BX,YS+i*(BH+YG),BW,BH,BLBL[i],i==5?2:0);
        btn.draw(win, font, mx, my);
    }
    drawStatusBar(win, font, "Doctor Menu", "Press 6 to logout");
}

void DoctorUI::drawToday(RenderWindow& win, const Font& font, int mx, int my,
                         HospitalSystem& system)
{
    drawHeader(win, font, "Today's Appointments",
               "Appointments scheduled for today");
    const int VIS=13;
    int maxOff=todayCount-VIS;
    if(todayScroll>maxOff && maxOff>=0) todayScroll=maxOff;
    if(todayScroll<0) todayScroll=0;

    static const char* thdr[]={"Appt ID","Patient","Time","Status"};
    static const float txp[]={10.f,90.f,390.f,498.f};
    static const float twi[]={78.f,298.f,106.f,250.f};
    drawTableHeader(win,font,95.f,22.f,thdr,4,txp);
    for(int i=0;i<VIS;++i){
        int idx=todayScroll+i;
        if(idx>=todayCount)break;
        const Appointment& a=todayList[idx];
        char idS[8]; std::snprintf(idS,8,"%d",a.getAppointmentId());
        const char* cols[4]={idS, system.getPatientName(a.getPatientId()),
            a.getTimeSlot(), a.getStatus()};
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*31.f,29.f,cols,4,txp,twi,rc);
    }
    drawScrollBar(win, todayCount, VIS, todayScroll, 886.f, 118.f, VIS*31.f);
    btnBack.draw(win, font, mx, my);
    if(todayCount==0) {
        Text t(font, "No appointments today.", 14);
        t.setFillColor(COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,300.f});
        win.draw(t);
    }
}

void DoctorUI::drawCompleteNoShow(RenderWindow& win, const Font& font,
                                  int screen, int mx, int my, HospitalSystem& system)
{
    bool isC=(screen==SCREEN_DOCTOR_COMPLETE);
    drawHeader(win, font,
               isC?"Mark Appointment Complete":"Mark Appointment No-Show",
               "Today's pending appointments listed below");
    const int VIS=7;
    int maxOff=todayCount-VIS;
    if(todayScroll>maxOff && maxOff>=0) todayScroll=maxOff;
    if(todayScroll<0) todayScroll=0;

    static const char* t2hdr[]={"Appt ID","Patient","Time","Status"};
    static const float t2xp[]={10.f,90.f,390.f,498.f};
    static const float t2wi[]={78.f,298.f,106.f,250.f};
    drawTableHeader(win,font,95.f,22.f,t2hdr,4,t2xp);
    for(int i=0;i<VIS;++i){
        int idx=todayScroll+i;
        if(idx>=todayCount)break;
        const Appointment& a=todayList[idx];
        char idS[8]; std::snprintf(idS,8,"%d",a.getAppointmentId());
        const char* cols[4]={idS, system.getPatientName(a.getPatientId()),
            a.getTimeSlot(), a.getStatus()};
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*31.f,29.f,cols,4,t2xp,t2wi,rc);
    }
    drawScrollBar(win, todayCount, VIS, todayScroll, 886.f, 118.f, VIS*31.f);

    bool blinkOn = (blink / 30) % 2 == 0;
    apptIdField.setActive(true);
    apptIdField.draw(win, font, blinkOn);
    btnConfirm.set(300.f, 372.f, 300.f, 42.f,
                   isC?"Mark Complete  (Enter)":"Mark No-Show  (Enter)", 0);
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

void DoctorUI::drawPrescribe(RenderWindow& win, const Font& font, int mx, int my)
{
    drawHeader(win, font, "Write Prescription",
               "Appointment must be marked completed first");

    bool blinkOn = (blink / 30) % 2 == 0;
    prescApptField.setActive(prescActiveField == 0);
    prescMedField.setActive(prescActiveField == 1);
    prescNotesField.setActive(prescActiveField == 2);
    prescApptField.draw(win, font, blinkOn);
    prescMedField.draw(win, font, blinkOn);
    prescNotesField.draw(win, font, blinkOn);

    btnConfirm.set(100.f, 430.f, 700.f, 42.f, "Save Prescription  (Enter)", 0);
    btnConfirm.draw(win, font, mx, my);
    btnBack.draw(win, font, mx, my);

    if (msgBuf[0]) {
        Text t(font, msgBuf, 14);
        t.setFillColor(msgOk ? COL_OK : COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,526.f});
        win.draw(t);
    }
    Text hint(font, "Tab=next field  Enter=save  Esc=back", 12);
    hint.setFillColor(Color(185, 178, 155)); hint.setPosition({100.f,560.f}); win.draw(hint);
}

void DoctorUI::drawHistory(RenderWindow& win, const Font& font,
                           int mx, int my, HospitalSystem& system)
{
    (void)system;
    drawHeader(win, font, "Patient Prescription History",
               "View prescriptions you have written for a patient");

    bool blinkOn = (blink / 30) % 2 == 0;
    histPidField.setActive(!histLoaded);
    histPidField.draw(win, font, blinkOn);
    btnConfirm.set(600.f, 130.f, 180.f, 38.f, "Load  (Enter)", 0);
    btnConfirm.draw(win, font, mx, my);

    if (histLoaded && histCount > 0) {
        const int VIS=9;
        int maxOff=histCount-VIS;
        if(histScroll>maxOff && maxOff>=0) histScroll=maxOff;
        if(histScroll<0) histScroll=0;
        static const char* hhdr[]={"Date","Medicines","Notes"};
        static const float hxp[]={10.f,120.f,530.f};
        static const float hwi[]={108.f,408.f,350.f};
        drawTableHeader(win,font,185.f,22.f,hhdr,3,hxp);
        for(int i=0;i<VIS;++i){
            int idx=histScroll+i;
            if(idx>=histCount)break;
            const Prescription& pr=histList[idx];
            const char* cols[3]={pr.getDate(), pr.getMedicines(), pr.getNotes()};
            Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
            drawTableRow(win,font,208.f+i*31.f,29.f,cols,3,hxp,hwi,rc);
        }
        drawScrollBar(win, histCount, VIS, histScroll, 886.f, 208.f, VIS*31.f);
    }

    btnBack.draw(win, font, mx, my);
    if (msgBuf[0]) {
        Text t(font, msgBuf, 14);
        t.setFillColor(msgOk ? COL_OK : COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,526.f});
        win.draw(t);
    }
}
