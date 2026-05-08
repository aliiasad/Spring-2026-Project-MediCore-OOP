#include "UIWidgets.h"
#include "Validator.h"

namespace sf {}
using namespace std;
using namespace sf;

UIButton::UIButton() : x(0.f), y(0.f), w(0.f), h(0.f), style(1) {
    label[0] = '\0';
}

void UIButton::set(float nx, float ny, float nw, float nh, const char* text, int st)
{
    x = nx; y = ny; w = nw; h = nh; style = st;
    Validator::strCopy(label, text ? text : "", 64);
}

void UIButton::draw(RenderWindow& win, const Font& font, int mx, int my) const
{
    bool hovered = FloatRect({x, y}, {w, h}).contains(Vector2f((float)mx, (float)my));
    RectangleShape btn = makeRect(x, y, w, h, Color::Transparent);

    if (style == 0) {
        btn.setFillColor(hovered ? COL_GOLD_BRIGHT : COL_GOLD);
        btn.setOutlineThickness(0.f);
    } else if (style == 2) {
        btn.setFillColor(hovered ? COL_SURFACE2 : COL_SURFACE);
        btn.setOutlineThickness(1.f);
        btn.setOutlineColor(COL_DIM);
    } else {
        btn.setFillColor(hovered ? COL_GOLD_BG : COL_SURFACE);
        btn.setOutlineThickness(1.5f);
        btn.setOutlineColor(hovered ? COL_GOLD : COL_GOLD_DIM);
    }
    win.draw(btn);

    const Font& df = g_displayFontLoaded ? g_displayFont : font;
    Text lbl(df, label, 15);
    if (style == 0)
        lbl.setFillColor(COL_BG);
    else if (style == 2)
        lbl.setFillColor(COL_MUTED);
    else
        lbl.setFillColor(hovered ? COL_GOLD_BRIGHT : COL_GOLD);

    centreTextX(lbl, x, w);
    float ly = y + (h - lbl.getLocalBounds().size.y) * 0.5f - 2.f;
    lbl.setPosition({lbl.getPosition().x, ly});
    win.draw(lbl);
}

bool UIButton::isHit(int mx, int my) const
{
    return FloatRect({x, y}, {w, h}).contains(Vector2f((float)mx, (float)my));
}

bool UIButton::isHit(float bx, float by) const
{
    return FloatRect({x, y}, {w, h}).contains(Vector2f(bx, by));
}

const char* UIButton::getLabel() const { return label; }
float UIButton::getX() const { return x; }
float UIButton::getY() const { return y; }
float UIButton::getW() const { return w; }
float UIButton::getH() const { return h; }

UIField::UIField() : x(0.f), y(0.f), w(0.f), h(0.f), len(0), active(false), isPassword(false) {
    label[0] = '\0';
    buf[0] = '\0';
}

void UIField::set(float nx, float ny, float nw, float nh,
                  const char* text, bool password)
{
    x = nx; y = ny; w = nw; h = nh; isPassword = password;
    Validator::strCopy(label, text ? text : "", 64);
    clear();
}

void UIField::setActive(bool a) { active = a; }

void UIField::append(unsigned int ch) { bufAppend(buf, len, 512, ch); }

void UIField::backspace() { bufBackspace(buf, len); }

void UIField::clear() { len = 0; buf[0] = '\0'; }

const char* UIField::getValue() const { return buf; }

bool UIField::getActive() const { return active; }

void UIField::draw(RenderWindow& win, const Font& font, bool blink) const
{
    Text lbl(font, label, 12);
    lbl.setFillColor(active ? COL_GOLD : COL_MUTED);
    lbl.setPosition({x, y - 20.f});
    win.draw(lbl);

    RectangleShape f = makeRect(x, y, w, h, active ? COL_ACTIVE : COL_FIELD);
    f.setOutlineThickness(active ? 2.f : 1.f);
    f.setOutlineColor(active ? COL_GOLD : COL_BORDER);
    win.draw(f);

    if (active)
        win.draw(makeRect(x, y, 3.f, h, COL_GOLD));

    char disp[512] = {};
    if (isPassword) maskPassword(buf, disp, 512);
    else Validator::strCopy(disp, buf, 512);

    Text tv(font, disp[0] ? disp : "", 15);
    tv.setFillColor(COL_WHITE);
    float ty = y + (h - tv.getLocalBounds().size.y) * 0.5f - 2.f;
    tv.setPosition({x + 10.f, ty});
    win.draw(tv);

    if (active && blink) {
        float cx = x + 10.f + tv.getLocalBounds().size.x + 2.f;
        win.draw(makeRect(cx, y + 6.f, 2.f, h - 12.f, COL_GOLD));
    }
}

bool UIField::isHit(int mx, int my) const
{
    return FloatRect({x, y}, {w, h}).contains(Vector2f((float)mx, (float)my));
}

UILabel::UILabel() : x(0.f), y(0.f), size(12), color(COL_WHITE) {
    text[0] = '\0';
}

void UILabel::set(float nx, float ny, const char* txt,
                  unsigned int sz, Color col)
{
    x = nx; y = ny; size = sz; color = col;
    Validator::strCopy(text, txt ? txt : "", 256);
}

void UILabel::setText(const char* txt)
{
    Validator::strCopy(text, txt ? txt : "", 256);
}

void UILabel::draw(RenderWindow& win, const Font& font) const
{
    Text t(font, text, size);
    t.setFillColor(color);
    t.setPosition({x, y});
    win.draw(t);
}

