#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

class NumberDisplay {
    private:
        TFT_eSPI tft;
    public:
        NumberDisplay();
        void clearScreen();
        void displayEllipses(bool isFilled);
        void displayNumber(int number);
};