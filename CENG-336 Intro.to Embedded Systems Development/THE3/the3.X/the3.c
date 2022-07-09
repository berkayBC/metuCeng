/*
 * GROUP INFO*
 * Anil Berdan Ceylan - 2304277
 * Berkay Bartug Cetin - 2309839
 * Fahri Numan Eskici - 2309995
 * Eray Erer - 2448348
 */

#include "Includes.h"
#include "lcd.h"
#include <xc.h>
#include <stdint.h>
#include <stdio.h>

#define TIMER0_PRELOAD 217

// variables
uint8_t inp_re0_btn_st = 0; // BUTTONS
uint8_t inp_re1_btn_st = 0;
uint8_t inp_re2_btn_st = 0;
uint8_t inp_re3_btn_st = 0;
uint8_t inp_re4_btn_st = 0;
uint8_t inp_re5_btn_st = 0;
int interruptCountTMR0 = 0;
uint8_t customNum = 0; // number of custom characters
uint8_t xCoord = 0;    // x coordinate of led cursor
uint8_t yCoord = 0;    // y coordinate of led cursor
uint8_t ledIndex = 0;  // indicates the index of the led cursor 0-15
uint16_t adcVal;       // value of ADC - used to determine the index of the led cursor
int adcReadFlag;
int digit = 0;
int currentLedIndex = 0;
int preLetterCount = 0;
int customCharCount = 0;
char lcdChars[16] = "                ";
char tempChars[16] = "                ";
uint8_t ms500WaitedFlag = 0;
uint8_t ms30WaitedFlag = 0;
int mode = 0; // mode 0 is TEM mode, mode 1 is CMD mode, mode 2 is TSM mode
uint8_t charmap[8][8] = {
    {0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000},
    {0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000},
    {0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000},
    {0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000},
    {0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000},
    {0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000},
    {0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000},
    {0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000,
     0b00000}};
int isFirstArrive = 0;
int customCharInsert = 0;
int isAtTheEdge;
int dummyCount = 0;
int check = 0;
int customArrayInfo[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}; // states indexes and the customCharCounts
// variables

// functions
void __interrupt() isr(void);
void set_xCoord_seven_segment();
void set_yCoord_seven_segment();
void set_custom_num_seven_segment();
void ADCtoLCD(uint16_t adcVal);
void initADC();
void tmr0_init();
void init_ports();
void TEMpressCheck();
void predefinedChange();
void customChange();
void modeChange();
void LCDdisplay();
char *leftRotate(char *lcdChars);
void stopInput();
void CDMcoordHelper();
void init_CDM_mode();
void CDMpressCheck();
void CMDledTask();
int *leftRotateArray(int *customArrayInfo);
void stopRE0and3();
void startRE0and3();
// functions

char *leftRotate(char *lcdChars)
{
  int j;
  char temp;
  // Stores the last element of the array
  temp = lcdChars[0];
  for (j = 0; j < 15; j++)
  {
    // Shift element of array by one
    lcdChars[j] = lcdChars[j + 1];
  }
  // Last element of the array will be added to the start of the array.
  lcdChars[15] = temp;
  return lcdChars;
}

int *leftRotateArray(int *customArrayInfo) // left rotating custom chars
{
  int j;
  int temp;
  // Stores the last element of the array
  temp = customArrayInfo[0];
  for (j = 0; j < 15; j++)
  {
    // Shift element of array by one
    customArrayInfo[j] = customArrayInfo[j + 1];
  }
  // Last element of the array will be added to the start of the array.
  customArrayInfo[15] = temp;
  return customArrayInfo;
}

void __interrupt() isr(void)
{
  interruptCountTMR0++;
  INTCONbits.T0IF = 0;
  TMR0 = TIMER0_PRELOAD;
  if (interruptCountTMR0 == 500) // flag for scroll mode shift time
  {
    ms500WaitedFlag = 1;
    interruptCountTMR0 = 0;
  }
  if (interruptCountTMR0 == 30)
  {
    ms30WaitedFlag = 1;
  }
  if (ADIF && mode == 0) // ADC interrupt check
  { // update LCD
    adcVal = (ADRESH << 8) + ADRESL;
    ADCtoLCD(adcVal);
    ADIF = 0;
  }
  digit = interruptCountTMR0 % 3;
  switch (digit)
  {
  case 0: // Rightmost digit
    set_custom_num_seven_segment();
    break;
  case 1: // Leftmost digit
    set_xCoord_seven_segment();
    break;
  case 2: // Leftmost digit
    set_yCoord_seven_segment();
    break;
  }
}

void set_xCoord_seven_segment()
{
  PORTH = 0b00000100; // 3th from left
  switch (xCoord)
  {
  case 0:
    PORTJ = 0b00111111;
    break;
  case 1:
    PORTJ = 0b00000110;
    break;
  case 2:
    PORTJ = 0b01011011;
    break;
  case 3:
    PORTJ = 0b01001111;
    break;
  }
}

void set_yCoord_seven_segment()
{
  PORTH = 0b00001000; // 4th from left
  switch (yCoord)
  {
  case 0:
    PORTJ = 0b00111111;
    break;
  case 1:
    PORTJ = 0b00000110;
    break;
  case 2:
    PORTJ = 0b01011011;
    break;
  case 3:
    PORTJ = 0b01001111;
    break;
  case 4:
    PORTJ = 0b01100110;
    break;
  case 5:
    PORTJ = 0b01101101;
    break;
  case 6:
    PORTJ = 0b01111101;
    break;
  case 7:
    PORTJ = 0b00000111;
    break;
  }
}

void set_custom_num_seven_segment()
{
  PORTH = 0b00000001; // first from left
  switch (customNum)
  {
  case 0:
    PORTJ = 0b00111111;
    break;
  case 1:
    PORTJ = 0b00000110;
    break;
  case 2:
    PORTJ = 0b01011011;
    break;
  case 3:
    PORTJ = 0b01001111;
    break;
  case 4:
    PORTJ = 0b01100110;
    break;
  case 5:
    PORTJ = 0b01101101;
    break;
  case 6:
    PORTJ = 0b01111101;
    break;
  case 7:
    PORTJ = 0b00000111;
    break;
  case 8:
    PORTJ = 0b01111111;
    break;
  }
}

void ADCtoLCD(uint16_t adcVal) // This function finds and sets cursor`s location according to potentiometer
{
  if (adcVal <= 63)
  {
    ledIndex = 0;
  }
  else if (adcVal >= 64 && adcVal <= 127)
  {
    ledIndex = 1;
  }
  else if (adcVal >= 128 && adcVal <= 191)
  {
    ledIndex = 2;
  }
  else if (adcVal >= 192 && adcVal <= 255)
  {
    ledIndex = 3;
  }
  else if (adcVal >= 256 && adcVal <= 319)
  {
    ledIndex = 4;
  }
  else if (adcVal >= 320 && adcVal <= 383)
  {
    ledIndex = 5;
  }
  else if (adcVal >= 384 && adcVal <= 447)
  {
    ledIndex = 6;
  }
  else if (adcVal >= 448 && adcVal <= 511)
  {
    ledIndex = 7;
  }
  else if (adcVal >= 512 && adcVal <= 575)
  {
    ledIndex = 8;
  }
  else if (adcVal >= 576 && adcVal <= 639)
  {
    ledIndex = 9;
  }
  else if (adcVal >= 640 && adcVal <= 703)
  {
    ledIndex = 10;
  }
  else if (adcVal >= 704 && adcVal <= 767)
  {
    ledIndex = 11;
  }
  else if (adcVal >= 768 && adcVal <= 831)
  {
    ledIndex = 12;
  }
  else if (adcVal >= 832 && adcVal <= 895)
  {
    ledIndex = 13;
  }
  else if (adcVal >= 896 && adcVal <= 959)
  {
    ledIndex = 14;
  }
  else if (adcVal >= 960)
  {
    ledIndex = 15;
  }
  if (ledIndex > currentLedIndex)
  {
    for (int i = 0; i < ledIndex - currentLedIndex; i++)
    {
      LcdCmd(0b00010100); // shift right
    }
    currentLedIndex = ledIndex;
  }
  else if (ledIndex < currentLedIndex)
  {
    for (int i = 0; i < currentLedIndex - ledIndex; i++)
    {
      LcdCmd(0b00010000); // shift left
    }
    currentLedIndex = ledIndex;
  }
  else
  {
    currentLedIndex = ledIndex;
  }
}

void init_irq() // timer enable
{
  INTCONbits.TMR0IE = 1;
  INTCONbits.TMR0IF = 0;
  INTCONbits.GIE = 1;
}

void tmr0_init() 
{
  // To achive 1 ms and use it in a loop our calculations is below
  //  256-TMR0 = T.(Fosc/4.PS)
  //  T = 1ms
  //  Fosc = 40MHz
  //  PS = 256
  //  TMR0 = 217
  T0CON = 0b11000111; // internal clock with 1:256 prescaler
  TMR0 = TIMER0_PRELOAD;
}

void init()
{
  // Set ADC Inputs
  TRISH = 0x10; // AN12 input RH4
  // Set LCD Outputs
  TRISB = 0x00; // LCD Control RB2/RB5
  TRISD = 0x00; // LCD Data  RD[4-7]
  // Configure ADC
  ADCON0 = 0x31; // Channel 12; Turn on AD Converter
  ADCON1 = 0x00; // All analog pins
  ADCON2 = 0xAA; // Right Align | 12 Tad | Fosc/32
  ADRESH = 0x00;
  ADRESL = 0x00;

  for (uint8_t i = 0; i < 8; i++) // clear the charmap 
  {
    for (uint8_t j = 0; j < 8; j++)
    {
      charmap[i][j] = 0;
    }
  }

  InitLCD();
  if (ms30WaitedFlag)
  {                     // after initialization wait for 30ms
    LcdCmd(0b00001110); // Display on, cursor off, blink off.
    ms30WaitedFlag = 0;
  }
}

void stopInput() // when we are in TSM mode stop getting input
{
  TRISA = 0x00;
  TRISB = 0x00;
  TRISC = 0x00;
  TRISD = 0x00;
  TRISE = 0x00;
  TRISF = 0x00;
  TRISG = 0x00;
  TRISH = 0x00;
  TRISJ = 0x00;
}

void init_ports() // only RE port is input remaining ports are output
{
  TRISA = 0x00;
  TRISB = 0x00;
  TRISC = 0x00;
  TRISD = 0x00;
  TRISE = 0b00111111; // RE-012345 input
  TRISF = 0x00;
  TRISG = 0x00;
  TRISH = 0x00;
  TRISJ = 0x00;

  LATA = 0x00;
  LATB = 0x00;
  LATC = 0x00;
  LATD = 0x00;
  LATE = 0x00;
  LATF = 0x00;
  LATG = 0x00;
  LATH = 0x00;
  LATJ = 0x00;

  PORTA = 0x00;
  PORTB = 0x00;
  PORTC = 0x00;
  PORTD = 0x00;
  PORTE = 0x00;
  PORTF = 0x00;
  PORTG = 0x00;
  PORTH = 0x00;
  PORTJ = 0x00;
}

void input_task() // checks which buttons are pressed
{
  if (PORTEbits.RE1)
  {
    inp_re1_btn_st = 1;
  }
  else if (PORTEbits.RE2)
  {
    inp_re2_btn_st = 1;
  }
  else if (PORTEbits.RE0)
  {
    inp_re0_btn_st = 1;
  }
  else if (PORTEbits.RE3)
  {
    inp_re3_btn_st = 1;
  }
  else if (PORTEbits.RE4)
  {
    inp_re4_btn_st = 1;
  }
  else if (PORTEbits.RE5)
  {
    inp_re5_btn_st = 1;
  }
}

void TEMpressCheck() // checks which buttons are pressed and released
{
  if ((!PORTEbits.RE1 && inp_re1_btn_st) || (!PORTEbits.RE2 && inp_re2_btn_st))
  {
    predefinedChange(); // if one of these buttons pressed we should rotate in the predefined chars
  }
  else if ((!PORTEbits.RE0 && inp_re0_btn_st) || (!PORTEbits.RE3 && inp_re3_btn_st))
  {
    customChange(); // if one of these buttons pressed we should rotate in the custom chars
  }
  else if ((!PORTEbits.RE4 && inp_re4_btn_st) || (!PORTEbits.RE5 && inp_re5_btn_st))
  {
    modeChange(); // if one of these buttons pressed we should change mode
  }
}

void customChange() // circular rotation of custom defined chars
{ 
  if (customNum != 0 || customNum != 8)
  {
    if (inp_re3_btn_st)
    {
      inp_re3_btn_st = 0;

      if (customCharInsert == 0)
      {
        customCharInsert = 1;
        customCharCount = customNum - 1;
      }
      else
      {
        if (customCharCount == 0)
        {
          customCharCount = customNum - 1;
          customCharInsert = 0;
        }
        else
        {
          customCharCount -= 1;
        }
      }
    }
    else if (inp_re0_btn_st)
    {
      inp_re0_btn_st = 0;

      if (customCharInsert == 0)
      {
        customCharInsert = 1;
        customCharCount = 0;
      }
      else
      {
        if (customCharCount == customNum - 1)
        {
          customCharCount = 0;
          customCharInsert = 0;
        }
        else
        {
          customCharCount += 1;
        }
      }
    }
    LCDdisplay();
  }
}

void CDMpressCheck() // when we are in CDM mode, the function checks if pressed and releases RE5 which leads to change mode
{
  if ((!PORTEbits.RE5 && inp_re5_btn_st))
  {
    modeChange();
  }
}

void CDMcoordHelper() // this function follows the coordinates of cursor(CDM)
{
  if (!PORTEbits.RE1 && inp_re1_btn_st == 1)
  {
    inp_re1_btn_st = 0;
    if (yCoord + 1 > 7)
    {
      yCoord = 7;
    }
    else
    {
      yCoord++;
    }
  }
  else if (!PORTEbits.RE2 && inp_re2_btn_st == 1)
  {
    inp_re2_btn_st = 0;
    if (yCoord - 1 < 0)
    {
      yCoord = 0;
    }
    else
    {
      yCoord--;
    }
  }
  else if (!PORTEbits.RE3 && inp_re3_btn_st == 1)
  {
    inp_re3_btn_st = 0;
    if (xCoord - 1 < 0)
    {
      xCoord = 0;
    }
    else
    {
      xCoord--;
    }
  }
  else if (!PORTEbits.RE0 && inp_re0_btn_st == 1)
  {
    inp_re0_btn_st = 0;
    if (xCoord + 1 > 3)
    {
      xCoord = 3;
    }
    else
    {
      xCoord++;
    }
  }
}

void predefinedChange() // circular rotation of predefined chars
{
  if (inp_re1_btn_st)
  {
    inp_re1_btn_st = 0;
    customCharInsert = 0;
    if (preLetterCount == 0)
    {
      preLetterCount = 36;
    }
    else
    {
      preLetterCount -= 1;
    }
  }
  else if (inp_re2_btn_st)
  {
    inp_re2_btn_st = 0;
    customCharInsert = 0;
    if (preLetterCount == 36)
    {
      preLetterCount = 0;
    }
    else
    {
      preLetterCount += 1;
    }
  }
  LCDdisplay();
}

void LCDdisplay()
{
  if (customCharInsert == 0)
  { // checks whether we are rotating in Predefined chars or custom chars
    if (currentLedIndex == 0)
    {
      LcdCmd(0x80);                        // moves cursor to appropriate location
      LcdData(PREDEFINED[preLetterCount]); // Displays  predefined char on the LCD
      LcdCmd(0b00010000);                  // Left shifts cursor after display
    }
    else if (currentLedIndex == 1)
    {
      LcdCmd(0x81);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 2)
    {
      LcdCmd(0x82);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 3)
    {
      LcdCmd(0x83);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 4)
    {
      LcdCmd(0x84);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 5)
    {
      LcdCmd(0x85);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 6)
    {
      LcdCmd(0x86);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 7)
    {
      LcdCmd(0x87);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 8)
    {
      LcdCmd(0x88);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 9)
    {
      LcdCmd(0x89);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 10)
    {
      LcdCmd(0x8a);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 11)
    {
      LcdCmd(0x8b);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 12)
    {
      LcdCmd(0x8c);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 13)
    {
      LcdCmd(0x8d);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 14)
    {
      LcdCmd(0x8e);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    else if (currentLedIndex == 15)
    {
      LcdCmd(0x8f);
      LcdData(PREDEFINED[preLetterCount]);
      LcdCmd(0b00010000);
    }
    lcdChars[currentLedIndex] = PREDEFINED[preLetterCount];
  }
  else
  { // we are rotating in custom chars

    if (currentLedIndex == 0)
    {
      LcdCmd(0x80);             // moves cursor to appropriate location
      LcdData(customCharCount); // Displays custom char on the LCD
      LcdCmd(0b00010000);       // left shifts cursor after display
      customArrayInfo[0] = customCharCount;
    }
    else if (currentLedIndex == 1)
    {
      LcdCmd(0x81);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[1] = customCharCount;
    }
    else if (currentLedIndex == 2)
    {
      LcdCmd(0x82);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[2] = customCharCount;
    }
    else if (currentLedIndex == 3)
    {
      LcdCmd(0x83);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[3] = customCharCount;
    }
    else if (currentLedIndex == 4)
    {
      LcdCmd(0x84);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[4] = customCharCount;
    }
    else if (currentLedIndex == 5)
    {
      LcdCmd(0x85);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[5] = customCharCount;
    }
    else if (currentLedIndex == 6)
    {
      LcdCmd(0x86);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[6] = customCharCount;
    }
    else if (currentLedIndex == 7)
    {
      LcdCmd(0x87);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[7] = customCharCount;
    }
    else if (currentLedIndex == 8)
    {
      LcdCmd(0x88);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[8] = customCharCount;
    }
    else if (currentLedIndex == 9)
    {
      LcdCmd(0x89);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[9] = customCharCount;
    }
    else if (currentLedIndex == 10)
    {
      LcdCmd(0x8a);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[10] = customCharCount;
    }
    else if (currentLedIndex == 11)
    {
      LcdCmd(0x8b);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[11] = customCharCount;
    }
    else if (currentLedIndex == 12)
    {
      LcdCmd(0x8c);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[12] = customCharCount;
    }
    else if (currentLedIndex == 13)
    {
      LcdCmd(0x8d);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[13] = customCharCount;
    }
    else if (currentLedIndex == 14)
    {
      LcdCmd(0x8e);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[14] = customCharCount;
    }
    else if (currentLedIndex == 15)
    {
      LcdCmd(0x8f);
      LcdData(customCharCount);
      LcdCmd(0b00010000);
      customArrayInfo[15] = customCharCount;
    }
  }
}

void modeChange() // this function changes modes according to buttons that are pressed
{
  if (mode == 1 && inp_re5_btn_st)
  {
    inp_re5_btn_st = 0;
    mode = 0;
    customNum++; // one custom character defined, increase number of custom characters
    xCoord = 0;
    yCoord = 0;
    uint8_t temp = 0x80;

    for (int i = 0; i < 16; i++) // when mode changes to TEM we write back the old text to LCD
    { 
      LcdCmd(temp);
      temp += 1;
      LcdData(lcdChars[i]);
    }
    LCDdisplay();
  }
  else if (mode == 0 && inp_re5_btn_st)
  {
    inp_re5_btn_st = 0;
    mode = 2;
  }
  else if (mode == 0 && inp_re4_btn_st)
  {
    inp_re4_btn_st = 0;
    mode = 1;
    isFirstArrive = 1;
  }
}

void TSM()
{
  stopInput();  // stops input
  LcdCmd(0x0C); // makes cursor invisible

  LcdCmd(0x80); // write finished to first line
  LcdData(' ');
  LcdData(' ');
  LcdData(' ');
  LcdData('f');
  LcdData('i');
  LcdData('n');
  LcdData('i');
  LcdData('s');
  LcdData('h');
  LcdData('e');
  LcdData('d');
  LcdData(' ');
  LcdData(' ');
  LcdData(' ');
  LcdData(' ');
  LcdData(' ');

  uint8_t temp = 0xC0;
  for (int i = 0; i < 16; i++) // writing the predefined char text to second line same with same X axis 
  {
    LcdCmd(temp);
    temp += 1;
    LcdData(lcdChars[i]);
  }
  temp = 0xC0;
  for (int i = 0; i < 16; i++) // writing the custom defined char text to second line same with same X axis
  {
    LcdCmd(temp);
    temp += 1;
    if (customArrayInfo[i] != -1)
    {
      LcdData(customArrayInfo[i]);
    }
  }

  if (ms500WaitedFlag) //  in every 500ms shift chars to left
  {
    ms500WaitedFlag = 0;
    temp = 0xC0;
    leftRotate(lcdChars); // circular left rotation of lcdChars
    leftRotateArray(customArrayInfo); // circular left rotation of custom chars
    for (int i = 0; i < 16; i++) // writing to second line the shifted text
    {
      LcdCmd(temp);
      temp += 1;
      if (customArrayInfo[i] != -1) // customArrayInfo holds the custom chars indexes and positions
      {                             // first initialized with all -1 
        LcdData(customArrayInfo[i]);
      }
    }
    temp = 0xc0;
    for (int i = 0; i < 16; i++) // writing to second line the shifted text
    {
      LcdCmd(temp);
      temp += 1;
      LcdData(lcdChars[i]);
    }
  }
}

void init_CDM_mode()
{
  LcdClear();   // when the mode changes clear lcd
  InitLCD();    // initialize again
  LcdCmd(0x0e); // move cursor to first cell of first line

  LATA = 0x00; //  turn off the LEDs
  LATB = 0x00;
  LATC = 0x00;
  LATD = 0x00;
}

int re4_pressed() // checks re4 pressed for to turn on LEDs and correct location on the LCD
{
  if (inp_re4_btn_st && !PORTEbits.RE4)
  {
    inp_re4_btn_st = 0;
    return 1;
  }
  else
  {
    return 0;
  }
}

void CMDledTask() // turns on the appropriate LEDs if re4 pressed
{ 

  uint8_t bitStatus = 0;

  for (int i = 0; i < 8; i++)
  {
    for (int j = 4; j > 0; j--)
    {
      if (j == 4 && i == 0)
      {
        bitStatus = ((charmap[customNum][0] >> j) & 1);
        PORTAbits.RA0 = bitStatus;
      }
      else if (j == 3 && i == 0)
      {
        bitStatus = ((charmap[customNum][0] >> j) & 1);
        PORTBbits.RB0 = bitStatus;
      }
      else if (j == 2 && i == 0)
      {
        bitStatus = ((charmap[customNum][0] >> j) & 1);
        PORTCbits.RC0 = bitStatus;
      }
      else if (j == 1 && i == 0)
      {
        bitStatus = ((charmap[customNum][0] >> j) & 1);
        PORTDbits.RD0 = bitStatus;
      }
      //
      else if (j == 4 && i == 1)
      {
        bitStatus = ((charmap[customNum][1] >> j) & 1);
        PORTAbits.RA1 = bitStatus;
      }
      else if (j == 3 && i == 1)
      {
        bitStatus = ((charmap[customNum][1] >> j) & 1);
        PORTBbits.RB1 = bitStatus;
      }
      else if (j == 2 && i == 1)
      {
        bitStatus = ((charmap[customNum][1] >> j) & 1);
        PORTCbits.RC1 = bitStatus;
      }
      else if (j == 1 && i == 1)
      {
        bitStatus = ((charmap[customNum][1] >> j) & 1);
        PORTDbits.RD1 = bitStatus;
      }
      //
      else if (j == 4 && i == 2)
      {
        bitStatus = ((charmap[customNum][2] >> j) & 1);
        PORTAbits.RA2 = bitStatus;
      }
      else if (j == 3 && i == 2)
      {
        bitStatus = ((charmap[customNum][2] >> j) & 1);
        PORTBbits.RB2 = bitStatus;
      }
      else if (j == 2 && i == 2)
      {
        bitStatus = ((charmap[customNum][2] >> j) & 1);
        PORTCbits.RC2 = bitStatus;
      }
      else if (j == 1 && i == 2)
      {
        bitStatus = ((charmap[customNum][2] >> j) & 1);
        PORTDbits.RD2 = bitStatus;
      }
      else if (j == 4 && i == 3)
      {
        bitStatus = ((charmap[customNum][3] >> j) & 1);
        PORTAbits.RA3 = bitStatus;
      }
      else if (j == 3 && i == 3)
      {
        bitStatus = ((charmap[customNum][3] >> j) & 1);
        PORTBbits.RB3 = bitStatus;
      }
      else if (j == 2 && i == 3)
      {
        bitStatus = ((charmap[customNum][3] >> j) & 1);
        PORTCbits.RC3 = bitStatus;
      }
      else if (j == 1 && i == 3)
      {
        bitStatus = ((charmap[customNum][3] >> j) & 1);
        PORTDbits.RD3 = bitStatus;
      }
      else if (j == 4 && i == 4)
      {
        bitStatus = ((charmap[customNum][4] >> j) & 1);
        PORTAbits.RA4 = bitStatus;
      }
      else if (j == 3 && i == 4)
      {
        bitStatus = ((charmap[customNum][4] >> j) & 1);
        PORTBbits.RB4 = bitStatus;
      }
      else if (j == 2 && i == 4)
      {
        bitStatus = ((charmap[customNum][4] >> j) & 1);
        PORTCbits.RC4 = bitStatus;
      }
      else if (j == 1 && i == 4)
      {
        bitStatus = ((charmap[customNum][4] >> j) & 1);
        PORTDbits.RD4 = bitStatus;
      }
      else if (j == 4 && i == 5)
      {
        bitStatus = ((charmap[customNum][5] >> j) & 1);
        PORTAbits.RA5 = bitStatus;
      }
      else if (j == 3 && i == 5)
      {
        bitStatus = ((charmap[customNum][5] >> j) & 1);
        PORTBbits.RB5 = bitStatus;
      }
      else if (j == 2 && i == 5)
      {
        bitStatus = ((charmap[customNum][5] >> j) & 1);
        PORTCbits.RC5 = bitStatus;
      }
      else if (j == 1 && i == 5)
      {
        bitStatus = ((charmap[customNum][5] >> j) & 1);
        PORTDbits.RD5 = bitStatus;
      }
      else if (j == 3 && i == 6)
      {
        bitStatus = ((charmap[customNum][6] >> j) & 1);
        PORTBbits.RB6 = bitStatus;
      }
      else if (j == 2 && i == 6)
      {
        bitStatus = ((charmap[customNum][6] >> j) & 1);
        PORTCbits.RC6 = bitStatus;
      }
      else if (j == 1 && i == 6)
      {
        bitStatus = ((charmap[customNum][6] >> j) & 1);
        PORTDbits.RD6 = bitStatus;
      }
      else if (j == 3 && i == 7)
      {
        bitStatus = ((charmap[customNum][7] >> j) & 1);
        PORTBbits.RB7 = bitStatus;
      }
      else if (j == 2 && i == 7)
      {
        bitStatus = ((charmap[customNum][7] >> j) & 1);
        PORTCbits.RC7 = bitStatus;
      }
      else if (j == 1 && i == 7)
      {
        bitStatus = ((charmap[customNum][7] >> j) & 1);
        PORTDbits.RD7 = bitStatus;
      }
    }
    bitStatus = 0;
  }
}

void CDM()
{
  if (customNum == 8)
  {
    mode = 0;
    return;
  }
  startRE0and3();
  input_task();

  if (re4_pressed())
  {
    charmap[customNum][yCoord] ^= 1 << (4 - xCoord); // toggle according to coordinates
    CMDledTask();
    if (customNum == 0)
    {
      LcdCmd(0x40);               // Set CGRAM address 0 -> 0x40
      for (int i = 0; i < 8; i++) // Start sending charmap
      {
        LcdData(charmap[customNum][i]);
      }
      LcdCmd(0x80);
      LcdData(0);
      LcdCmd(0b00010000);
    }
    else if (customNum == 1) 
    {
      LcdCmd(0x48);             // Set CGRAM address 1 -> 0x48
      for (int i = 0; i < 8; i++) // Start sending charmap
      {
        LcdData(charmap[customNum][i]);
      }
      LcdCmd(0x80);
      LcdData(1);
      LcdCmd(0b00010000);
    }
    else if (customNum == 2)
    {
      LcdCmd(0x50);
      for (int i = 0; i < 8; i++)
      {
        LcdData(charmap[customNum][i]);
      }
      LcdCmd(0x80);
      LcdData(2);
      LcdCmd(0b00010000);
    }
    else if (customNum == 3)
    {
      LcdCmd(0x58);
      for (int i = 0; i < 8; i++)
      {
        LcdData(charmap[customNum][i]);
      }
      LcdCmd(0x80);
      LcdData(3);
      LcdCmd(0b00010000);
    }
    else if (customNum == 4)
    {
      LcdCmd(0x60);
      for (int i = 0; i < 8; i++)
      {
        LcdData(charmap[customNum][i]);
      }
      LcdCmd(0x80);
      LcdData(4);
      LcdCmd(0b00010000);
    }
    else if (customNum == 5)
    {
      LcdCmd(0x68);
      for (int i = 0; i < 8; i++)
      {
        LcdData(charmap[customNum][i]);
      }
      LcdCmd(0x80);
      LcdData(5);
      LcdCmd(0b00010000);
    }
    else if (customNum == 6)
    {
      LcdCmd(0x70);
      for (int i = 0; i < 8; i++)
      {
        LcdData(charmap[customNum][i]);
      }
      LcdCmd(0x80);
      LcdData(6);
      LcdCmd(0b00010000);
    }
    else if (customNum == 7)
    {
      LcdCmd(0x78);
      for (int i = 0; i < 8; i++)
      {
        LcdData(charmap[customNum][i]);
      }
      LcdCmd(0x80);
      LcdData(7);
      LcdCmd(0b00010000);
    }
  }
}

void stopRE0and3() // when there is not any custom char we don`t allow to rotate 
{
  TRISE = 0b00110110;
}

void startRE0and3() 
{
  TRISE = 0b00111111;
}

// First our program does initializations such as ADC, TMR0, ADC interrupt and the ports. 
// Secondly, ADC conversion starts.
// Then we start the default mode which is TEM mode
// TEM mode includes input_task() which accordingly does the necessary changes with the chars and the modes
// If mode changes to TSM we send our text to second line and left shift constantly and circularly
// If mode changes to CDM we start to create custom chars and send them to correct locations of CGRAM
// While we are creating the custom chars we also turn the LEDs accordingly.
// Then mode changes to TEM but this time we can insert custom chars as well and also we reset the coordinates of cursor in CDM
// While we are doing mode changes we constantly check edges like custom defined char number
// Finally, we change mode to TSM and with the predefined chars we also send custom defined chars to second line and left shift circularly.

void main(void)
{
  init_ports();
  tmr0_init();
  init_irq();
  init();
  while (1)
  {
    GODONE = 1; // Start ADC conversion

    if (mode == 0)
    {
      if (customNum == 0) // when there is not any custom char we don`t allow to rotate
      {
        stopRE0and3();
      }
      else
      {
        startRE0and3();
      }
      input_task();
      TEMpressCheck();
    }
    else if (mode == 2)
    {

      TSM();
    }
    else if (mode == 1)
    {
      if (customNum != 8) //if there are 8 custom defined chars then we don`t allow to enter CDM mode
      {
        input_task();
        CDMpressCheck();
        CDMcoordHelper();
        if (isFirstArrive) // first arrive to CDM, do the initializations for CDM mode
        {
          init_CDM_mode();
          isFirstArrive = 0;
        }
      }
      CDM();
    }
  }
}