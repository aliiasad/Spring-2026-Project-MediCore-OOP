#include "SFMLUI.h"

#ifdef HMS_ENABLE_SFML
#include <SFML/Graphics.hpp>
#include <optional>

#ifdef _WIN32
#  include <windows.h>
#endif

#include "UITheme.h"
#include "LoginUI.h"
#include "PatientUI.h"
#include "DoctorUI.h"
#include "AdminUI.h"
#include "Validator.h"
#endif

namespace sf {}
using namespace std;
using namespace sf;

SFMLUI::SFMLUI() {}

#ifdef HMS_ENABLE_SFML

static void hideConsole()
{
#ifdef _WIN32
    HWND h = GetConsoleWindow();
    if (h) ShowWindow(h, SW_HIDE);
#endif
}

static void drawMainMenu(RenderWindow& window, const Font& font,
                         int mx, int my)
{
    window.draw(makeRect(0.f,0.f,900.f,600.f,COL_BG));

    float crossX=450.f, crossY=52.f;
    window.draw(makeRect(crossX-3.f, crossY-18.f, 6.f, 36.f, COL_GOLD));
    window.draw(makeRect(crossX-18.f, crossY-3.f, 36.f, 6.f, COL_GOLD));

    const Font& df = g_displayFontLoaded ? g_displayFont : font;
    Text title(df,"MEDICORE",38);
    title.setFillColor(COL_WHITE);
    centreTextX(title,0.f,900.f);
    title.setPosition({title.getPosition().x,80.f});
    window.draw(title);

    Text sub2(font,"HOSPITAL  MANAGEMENT  SYSTEM",12);
    sub2.setFillColor(COL_GOLD_DIM);
    centreTextX(sub2,0.f,900.f);
    sub2.setPosition({sub2.getPosition().x,130.f});
    window.draw(sub2);

    window.draw(makeRect(200.f,152.f,500.f,1.f,COL_BORDER));
    window.draw(makeRect(447.f,149.f,6.f,6.f,COL_GOLD));

    const float CW=320.f, CH=88.f, CX=(900.f-CW)*0.5f;
    const float CY[4]={172.f,272.f,372.f,472.f};
    const char* CLBL[4]={"1.  Patient Login","2.  Doctor Login","3.  Admin Login","4.  Exit"};
    const char* CSUB[4]={"Book appointments & view records","Manage patients & prescriptions","System administration","Close application"};
    const char* CHINT[4]={"Press 1","Press 2","Press 3","Press 4"};
    Color CBARS[4]={Color(40,160,120),Color(60,120,220),COL_GOLD,Color(180,50,50)};

    for (int i=0;i<4;++i) {
        bool hov = FloatRect({CX,CY[i]},{CW,CH})
                       .contains(Vector2f((float)mx,(float)my));
        window.draw(makeRect(CX,CY[i],CW,CH,hov?COL_SURFACE2:COL_SURFACE));
        window.draw(makeRect(CX,CY[i],4.f,CH,hov?COL_GOLD_BRIGHT:CBARS[i]));
        if (hov) {
            RectangleShape outline=makeRect(CX,CY[i],CW,CH,Color::Transparent);
            outline.setOutlineThickness(1.5f);
            outline.setOutlineColor(COL_GOLD);
            window.draw(outline);
        }
        Text ct(df,CLBL[i],17);
        ct.setFillColor(hov?COL_GOLD_BRIGHT:COL_WHITE);
        ct.setPosition({CX+16.f,CY[i]+14.f});
        window.draw(ct);
        Text cs(font,CSUB[i],12);
        cs.setFillColor(COL_MUTED);
        cs.setPosition({CX+16.f,CY[i]+44.f});
        window.draw(cs);
        Text ch2(font,CHINT[i],10);
        ch2.setFillColor(COL_DIM);
        ch2.setPosition({CX+CW-48.f,CY[i]+CH-20.f});
        window.draw(ch2);
    }

    drawStatusBar(window, font, "Main Menu", "Press 1-3 to log in  |  Press 4 to exit");
}

static bool isPatientScreen(int screen)
{
    return screen == SCREEN_PATIENT_MENU ||
           (screen >= SCREEN_PATIENT_BOOK && screen <= SCREEN_PATIENT_PAYBILL);
}

static bool isDoctorScreen(int screen)
{
    return screen == SCREEN_DOCTOR_MENU ||
           (screen >= SCREEN_DOCTOR_TODAY && screen <= SCREEN_DOCTOR_HISTORY);
}

static bool isAdminScreen(int screen)
{
    return screen == SCREEN_ADMIN_MENU ||
           (screen >= SCREEN_ADMIN_ADD_DOCTOR && screen <= SCREEN_ADMIN_ADD_PATIENT);
}

void SFMLUI::run(HospitalSystem& system)
{
    loadFonts();

    RenderWindow window(VideoMode({900u, 600u}), "MediCore by 25L-0519");
    window.setFramerateLimit(60);

    const Font& font = g_bodyFontLoaded ? g_bodyFont : g_displayFont;

    hideConsole();

    int screen   = SCREEN_MAIN;
    int prevScreen = -1;
    int loggedId = 0;
    int role     = 0;
    int mx = 0, my = 0;

    LoginUI   loginUI;
    PatientUI patientUI;
    DoctorUI  doctorUI;
    AdminUI   adminUI;

    while (window.isOpen()) {
        while (auto ev = window.pollEvent()) {
            if (ev->is<Event::Closed>()) { window.close(); break; }

            if (auto* mm = ev->getIf<Event::MouseMoved>()) {
                mx = mm->position.x; my = mm->position.y;
            }

            if (auto* k = ev->getIf<Event::KeyPressed>()) {
                if (screen == SCREEN_LOGIN) {
                    loginUI.handleKey(k->code, screen, loggedId, system);
                } else if (isPatientScreen(screen)) {
                    patientUI.handleKey(k->code, screen, system);
                } else if (isDoctorScreen(screen)) {
                    doctorUI.handleKey(k->code, screen, system);
                } else if (isAdminScreen(screen)) {
                    adminUI.handleKey(k->code, screen, system);
                }
            }

            if (auto* t = ev->getIf<Event::TextEntered>()) {
                unsigned int c = t->unicode;
                if (c >= 32u && c < 127u) {
                    if (screen == SCREEN_MAIN) {
                        if (c >= '1' && c <= '4') {
                            int sel = (int)(c - '0');
                            if (sel == 4) { window.close(); break; }
                            role = sel;
                            screen = SCREEN_LOGIN;
                            loginUI.setRole(role);
                        }
                    } else if (screen == SCREEN_LOGIN) {
                        loginUI.handleText(c);
                    } else if (isPatientScreen(screen)) {
                        patientUI.handleText(c, screen);
                    } else if (isDoctorScreen(screen)) {
                        doctorUI.handleText(c, screen);
                    } else if (isAdminScreen(screen)) {
                        adminUI.handleText(c, screen);
                    }
                }
            }

            if (auto* w2 = ev->getIf<Event::MouseWheelScrolled>()) {
                if (isPatientScreen(screen))
                    patientUI.handleScroll(w2->delta, screen);
                else if (isDoctorScreen(screen))
                    doctorUI.handleScroll(w2->delta, screen);
                else if (isAdminScreen(screen))
                    adminUI.handleScroll(w2->delta, screen);
            }

            if (auto* m = ev->getIf<Event::MouseButtonPressed>()) {
                if (m->button != Mouse::Button::Left) continue;
                int bx = m->position.x, by = m->position.y;

                if (screen == SCREEN_MAIN) {
                    const float BW=320.f,BH=58.f,BX=(900.f-BW)*0.5f;
                    const float BY[4]={200.f,278.f,356.f,434.f};
                    for (int i=0;i<4;++i) {
                        if (rectHit(makeRect(BX,BY[i],BW,BH,COL_ACCENT),bx,by)) {
                            if (i==3){window.close();break;}
                            role=i+1; screen=SCREEN_LOGIN;
                            loginUI.setRole(role);
                        }
                    }
                } else if (screen == SCREEN_LOGIN) {
                    loginUI.handleClick((float)bx, (float)by, screen);
                    if (rectHit(makeRect(250.f,395.f,400.f,46.f,COL_ACCENT),bx,by))
                        loginUI.handleKey(Keyboard::Key::Enter, screen, loggedId, system);
                } else if (isPatientScreen(screen)) {
                    patientUI.handleClick((float)bx, (float)by, screen, system);
                } else if (isDoctorScreen(screen)) {
                    doctorUI.handleClick((float)bx, (float)by, screen, system);
                } else if (isAdminScreen(screen)) {
                    adminUI.handleClick((float)bx, (float)by, screen, system);
                }
            }
        }

        if (screen != prevScreen) {
            prevScreen = screen;
            if (screen == SCREEN_MAIN) {
                loggedId = 0;
                role = 0;
            }
            if (isPatientScreen(screen)) {
                patientUI.setPatientId(loggedId);
                patientUI.loadForScreen(screen, system);
            }
            if (isDoctorScreen(screen)) {
                doctorUI.setDoctorId(loggedId);
                doctorUI.loadForScreen(screen, system);
            }
            if (isAdminScreen(screen)) {
                adminUI.loadForScreen(screen, system);
            }
        }

        loginUI.update();
        patientUI.update();
        doctorUI.update();
        adminUI.update();

        window.clear(COL_BG);

        if (screen == SCREEN_MAIN) drawMainMenu(window, font, mx, my);
        else if (screen == SCREEN_LOGIN) loginUI.draw(window, mx, my);
        else if (isPatientScreen(screen))
            patientUI.draw(window, font, screen, mx, my, system);
        else if (isDoctorScreen(screen))
            doctorUI.draw(window, font, screen, mx, my, system);
        else
            adminUI.draw(window, font, screen, mx, my, system);

        window.display();
    }
}

#else

void SFMLUI::run(HospitalSystem& system) { (void)system; }

#endif
