/* 
 * Anil Berdan Ceylan 2304277
 * Berkay Bartug Cetin 2309839
 * Fahri Numan Eskici 2309995
 * Eray Erer 2448348
 */
#pragma config OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

#include <xc.h>
#include <stdint.h>

#define TIMER0_PRELOAD 217

// Variables
uint8_t health = 9;
uint16_t tmr1Value;
uint8_t rightMost3bit;
uint8_t lightA;
uint8_t level = 1;
uint8_t gameStarted = 0;
uint8_t gameEnded = 0;
uint8_t ms500WaitedFlag = 0;
uint8_t ms400WaitedFlag = 0;
uint8_t ms300WaitedFlag = 0;
uint8_t firstNote = 1;
int generatedNotesNum = 0;
int interruptCountTMR0 = 0;
int bounceCount = 0;
uint8_t portGFlag;
int RGPressed;
uint8_t portGFlagOnPress;
int lastCountOnPress;
int restartGame = 0;

uint8_t inp_rc0_btn_st = 0;
uint8_t inp_rg0_btn_st = 0;
uint8_t inp_rg1_btn_st = 0;
uint8_t inp_rg2_btn_st = 0;
uint8_t inp_rg3_btn_st = 0;
uint8_t inp_rg4_btn_st = 0;

uint8_t rf0_status;
uint8_t rf1_status;
uint8_t rf2_status;
uint8_t rf3_status;
uint8_t rf4_status;

int rf_checktime = 0;

int rg0_checktime = 0;
int rg1_checktime = 0;
int rg2_checktime = 0;
int rg3_checktime = 0;
int rg4_checktime = 0;

int isPressingRG0 = 0;
int isPressingRG1 = 0;
int isPressingRG2 = 0;
int isPressingRG3 = 0;
int isPressingRG4 = 0;

int miliseconds[3] = {500, 400, 300};
int successfullPressNum = 0;
int lastShiftCount = 0;

// Variables

// Functions
uint16_t rightRotate(uint16_t n, unsigned int d);
int count1s(int n);
void set_level_seven_segment();
void set_health_seven_segment();
void set_lose_seven_segment(int digit);
void set_end_seven_segment(int digit);
void init_game_configurations();
void init_ports();
void init_irq();
void tmr_init();
void generate_notes();
void shift();
void input_task();
void __interrupt() isr(void);
// Functions

/*
 * Rotate byte 'n' to right by 'd'.
 */
uint16_t rightRotate(uint16_t n, unsigned int d) {
    return (n >> d) | (n << (16 - d));
}

/*
 * Counts 1s in given 'int n'.
 */
int count1s(int n) {
    int result = 0;

    while (n > 0) {
        if (n & 1) {
            result++;
        }
        n = n >> 1;
    }
    return result;
}

/*
 * Write current level to rightmost digit of seven segment display.
 */
void set_level_seven_segment() {
    PORTH = 0b00001000; // Pick rightmost digit
    switch (level) {
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

/*
 * Write current health to leftmost digit of seven segment display.
 */
void set_health_seven_segment() {
    PORTH = 0b00000001; // Pick leftmost digit
    switch (health) {
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
        case 9:
            PORTJ = 0b01101111;
            break;
    }
}

/*
 * Write 'LOSE' to seven segment display. This function takes 
 * one parameter, int digit. In ISR, this function 
 * is called with 1 ms intervals for every digit continuously.
 * Write 'E' for 4th digit,
 * Write 'S' for 3th digit,
 * Write 'O' for 2th digit,
 * Write 'L' for 1th digit,
 * Write 'E' for 4th digit, and so on.
 */
void set_lose_seven_segment(int digit) {
    switch (digit) {
        case 3: //  Leftmost digit 'L'
            PORTH = 0b00000001;
            PORTJ = 0b00111000;
            break;
        case 2: // 2nd digit 'O'
            PORTH = 0b00000010;
            PORTJ = 0b00111111;
            break;
        case 1: // 3rd digit 'S'
            PORTH = 0b00000100;
            PORTJ = 0b01101101;
            break;
        case 0: // Rightmost digit 'E'
            PORTH = 0b00001000;
            PORTJ = 0b01111001;
            break;
    }
}

/*
 * Write 'End' to seven segment display. This function takes 
 * one parameter, int digit. In ISR, this function 
 * is called with 1 ms intervals for leftmost three digits continuously.
 * Write 'd' for 3th digit,
 * Write 'n' for 2th digit,
 * Write 'E' for 1th digit,
 * Write 'd' for 3th digit, and so on.
 */
void set_end_seven_segment(int digit) {
    switch (digit) {
        case 2: // Leftmost digit 'E'
            PORTH = 0b00000001;
            PORTJ = 0b01111001;
            break;
        case 1: // 2nd digit 'n'
            PORTH = 0b00000010;
            PORTJ = 0b01010100;
            break;
        case 0: // 3rd digit 'd'
            PORTH = 0b00000100;
            PORTJ = 0b01011110;
            break;
    }
}

/*
 * Initialize game configurations.
 */
void init_game_configurations() {
    level = 1;
    firstNote = 1;
    health = 9;
    interruptCountTMR0 = 0;
    ms500WaitedFlag = 0;
    ms400WaitedFlag = 0;
    ms300WaitedFlag = 0;
    generatedNotesNum = 0;
    portGFlag = 0x00;
    RGPressed = 0;
    successfullPressNum = 0;
    lastShiftCount = 0;
}

/*
 * Initialize all ports.
 */
void init_ports() {
    TRISA = 0x00; // RA`s are all output
    TRISB = 0x00; // RB`s are all output
    TRISC = 0x01; // RC0 is input
    TRISD = 0x00; // RD`s are all output
    TRISE = 0x00; // RE`s are all output
    TRISF = 0x00; // RF`s are all output
    TRISG = 0x1f; // RG0-RG4 are input
    TRISH = 0x00; // RH`s are all output
    TRISJ = 0x00; // RJ`s are all output
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

    ADCON1 = 0x0F;
}

/*
 * Initialize INCTON bits.
 */
void init_irq() {
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCONbits.GIE = 1;
}

/*
 * Initialize timers.
 */
void tmr_init() {
    //To achive 1 ms and use it in a loop our calculations is below 
    // 256-TMR0 = T.(Fosc/4.PS)
    // T = 1ms
    // Fosc = 40MHz
    // PS = 256
    // TMR0 = 217
    T0CON = 0b11000111; // internal clock with 1:256 prescaler 
    TMR0 = TIMER0_PRELOAD;
    T1CONbits.TMR1ON = 1; // We enable TIMER1
    T1CONbits.RD16 = 1; // We set TIMER1 16 bits Read/Write Mode
}

/*
 * Generate notes randomly while gameStarted is true. 
 * This function is called by main(), when related 
 * msXXXWaitedFlag is true. msXXXWaitedFlag is set
 * in ISR.
 */
void generate_notes() {
    if (gameStarted == 1) {
        generatedNotesNum++; // level counter part
        if (generatedNotesNum <= 5) {
            level = 1;
        } else if (generatedNotesNum > 5 && generatedNotesNum <= 15) {
            level = 2;
        } else if (generatedNotesNum > 15 && generatedNotesNum <= 30) {
            level = 3;
        }
        

        rightMost3bit = tmr1Value & 0b00000111;
        lightA = rightMost3bit % 5;

        switch (lightA) {
            case 0:
                LATA = 0b00000001;
                break;
            case 1:
                LATA = 0b00000010;
                break;
            case 2:
                LATA = 0b00000100;
                break;
            case 3:
                LATA = 0b00001000;
                break;
            case 4:
                LATA = 0b00010000;
                break;
            default:
                break;
        }

        unsigned int rotateCount;

        if (level == 1)rotateCount = 1;
        else if (level == 2) rotateCount = 3;
        else if (level == 3) rotateCount = 5;

        tmr1Value = rightRotate(tmr1Value, rotateCount);
    }
}

/*
 * Shifts all notes to right one by one.
 * This function is called by main(), when related 
 * msXXXWaitedFlag is true. msXXXWaitedFlag is set
 * in ISR.
 * If a note was placed in RFx in previous call and
 * player misses to press RGx at time, health is 
 * decremented in current call.
 */
void shift() {
    uint8_t portA;
    uint8_t portB;
    uint8_t portC;
    uint8_t portD;
    uint8_t portE;
    uint8_t portF;
    uint8_t ports[6] = {portA, portB, portC, portD, portE, portF};
    uint8_t latches[6] = {LATA, LATB, LATC, LATD, LATE, LATF};
    
    for (int i = 0; i < 6; i++) {
        ports[i] = latches[i];
    }
        LATB = ports[0];
        LATC = ports[1];
        LATD = ports[2];
        LATE = ports[3];
        LATF = ports[4];
    

    // Decrease health when player fails to press
    // the button.
    if (ports[5] != 0x00) {
        --health;
    }


}

// This function is called by main constantly to check if RC0 is pressed in other words if game started.
void input_task() {
    if (PORTCbits.RC0) inp_rc0_btn_st = 1;
}

//This function is called by interrupt`s ISR function to see which leds are on.
void check_portf() {
    if (LATF == 0x01) rf0_status = 1;
    else if (LATF == 0b00000010) rf1_status = 1;
    else if (LATF == 0b00000100) rf2_status = 1;
    else if (LATF == 0b00001000) rf3_status = 1;
    else if (LATF == 0b00010000) rf4_status = 1;

}

void reset_portf() {
    rf0_status = 0;
    rf1_status = 0;
    rf2_status = 0;
    rf3_status = 0;
    rf4_status = 0;
}

void reset_portg() {
    inp_rg0_btn_st = 0;
    inp_rg1_btn_st = 0;
    inp_rg2_btn_st = 0;
    inp_rg3_btn_st = 0;
    inp_rg4_btn_st = 0;
}

/*
 * ISR has three functionality.
 *  - It continuously write related informations to seven
 *  digit display, digit by digit, with 1 ms intervals.
 *  - It counts interruptCountTMR0 ,which is incremented by one in 1 ms,
 *  then sets ms500WaitedFlag | ms400WaitedFlag | ms300WaitedFlag 
 *  according to current level.
 * - Checks the pressed buttons whether they are right or not.
 */
void __interrupt() isr(void) {
    INTCONbits.T0IF = 0;
    interruptCountTMR0++;
    TMR0 = TIMER0_PRELOAD;

    if (gameStarted) {
        // Seven segment display 
        // Write every digit one by one with 1 ms interval
        // leftMost digit -> health, rightMost digit -> level, others -> null
        int digit = interruptCountTMR0 % 2;
        switch (digit) {
            case 0: // Rightmost digit
                set_level_seven_segment();
                break;
            case 1: // Leftmost digit
                set_health_seven_segment();
                break;
        }

        // Wait for new note generation
        if (interruptCountTMR0 >= miliseconds[level - 1] && level == 1) {
            interruptCountTMR0 = 0;
            ms500WaitedFlag = 1;
        } else if (interruptCountTMR0 >= miliseconds[level - 1] && level == 2) {
            interruptCountTMR0 = 0;
            ms400WaitedFlag = 1;
        } else if (interruptCountTMR0 >= miliseconds[level - 1] && level == 3) {
            interruptCountTMR0 = 0;
            ms300WaitedFlag = 1;
        }

        // RF check
        reset_portf();
        check_portf();


        //RG check
        if (PORTGbits.RG0) {
            isPressingRG0 = 1;
        } else if (!PORTGbits.RG0 && isPressingRG0) {
            inp_rg0_btn_st = 1;
            isPressingRG0 = 0;
        }
        if (PORTGbits.RG1) {
            isPressingRG1 = 1;
        } else if (!PORTGbits.RG1 && isPressingRG1) {
            inp_rg1_btn_st = 1;
            isPressingRG1 = 0;
        }
        if (PORTGbits.RG2) {
            isPressingRG2 = 1;
        } else if (!PORTGbits.RG2 && isPressingRG2) {
            inp_rg2_btn_st = 1;
            isPressingRG2 = 0;
        }
        if (PORTGbits.RG3) {
            isPressingRG3 = 1;
        } else if (!PORTGbits.RG3 && isPressingRG3) {
            inp_rg3_btn_st = 1;
            isPressingRG3 = 0;
        }
        if (PORTGbits.RG4) {
            isPressingRG4 = 1;
        } else if (!PORTGbits.RG4 && isPressingRG4) {
            inp_rg4_btn_st = 1;
            isPressingRG4 = 0;
        }

        //This code segment checks whether right button pressed or not.
        if (inp_rg0_btn_st) {
            LATF = 0x00;
            if (rf0_status) { // True
            } else { // False
                health--;
            }
        } else if (inp_rg1_btn_st) {
            LATF = 0x00;
            if (rf1_status) { 
            } else { 
                health--;
            }
        } else if (inp_rg2_btn_st) {
            LATF = 0x00;
            if (rf2_status) { 
            } else {
                health--;
            }
        } else if (inp_rg3_btn_st) {
            LATF = 0x00;
            if (rf3_status) { 
            } else { 
                health--;
            }
        } else if (inp_rg4_btn_st) {
            LATF = 0x00;
            if (rf4_status) { 
            } else { 
                health--;
            }
        }
        reset_portg();
    } else if (gameEnded) { // Game ended successfully, write 'End'.
        int digit = interruptCountTMR0 % 4;
        set_end_seven_segment(digit);
    } else {
        if (health == 0) { // Player loses, write 'LOSE'.
            int digit = interruptCountTMR0 % 4;
            set_lose_seven_segment(digit);
        }
    }
}

void main(void) {
    while (1) {
        CLRWDT();
        init_ports();
        tmr_init();
        init_irq();
        init_game_configurations();
        while (1) {
            input_task();
            if (gameEnded) {
                // When game successfully finished, end the game and
                // wait for RC0 input to restart the game.
                gameStarted = 0;
                // Set RC0 as input
                LATC = 0x00;
                TRISC = 0x01;
                while (1) {
                    // Wait for RC0 press to restart the game.
                    inp_rc0_btn_st = 0;
                    input_task();
                    restartGame = 0;
                    if (inp_rc0_btn_st) {
                        restartGame = 1;
                        break;
                    }
                }
                if (restartGame == 1) {
                    // If RC0 button pressed in above loop,
                    // break second main loop and restart game
                    // with default configurations.
                    gameEnded = 0;
                    tmr1Value = T1CON;
                    break;
                }
            } else if (gameStarted == 0) {
                // Game has not been started yet.
                // Check if RC0 pressed. If pressed start the game.
                if (inp_rc0_btn_st) {
                    inp_rc0_btn_st = 0;
                    gameStarted = 1;
                    restartGame = 0;
                    TRISC = 0x00;
                    tmr1Value = T1CON;
                }
            } else {
                // Game has been started.
                if (health == 0) {
                    // When health becomes 0, end the game and 
                    // wait for RC0 input to restart the game.
                    gameStarted = 0;
                    // Set RC0 as input
                    LATC = 0x00;
                    TRISC = 0x01;
                    while (1) {
                        // Wait for RC0 press to restart the game.
                        inp_rc0_btn_st = 0;
                        input_task();
                        restartGame = 0;
                        if (inp_rc0_btn_st) {
                            restartGame = 1;
                            break;
                        }
                    }
                    if (restartGame == 1) {
                        // If RC0 button pressed in above loop,
                        // break second main loop and restart game
                        // with default configurations.
                        tmr1Value = T1CON;
                        break;
                    }
                } else if (firstNote) {
                    // When we press RC0 we want to start the game immediately
                    // that`s why for the first note we don`t need any msXXXWaitedFlag.
                    generate_notes();
                    firstNote = 0;
                    interruptCountTMR0 = 0;
                } else if (level == 1) {
                    if (ms500WaitedFlag) {
                        shift();          // After creating first note, first we need to shift the notes
                        generate_notes(); // Then we generate notes.
                        ms500WaitedFlag = 0; // Reset flag
                    }
                } else if (level == 2) {
                    if (ms400WaitedFlag) {
                        shift();
                        generate_notes();
                        ms400WaitedFlag = 0;
                    }
                } else if (level == 3) {  
                    if (ms300WaitedFlag) {  
                        ms300WaitedFlag = 0;
                        if(generatedNotesNum < 30){
                            shift();
                            generate_notes();
                        }
                        else {       // At the end of the game, we need to stop creating notes after 30th note.
                            shift(); // Then we need to shift remaining notes to PORTF
                            LATA = 0x00;
                            lastShiftCount++;
                            if(lastShiftCount == 6){
                                if(health == 0){ // If our health is equal to 1 at the last note if we miss that note we lose.
                                    gameStarted = 0;
                                }else{           // We have a winner.
                                    gameEnded = 1;
                                    gameStarted = 0;
                                }
                            }
                        }   
                    }
                }
            }
        }
    }
}