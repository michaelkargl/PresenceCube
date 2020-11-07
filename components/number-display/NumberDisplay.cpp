#include "number_display.h"

NumberDisplay::NumberDisplay() : tft(TFT_eSPI())
{
    tft.init();
    tft.setRotation(0);
}

void NumberDisplay::clearScreen()
{
    tft.fillScreen(TFT_BLACK);
}

void NumberDisplay::displayNumber(int number)
{
    // Set "cursor" at top left corner of display (0,0) and select font 2
    const int cursorX = 35;
    const int cursorY = 30;
    const int font = 2;
    tft.setCursor(cursorX, cursorY, font);

    // set text style
    const int textSizeMultiplier = 6;
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(textSizeMultiplier);

    tft.println(number);
}

void NumberDisplay::displayEllipses(bool isFilled)
{
    const int screenSizeX = 160;
    const int screenSizeY = 128;
    const int maxRadius = screenSizeY; // default: 40

    for (int i = 0; i < 5; i++)
    {
        int rx = random(maxRadius);
        int ry = random(maxRadius);
        int x = rx + random(screenSizeX - rx - rx);
        int y = ry + random(screenSizeY - ry - ry);

        if (isFilled)
        {
            tft.fillEllipse(x, y, rx, ry, random(0xFFFF));
        }
        else
        {
            tft.drawEllipse(x, y, rx, ry, random(0xFFFF));
        }
    }
}