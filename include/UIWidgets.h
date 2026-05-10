#ifndef UIWIDGETS_H
#define UIWIDGETS_H

#include "UITheme.h"

class UIButton {
private:
    float x, y, w, h;
    char label[64];
    int style;
public:
    UIButton();
    void set(float x, float y, float w, float h, const char* label, int style = 1);
    void draw(sf::RenderWindow& win, const sf::Font& font, int mx, int my) const;
    bool isHit(int mx, int my) const;
    bool isHit(float bx, float by) const;
    const char* getLabel() const;
    float getX() const; float getY() const;
    float getW() const; float getH() const;
};

class UIField {
private:
    float x, y, w, h;
    char label[64];
    char buf[512];
    int len;
    bool active;
    bool isPassword;
public:
    UIField();
    void set(float x, float y, float w, float h,
             const char* label, bool isPassword = false);
    void setActive(bool a);
    void append(unsigned int ch);
    void backspace();
    void clear();
    const char* getValue() const;
    bool getActive() const;
    void draw(sf::RenderWindow& win, const sf::Font& font, bool blink) const;
    bool isHit(int mx, int my) const;
};

class UILabel {
private:
    float x, y;
    char text[256];
    unsigned int size;
    sf::Color color;
public:
    UILabel();
    void set(float x, float y, const char* text,
             unsigned int size, sf::Color color);
    void setText(const char* text);
    void draw(sf::RenderWindow& win, const sf::Font& font) const;
};

#endif
