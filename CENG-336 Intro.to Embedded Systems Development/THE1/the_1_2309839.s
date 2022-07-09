PROCESSOR 18F8722
    
#include <xc.inc>

; configurations
CONFIG OSC = HSPLL, FCMEN = OFF, IESO = OFF, PWRT = OFF, BOREN = OFF, WDT = OFF, MCLRE = ON, LPT1OSC = OFF, LVP = OFF, XINST = OFF, DEBUG = OFF

; global variable declarations
GLOBAL _t1, _t2, _t3, var1, var2, var3, _lightsON; variables for time delay
GLOBAL port_select, A_B, A_C, A_D

; allocating memory for variables
PSECT udata_acs
    port_select:
	DS 1    ; allocates 1 byte
    _t1:
        DS 1    ; allocate 1 byte
    _t2:
        DS 1    ; allocate 1 byte
    _t3:
        DS 1    ; allocate 1 byte
    A_B:
        DS 1    ; allocate 1 byte
    A_C:
        DS 1    ; allocate 1 byte
    A_D:
        DS 1    ; allocate 1 byte
    var1:
        DS 2    ; allocate 2 byte
    var2:
        DS 1    ; allocate 1 byte
    var3:
        DS 1    ; allocate 1 byte
    _lightsON:
        DS 1    ; allocate 1 byte

PSECT resetVec,class=CODE,reloc=2
resetVec:
    goto    main
    
ms500_counter:
    movlw 0xFFFF      
    cpfseq var1
    goto var1_increment
    goto var1_reset
    var1_increment:
	incf var1
	return
    
    var1_reset:
	movlw 0x00
	movwf var1
	movlw 0xFF
	cpfseq var2
	goto var2_increment
	goto var2_reset

	var2_increment:
	    incf var2
	    return

	var2_reset:
	    movlw 0x00
	    movwf var2
	    movlw 0b00001111
	    cpfseq var3
	    incf var3
	    return

busy_delay_1sec:
    movlw 0x84      ; copy desired value to W
    movwf _t3       ; copy W into t3
    _loop3:
        movlw 0xAF      ; copy desired value to W
        movwf _t2       ; copy W into t2
        _loop2:
            movlw 0x8F      ; copy desired value to W
            movwf _t1       ; copy W into t1
            _loop1:
                decfsz _t1, 1   ; decrement t1, if 0 skip next 
                goto _loop1     ; else keep counting down
                decfsz _t2, 1   ; decrement t2, if 0 skip next 
                goto _loop2     ; else keep counting down
                decfsz _t3, 1   ; decrement t3, if 0 skip next 
                goto _loop3     ; else keep counting down
                return 
busy_delay_half_sec:
    movlw 0x41      ; copy desired value to W
    movwf _t3       ; copy W into t3
    _loop6:
        movlw 0x6F      ; copy desired value to W
        movwf _t2       ; copy W into t2
        _loop5:
            movlw 0x8F      ; copy desired value to W
            movwf _t1       ; copy W into t1
            _loop4:
                decfsz _t1, 1   ; decrement t1, if 0 skip next 
                goto _loop1     ; else keep counting down
                decfsz _t2, 1   ; decrement t2, if 0 skip next 
                goto _loop2     ; else keep counting down
                decfsz _t3, 1   ; decrement t3, if 0 skip next 
                goto _loop3     ; else keep counting down
                return 

light_blinker:
    movlw 0b00000001     
    cpfseq _lightsON
    goto turnON_1
    goto turnOFF_

turnON_1:
    movlw 0b00000001
    cpfseq port_select
    goto turnON_2
    call lighter
    return
turnON_2:
    movlw 0b00000010
    cpfseq port_select
    goto turnON_dummy1
    call lighter
    return
turnON_dummy1: ;; DUMMY FOR "INITIAL"
    movlw 0b00000000
    cpfseq port_select
    return
    goto turnON_dummy2
turnON_dummy2: ;; DUMMY FOR "d"
    movlw 0b00000011
    cpfseq port_select
    return
    return
    
turnOFF_:
    movlw 0b00000001
    cpfseq port_select
    goto turnOFF_1
    goto turnOFF_B
turnOFF_1:
    movlw 0b00000010
    cpfseq port_select
    return
    goto turnOFF_C
    
turnOFF_B:
    movlw 0b00000000
    movwf LATB
    movwf _lightsON
    return
turnOFF_C:
    movlw 0b00000000
    movwf LATC
    movwf _lightsON
    return
;; RE4 RESPONSE
re4_press:
    btfsc PORTE, 4          ; if re4 is currently not pressed, return
    goto wait_re4_release   ; if pressed, wait until released
    return
    
    wait_re4_release:
	call ms500_counter
	movlw 0b00000101
	cpfseq var3
	goto checkrelease
	call ms500_passed
	call light_blinker
	movlw 0x00
	movwf var3
	goto checkrelease
	
    checkrelease:
        btfsc PORTE, 4          ; if re4 is not released
        goto wait_re4_release   ; wait until it is released (else skip here)
	call light_blinker
        goto check_port        ; change led_flag to change the state of the leds
    ;->b
    check_port:
        movlw 0b00000000          ; move 0 to W
        cpfseq port_select     ; compare led_flag with W
        goto check_port1     ; if not equal to W (not zero), reset
        goto move_portB     ; if equal to W (zero), set
    ;b->c
    check_port1:
        movlw 0b00000001          ; move 0 to W
        cpfseq port_select     ; compare led_flag with W
        goto check_port2     ; if not equal to W (not zero), reset
        goto move_portC     ; if equal to W (zero), set
    ;c->d
    check_port2:
        movlw 0b00000010          ; move 0 to W
        cpfseq port_select     ; compare led_flag with W
        goto check_port3     ; if not equal to W (not zero), reset
        goto move_portD     ; if equal to W (zero), set
    ;d->b
    check_port3:
        movlw 0b00000011          ; move 0 to W
        cpfseq port_select     ; compare led_flag with W
        goto check_port2     ; ??
        goto move_portB     ; if equal to W (zero), set
    ;b->c
    move_portC:
	movlw 0b00000010    ; portC is selected
	movwf port_select
	return
    ;c->d
    move_portD:
	movlw 0b00000011    ; portD is selected
	movwf port_select
	return
    ;d->b
    move_portB:
	movlw 0b00000001    ; portB is selected
	movwf port_select
	return
;; RA4 RESPONSE
	
ra4_press:
    call ms500_counter
    movlw 0b00000010
    cpfseq var3
    goto ra4_checker
    call ms500_passed
    call light_blinker
    movlw 0x00
    movwf var3
    goto ra4_checker
    ra4_checker:
    btfsc PORTA, 4          ; if ra4 is currently not pressed, return
    goto wait_ra4_release   ; if pressed, wait until released
    return

    wait_ra4_release:
	call ms500_counter
	movlw 0b00000101
	cpfseq var3
	goto check_releaseA
	call ms500_passed
	call light_blinker
	movlw 0x00
	movwf var3
	goto check_releaseA
	
    check_releaseA:
        btfsc PORTA, 4          
        goto wait_ra4_release   
        goto change_config1        
	
    change_config1:
	movlw 0b00000001
	cpfseq port_select    
	goto change_config2     
	goto change_config_B  
    
    change_config2:
	movlw 0b00000010
	cpfseq port_select     ; compare led_flag with W
	goto change_config3     ; if not equal to W (not zero), reset
	goto change_config_C     ; if equal to W (zero), set

    change_config3:
	movlw 0b00000011
	cpfseq port_select     ; compare led_flag with W
	return    ; ??
	return     ; D STATE

    change_config_B:
	movlw 0b00000100
	cpfseq A_B     
	goto inc_A_B
	goto reset_A_B
    inc_A_B:
	incf A_B
	return
    reset_A_B:
	movlw 0b00000001     ; move literal written as a binary value to W 
	movwf A_B
	return
	
    change_config_C:
	movlw 0b00000010
	cpfseq A_C     ; compare led_flag with W
	goto inc_A_C
	goto reset_A_C
    inc_A_C:
	incf A_C
	return
    reset_A_C:
	movlw 0b00000001     ; move literal written as a binary value to W 
	movwf A_C
	return

	;; LIGHT B-C
lighter:
    movlw 0b00000001
    cpfseq port_select
    goto lighter_port1
    goto lighter_B
	
lighter_port1:
    movlw 0b00000010
    cpfseq port_select
    goto lighter_port2
    goto lighter_C

lighter_port2: ;port_select=0
    movlw 0b00000000
    cpfseq port_select
    return
    return
    
lighter_B:
    movlw 0b00000001
    cpfseq A_B
    goto lighter_B_2
    goto light_B1

lighter_B_2:
    movlw 0b00000010
    cpfseq A_B
    goto lighter_B_3
    goto light_B2

lighter_B_3:
    movlw 0b00000011
    cpfseq A_B
    goto lighter_B_4
    goto light_B3

lighter_B_4:
    movlw 0b00000100
    cpfseq A_B
    return ;??
    goto light_B4
    
light_B1:
    movlw 0b00000001
    movwf LATB
    movlw 0b00000001
    movwf _lightsON
    return
light_B2:
    movlw 0b00000011
    movwf LATB
    movlw 0b00000001
    movwf _lightsON
    return
light_B3:
    movlw 0b00000111
    movwf LATB
    movlw 0b00000001
    movwf _lightsON
    return
light_B4:
    movlw 0b00001111
    movwf LATB
    movlw 0b00000001
    movwf _lightsON
    return
    
lighter_C:
    movlw 0b00000001
    cpfseq A_C
    goto lighter_C_1
    goto light_C1

lighter_C_1:
    movlw 0b00000010
    cpfseq A_C
    return ;??
    goto light_C2

light_C1:
    movlw 0b00000001
    movwf LATC
    movlw 0b00000001
    movwf _lightsON
    return
light_C2:
    movlw 0b00000010
    movwf LATC
    movlw 0b00000001
    movwf _lightsON
    return    
    
    ;; CALCULATE-D
calculate_light_D:
    movlw 0b00000011
    cpfseq port_select
    return
    goto calculate_D_B1
 ;, FIND B VALUE   
calculate_D_B1:
    movlw 0b00000001
    cpfseq A_B
    goto calculate_D_B2
    goto calculate_D_B1_C_
calculate_D_B2:
    movlw 0b00000010
    cpfseq A_B
    goto calculate_D_B3
    goto calculate_D_B2_C_
calculate_D_B3:
    movlw 0b00000011
    cpfseq A_B
    goto calculate_D_B4
    goto calculate_D_B3_C_
calculate_D_B4:
    movlw 0b00000100
    cpfseq A_B
    return
    goto calculate_D_B4_C_
;; B FOUND - FIND C VALUE
calculate_D_B1_C_:
    movlw 0b00000001
    cpfseq A_C
    goto calculate_D_B1_C2
    goto calculate_D_B1_C1
calculate_D_B2_C_:
    movlw 0b00000001
    cpfseq A_C
    goto calculate_D_B2_C2
    goto calculate_D_B2_C1
calculate_D_B3_C_:
    movlw 0b00000001
    cpfseq A_C
    goto calculate_D_B3_C2
    goto calculate_D_B3_C1
calculate_D_B4_C_:
    movlw 0b00000001
    cpfseq A_C
    goto calculate_D_B4_C2
    goto calculate_D_B4_C1
;; B-C FOUND ;; CALCULATE D
calculate_D_B1_C1:
    movlw 0x01
    movwf A_D
    return
calculate_D_B1_C2:
    movlw 0x02
    movwf A_D
    return
calculate_D_B2_C1:
    movlw 0x02
    movwf A_D
    return
calculate_D_B2_C2:
    movlw 0x04
    movwf A_D
    return
calculate_D_B3_C1:
    movlw 0x03
    movwf A_D
    return
calculate_D_B3_C2:
    movlw 0x06
    movwf A_D
    return
calculate_D_B4_C1:
    movlw 0x04
    movwf A_D
    return
calculate_D_B4_C2:
    movlw 0x08
    movwf A_D
    return
    
;; Calculations are done, time to light port-D
D_light:
    movlw 0b00000011
    cpfseq port_select
    return
    goto D_light1
D_light1:
    movlw 0x01
    cpfseq A_D
    goto D_light2
    goto light_1_D
D_light2:
    movlw 0x02
    cpfseq A_D
    goto D_light3
    goto light_2_D
D_light3:
    movlw 0x03
    cpfseq A_D
    goto D_light4
    goto light_3_D
D_light4:
    movlw 0x04
    cpfseq A_D
    goto D_light6
    goto light_4_D
D_light6:
    movlw 0x06
    cpfseq A_D
    goto D_light8
    goto light_6_D
D_light8:
    movlw 0x08
    cpfseq A_D
    return
    goto light_8_D
;; A_D situations; IMPLEMENTED WITH BUSY DELAY
light_1_D:
    movlw 0b00000001
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000000
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    return
light_2_D:
    movlw 0b00000011
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000001
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000000
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    return
light_3_D:
    movlw 0b00000111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000011
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000001
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000000
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    return
light_4_D:
    movlw 0b00001111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000011
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000001
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000000
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    return
light_6_D:
    movlw 0b00111111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00011111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00001111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000011
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000001
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000000
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    return
light_8_D:
    movlw 0b11111111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b01111111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00111111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00011111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00001111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000111
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000011
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000001
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    movlw 0b00000000
    movwf LATD
    call busy_delay_half_sec
    call ms500_passed
    return
    
init:
    ; set RA4-RE4 as input source, clear PORTA-PORTE LEDs
    movlw 00010000B     ; move literal written as a binary value to W 
    movwf TRISA         ; move W to TRISA to indicate input/output direction 
    movwf TRISE         ; move W to TRISE
    clrf  LATA          ; clear output latch so that all LEDs in PORTA is turned off
    clrf  LATE		; clear output latch so that all LEDs in PORTE is turned off

    ; initialize PORTB as output source
    movlw 11110000B     ; move literal written as a hexadecimal value to W
    movwf TRISB         ; move W to TRISB to indicate input/output direction 
                        ; 0-3 pins are outputs
    
    ; initialize PORTC as output source
    movlw 11111100B     ; move literal written as a hexadecimal value to W
    movwf TRISC         ; move W to TRISC to indicate input/output direction 
                        ; 0-1 pins are outputs
    
    ; initialize PORTD as output source
    movlw 00000000B     ; move literal written as a hexadecimal value to W
    movwf TRISD         ; move W to TRISC to indicate input/output direction 
                        ; 0-7 pins are outputs
    ; light_portd_leds
    movlw 0xFF          ; move all 1's to W
    movwf LATB          ; move W to LATB to light all leds 0-3
    movwf LATC          ; move W to LATC to light all leds 0-1
    movwf LATD          ; move W to LATD to light all leds 0-7
    return

; DO NOT DELETE OR MODIFY
; 500ms pass check for test scripts
ms500_passed:
    nop
    return

; DO NOT DELETE OR MODIFY
; 1sec pass check for test scripts
ms1000_passed:
    nop
    return
	
PSECT CODE
main:
    call init               ; call initialization procedure
    call busy_delay_1sec    ; some code to initialize and wait 1000ms here, maybe   
    call ms1000_passed
    ; a loop here, maybe

default:
    clrf LATB
    clrf LATC
    clrf LATD
    movlw 0x01    
    movwf LATB          ; B0
    movwf LATC          ; C0
    movwf A_B ;=1
    movwf A_C ;=1
    movlw 0b00000000
    movwf _lightsON 
    
    movlw 0x00   ; at the beginning no port selected
    movwf A_D
    movwf port_select
    movwf var1
    movwf var2
    movwf var3
    
loop:
        ; inside the loop, once it is confirmed 500ms passed
	
        call re4_press
	call ra4_press
	call calculate_light_D
	call D_light
        movlw 0b00000011
	cpfseq port_select
	goto loop
	goto default

end resetVec