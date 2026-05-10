#include "SFMLUI.h"

#ifdef HMS_ENABLE_SFML
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <optional>
#include <cmath>

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
                         int mx, int my, int frame)
{
    // ── Animated light gradient background ──────────────────────────────────
    // Soft cream/warm-white background with subtle animated pulse
    float pulse = 0.5f + 0.5f * std::sin(frame * 0.02f);
    unsigned char bgR = (unsigned char)(235 + pulse * 8);
    unsigned char bgG = (unsigned char)(228 + pulse * 6);
    unsigned char bgB = (unsigned char)(210 + pulse * 4);
    window.draw(makeRect(0.f, 0.f, 900.f, 600.f, Color(bgR, bgG, bgB)));

    // Animated decorative circles — soft gold rings pulsing in background
    float r1 = 180.f + pulse * 15.f;
    float r2 = 280.f + pulse * 10.f;
    float r3 = 380.f + pulse * 6.f;
    float cx = 450.f, cy = 300.f;
    for (float r : {r1, r2, r3}) {
        RectangleShape ring = makeRect(cx-r, cy-r, r*2.f, r*2.f, Color::Transparent);
        ring.setOutlineThickness(1.f);
        ring.setOutlineColor(Color(212, 175, 55, (unsigned char)(18 + pulse * 12)));
        window.draw(ring);
    }

    // Subtle diagonal grid lines — very faint
    for (int i = -10; i < 20; ++i) {
        float ox = (float)(i * 80);
        RectangleShape line = makeRect(ox, 0.f, 1.f, 900.f, Color(200, 185, 140, 18));
        line.setRotation(degrees(30.f));
        window.draw(line);
    }

    // Header area — semi-transparent dark panel
    window.draw(makeRect(0.f, 0.f, 900.f, 155.f, Color(12, 10, 8, 220)));
    window.draw(makeRect(0.f, 153.f, 900.f, 2.f, COL_GOLD));

    // Animated cross — slowly scales
    float crossScale = 1.f + pulse * 0.08f;
    float crossX=450.f, crossY=42.f;
    float cv=18.f*crossScale, ch2s=18.f*crossScale, cw=3.f*crossScale;
    window.draw(makeRect(crossX-cw, crossY-cv, cw*2.f, cv*2.f, COL_GOLD));
    window.draw(makeRect(crossX-ch2s, crossY-cw, ch2s*2.f, cw*2.f, COL_GOLD));

    // Title
    const Font& df = g_displayFontLoaded ? g_displayFont : font;
    Text title(df,"MEDICORE",38);
    title.setFillColor(COL_WHITE);
    centreTextX(title,0.f,900.f);
    title.setPosition({title.getPosition().x,72.f});
    window.draw(title);

    Text sub2(font,"HOSPITAL  MANAGEMENT  SYSTEM",12);
    sub2.setFillColor(COL_GOLD);
    centreTextX(sub2,0.f,900.f);
    sub2.setPosition({sub2.getPosition().x,120.f});
    window.draw(sub2);

    // Role cards — light themed
    const float CW=320.f, CH=90.f, CX=(900.f-CW)*0.5f;
    const float CY[4]={162.f,262.f,362.f,462.f};
    const char* CLBL[4]={"1.  Patient Login","2.  Doctor Login","3.  Admin Login","4.  Exit"};
    const char* CSUB[4]={"Book appointments & view records","Manage patients & prescriptions","System administration","Close application"};
    const char* CHINT[4]={"Press 1","Press 2","Press 3","Press 4"};
    Color CBARS[4]={Color(40,160,120),Color(60,120,220),Color(180,130,20),Color(180,50,50)};
    Color CARD_BG   = Color(255, 252, 240, 230);
    Color CARD_HOV  = Color(255, 250, 225, 245);

    for (int i=0;i<4;++i) {
        bool hov = FloatRect({CX,CY[i]},{CW,CH})
                       .contains(Vector2f((float)mx,(float)my));

        // Card shadow
        window.draw(makeRect(CX+3.f, CY[i]+3.f, CW, CH, Color(0,0,0,30)));

        // Card background — warm white/cream
        RectangleShape card = makeRect(CX, CY[i], CW, CH,
                                       hov ? CARD_HOV : CARD_BG);
        card.setOutlineThickness(hov ? 1.5f : 1.f);
        card.setOutlineColor(hov ? CBARS[i] : Color(200, 185, 155, 180));
        window.draw(card);

        // Left accent bar
        window.draw(makeRect(CX, CY[i], 5.f, CH, hov ? CBARS[i] : Color(
            CBARS[i].r, CBARS[i].g, CBARS[i].b, 180)));

        // Card title — dark text on light card
        Text ct(df, CLBL[i], 16);
        ct.setFillColor(Color(25, 20, 10));
        ct.setPosition({CX+18.f, CY[i]+14.f});
        window.draw(ct);

        // Card subtitle
        Text cs(font, CSUB[i], 12);
        cs.setFillColor(Color(55, 45, 25));
        cs.setPosition({CX+18.f, CY[i]+46.f});
        window.draw(cs);

        // Keyboard hint
        Text ch3(font, CHINT[i], 10);
        ch3.setFillColor(hov ? CBARS[i] : Color(100, 85, 55));
        ch3.setPosition({CX+CW-50.f, CY[i]+CH-20.f});
        window.draw(ch3);

        // Animated hover indicator — small gold dot that pulses
        if (hov) {
            float dotR = 4.f + pulse * 2.f;
            window.draw(makeRect(CX+CW-20.f, CY[i]+CH*0.5f-dotR*0.5f,
                                  dotR, dotR, CBARS[i]));
        }
    }

    // Bottom status bar — dark
    window.draw(makeRect(0.f, 580.f, 900.f, 20.f, Color(12,10,8,220)));
    window.draw(makeRect(0.f, 580.f, 900.f, 1.f, COL_GOLD));
    Text hint(font,"Main Menu",11);
    hint.setFillColor(COL_GOLD_DIM);
    hint.setPosition({8.f,583.f});
    window.draw(hint);
    Text hint2(font,"Press 1-3 to log in  |  Press 4 to exit",11);
    hint2.setFillColor(Color(80, 65, 35));
    centreTextX(hint2,0.f,900.f);
    hint2.setPosition({hint2.getPosition().x,583.f});
    window.draw(hint2);
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

// ---------------------------------------------------------------------------
// Audio state  (file-scope so helpers can access)
// ---------------------------------------------------------------------------
static int  g_musicChoice  = 1;   // 0=off, 1=music1, 2=music2
static int  g_clickChoice  = 1;   // 0=off, 1=click1, 2=click2
static int  g_errorChoice  = 1;   // 0=off, 1=error1, 2=error2
static bool g_settingsOpen = false;

static Music      g_music;
static SoundBuffer g_clickBuf1, g_clickBuf2;
static SoundBuffer g_errorBuf1, g_errorBuf2;
static Sound*      g_clickSound = nullptr;
static Sound*      g_errorSound = nullptr;

static void loadAudio()
{
    (void)g_clickBuf1.loadFromFile("assets/sounds/click1.mp3");
    (void)g_clickBuf2.loadFromFile("assets/sounds/click2.mp3");
    (void)g_errorBuf1.loadFromFile("assets/sounds/error1.mp3");
    (void)g_errorBuf2.loadFromFile("assets/sounds/error2.mp3");
    g_clickSound = new Sound(g_clickBuf1);
    g_errorSound = new Sound(g_errorBuf1);
}

static void applyMusicChoice()
{
    g_music.stop();
    if (g_musicChoice == 0) return;
    const char* path = (g_musicChoice == 1)
                       ? "assets/sounds/music1.mp3"
                       : "assets/sounds/music2.mp3";
    if (g_music.openFromFile(path)) {
        g_music.setLooping(true);
        g_music.setVolume(35.f);
        g_music.play();
    }
}

static void playClick()
{
    if (g_clickChoice == 0 || !g_clickSound) return;
    g_clickSound->setBuffer(g_clickChoice == 1 ? g_clickBuf1 : g_clickBuf2);
    g_clickSound->play();
}

static void playError()
{
    if (g_errorChoice == 0 || !g_errorSound) return;
    g_errorSound->setBuffer(g_errorChoice == 1 ? g_errorBuf1 : g_errorBuf2);
    g_errorSound->play();
}

// ---------------------------------------------------------------------------
// Gear button — drawn on every screen top-right
// ---------------------------------------------------------------------------
static void drawGearButton(RenderWindow& win, const Font& font, int mx, int my)
{
    bool hov = FloatRect({818.f, 2.f}, {78.f, 28.f})
                   .contains(Vector2f((float)mx, (float)my));
    RectangleShape bg = makeRect(818.f, 2.f, 78.f, 28.f,
                                  hov ? COL_GOLD_BG : COL_SURFACE);
    bg.setOutlineThickness(1.f);
    bg.setOutlineColor(hov ? COL_GOLD : COL_GOLD_DIM);
    win.draw(bg);
    const Font& df = g_displayFontLoaded ? g_displayFont : font;
    Text g(df, "* Settings", 11);
    g.setFillColor(hov ? COL_GOLD_BRIGHT : COL_GOLD);
    centreTextX(g, 818.f, 78.f);
    g.setPosition({g.getPosition().x, 10.f});
    win.draw(g);
}

static bool gearHit(int mx, int my)
{
    return FloatRect({818.f, 2.f}, {78.f, 28.f})
               .contains(Vector2f((float)mx, (float)my));
}

// ---------------------------------------------------------------------------
// Settings overlay
// ---------------------------------------------------------------------------
static void drawSettings(RenderWindow& win, const Font& font, int mx, int my)
{
    // Full-screen dim backdrop
    win.draw(makeRect(0.f, 0.f, 900.f, 600.f, Color(0, 0, 0, 200)));

    // Panel — wide enough so buttons don't overlap
    float PX=110.f, PY=60.f, PW=680.f, PH=470.f;
    win.draw(makeRect(PX, PY, PW, PH, COL_SURFACE));
    win.draw(makeRect(PX, PY, PW, 3.f, COL_GOLD));
    win.draw(makeRect(PX, PY, 4.f, PH, COL_GOLD));
    RectangleShape ol = makeRect(PX, PY, PW, PH, Color::Transparent);
    ol.setOutlineThickness(1.f);
    ol.setOutlineColor(COL_BORDER);
    win.draw(ol);
    drawCornerAccents(win, PX, PY, PW, PH, COL_GOLD_DIM);

    // Header
    const Font& df = g_displayFontLoaded ? g_displayFont : font;
    Text hdr(df, "Settings", 24);
    hdr.setFillColor(COL_WHITE);
    centreTextX(hdr, PX, PW);
    hdr.setPosition({hdr.getPosition().x, PY + 12.f});
    win.draw(hdr);
    win.draw(makeRect(PX+20.f, PY+50.f, PW-40.f, 1.f, COL_GOLD_DIM));

    // Layout constants — all derived from panel width so nothing overflows
    const char* rowLabels[3] = {"Background Music", "Click Sound", "Error Sound"};
    const char* opts[3][3]   = {{"Off","Track 1","Track 2"},
                                 {"Off","Click 1","Click 2"},
                                 {"Off","Error 1","Error 2"}};
    int* choices[3]          = {&g_musicChoice, &g_clickChoice, &g_errorChoice};

    float margin   = 20.f;
    float usableW  = PW - margin * 2.f;           // 640
    float BGA      = 10.f;
    float BW       = (usableW - BGA * 2.f) / 3.f; // ~206
    float BH       = 42.f;
    float BX0      = PX + margin;
    float rowStartY= PY + 65.f;
    float rowGap   = 124.f;

    for (int r = 0; r < 3; ++r) {
        float ry = rowStartY + r * rowGap;

        Text lbl(font, rowLabels[r], 14);
        lbl.setFillColor(COL_GOLD);
        lbl.setPosition({BX0, ry});
        win.draw(lbl);
        win.draw(makeRect(BX0, ry+22.f, usableW, 1.f, COL_BORDER));

        for (int b = 0; b < 3; ++b) {
            float bx  = BX0 + b * (BW + BGA);
            float by  = ry + 30.f;
            bool  sel = (*choices[r] == b);
            bool  hov = FloatRect({bx, by}, {BW, BH})
                            .contains(Vector2f((float)mx, (float)my));

            RectangleShape btn = makeRect(bx, by, BW, BH, Color::Transparent);
            if (sel) {
                btn.setFillColor(COL_GOLD);
                btn.setOutlineThickness(0.f);
            } else {
                btn.setFillColor(hov ? COL_GOLD_BG : COL_FIELD);
                btn.setOutlineThickness(1.5f);
                btn.setOutlineColor(hov ? COL_GOLD : COL_BORDER);
            }
            win.draw(btn);

            Text bt(font, opts[r][b], 14);
            bt.setFillColor(sel ? COL_BG : (hov ? COL_GOLD_BRIGHT : Color(190,182,158)));
            centreTextX(bt, bx, BW);
            bt.setPosition({bt.getPosition().x,
                            by + (BH - bt.getLocalBounds().size.y)*0.5f - 2.f});
            win.draw(bt);
        }
    }

    // Close button — centered
    float CBW=160.f, CBH=42.f;
    float CBX = PX + (PW - CBW) * 0.5f;
    float CBY = PY + PH - 62.f;
    bool chov = FloatRect({CBX, CBY}, {CBW, CBH})
                    .contains(Vector2f((float)mx, (float)my));
    win.draw(makeRect(CBX, CBY, CBW, CBH, chov ? COL_GOLD_BRIGHT : COL_GOLD));
    Text ct(font, "Close  (Esc)", 14);
    ct.setFillColor(COL_BG);
    centreTextX(ct, CBX, CBW);
    ct.setPosition({ct.getPosition().x,
                    CBY + (CBH - ct.getLocalBounds().size.y)*0.5f - 2.f});
    win.draw(ct);
}

static bool handleSettingsClick(float bx, float by)
{
    // Layout values MUST match drawSettings exactly
    float PX=110.f, PY=60.f, PW=680.f, PH=470.f;
    int*  choices[3]  = {&g_musicChoice, &g_clickChoice, &g_errorChoice};
    float margin      = 20.f;
    float usableW     = PW - margin * 2.f;
    float BGA         = 10.f;
    float BW          = (usableW - BGA * 2.f) / 3.f;
    float BH          = 42.f;
    float BX0         = PX + margin;
    float rowStartY   = PY + 65.f;
    float rowGap      = 124.f;

    for (int r = 0; r < 3; ++r) {
        float ry = rowStartY + r * rowGap;
        for (int b = 0; b < 3; ++b) {
            float btnX = BX0 + b * (BW + BGA);
            float btnY = ry + 30.f;
            if (FloatRect({btnX, btnY}, {BW, BH})
                    .contains(Vector2f(bx, by))) {
                *choices[r] = b;
                if (r == 0) applyMusicChoice();
                playClick();
                return true;
            }
        }
    }

    // Close button
    float CBW=160.f, CBH=42.f;
    float CBX = PX + (PW - CBW) * 0.5f;
    float CBY = PY + PH - 62.f;
    if (FloatRect({CBX, CBY}, {CBW, CBH}).contains(Vector2f(bx, by))) {
        g_settingsOpen = false;
        playClick();
        return true;
    }

    return true; // always consume clicks when settings open
}

void SFMLUI::run(HospitalSystem& system)
{
    loadFonts();
    loadAudio();
    applyMusicChoice();

    RenderWindow window(VideoMode({900u, 600u}),
                        "MediCore by 25L-0519",
                        Style::Default | Style::Resize);
    window.setFramerateLimit(60);

    const Font& font = g_bodyFontLoaded ? g_bodyFont : g_displayFont;

    hideConsole();

    int screen     = SCREEN_MAIN;
    int prevScreen = -1;
    int loggedId   = 0;
    int role       = 0;
    int mx = 0, my = 0;
    int frame      = 0;   // animation counter

    LoginUI   loginUI;
    PatientUI patientUI;
    DoctorUI  doctorUI;
    AdminUI   adminUI;

    while (window.isOpen()) {
        while (auto ev = window.pollEvent()) {
            if (ev->is<Event::Closed>()) { window.close(); break; }

            if (auto* mm = ev->getIf<Event::MouseMoved>()) {
                Vector2u ws = window.getSize();
                mx = (int)(mm->position.x * 900.f / ws.x);
                my = (int)(mm->position.y * 600.f / ws.y);
            }

            if (auto* k = ev->getIf<Event::KeyPressed>()) {
                // Esc closes settings if open
                if (k->code == Keyboard::Key::Escape && g_settingsOpen) {
                    g_settingsOpen = false;
                    continue;
                }
                // F11 — toggle fullscreen
                if (k->code == Keyboard::Key::F11) {
                    static bool isFullscreen = false;
                    isFullscreen = !isFullscreen;
                    if (isFullscreen) {
                        window.create(VideoMode::getDesktopMode(),
                                  "MediCore by 25L-0519",
                                  State::Fullscreen);
                    } else {
                        window.create(VideoMode({900u, 600u}),
                                      "MediCore by 25L-0519",
                                      Style::Default | Style::Resize);
                    }
                    window.setFramerateLimit(60);
                }
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
                Vector2u ws = window.getSize();
                int bx = (int)(m->position.x * 900.f / ws.x);
                int by = (int)(m->position.y * 600.f / ws.y);

                // Settings overlay consumes all clicks when open
                if (g_settingsOpen) {
                    handleSettingsClick((float)bx, (float)by);
                    continue;
                }

                // Gear button
                if (gearHit(mx, my)) {
                    g_settingsOpen = true;
                    playClick();
                    continue;
                }

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

        ++frame;
        loginUI.update();
        patientUI.update();
        doctorUI.update();
        adminUI.update();

        window.clear(COL_BG);
        View view(FloatRect({0.f, 0.f}, {900.f, 600.f}));
        window.setView(view);

        if (screen == SCREEN_MAIN) drawMainMenu(window, font, mx, my, frame);
        else if (screen == SCREEN_LOGIN) loginUI.draw(window, mx, my);
        else if (isPatientScreen(screen))
            patientUI.draw(window, font, screen, mx, my, system);
        else if (isDoctorScreen(screen))
            doctorUI.draw(window, font, screen, mx, my, system);
        else
            adminUI.draw(window, font, screen, mx, my, system);

        // Gear button on every screen
        drawGearButton(window, font, mx, my);

        // Settings overlay on top of everything
        if (g_settingsOpen)
            drawSettings(window, font, mx, my);

        window.display();
    }
}

#else

void SFMLUI::run(HospitalSystem& system) { (void)system; }

#endif
