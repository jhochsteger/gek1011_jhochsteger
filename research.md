# Mitschrift

## Fragen

* Was ist ein SPI-Bus und wie ist dieser aufgebaut?

> SPI steht für Serial Peripheral Interface. Es ist eine Kommunikationsschnittstelle, welche Voll-Duplex zur Verfügung
> stellt.
> Diese Funktionalität wird über 4 pins zur Verfügung gestellt:
> * SCK: Clock, gibt die Taktrate an
> * CS: Chip Select, gibt an ob der Slave angesprochen werden soll
> * MOSI: Main Out Secondary In, Daten die vom Main zum Secondary geschickt werden
> * MISO: Main In Secondary Out, Daten die vom Secondary zum Main geschickt werden
>
> Falls mehrere Secondarys verbunden sind, braucht man für jeden Secondary einen eigenen CS Pin.
> [1]

* Welche Vorteile ergeben sich bei der Verwendung eines Kommunikationsbusses?

> Die Kommunikation ist standardisiert und kann durch existierende Libraries leichter implementiert werden.

* Welche Möglichkeiten der Beschaltung sind beim SPI-Bus möglich und wie wirkt sich die Clock darauf aus?

> Es gibt 4 Möglichkeiten der Beschaltung:
> diese hängen mit Clock Polarity (CPOL) und Clock Phase (CPHA) zusammen.
> CPOL gibt an ob, die Clock bei 0 oder 1 startet.
> CPHA gibt an ob, die Daten bei der steigenden oder fallenden Flanke der Clock übertragen werden.
>
> * CPOL = 0, CPHA = 0:
> * CPOL = 0, CPHA = 1:
> * CPOL = 1, CPHA = 0:
> * CPOL = 1, CPHA = 1:

* Wie werden zeitkritische Anwendungen (real-time) eingeteilt?
* Wie kommt ein Watchdog bei zeitkritischen Anwendungen zum Einsatz?

> Der Watchdog ist ein Timer der einen Prozess nach einer bestimmten Zeit beendet, wenn dieser nicht rechtzeitig
> zurückmeldet, dass er noch läuft.[2]

* Was sind Real-Time Operating-Systems (RTOS) und wie kann man diese auf Mikrokontrollern einsetzen?

> Ein RTOS ist ein Betriebssystem, welches für die Verwendung in Echtzeit-Anwendungen optimiert ist.

## Umsetzung

Zur Umsetzung habe ich das freertos blink Beispiel genommen und so umgeschrieben, dass ich für jeden Zustand der Ampel
einen eigenen Task habe, und über die Priorität der Tasks die Ampel steuere. Das Setzen der Priorität wird über einen
Main Task umgesetzt, welcher nach der Absolvierung jeder Ampelperiode wieder die Kontrolle bekommt und die Priorität des
Nächsten Tasks hochsetzt. Die Priorität des Main Tasks ist immer 2, somit wird dieser als erster ausgeführt. Dieser
setzt die Priorität des nächsten ampel Tasks auf 3, um diesen auszuführen. Wenn der Task fertig ist, setzt er seine eigene
Priorität wieder auf 1 und setzt den nächsten Taskhandler in eine Variable, damit der Maintask diesen starten kann.

## Quellen

1. https://www.analog.com/en/analog-dialogue/articles/introduction-to-spi-interface.html
2. https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
