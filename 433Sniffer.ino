// 433Sniffer.ino v0.6.2 Beta
// Non sono un programmatore ma posso imparare
// By Filippo Tamarindo 

#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <RCSwitch.h>
// #include <Wire.h>

//------ PIN CONFIG -----------------------------------------------------------------------
#define BatteryInput A0             // Pin analogico lettura volt partitore resistivo
#define RxPin 0                     // Pin D2 (0) per la ricevente
#define TxPin 3                     // Pin D3 per la trasmittente
#define WekeUpRx 5                  // Sveglia/Addormenta la ricevente
#define JammerLed 10                // Indicatore modalità jammer
#define CapturedLed 11              // Indicatore codice catturato
#define RunLed 12                   // Indicatore programma in esecuzione
#define SendCode 7                  // Pulsante invia codice
#define JammerMode 8                // Pulsante modalità jammer
#define DisplayReset -1             // Reset pin (-1 condiviso con reset pin arduino)

//------ SETTINGS -------------------------------------------------------------------------
#define DisplayAddress 0x3C         // Indirizzo I2C display
#define SarialBauds  9600           // Velocità porta seriale bits/s
#define BatteryLimit 6.5            // Limite volt batteria scarica
#define R1 100000.0                 // Resistore 100K (collegata verso VIN)
#define R2 10000.0                  // Resistore 10K  (collegata verso GND)
#define JammerProtocol 1            // Protocollo del codice jammer (1-12)
#define JammerBitlength 24          // Lunghezza del codice jammer
#define JammerPulseLength 350       // Durata impulso del codice jammer (µs microsecondi)
#define JammerRepeatTransmit 1000   // Ripetizioni del codice jammer


Adafruit_SSD1306 display(DisplayReset);

float vout;                         // Mappatura lineare di vbatt   out = (vbatt * 5.0) / 1023;
float vin;                          // Valore effettivo di tensione vin = vout / (R2 / (R1 + R2)
int vbatt;                          // Lettura ingresso analogico
int ReceivedProtocol;               // Protocollo del codice catturato
int ReceivedBitlength;              // Lunghezza in bit del codice catturato
int ReceivedDelay;                  // Durata dell'impulso del codice catturato (µs microsecondi)
long JammerCode;                    // Codice pseudocasuale per jammer
long ReceivedValue;                 // Codice catturato

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(SarialBauds);                               // Inizializza seriale alla velocità impostata
  display.begin(SSD1306_SWITCHCAPVCC, DisplayAddress);     // Inizializza display con indirizzo impostato

  mySwitch.enableReceive(RxPin);                           // Asssegna alla libreria il pin di ricezione
  mySwitch.enableTransmit(TxPin);                          // Asssegna alla libreria il pin di trasmissione

  pinMode(BatteryInput, INPUT);
  pinMode(SendCode, INPUT_PULLUP);                         // Imposta il pulsante come INPUT_PULLUP
  pinMode(JammerMode, INPUT_PULLUP);                       // Imposta il pulsante come INPUT_PULLUP
  pinMode(CapturedLed, OUTPUT);
  pinMode(JammerLed, OUTPUT);
  pinMode(RunLed, OUTPUT);

  digitalWrite(RunLed, HIGH);                              // Accende LED inizio esecuzione programma
  
  battery();                                               // Esegue il controllo della batteria
}

void loop() {
  rx();                                                     // Avvia l'intercettazione
  if (digitalRead(JammerMode) == HIGH) {                    // Verifica se il pulsante "JammerMode" è stato premuto
    jammer();                                               // Avvia modalità di disturbo
  }
}

void tx() {
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

  digitalWrite(WekeUpRx, LOW);                             // Addormenta rx
  mySwitch.setProtocol(ReceivedProtocol);                  // Imposta il protocollo
  mySwitch.setPulseLength(ReceivedDelay);                  // Imposta la lunghezza dell'impulso (in microsecondi)
  mySwitch.send(ReceivedValue, ReceivedBitlength);         // Trasmette il codice ricevuto con la stessa lunghezza di Bit

  battery();                                               // Esegue il controllo della batteria
}

void rx() {
  digitalWrite(WekeUpRx, HIGH);                            // Sveglia rx
  if (mySwitch.available()) {
    digitalWrite(CapturedLed, HIGH);                       // Accende indicatore codice ricevuto

    ReceivedProtocol = mySwitch.getReceivedProtocol();     // Assegna valore del protocollo ricevuto alla variabile globale
    ReceivedValue = mySwitch.getReceivedValue();           // Assegna valore del codice ricevuto alla variabile globale
    ReceivedBitlength = mySwitch.getReceivedBitlength();   // Assegna valore della lunghezza Bit ricevuti alla variabile globale
    ReceivedDelay = mySwitch.getReceivedDelay();           // Assegna valore della durata dell'impulso ricevuto alla variabile globale

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
    display.println("Delay:    us");
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
    tx();                                                                   // Esegue l'invio del codice catturato
  }
  if (digitalRead(SendCode) == HIGH && digitalRead(CapturedLed) == LOW) {

    Serial.print("NO CODE CAPTURED!");
    Serial.print(" \n ");
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
  digitalWrite(WekeUpRx, LOW);            // Addormenta rx
  digitalWrite(JammerLed, HIGH);          // Accende indicatore modalità jammer

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Jammer         On...");
  display.display();

  randomSeed(millis());
  JammerCode = random(1, 9999999);

  Serial.print("Jamming:   ");
  Serial.print(JammerCode);
  Serial.print("   LOCKED AT >> ");
  Serial.print("Bit: ");
  Serial.print(JammerBitlength);
  Serial.print("   Delay(us): ");
  Serial.print(JammerPulseLength);
  Serial.print("   Protocol: ");
  Serial.print(JammerProtocol);
  Serial.print("   Repetitions: ");
  Serial.print(JammerRepeatTransmit);
  Serial.print(" \n ");
  Serial.print(" \n ");


  mySwitch.setPulseLength(JammerPulseLength);
  mySwitch.setProtocol(JammerProtocol);
  mySwitch.setRepeatTransmit(JammerRepeatTransmit);
  mySwitch.send(JammerCode, JammerBitlength);           // Usa codice pseudocasuale alla lungh di bit impostata
  digitalWrite(JammerLed, LOW);                         // Spegne indicatore modalità disturbo


  battery();
}

void battery() {
  vbatt = analogRead(BatteryInput);
  vout = (vbatt * 5.0) / 1023;
  vin = vout / (R2 / (R1 + R2));

  if (vin < BatteryLimit ) {
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
