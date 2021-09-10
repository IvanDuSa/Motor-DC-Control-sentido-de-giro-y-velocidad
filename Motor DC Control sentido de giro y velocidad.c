#include<18F4550.H>

//#include <stdlib.h>
//Parte del LCD
#fuses HSPLL, PLL5, USBDIV, NOWDT, CPUDIV1, NOPROTECT, NOWRT, NOPUT
#fuses NODEBUG, NOPBADEN, NOLVP, NOVREGEN, hs
#use delay(clock=48MHz)
//#use delay(crystal=8MHz)
#define LCD_DATA_PORT getenv("SFR:PORTB")   
#include <lcd.c>
//Parte de la entrada del pot
#define AN0          0x0E   // A0 --Revisar en archivo .h

/*RECORDAR
ENTRADAS:
input(pin_a0)==1
SALIDAS:
output_high(pin_a0)
*/

//Declaración de la variable Velocidad
long bits;     //Variable almacena los bits
float vel;     //Almacena la temperatura
int valor;

void configuracion ()
{
setup_ccp1(CCP_PWM);
setup_ccp2(CCP_PWM);
setup_timer_2(T2_DIV_BY_4,249,1);
}


void main(){
set_tris_a(0b00000001);
setup_adc_ports(AN0);
setup_adc(adc_clock_internal);


configuracion();
lcd_init();
lcd_putc("\f");


while(true)
{
    set_adc_channel(0);
    delay_ms(1);
    bits = read_adc();
    vel = bits*11.7647;
    valor = 1.2*bits ;
    if ((input(pin_a2)==0) && (input(pin_a1)==1)){
        set_pwm1_duty(valor);
        lcd_gotoxy(1,1);
        lcd_putc("Giro: IZQUIERDA  ");   
        lcd_gotoxy(1,2);
        printf(lcd_putc,"Vel(RPM): %f ",vel);    }
        
    else if ((input(pin_a2)==1) && (input(pin_a1)==0)) {
        set_pwm2_duty(valor);
        lcd_gotoxy(1,1);
        lcd_putc("Giro: DERECHA  ");   
        lcd_gotoxy(1,2);
        printf(lcd_putc,"Vel(RPM): %f ",vel);    }
    
    else if ((input(pin_a2)==1) && (input(pin_a1)==1)){
        set_pwm1_duty(0);
        set_pwm2_duty(0);
        lcd_gotoxy(1,1);
        lcd_putc("   CUIDADO 2    ");      
        lcd_gotoxy(1,2);
        lcd_putc("Todo en Marcha  ");}
    
    else {
        set_pwm1_duty(0);
        set_pwm2_duty(0);
        lcd_gotoxy(1,1);
        lcd_putc("Sel. Direccion  ");      
        lcd_gotoxy(1,2);
        lcd_putc("Sel. Velocidad ");}
}
}
