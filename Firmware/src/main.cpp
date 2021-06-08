#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define OLED_SCK PA5
#define OLED_SDI PA7
#define OLED_RST PA11
#define OLED_DC PA12
#define OLED_CS PA4

#define GM_INPUT PB1

char disp[20] = { 0 };
uint32_t count0 = 0;
uint32_t cpm = 0;
uint32_t usv_scaled = 0;

U8G2_SSD1306_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /*OLED_SCK, OLED_SDI,*/ OLED_CS, OLED_DC, OLED_RST);

void Count()
{
    count0++;
    while(!digitalRead(GM_INPUT));
}

void u8g2_prepare(void)
{
    u8g2.setFont(u8g2_font_9x15_tf);
    u8g2.setFontRefHeightExtendedText();
    u8g2.setDrawColor(1);
    u8g2.setFontPosTop();
    u8g2.setFontDirection(0);
}

void setup(void)
{
    u8g2.begin();
    u8g2_prepare();
    pinMode(GM_INPUT, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(GM_INPUT), Count, FALLING);
}

void loop(void)
{
    u8g2.clearBuffer();
    u8g2.drawStr(0, 1, "Geiger Counter");
    sprintf(disp, "Count: %ld/2s", count0);
    u8g2.drawStr(0, 17, disp);
    usv_scaled = count0 * 30 * 57;
    sprintf(disp, "%d.%04d uSv/hr", usv_scaled / 10000, usv_scaled % 10000);
    u8g2.drawStr(0, 33, disp);

    if(usv_scaled < 4000)
    {
        u8g2.drawStr(0, 49, ">>>> Safe <<<<");
    }
    else if(usv_scaled < 1000000)
    {
        u8g2.drawStr(0, 49, ">>> Safe-R <<<");
    }
    else
    {
        u8g2.drawStr(0, 49, "!!! DANGER !!!");
    }

    u8g2.sendBuffer();
    count0 = 0;
    delay(2000);
}
