433Sniffer v0.7 Beta

**Descrizione:**

`433Snifferino` progettato per intercettare e ritrasmettere segnali RF a 433 MHz. Il dispositivo permette di catturare i codici inviati dai radiocomandi a 433 MHz e ritrasmetterli su richiesta. (i moduli a 433Mhz possono essere sostituiti tranquillamente con quelli a 315Mhz)Inoltre, include una modalità jammer che genera segnali pseudocasuali per disturbare le comunicazioni RF nella stessa frequenza.
Il codice sfrutta un display OLED per visualizzare i dettagli dei segnali intercettati inclusi il protocollo, la lunghezza in bit, e la durata dell'impulso. È dotato di LED di stato che indicano le operazioni in corso, come la cattura dei segnali, la modalità jammer e lo stato di funzionamento del sistema.

### Requisiti Hardware

1. **Microcontrollore**

- **Arduino Nano**:Effettua tutte le operazioni di ricezione e trasmissione dei segnali RF e i controlli dei vari LED, pulsanti, e display.

2. **Moduli RF a 433 MHz**

   2. **Moduli RF a 433 MHz**
   - **STX882**: Trasmettitore RF per inviare i segnali intercettati o generare disturbi in modalità jammer.
   - **SRX882**: Ricevitore RF per captare i segnali trasmessi.

3. **Display OLED**
   - **SSD1306, 128x32 pixels**: Collegato tramite I2C, mostra informazioni come i codici RF intercettati e lo stato del sistema, inclusa la batteria.

4. **Pulsanti e LED**
   - **Pulsanti**: Due pulsanti dedicati: uno per attivare la modalità jammer (`JammerMode`) e l'altro per ritrasmettere i codici catturati (`SendCode`).
   - **LED di Stato**: 
     - `JammerLed`: Indica la modalità jammer attiva.
     - `CapturedLed`: Si illumina quando un codice RF è stato correttamente catturato.
     - `RunLed`: Segnala che il sistema è in funzione.
     - `PowerLed`: Mostra che il dispositivo è acceso.

5. **Componenti Passivi**
   - **Resistenze**:
     - **470Ω**: Collegati ai LED per limitarne la corrente.
     - **1kΩ**: Utilizzati come pull-down resistors per i pulsanti.
     - **100kΩ e 10kΩ**: Componenti del partitore resistivo per il monitoraggio della tensione della batteria.

6. **Alimentazione**
   - **Batteria 9V (PP3)**: Fornisce l'energia necessaria al sistema, gestita tramite un interruttore per accensione e spegnimento.

7. **Connessioni e Comunicazione**
   - **Adattatore USB-C a Mini USB**: Permette la connessione al computer per la programmazione e il monitoraggio seriale del sistema.



Filippo Tamarindo
