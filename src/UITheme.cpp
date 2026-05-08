#include "UITheme.h"
#include "Validator.h"

namespace sf {}
using namespace std;
using namespace sf;

const Color COL_BG          = Color(  8,   8,  10);
const Color COL_PANEL       = Color( 15,  15,  18);
const Color COL_SURFACE     = Color( 20,  20,  25);
const Color COL_SURFACE2    = Color( 28,  28,  35);
const Color COL_GOLD        = Color(212, 175,  55);
const Color COL_GOLD_BRIGHT = Color(255, 215,   0);
const Color COL_GOLD_DIM    = Color(140, 110,  30);
const Color COL_GOLD_BG     = Color( 30,  25,   5);
const Color COL_WHITE       = Color(240, 235, 220);
const Color COL_MUTED       = Color(120, 115, 100);
const Color COL_DIM         = Color( 65,  62,  55);
const Color COL_FIELD       = Color( 18,  18,  22);
const Color COL_ACTIVE      = Color( 25,  22,   8);
const Color COL_BORDER      = Color( 45,  42,  30);
const Color COL_ERROR       = Color(220,  60,  60);
const Color COL_OK          = Color( 80, 200, 100);
const Color COL_ROW_ODD     = Color( 16,  16,  20);
const Color COL_ROW_EVEN    = Color( 22,  22,  28);
const Color COL_ACCENT      = Color(212, 175,  55);

bool g_displayFontLoaded = false;
bool g_bodyFontLoaded    = false;
Font g_displayFont;
Font g_bodyFont;

static const float AVG_CHAR_WIDTH  = 7.5f;

void loadFonts()
{
    const char* displayPaths[10] = {
        "C:/Windows/Fonts/georgiabd.ttf",
        "C:/Windows/Fonts/georgia.ttf",
        "C:/Windows/Fonts/cambriab.ttf",
        "C:/Windows/Fonts/cambria.ttc",
        "C:/Windows/Fonts/times.ttf",
        "C:/Windows/Fonts/trebucbd.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSerif-Bold.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSerif-Bold.ttf",
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf",
        "/System/Library/Fonts/Georgia.ttf"
    };
    const char* bodyPaths[10] = {
        "C:/Windows/Fonts/consola.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/tahoma.ttf",
        "C:/Windows/Fonts/verdana.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/System/Library/Fonts/Helvetica.ttc"
    };
    for (int i = 0; i < 10; ++i)
        if (g_displayFont.openFromFile(displayPaths[i])) { g_displayFontLoaded = true; break; }
    for (int i = 0; i < 10; ++i)
        if (g_bodyFont.openFromFile(bodyPaths[i]))    { g_bodyFontLoaded    = true; break; }
}

RectangleShape makeRect(float x, float y, float w, float h, Color fill)
{
    RectangleShape r(Vector2f(w, h));
    r.setPosition({x, y});
    r.setFillColor(fill);
    return r;
}

void centreTextX(Text& t, float x, float w)
{
    FloatRect b = t.getLocalBounds();
    t.setOrigin({b.position.x + b.size.x * 0.5f, b.position.y});
    t.setPosition({x + w * 0.5f, t.getPosition().y});
}

void drawHeader(RenderWindow& win, const Font& font,
                const char* title, const char* sub)
{
    win.draw(makeRect(0.f, 0.f, 900.f, 95.f, COL_PANEL));
    win.draw(makeRect(0.f, 0.f, 4.f, 95.f, COL_GOLD));
    win.draw(makeRect(0.f, 93.f, 900.f, 2.f, COL_GOLD));

    const Font& df = g_displayFontLoaded ? g_displayFont : font;
    Text t(df, title, 26);
    t.setFillColor(COL_WHITE);
    centreTextX(t, 0.f, 900.f);
    t.setPosition({t.getPosition().x, 14.f});
    win.draw(t);

    Text s(font, sub, 13);
    s.setFillColor(COL_GOLD_DIM);
    centreTextX(s, 0.f, 900.f);
    s.setPosition({s.getPosition().x, 58.f});
    win.draw(s);

    Text wm(font, "MEDICORE HMS", 10);
    wm.setFillColor(COL_DIM);
    wm.setPosition({820.f, 12.f});
    win.draw(wm);
}

void drawScrollBar(RenderWindow& win, int total, int visible, int offset,
                   float x, float y, float h)
{
    if (total <= visible) return;
    win.draw(makeRect(x, y, 7.f, h, COL_SURFACE));
    float thumbH = h * visible / (float)total;
    float thumbY = y + h * offset / (float)total;
    RectangleShape thumb = makeRect(x + 1.f, thumbY, 5.f, thumbH, COL_GOLD_DIM);
    win.draw(thumb);
    win.draw(makeRect(x, y,       7.f, 1.f, COL_BORDER));
    win.draw(makeRect(x, y + h - 1.f, 7.f, 1.f, COL_BORDER));
}

void drawTableHeader(RenderWindow& win, const Font& font,
                     float y, float rh,
                     const char* cols[], int nCols,
                     const float xPos[])
{
    win.draw(makeRect(0.f, y, 900.f, rh + 2.f, COL_GOLD_BG));
    win.draw(makeRect(0.f, y + rh + 1.f, 900.f, 1.f, COL_GOLD_DIM));
    for (int c = 0; c < nCols; ++c) {
        if (!cols[c]) continue;
        Text t(font, cols[c], 12);
        t.setFillColor(COL_GOLD);
        t.setPosition({xPos[c], y + 4.f});
        win.draw(t);
    }
}

void drawTableRow(RenderWindow& win, const Font& font,
                  float y, float rh,
                  const char* cols[], int nCols,
                  const float xPos[], const float colW[], Color rowCol)
{
    win.draw(makeRect(0.f, y, 900.f, rh, rowCol));
    win.draw(makeRect(0.f, y + rh - 1.f, 900.f, 1.f, COL_BORDER));
    for (int c = 0; c < nCols; ++c) {
        if (!cols[c]) continue;
        char buf[128];
        int maxChars = (int)(colW[c] / AVG_CHAR_WIDTH);
        if (maxChars < 2) maxChars = 2;
        if (maxChars > 127) maxChars = 127;
        int slen = Validator::strLength(cols[c]);
        if (slen > maxChars) {
            for (int k = 0; k < maxChars - 1; ++k) buf[k] = cols[c][k];
            buf[maxChars - 1] = '.'; buf[maxChars] = '\0';
        } else {
            Validator::strCopy(buf, cols[c], 128);
        }
        Text t(font, buf, 13);
        t.setFillColor(COL_WHITE);
        t.setPosition({xPos[c], y + (rh - t.getLocalBounds().size.y) * 0.5f - 1.f});
        win.draw(t);
    }
}

void drawCornerAccents(RenderWindow& win, float x, float y, float w, float h,
                       Color color, float size)
{
    float t = 1.5f;
    win.draw(makeRect(x,         y,         size, t,    color));
    win.draw(makeRect(x,         y,         t,    size, color));
    win.draw(makeRect(x+w-size,  y,         size, t,    color));
    win.draw(makeRect(x+w-t,     y,         t,    size, color));
    win.draw(makeRect(x,         y+h-t,     size, t,    color));
    win.draw(makeRect(x,         y+h-size,  t,    size, color));
    win.draw(makeRect(x+w-size,  y+h-t,     size, t,    color));
    win.draw(makeRect(x+w-t,     y+h-size,  t,    size, color));
}

void drawStatusBar(RenderWindow& win, const Font& font,
                   const char* leftText, const char* rightText)
{
    win.draw(makeRect(0.f, 580.f, 900.f, 20.f, COL_PANEL));
    win.draw(makeRect(0.f, 580.f, 900.f, 1.f, COL_BORDER));
    if (leftText && leftText[0]) {
        Text hint(font, leftText, 11);
        hint.setFillColor(COL_DIM);
        hint.setPosition({8.f, 583.f});
        win.draw(hint);
    }
    if (rightText && rightText[0]) {
        Text hint2(font, rightText, 11);
        hint2.setFillColor(COL_DIM);
        FloatRect b = hint2.getLocalBounds();
        hint2.setPosition({900.f - b.size.x - 8.f, 583.f});
        win.draw(hint2);
    }
}

bool rectHit(const RectangleShape& r, int mx, int my)
{
    return r.getGlobalBounds().contains(Vector2f((float)mx, (float)my));
}

void maskPassword(const char* in, char* out, int maxLen)
{
    int i = 0;
    while (in[i] && i < maxLen - 1) { out[i] = '*'; ++i; }
    out[i] = '\0';
}

void bufAppend(char* buf, int& len, int maxLen, unsigned int ch)
{
    if (len < maxLen - 1) {
        buf[len++] = (char)ch;
        buf[len] = '\0';
    }
}

void bufBackspace(char* buf, int& len)
{
    if (len > 0) { buf[--len] = '\0'; }
}

