433Sniffer v0.7 Beta Filippo Tamarindo

![box](https://github.com/FilippoT/433Sniffer.ino/blob/master/photo_433Sniffer.ino.jpg)

**Descrizione:**

`433Sniffer.ino` é stato progettato per intercettare e ritrasmettere segnali RF a 433MHz (i moduli a 433Mhz possono essere sostituiti tranquillamente con quelli a 315Mhz), per semplificare le applicazioni di domotica.
Utilizza un display OLED per visualizzare i dettagli dei segnali intercettati inclusi il protocollo, la lunghezza in bit, e la durata dell'impulso.
È dotato di LED di stato che indicano le operazioni in corso, come la cattura dei segnali, la modalità jammer e lo stato di funzionamento del sistema.
Inoltre, c'è una modalità jammer che genera segnali pseudocasuali per disturbare le comunicazioni RF nella stessa frequenza. 

NB: Anche se la modalità Jammer ha una durata temporale limitata, (vedi codice) può essere interrotta solo con l'interruttore di alimentazione.

### Hardware

1. **Microcontrollore**
   - **Arduino Nano**:il progetto con poche modifiche piu essere utilizzato anche con microcontrollori simili.

2. **Moduli RF a 433 MHz**
   - **STX882**: Trasmettitore RF per inviare i segnali intercettati o generare disturbi in modalità jammer.
   - **SRX882**: Ricevitore RF per captare i segnali trasmessi.

3. **Display OLED**
   - **SSD1306, 128x32 pixels**: Collegato tramite I2C, mostra informazioni come i codici RF intercettati e lo stato del sistema, inclusa la batteria.

4. **Componenti**
   - **Pulsanti**: Due pulsanti dedicati: uno per attivare la modalità jammer (`JammerMode`) e l'altro per ritrasmettere i codici catturati (`SendCode`).
   - **Interruttore**: contolla direttamente l'alimentazione della batteria (collegando direttamente il cavo usb non è necessario che sia su ON)
   - **LED rossi di Stato**: 
     - `JammerLed`: Indica la modalità jammer attiva.
     - `CapturedLed`: Si illumina quando un codice RF è stato correttamente catturato.
     - `RunLed`: Segnala che il sistema è in funzione.
     - `PowerLed`: Mostra che il dispositivo è alimentato.
   - **Resistenze**:
     - **470Ω**: Collegati ai LED per limitarne la corrente.
     - **1kΩ**: Utilizzati come pull-down per i pulsanti.
     - **100kΩ e 10kΩ**: Componenti del partitore resistivo per il monitoraggio della tensione della batteria.

5. **Alimentazione**
   - **Batteria 9V**: Fornisce l'energia necessaria al sistema, gestita tramite un interruttore.

7. **Comunicazione**
   - **Connessione USB**: Per la programmazione e il monitoraggio seriale del sistema, inoltre può fornire l'alimentazione al posto della batteria 9v.


![Collegamenti](https://github.com/FilippoT/433Sniffer.ino/blob/master/wiring_diagram.png)




