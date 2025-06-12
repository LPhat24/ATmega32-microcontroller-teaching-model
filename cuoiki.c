#include <mega32.h>
#include <alcd.h>
#include <delay.h>
#include <string.h>
#include <stdlib.h>

#define ADC_VREF_TYPE 0x20

char c[] = "HELLO";
int length;
int i, j, k, x;
unsigned char ADC0;
char mystr[10];
int nhietdo;
char mystr2[5];
unsigned char LED7_Code[10] = {
    0b11000000, 
    0b11111001, 
    0b10100100, 
    0b10110000, 
    0b10011001, 
    0b10010010, 
    0b10000010, 
    0b11111000, 
    0b10000000, 
    0b10010000  
};
unsigned int demxung = 0;

interrupt [ EXT_INT0 ] void ext_int0_isr(void){
 demxung++;
}
unsigned char read_adc(unsigned char adc_input) {
    ADMUX = adc_input | (ADC_VREF_TYPE & 0xff);
    delay_us(10); 
    ADCSRA |= 0x40;  
    while ((ADCSRA & 0x10) == 0);  
    ADCSRA |= 0x10;  
    return ADCH;
}


void htso_mayphat(unsigned int so){
 lcd_putchar(so/10000 + 48);      
 lcd_putchar(so/1000%10 + 48);   
 lcd_putchar(so/100%10 + 48);         
 lcd_putchar(so/10%10 + 48); 
 lcd_putchar(so%10 + 48);
}


void bai_35() {
    
        for (i = 15; i >= -length; i--) {
            lcd_clear();
            for (j = 0; j < length; j++) {
                if (i + j >= 0 && i + j <= 15) {
                    lcd_gotoxy(i + j, 0);
                    lcd_putchar(c[j]);
                }
            }
            delay_ms(100);
        }
    
}

void bai_31_A() {
        for (k = 0; k <= 8; k++) {
            PORTB = 0b00000001 << k;
            delay_ms(100);
        }
} 


void bai_31_B(){   
        for (k = 0; k <= 8; k++) {
            PORTB = 0b10000000 >> k;
            delay_ms(100);
        }
    
}

void bai_32() {
    
        for (x = 0; x <= 9; x++) {
            PORTC = LED7_Code[x];
            PORTB = ~LED7_Code[x];
            delay_ms(100);
        }
    
}

void bai_38_39() {
    ADC0 = read_adc(0);
    lcd_clear();
    lcd_gotoxy(0, 0);
    itoa(ADC0, mystr);
    lcd_puts(mystr);
    lcd_gotoxy(0, 1);
    nhietdo = read_adc(1) * 2;
    itoa(nhietdo, mystr2);
    lcd_putsf("Nhiet do: ");
    lcd_puts(mystr2);

    while (nhietdo >= 50) {
        lcd_clear();  
        lcd_gotoxy(0, 1);
        lcd_putsf(" NGUY HIEM !!!"); 
        nhietdo = read_adc(1) * 2; 
        PORTD.3 = 1;
        delay_ms(200);  
        PORTD.3 = 0; 
        delay_ms(200);
    }
}


void bai_310(){
     
        demxung = 0;
        TCNT0 = 0;
        while (TCNT0 < 78){}
        lcd_gotoxy(0,1);
        //lcd_putsf("Tan so: ");
        htso_mayphat(demxung*100);  
        
}


void main(void) {
    DDRB = 0xFF;
    PORTB = 0x00;
    
    DDRD.3 = 1;
    PORTD.3 = 0;
    
    DDRC = 0xFF;
    PORTC = 0xFF;
    
    DDRA = 0x00;
    PORTA = 0b11111100; 
    
    ACSR = 0x80;
    SFIOR = 0x00;
    ADMUX = ADC_VREF_TYPE & 0xff;
    ADCSRA = 0x84;
    
    length = strlen(c); 
    
    GICR = (1<<INT0);
    MCUCR = (1<<ISC01)|(1<<ISC00);
    TCCR0 = (1<<CS02)|(0<<CS01)|(1<<CS00);
    lcd_init(16);
    #asm("sei")
    while (1) {
        while (PINA.3 == 0)bai_310();
        if  (PINA.5 == 0) bai_31_A();
        else if (PINA.7 == 0) bai_32();
        else if (PINA.4 == 0) bai_35(); 
        else if (PINA.2 == 0)bai_38_39(); 
        else if (PINA.6 == 0) bai_31_B();
        else lcd_clear();
    }
}
