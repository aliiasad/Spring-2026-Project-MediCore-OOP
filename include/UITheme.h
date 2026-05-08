#ifndef UITHEME_H
#define UITHEME_H

#include <SFML/Graphics.hpp>

// ---------------------------------------------------------------------------
// Screen constants
// ---------------------------------------------------------------------------
static const int SCREEN_MAIN               = 0;
static const int SCREEN_LOGIN              = 1;
static const int SCREEN_PATIENT_MENU       = 2;
static const int SCREEN_DOCTOR_MENU        = 3;
static const int SCREEN_ADMIN_MENU         = 4;

static const int SCREEN_PATIENT_BOOK         = 5;
static const int SCREEN_PATIENT_APPOINTMENTS = 6;
static const int SCREEN_PATIENT_RECORDS      = 7;
static const int SCREEN_PATIENT_BILLS        = 8;
static const int SCREEN_PATIENT_TOPUP        = 9;
static const int SCREEN_PATIENT_CANCEL       = 10;
static const int SCREEN_PATIENT_PAYBILL      = 11;

static const int SCREEN_DOCTOR_TODAY      = 12;
static const int SCREEN_DOCTOR_COMPLETE   = 13;
static const int SCREEN_DOCTOR_NOSHOW     = 14;
static const int SCREEN_DOCTOR_PRESCRIBE  = 15;
static const int SCREEN_DOCTOR_HISTORY    = 16;

static const int SCREEN_ADMIN_ADD_DOCTOR    = 17;
static const int SCREEN_ADMIN_REMOVE_DOCTOR = 18;
static const int SCREEN_ADMIN_PATIENTS      = 19;
static const int SCREEN_ADMIN_DOCTORS       = 20;
static const int SCREEN_ADMIN_APPOINTMENTS  = 21;
static const int SCREEN_ADMIN_UNPAID        = 22;
static const int SCREEN_ADMIN_DISCHARGE     = 23;
static const int SCREEN_ADMIN_SECURITY_LOG  = 24;
static const int SCREEN_ADMIN_REPORT        = 25;
static const int SCREEN_ADMIN_ADD_PATIENT   = 26;

static const int ROLE_PATIENT = 1;
static const int ROLE_DOCTOR  = 2;
static const int ROLE_ADMIN   = 3;

// ---------------------------------------------------------------------------
// Colours
// ---------------------------------------------------------------------------
extern const sf::Color COL_BG;
extern const sf::Color COL_PANEL;
extern const sf::Color COL_SURFACE;
extern const sf::Color COL_SURFACE2;
extern const sf::Color COL_GOLD;
extern const sf::Color COL_GOLD_BRIGHT;
extern const sf::Color COL_GOLD_DIM;
extern const sf::Color COL_GOLD_BG;
extern const sf::Color COL_WHITE;
extern const sf::Color COL_MUTED;
extern const sf::Color COL_DIM;
extern const sf::Color COL_FIELD;
extern const sf::Color COL_ACTIVE;
extern const sf::Color COL_BORDER;
extern const sf::Color COL_ERROR;
extern const sf::Color COL_OK;
extern const sf::Color COL_ROW_ODD;
extern const sf::Color COL_ROW_EVEN;
extern const sf::Color COL_ACCENT;

// ---------------------------------------------------------------------------
// Fonts
// ---------------------------------------------------------------------------
extern bool g_displayFontLoaded;
extern bool g_bodyFontLoaded;
extern sf::Font g_displayFont;
extern sf::Font g_bodyFont;

void loadFonts();

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------
sf::RectangleShape makeRect(float x, float y, float w, float h, sf::Color fill);
void centreTextX(sf::Text& t, float x, float w);
void drawHeader(sf::RenderWindow& win, const sf::Font& font,
                const char* title, const char* sub);
void drawScrollBar(sf::RenderWindow& win, int total, int visible, int offset,
                   float x, float y, float h);
void drawTableHeader(sf::RenderWindow& win, const sf::Font& font,
                     float y, float rh, const char* cols[], int nCols,
                     const float xPos[]);
void drawTableRow(sf::RenderWindow& win, const sf::Font& font,
                  float y, float rh, const char* cols[], int nCols,
                  const float xPos[], const float colW[], sf::Color rowCol);
void drawCornerAccents(sf::RenderWindow& win, float x, float y, float w, float h,
                       sf::Color color, float size = 14.f);
void drawStatusBar(sf::RenderWindow& win, const sf::Font& font,
                   const char* leftText, const char* rightText);
bool rectHit(const sf::RectangleShape& r, int mx, int my);
void maskPassword(const char* in, char* out, int maxLen);
void bufAppend(char* buf, int& len, int maxLen, unsigned int ch);
void bufBackspace(char* buf, int& len);

#endif
