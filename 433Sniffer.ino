//433Sniffer.ino v1.0 Non sono un programmatore ma posso imparare
//By Filippo Tamarindo 

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <RCSwitch.h>

#define JammerLed 10    // Indicatore modalità jammer
#define CapturedLed 11  // Indicatore codice ricevuto
#define RunLed 12       // Indicatore programma in esecuzione
#define WekeUpRX 5      // Sveglia/Addormenta la ricevente
#define SendCode 7      // Pulsante invia codice
#define JammerMode 8    // Pulsante modalità jammer
#define BatteryInput 0  // Pin lettura volt batteria
#define OLED_RESET -1   // Reset pin # (-1 condiviso con reset pin arduino)
Adafruit_SSD1306 display(OLED_RESET);


// Variabili gestione livello batteria
float vout = 0.0;
float vin = 0.0;
float R2 = 10000.0;   //Valore di 10K  (collegata verso GND)
float R1 = 100000.0;  //Valore di 100K (collegata verso VIN)
int vbatt = 0;
//------------------
long JammerCode;            //Variabile codice pseudocasuale per jammer
long ReceivedValue = 0;     //Variabile condivisa codice ricevuto
int ReceivedProtocol = 0;   //Variabile condivisa per il protocollo ricevuto
int ReceivedBitlength = 0;  //Variabile condivisa lunghezza bit del codice ricevuto
int ReceivedDelay = 0;      //Variabile condivisa durata dell'impulso ricevuto
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);                         // Inizializza seriale
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Inizializza display

  mySwitch.enableReceive(0);   // Imposta PIN 0 (D2) per RX
  mySwitch.enableTransmit(3);  // Imposta PIN 3 per TX

  pinMode(BatteryInput, INPUT);
  pinMode(SendCode, INPUT_PULLUP);    // Imposta il pulsante come INPUT_PULLUP
  pinMode(JammerMode, INPUT_PULLUP);  // Imposta il pulsante come INPUT_PULLUP
  pinMode(CapturedLed, OUTPUT);
  pinMode(JammerLed, OUTPUT);
  pinMode(RunLed, OUTPUT);

  digitalWrite(RunLed, HIGH);  // Accende LED inizio esecuzione programma

  battery();  // Esegue il controllo della batteria
}

void loop() {
  RX();
  if (digitalRead(JammerMode) == HIGH) {  // Verifica se il pulsante "JammerMode" è stato premuto
    jammer();
  }
}

void TX() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Send Code:");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 18);
  display.println(ReceivedValue);
  display.display();

  Serial.print("Send Code: ");
  Serial.print(ReceivedValue);
  Serial.print("-");
  Serial.print(ReceivedBitlength);
  Serial.print("Bit");
  Serial.print("   Delay(us): ");
  Serial.print(ReceivedDelay);
  Serial.print("   Protocol: ");
  Serial.println(ReceivedProtocol);
  Serial.print(" \n ");

  digitalWrite(WekeUpRX, LOW);                      // Addormenta RX
  mySwitch.setProtocol(ReceivedProtocol);           // Imposta il protocollo
  mySwitch.setPulseLength(ReceivedDelay);           // Imposta la lunghezza dell'impulso (in microsecondi)
  mySwitch.setRepeatTransmit(10);                   // Imposta il numero di ripetizioni del codice
  mySwitch.send(ReceivedValue, ReceivedBitlength);  // Trasmette il codice ricevuto con la stessa lunghezza di Bit

  battery();
}

void RX() {
  digitalWrite(WekeUpRX, HIGH);  // Sveglia RX
  if (mySwitch.available()) {
    digitalWrite(CapturedLed, HIGH);  // Accende indicatore codice ricevuto

    ReceivedProtocol = mySwitch.getReceivedProtocol();    // Assegna valore del protocollo ricevuto alla variabile globale
    ReceivedValue = mySwitch.getReceivedValue();          // Assegna valore del codice ricevuto alla variabile globale
    ReceivedBitlength = mySwitch.getReceivedBitlength();  // Assegna valore della lunghezza Bit ricevuti alla variabile globale
    ReceivedDelay = mySwitch.getReceivedDelay();          // Assegna valore della durata dell'impulso ricevuto alla variabile globale

    Serial.print("Captured:  ");
    Serial.print(mySwitch.getReceivedValue());
    Serial.print("-");
    Serial.print(mySwitch.getReceivedBitlength());
    Serial.print("Bit");
    Serial.print("   Delay(us): ");
    Serial.print(mySwitch.getReceivedDelay());
    Serial.print("   Protocol: ");
    Serial.println(mySwitch.getReceivedProtocol());
    Serial.print(" \n ");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Code:");
    display.setCursor(40, 0);
    display.println(mySwitch.getReceivedValue());
    display.setCursor(0, 13);
    display.println("Delay:    us");  // µs microsecondi
    display.setCursor(40, 13);
    display.println(mySwitch.getReceivedDelay());
    display.setCursor(88, 13);
    display.println("Bit:");
    display.setCursor(116, 13);
    display.println(mySwitch.getReceivedBitlength());
    display.setCursor(0, 25);
    display.println("Protocol: ");
    display.setCursor(70, 25);
    display.println(mySwitch.getReceivedProtocol());
    display.display();

    mySwitch.resetAvailable();
  }

  if (digitalRead(SendCode) == HIGH && digitalRead(CapturedLed) == HIGH) {  // Controlla se pulsante "SendCode" è stato premuto, quando catturato un codice
    TX();
  }
  if (digitalRead(SendCode) == HIGH && digitalRead(CapturedLed) == LOW) {
    display.clearDisplay();
    display.invertDisplay(true);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 1);
    display.println(" NO   CODE CAPTURED!");
    display.display();
    delay(200);
    display.invertDisplay(false);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Sniffer        On...");
    display.display();
  }
}

void jammer() {
  digitalWrite(WekeUpRX, LOW);    // Addormenta RX
  digitalWrite(JammerLed, HIGH);  // Accende indicatore modalità jammer

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Jammer         On...");
  display.display();

  randomSeed(millis());
  JammerCode = random(1, 1000000);

  Serial.print("Jamming:   ");
  Serial.print(JammerCode);
  Serial.print("      Locked at: 24Bit - Protocol 1 - PulseLength 350 - RepeatTransmit 1000");
  Serial.print(" \n ");

  mySwitch.setProtocol(1);           // Imposta il protocollo desiderato
  mySwitch.setPulseLength(350);      // Imposta la lunghezza dell'impulso (in microsecondi)
  mySwitch.setRepeatTransmit(1000);  // Imposta il numero di ripetizioni del segnale di trasmissione
  mySwitch.send(JammerCode, 24);     // Trasmette il codice ricevuto con la stessa lunghezza
  digitalWrite(JammerLed, LOW);      // Spegne indicatore modalità jammer

  battery();
}

void battery() {
  vbatt = analogRead(BatteryInput);
  vout = (vbatt * 5.0) / 1023;
  vin = vout / (R2 / (R1 + R2));

  if (vin < 5.2) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(00, 01);
    display.println("    LOW     BATTERY");
    display.display();
    display.invertDisplay(true);
    delay(100);
    display.invertDisplay(false);
    delay(100);
    display.invertDisplay(true);
    delay(100);
    display.invertDisplay(false);
    delay(100);
    display.invertDisplay(true);
    delay(100);
    display.invertDisplay(false);
    delay(100);
    display.invertDisplay(true);
    delay(100);
    display.invertDisplay(false);
    delay(100);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Volt:");
  display.setCursor(80, 0);
  display.println(vin);
  display.setTextSize(1);
  display.setCursor(0, 24);
  display.println(" use PP3 / 9V battery");
  display.display();

  delay(700);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Sniffer        On...");
  display.display();
}