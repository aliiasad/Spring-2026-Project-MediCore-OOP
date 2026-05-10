#include "PatientUI.h"
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

PatientUI::PatientUI()
    : patientId(0), doctorCount(0), bookActiveField(0), bookScroll(0),
      apptCount(0), apptScroll(0), viewApptScroll(0),
      rxCount(0), rxScroll(0), billCount(0), billScroll(0),
      msgOk(false), blink(0)
{
    msgBuf[0] = '\0';
    bookDrIdField.set(490.f, 130.f, 390.f, 38.f, "Doctor ID", false);
    bookDateField.set(490.f, 205.f, 390.f, 38.f, "Date (DD-MM-YYYY)", false);
    bookSlotField.set(490.f, 280.f, 390.f, 38.f, "Time Slot (09:00 - 16:00)", false);

    cancelIdField.set(300.f, 380.f, 300.f, 38.f,
                      "Appointment ID to Cancel", false);
    payBillIdField.set(300.f, 375.f, 300.f, 38.f, "Bill ID to Pay", false);
    topupAmtField.set(300.f, 215.f, 300.f, 42.f, "Amount to Add (PKR)", false);

    btnBack.set(20.f, 540.f, 130.f, 34.f, "< Back (Esc)", 2);
}

void PatientUI::setPatientId(int id) { patientId = id; }

void PatientUI::loadForScreen(int screen, HospitalSystem& system)
{
    msgBuf[0] = '\0';
    msgOk = false;
    if (screen == SCREEN_PATIENT_BOOK) {
        system.getAllDoctors(doctorList, doctorCount);
        bookDrIdField.clear();
        bookDateField.clear();
        bookSlotField.clear();
        bookActiveField = 0;
        bookScroll = 0;
    } else if (screen == SCREEN_PATIENT_CANCEL) {
        system.getPatientAppointments(patientId, apptList, apptCount);
        cancelIdField.clear();
        apptScroll = 0;
    } else if (screen == SCREEN_PATIENT_APPOINTMENTS) {
        system.getPatientAppointments(patientId, apptList, apptCount);
        viewApptScroll = 0;
    } else if (screen == SCREEN_PATIENT_RECORDS) {
        system.getPatientPrescriptions(patientId, rxList, rxCount);
        rxScroll = 0;
    } else if (screen == SCREEN_PATIENT_BILLS) {
        system.getPatientBills(patientId, billList, billCount);
        billScroll = 0;
    } else if (screen == SCREEN_PATIENT_PAYBILL) {
        system.getPatientBills(patientId, billList, billCount);
        payBillIdField.clear();
        billScroll = 0;
    } else if (screen == SCREEN_PATIENT_TOPUP) {
        topupAmtField.clear();
    }
}

void PatientUI::handleKey(Keyboard::Key key, int& screen, HospitalSystem& system)
{
    if (key == Keyboard::Key::Escape) {
        if (screen == SCREEN_PATIENT_MENU) {
            screen = SCREEN_MAIN;
            patientId = 0;
        } else {
            screen = SCREEN_PATIENT_MENU;
        }
        return;
    }

    if (key == Keyboard::Key::Up) {
        if (screen == SCREEN_PATIENT_BOOK && bookScroll > 0) --bookScroll;
        else if (screen == SCREEN_PATIENT_CANCEL && apptScroll > 0) --apptScroll;
        else if (screen == SCREEN_PATIENT_APPOINTMENTS && viewApptScroll > 0) --viewApptScroll;
        else if (screen == SCREEN_PATIENT_RECORDS && rxScroll > 0) --rxScroll;
        else if (screen == SCREEN_PATIENT_BILLS && billScroll > 0) --billScroll;
        else if (screen == SCREEN_PATIENT_PAYBILL && billScroll > 0) --billScroll;
    }
    if (key == Keyboard::Key::Down) {
        if (screen == SCREEN_PATIENT_BOOK) ++bookScroll;
        else if (screen == SCREEN_PATIENT_CANCEL) ++apptScroll;
        else if (screen == SCREEN_PATIENT_APPOINTMENTS) ++viewApptScroll;
        else if (screen == SCREEN_PATIENT_RECORDS) ++rxScroll;
        else if (screen == SCREEN_PATIENT_BILLS) ++billScroll;
        else if (screen == SCREEN_PATIENT_PAYBILL) ++billScroll;
    }

    if (key == Keyboard::Key::Tab && screen == SCREEN_PATIENT_BOOK) {
        bookActiveField = (bookActiveField + 1) % 3;
        return;
    }

    if (key == Keyboard::Key::Backspace) {
        if (screen == SCREEN_PATIENT_BOOK) {
            if (bookActiveField == 0) bookDrIdField.backspace();
            else if (bookActiveField == 1) bookDateField.backspace();
            else bookSlotField.backspace();
        } else if (screen == SCREEN_PATIENT_CANCEL) {
            cancelIdField.backspace();
        } else if (screen == SCREEN_PATIENT_PAYBILL) {
            payBillIdField.backspace();
        } else if (screen == SCREEN_PATIENT_TOPUP) {
            topupAmtField.backspace();
        }
        msgBuf[0] = '\0';
        return;
    }

    if (key != Keyboard::Key::Enter) return;

    if (screen == SCREEN_PATIENT_BOOK) {
        int dId = Validator::toInt(bookDrIdField.getValue());
        bool ok = system.bookAppointment(patientId, dId,
                                         bookDateField.getValue(),
                                         bookSlotField.getValue(), msgBuf);
        msgOk = ok;
        if (!ok) playError(); else playClick();
        if (ok) {
            bookDrIdField.clear();
            bookDateField.clear();
            bookSlotField.clear();
            system.getAllDoctors(doctorList, doctorCount);
        }
    } else if (screen == SCREEN_PATIENT_CANCEL) {
        int aid = Validator::toInt(cancelIdField.getValue());
        bool ok = system.cancelAppointment(patientId, aid, msgBuf);
        msgOk = ok;
        if (!ok) playError(); else playClick();
        if (ok) {
            cancelIdField.clear();
            system.getPatientAppointments(patientId, apptList, apptCount);
        }
    } else if (screen == SCREEN_PATIENT_PAYBILL) {
        int bid = Validator::toInt(payBillIdField.getValue());
        bool ok = system.payBill(patientId, bid, msgBuf);
        msgOk = ok;
        if (!ok) playError(); else playClick();
        if (ok) {
            payBillIdField.clear();
            system.getPatientBills(patientId, billList, billCount);
        }
    } else if (screen == SCREEN_PATIENT_TOPUP) {
        double amt = Validator::toDouble(topupAmtField.getValue());
        bool ok = system.topUpBalance(patientId, amt, msgBuf);
        msgOk = ok;
        if (!ok) playError(); else playClick();
        if (ok) topupAmtField.clear();
    }
}

void PatientUI::handleText(unsigned int ch, int screen)
{
    if (screen == SCREEN_PATIENT_BOOK) {
        if (bookActiveField == 0) bookDrIdField.append(ch);
        else if (bookActiveField == 1) bookDateField.append(ch);
        else bookSlotField.append(ch);
    } else if (screen == SCREEN_PATIENT_CANCEL) {
        cancelIdField.append(ch);
    } else if (screen == SCREEN_PATIENT_PAYBILL) {
        payBillIdField.append(ch);
    } else if (screen == SCREEN_PATIENT_TOPUP) {
        topupAmtField.append(ch);
    }
    msgBuf[0] = '\0';
}

void PatientUI::handleClick(float bx, float by, int& screen, HospitalSystem& system)
{
    if (screen == SCREEN_PATIENT_MENU) {
        const float BW=400.f,BH=46.f,BX=(900.f-BW)*0.5f,YS=104.f,YG=6.f;
        const int DEST[8]={SCREEN_PATIENT_BOOK,SCREEN_PATIENT_CANCEL,
            SCREEN_PATIENT_APPOINTMENTS,SCREEN_PATIENT_RECORDS,
            SCREEN_PATIENT_BILLS,SCREEN_PATIENT_PAYBILL,
            SCREEN_PATIENT_TOPUP,-1};
        for (int i=0;i<8;++i) {
            if (rectHit(makeRect(BX,YS+i*(BH+YG),BW,BH,COL_ACCENT),(int)bx,(int)by)) {
                if (DEST[i]==-1){screen=SCREEN_MAIN;patientId=0;}
                else screen=DEST[i];
            }
        }
        return;
    }

    if (btnBack.isHit(bx, by)) {
        screen = SCREEN_PATIENT_MENU;
        return;
    }

    if (screen == SCREEN_PATIENT_BOOK) {
        if (bookDrIdField.isHit((int)bx,(int)by)) bookActiveField = 0;
        else if (bookDateField.isHit((int)bx,(int)by)) bookActiveField = 1;
        else if (bookSlotField.isHit((int)bx,(int)by)) bookActiveField = 2;
        if (rectHit(makeRect(490.f,340.f,390.f,42.f,COL_ACCENT),(int)bx,(int)by)) {
            int dId = Validator::toInt(bookDrIdField.getValue());
            bool ok = system.bookAppointment(patientId, dId,
                                             bookDateField.getValue(),
                                             bookSlotField.getValue(), msgBuf);
            msgOk = ok;
        if (!ok) playError(); else playClick();
            if (ok) {
                bookDrIdField.clear();
                bookDateField.clear();
                bookSlotField.clear();
                system.getAllDoctors(doctorList, doctorCount);
            }
        }
    } else if (screen == SCREEN_PATIENT_CANCEL) {
        if (rectHit(makeRect(340.f,430.f,220.f,42.f,COL_ERROR),(int)bx,(int)by)) {
            int aid = Validator::toInt(cancelIdField.getValue());
            bool ok = system.cancelAppointment(patientId, aid, msgBuf);
            msgOk = ok;
        if (!ok) playError(); else playClick();
            if (ok) {
                cancelIdField.clear();
                system.getPatientAppointments(patientId, apptList, apptCount);
            }
        }
    } else if (screen == SCREEN_PATIENT_PAYBILL) {
        if (rectHit(makeRect(300.f,430.f,300.f,42.f,COL_ACCENT),(int)bx,(int)by)) {
            int bid = Validator::toInt(payBillIdField.getValue());
            bool ok = system.payBill(patientId, bid, msgBuf);
            msgOk = ok;
        if (!ok) playError(); else playClick();
            if (ok) {
                payBillIdField.clear();
                system.getPatientBills(patientId, billList, billCount);
            }
        }
    } else if (screen == SCREEN_PATIENT_TOPUP) {
        if (rectHit(makeRect(300.f,275.f,300.f,42.f,COL_ACCENT),(int)bx,(int)by)) {
            double amt = Validator::toDouble(topupAmtField.getValue());
            bool ok = system.topUpBalance(patientId, amt, msgBuf);
            msgOk = ok;
        if (!ok) playError(); else playClick();
            if (ok) topupAmtField.clear();
        }
    }
}

void PatientUI::handleScroll(float delta, int screen)
{
    if (delta > 0.f) {
        if (screen == SCREEN_PATIENT_BOOK && bookScroll > 0) --bookScroll;
        else if (screen == SCREEN_PATIENT_CANCEL && apptScroll > 0) --apptScroll;
        else if (screen == SCREEN_PATIENT_APPOINTMENTS && viewApptScroll > 0) --viewApptScroll;
        else if (screen == SCREEN_PATIENT_RECORDS && rxScroll > 0) --rxScroll;
        else if (screen == SCREEN_PATIENT_BILLS && billScroll > 0) --billScroll;
        else if (screen == SCREEN_PATIENT_PAYBILL && billScroll > 0) --billScroll;
    } else if (delta < 0.f) {
        if (screen == SCREEN_PATIENT_BOOK) ++bookScroll;
        else if (screen == SCREEN_PATIENT_CANCEL) ++apptScroll;
        else if (screen == SCREEN_PATIENT_APPOINTMENTS) ++viewApptScroll;
        else if (screen == SCREEN_PATIENT_RECORDS) ++rxScroll;
        else if (screen == SCREEN_PATIENT_BILLS) ++billScroll;
        else if (screen == SCREEN_PATIENT_PAYBILL) ++billScroll;
    }
}

void PatientUI::update() { ++blink; }

void PatientUI::draw(RenderWindow& win, const Font& font,
                     int screen, int mx, int my, HospitalSystem& system)
{
    if (screen == SCREEN_PATIENT_MENU) drawMenu(win, font, mx, my, system);
    else if (screen == SCREEN_PATIENT_BOOK) drawBook(win, font, mx, my);
    else if (screen == SCREEN_PATIENT_CANCEL) drawCancel(win, font, mx, my, system);
    else if (screen == SCREEN_PATIENT_APPOINTMENTS) drawAppointments(win, font, mx, my, system);
    else if (screen == SCREEN_PATIENT_RECORDS) drawRecords(win, font, mx, my, system);
    else if (screen == SCREEN_PATIENT_BILLS) drawBills(win, font, mx, my);
    else if (screen == SCREEN_PATIENT_PAYBILL) drawPayBill(win, font, mx, my);
    else if (screen == SCREEN_PATIENT_TOPUP) drawTopUp(win, font, mx, my, system);
}

void PatientUI::drawMenu(RenderWindow& win, const Font& font, int mx, int my,
                         HospitalSystem& system)
{
    char sub[128], bal[32];
    Validator::formatDouble(system.getPatientBalance(patientId), bal, 32);
    std::snprintf(sub, 128, "Welcome, %s  |  Balance: PKR %s",
                  system.getPatientName(patientId), bal);
    drawHeader(win, font, "Patient Dashboard", sub);

    const float BW=400.f,BH=46.f,BX=(900.f-BW)*0.5f,YS=104.f,YG=6.f;
    const char* BLBL[8]={
        "1. Book Appointment","2. Cancel Appointment",
        "3. View My Appointments","4. View Medical Records",
        "5. View Bills","6. Pay Bill",
        "7. Top Up Balance","8. Logout"};
    for (int i=0;i<8;++i) {
        UIButton btn;
        btn.set(BX,YS+i*(BH+YG),BW,BH,BLBL[i],i==7?2:0);
        btn.draw(win, font, mx, my);
    }

    drawStatusBar(win, font, "Patient Menu", "Press 8 to logout");
}

void PatientUI::drawBook(RenderWindow& win, const Font& font, int mx, int my)
{
    drawHeader(win, font, "Book Appointment",
               "Select a doctor and fill in the details");

    const int VIS=9;
    int maxOff = doctorCount - VIS;
    if (bookScroll > maxOff && maxOff >= 0) bookScroll = maxOff;
    if (bookScroll < 0) bookScroll = 0;

    win.draw(makeRect(0.f, 92.f, 482.f, 460.f, COL_PANEL));
    Text dhdrT(font, "  ID   Name               Spec          Fee", 12);
    dhdrT.setFillColor(COL_GOLD);
    dhdrT.setPosition({4.f, 96.f});
    win.draw(dhdrT);

    static const float dxp[]={4.f,46.f,180.f,360.f};
    const float ROW_H=44.f;
    for (int i=0;i<VIS;++i) {
        int idx=bookScroll+i;
        if(idx>=doctorCount)break;
        const Doctor& d=doctorList[idx];
        float ry=112.f+i*ROW_H;
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        win.draw(makeRect(0.f,ry,480.f,ROW_H-2.f,rc));
        char idS[8],fS[16];
        std::snprintf(idS,8,"%d",d.getId());
        Validator::formatDouble(d.getFee(),fS,16);
        Text tId(font,idS,12); tId.setFillColor(Color(210, 200, 170)); tId.setPosition({dxp[0],ry+3.f}); win.draw(tId);
        Text tNm(font,d.getName(),12); tNm.setFillColor(COL_WHITE); tNm.setPosition({dxp[1],ry+3.f}); win.draw(tNm);
        Text tSp(font,d.getSpecialization(),12); tSp.setFillColor(Color(210, 200, 170)); tSp.setPosition({dxp[2],ry+3.f}); win.draw(tSp);
        Text tFe(font,fS,12); tFe.setFillColor(COL_OK); tFe.setPosition({dxp[3],ry+3.f}); win.draw(tFe);
    }
    drawScrollBar(win, doctorCount, VIS, bookScroll, 474.f, 112.f, VIS*ROW_H);

    bool blinkOn = (blink / 30) % 2 == 0;
    bookDrIdField.setActive(bookActiveField == 0);
    bookDateField.setActive(bookActiveField == 1);
    bookSlotField.setActive(bookActiveField == 2);
    bookDrIdField.draw(win, font, blinkOn);
    bookDateField.draw(win, font, blinkOn);
    bookSlotField.draw(win, font, blinkOn);

    btnConfirm.set(490.f, 340.f, 390.f, 42.f, "Confirm Booking  (Enter)", 0);
    btnConfirm.draw(win, font, mx, my);
    btnBack.draw(win, font, mx, my);

    if (msgBuf[0]) {
        Text t(font, msgBuf, 14);
        t.setFillColor(msgOk ? COL_OK : COL_ERROR);
        centreTextX(t, 0.f, 900.f);
        t.setPosition({t.getPosition().x, 526.f});
        win.draw(t);
    }
    Text hint(font, "Tab=next field  Enter=confirm  Esc=back", 12);
    hint.setFillColor(Color(185, 178, 155)); hint.setPosition({490.f,560.f}); win.draw(hint);
}

void PatientUI::drawCancel(RenderWindow& win, const Font& font, int mx, int my,
                           HospitalSystem& system)
{
    drawHeader(win, font, "Cancel Appointment",
               "Enter the appointment ID to cancel (pending only)");
    const int VIS=8;
    int maxOff=apptCount-VIS;
    if (apptScroll>maxOff && maxOff>=0) apptScroll=maxOff;
    if (apptScroll<0) apptScroll=0;

    static const char* chdr[]={"ID","Doctor","Date","Time","Status"};
    static const float cxp[]={10.f,70.f,290.f,400.f,488.f};
    static const float cwi[]={58.f,218.f,108.f,86.f,160.f};
    drawTableHeader(win,font,95.f,22.f,chdr,5,cxp);
    for(int i=0;i<VIS;++i){
        int idx=apptScroll+i;
        if(idx>=apptCount)break;
        const Appointment& a=apptList[idx];
        char idS[8]; std::snprintf(idS,8,"%d",a.getAppointmentId());
        const char* cols[5]={idS, system.getDoctorName(a.getDoctorId()),
            a.getDate(), a.getTimeSlot(), a.getStatus()};
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*32.f,30.f,cols,5,cxp,cwi,rc);
    }
    drawScrollBar(win, apptCount, VIS, apptScroll, 886.f, 118.f, VIS*32.f);

    bool blinkOn = (blink / 30) % 2 == 0;
    cancelIdField.setActive(true);
    cancelIdField.draw(win, font, blinkOn);
    btnConfirm.set(300.f, 432.f, 300.f, 42.f, "Cancel Appointment (Enter)", 0);
    btnConfirm.draw(win, font, mx, my);
    btnBack.draw(win, font, mx, my);

    if (msgBuf[0]) {
        Text t(font, msgBuf, 14);
        t.setFillColor(msgOk ? COL_OK : COL_ERROR);
        centreTextX(t, 0.f, 900.f);
        t.setPosition({t.getPosition().x, 526.f});
        win.draw(t);
    }
}

void PatientUI::drawAppointments(RenderWindow& win, const Font& font, int mx, int my,
                                 HospitalSystem& system)
{
    drawHeader(win, font, "My Appointments", "All your appointments");
    const int VIS=13;
    int maxOff=apptCount-VIS;
    if(viewApptScroll>maxOff && maxOff>=0) viewApptScroll=maxOff;
    if(viewApptScroll<0) viewApptScroll=0;

    static const char* ahdr[]={"ID","Doctor","Date","Time","Status"};
    static const float axp[]={10.f,70.f,290.f,400.f,488.f};
    static const float awi[]={58.f,218.f,108.f,86.f,200.f};
    drawTableHeader(win,font,95.f,22.f,ahdr,5,axp);
    for(int i=0;i<VIS;++i){
        int idx=viewApptScroll+i;
        if(idx>=apptCount)break;
        const Appointment& a=apptList[idx];
        char idS[8]; std::snprintf(idS,8,"%d",a.getAppointmentId());
        const char* cols[5]={idS, system.getDoctorName(a.getDoctorId()),
            a.getDate(), a.getTimeSlot(), a.getStatus()};
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*31.f,29.f,cols,5,axp,awi,rc);
    }
    drawScrollBar(win, apptCount, VIS, viewApptScroll, 886.f, 118.f, VIS*31.f);
    btnBack.draw(win, font, mx, my);
    if(apptCount==0) {
        Text t(font, "No appointments found.", 14);
        t.setFillColor(COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,300.f});
        win.draw(t);
    }
}

void PatientUI::drawRecords(RenderWindow& win, const Font& font, int mx, int my,
                            HospitalSystem& system)
{
    drawHeader(win, font, "Medical Records", "Your prescriptions");
    const int VIS=12;
    int maxOff=rxCount-VIS;
    if(rxScroll>maxOff && maxOff>=0) rxScroll=maxOff;
    if(rxScroll<0) rxScroll=0;

    static const char* rhdr[]={"Date","Doctor","Medicines","Notes"};
    static const float rxp[]={10.f,105.f,270.f,600.f};
    static const float rwi[]={92.f,163.f,328.f,280.f};
    drawTableHeader(win,font,95.f,22.f,rhdr,4,rxp);
    for(int i=0;i<VIS;++i){
        int idx=rxScroll+i;
        if(idx>=rxCount)break;
        const Prescription& pr=rxList[idx];
        const char* cols[4]={pr.getDate(), system.getDoctorName(pr.getDoctorId()),
            pr.getMedicines(), pr.getNotes()};
        Color rc=(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*32.f,30.f,cols,4,rxp,rwi,rc);
    }
    drawScrollBar(win, rxCount, VIS, rxScroll, 886.f, 118.f, VIS*32.f);
    btnBack.draw(win, font, mx, my);
    if(rxCount==0) {
        Text t(font, "No medical records found.", 14);
        t.setFillColor(COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,300.f});
        win.draw(t);
    }
}

void PatientUI::drawBills(RenderWindow& win, const Font& font, int mx, int my)
{
    drawHeader(win, font, "My Bills", "All your bills — unpaid highlighted in red");
    const int VIS=12;
    int maxOff=billCount-VIS;
    if(billScroll>maxOff && maxOff>=0) billScroll=maxOff;
    if(billScroll<0) billScroll=0;

    static const char* bhdr[]={"Bill ID","Appt ID","Amount","Status","Date"};
    static const float bxp[]={10.f,90.f,170.f,320.f,450.f};
    static const float bwi[]={78.f,78.f,148.f,128.f,200.f};
    drawTableHeader(win,font,95.f,22.f,bhdr,5,bxp);
    for(int i=0;i<VIS;++i){
        int idx=billScroll+i;
        if(idx>=billCount)break;
        const Bill& b=billList[idx];
        char bidS[8],aidS[8],amtS[24];
        std::snprintf(bidS,8,"%d",b.getBillId());
        std::snprintf(aidS,8,"%d",b.getAppointmentId());
        Validator::formatDouble(b.getAmount(),amtS,24);
        bool unp=Validator::strEqual(b.getStatus(),"unpaid");
        const char* cols[5]={bidS,aidS,amtS,b.getStatus(),b.getDate()};
        Color rc=unp?Color(50,15,15):(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*32.f,30.f,cols,5,bxp,bwi,rc);
    }
    drawScrollBar(win, billCount, VIS, billScroll, 886.f, 118.f, VIS*32.f);
    btnBack.draw(win, font, mx, my);
    if(billCount==0) {
        Text t(font, "No bills found.", 14);
        t.setFillColor(COL_ERROR);
        centreTextX(t,0.f,900.f);
        t.setPosition({t.getPosition().x,300.f});
        win.draw(t);
    }
}

void PatientUI::drawPayBill(RenderWindow& win, const Font& font, int mx, int my)
{
    drawHeader(win, font, "Pay Bill", "Enter bill ID to pay");
    const int VIS=7;
    int maxOff=billCount-VIS;
    if(billScroll>maxOff && maxOff>=0) billScroll=maxOff;
    if(billScroll<0) billScroll=0;

    static const char* pbhdr[]={"Bill ID","Appt ID","Amount","Status","Date"};
    static const float pbxp[]={10.f,90.f,170.f,320.f,450.f};
    static const float pbwi[]={78.f,78.f,148.f,128.f,200.f};
    drawTableHeader(win,font,95.f,22.f,pbhdr,5,pbxp);
    for(int i=0;i<VIS;++i){
        int idx=billScroll+i;
        if(idx>=billCount)break;
        const Bill& b=billList[idx];
        char bidS[8],aidS[8],amtS[24];
        std::snprintf(bidS,8,"%d",b.getBillId());
        std::snprintf(aidS,8,"%d",b.getAppointmentId());
        Validator::formatDouble(b.getAmount(),amtS,24);
        bool unp=Validator::strEqual(b.getStatus(),"unpaid");
        const char* cols[5]={bidS,aidS,amtS,b.getStatus(),b.getDate()};
        Color rc=unp?Color(50,15,15):(i%2==0)?COL_ROW_ODD:COL_ROW_EVEN;
        drawTableRow(win,font,118.f+i*32.f,30.f,cols,5,pbxp,pbwi,rc);
    }
    drawScrollBar(win, billCount, VIS, billScroll, 886.f, 118.f, VIS*32.f);

    bool blinkOn = (blink / 30) % 2 == 0;
    payBillIdField.setActive(true);
    payBillIdField.draw(win, font, blinkOn);
    btnConfirm.set(300.f, 430.f, 300.f, 42.f, "Pay Bill  (Enter)", 0);
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

void PatientUI::drawTopUp(RenderWindow& win, const Font& font, int mx, int my,
                          HospitalSystem& system)
{
    char sub[64], bal[32];
    Validator::formatDouble(system.getPatientBalance(patientId), bal, 32);
    std::snprintf(sub,64,"Current Balance: PKR %s",bal);
    drawHeader(win, font, "Top Up Balance", sub);

    bool blinkOn = (blink / 30) % 2 == 0;
    topupAmtField.setActive(true);
    topupAmtField.draw(win, font, blinkOn);
    btnConfirm.set(300.f, 275.f, 300.f, 42.f, "Confirm Top Up  (Enter)", 0);
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
