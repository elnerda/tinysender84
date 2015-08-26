#include <Manchester.h>
#include <TinyWireM.h>
#include <USI_TWI_MASTER.h>
#include <tinybmp.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
/*
  Prescaler für 433MHz Transmitter und Receiver
  MAN_300 0
  MAN_600 1
  MAN_1200 2
  MAN_2400 3
  MAN_4800 4
  MAN_9600 5
  MAN_19200 6
  MAN_38400 7
  
     ATTINY84-PU tiny Core Pinout
               ____
          VCC-|O   |- GND
      (0) PB0-|    |- PA0 (10)
      (1) PB1-|    |- PA1 (9)
          RST-|    |- PA2 (8)
      (2) PB2-|    |- PA3 (7)
      (3) PA7-|    |- PA4/SCL (6)
  (4) SDA/PA6-|____|- PA5 (5)

  ~70mA ohne sleep
  ~45uA schlafend
*/
//Variablen deklarieren
#define mt 7
#define BUFFER_SIZE 9
float Temp;
uint32_t Pres,sleeptime=360;
uint16_t pLow,pHigh,watchdog_counter=0;
uint8_t lLow,hLow,lHigh,hHigh;
uint8_t a[BUFFER_SIZE];
//bmp klasse initialisieren
tinybmp180 tiny;

//Interrupt Service Routine für Watchdog timer (ISR)
ISR(WDT_vect){
  //erstmal leer weil, muss nur da sein damit der watchdog funktioniert
  //für jeden interrupt auch eine ISR
  watchdog_counter++;
}

void setup() {
  //Sensor und Transmitter initialisieren
tiny.begin();
man.setupTransmit(mt, MAN_300);
  //sleep modes initialisieren
set_sleep_mode(SLEEP_MODE_PWR_DOWN);//nicht das beste zum stromsparen, aber ein anfang
sleep_enable();

}
//Union um float in 8 byte zu teilen
union u_tag {
   byte b[4]; 
   float fval;
 } u;
void loop() {
  //erstmal schlafen
ADCSRA &= ~(1<<ADEN); //ADC aussschalten um strom zu sparen, erstmal nicht
setup_watchdog(6);  // watchdog geht nach 500ms los
sleep_mode();       // erstmal schlafen, nach 500ms aufwachen
ADCSRA |= (1<<ADEN); //ADC wieder einschalten
                    // danach muss die erste messung verworfen werden

  if(watchdog_counter >sleeptime){
    //30 sekunden schlafen, danach timer wieder auf 0 stelllen um wieder 30 sekunden zu schlafen
    watchdog_counter=0;
  //Werte aus dem Sensor auslesen
    Temp=tiny.readTemp();
    Pres=tiny.readPressure();
//Temperatur and Union für float to byte(4) übergeben
    u.fval=Temp;
//Luftdruck in 4 ( Bit werte aufteilen
    pLow=Pres;
    pHigh=Pres>>16;
    lLow=pLow;
    hLow=pLow>>8&0xFF;
    lHigh=pHigh;
    hHigh=pHigh>>8&0xFF;
    //Array befüllen:
    a[0]=255;
    a[1]=u.b[0];
    a[2]=u.b[1];
    a[3]=u.b[2];
    a[4]=u.b[3];
    a[5]=lLow;
    a[6]=hLow;
    a[7]=lHigh;
    a[8]=hHigh;
    //Array versenden
    man.transmitArray(9,a);
  }
}
//Sets the watchdog timer to wake us up, but not reset
//0=16ms, 1=32ms, 2=64ms, 3=128ms, 4=250ms, 5=500ms
//6=1sec, 7=2sec, 8=4sec, 9=8sec
//From: http://interface.khm.de/index.php/lab/experiments/sleep_watchdog_battery/

void setup_watchdog(int Prescaler){
  // Kleines Vorwort zu Bit Manipulation in Registern des AVR
  // &= ~(x<<y) löscht bit x an position y im AVR Register
  // |= (x<<y) setzt bit x an position y im AVR Register  
  // _BV steht für Bit Value, kann fürs setzen oder löschen verwendet werden
  // unabhängig von vorherigen Wert


  
  if(Prescaler>9) Prescaler=9;  //größere Prescaler als 9 gibt es nicht
                                //deswegen auf 9 limitieren
  byte bb=Prescaler&7;
  if(Prescaler>7) bb|=(1<<5);   //das 5 Bit Setzen

//Reihenfolge des Bitsetzens ist wichtig und kann NICHT kombiniert werden

MCUSR &= ~(1<<WDRF);            //Watchdog reset clearen
WDTCSR |= (1<<WDCE) | (1<<WDE);  // WD_change enable setzen, WD enable setzen
WDTCSR = bb;                     //neuen watchdog timer wert setzen
WDTCSR |= _BV(WDIE);             //Interrupt enale setzen, hält den MCU vom resetten beim aufwachen ab
                                //kann notfalls geändert werden damit er resettet falls notwendig   
}
