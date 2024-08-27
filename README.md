433Sniffer v0.7.3 Beta


433Sniffer è uno strumento sviluppato per intercettare, visualizzare e ritrasmettere segnali a 433 MHz. (E' comunque possibile utilizzare una Tx e RX a 315MHZ)
Questo progetto è pensato per permettere l'analisi e la manipolazione di segnali RF (Radio Frequency) utilizzati in telecomandi e dispositivi simili. 
L'utente può anche utilizzare una modalità jammer per disturbare le comunicazioni RF.

Funzionalità principali
Intercettazione segnali RF: Il programma intercetta segnali a 433 MHz, rilevando il protocollo, la lunghezza in bit e la durata degli impulsi.

Visualizzazione su display: Le informazioni catturate vengono visualizzate su un display OLED tramite I2C.

Ritrasmissione codici: È possibile ritrasmettere i codici catturati con la stessa configurazione del segnale originale.

Modalità Jammer: Una modalità dedicata permette di generare segnali pseudocasuali per disturbare la trasmissione RF.

Monitoraggio della batteria: Implementa un controllo dello stato della batteria con avviso su display in caso di batteria scarica.

Requisiti Hardware

Arduino Nano: Questo progetto utilizza una scheda Arduino Nano come microcontrollore principale per gestire la ricezione e trasmissione dei segnali RF, controllare i LED di stato, i pulsanti, e l'output su un display OLED.
Moduli RF

STX882: Modulo trasmettitore RF a 433 MHz. Utilizzato per inviare i segnali catturati o generare segnali pseudocasuali in modalità jammer.
SRX882: Modulo ricevitore RF a 433 MHz. Utilizzato per intercettare i segnali trasmessi a 433 MHz.
Display

Display OLED 128x32 pixels (SSD1306): Un piccolo display OLED collegato tramite I2C (indirizzo 0x3C) utilizzato per mostrare informazioni come il codice RF intercettato, lo stato della batteria, e altre notifiche operative.
Pulsanti

Pulsante JammerMode: Pulsante utilizzato per attivare la modalità jammer, che genera e invia segnali pseudocasuali per disturbare le comunicazioni RF.
Pulsante SendCode: Pulsante utilizzato per ritrasmettere un codice intercettato precedentemente.
LED di Indicazione

JammerLed: LED che indica quando il dispositivo è in modalità jammer.
CapturedLed: LED che si accende quando un codice RF è stato intercettato correttamente.
RunLed: LED che indica che il programma è in esecuzione.
PowerLed: LED che indica quando il dispositivo è acceso.
Resistenze

4 x 470Ω: Collegate ai LED per limitare la corrente e prevenire danni.
2 x 1kΩ: Collegate ai pulsanti JammerMode e SendCode come pull-down resistors.
1 x 100kΩ e 1 x 10kΩ: Utilizzate per il partitore resistivo collegato al pin BatteryInput per misurare la tensione della batteria.
Alimentazione

Batteria 9V (PP3): La scheda è alimentata da una batteria da 9V, con un interruttore di accensione/spegnimento per controllare l'alimentazione.
Interruttore

Power Switch: Interruttore collegato tra la batteria e la scheda Arduino per accendere e spegnere il dispositivo.
Connessione USB

Adattatore USB-C a Mini USB: Utilizzato per collegare la scheda Arduino Nano al computer per la programmazione e il monitoraggio seriale.


Configurazioni principali
PIN: Configurazione dei pin per i vari componenti (ricevente, trasmettitore, LED, pulsanti, ecc.).

Librerie utilizzate
Adafruit SSD1306: Per la gestione del display OLED.
RCSwitch: Per la gestione della ricezione e trasmissione dei segnali RF.

Note
Questo progetto è in fase Beta e potrebbe ancora avere bug o necessitare di ottimizzazioni.
Nonostante non sia un programmatore esperto, questo progetto è un'opportunità per imparare e sperimentare con Arduino e la comunicazione RF.

Filippo Tamarindo
