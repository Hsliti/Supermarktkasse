/*

 Dateiname              :Supermarktkasse.c

 Kurzbeschreibung       :Programm soll eine einfache Supermarktkasse simulieren.

 EingabeParameter       :VOID

 Rückgabeparameter      : 0 (Programm beenden )

 Author            	    :Hamza Sliti

 Mat.Nr                 :11131474

 Version          	    :1

 Datum letzte Änderung  :18.12.2020

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <memory.h>

#define MAX_SIZE 256

// Struktur-Objekten erstellen

typedef struct rechnung {
	char ANR[20];
	char MHD[20];
	char Bezeichnung[20];
	char VPE[20];
	float Preis;
	int Anzahl;
} rechnungtype;

// Struktur-Objekten erstellen

typedef struct {
	char ANR[20];
	char MHD[20];
	char Bezeichnung[20];
	char VPE[20];
	float Preis;

} artikel;

/*
 Funktionsname : Txt_einlesen
 Kurzbeschreibung der Aufgabe der Funktion:  Txt_einlesen soll die Datei „artikel.txt“ in den Speicher einlesen


 Eingaben  : Waren

 Ausgaben  : i

 Autor     : Hamza Sliti

 Version   : 1

 Quelle : https://mirlab.wordpress.com/2014/06/11/c-scanf-with-comma-separated-values/

 Datum letzte Änderung  :18.12.2020

 */
int Txt_einlesen(artikel *Waren) {
	// die Textdatei mit der Funktion fopen () öffnen
	FILE *fin = fopen("/home/pi/workspace/Supermarktkasse/src/artikel.txt",
			"r");
	if (fin == NULL) {
		printf("Konnte die Datei nicht öffnen ");
	}
	int i = 0;

	while (fscanf(fin, "%[^,],%[^,],%[^,],%[^,],%f\n", Waren[i].ANR,
			Waren[i].MHD, Waren[i].Bezeichnung, Waren[i].VPE, &Waren[i].Preis)
			!= EOF)
		++i;

	fclose(fin);
	return i;
}

/*
 Funktionsname : Txt_einlesen
 Kurzbeschreibung der Aufgabe der Funktion:  die Warenliste ausgeben


 Eingaben  : Waren , GroeSSe
 Ausgaben  : void

 Autor     : Hamza Sliti

 Version   : 1

 Datum letzte Änderung  :18.12.2020

 */

void Warenliste_ausgeben(artikel *Waren, int GroeSSe) {

	for (int i = 0; i < GroeSSe; ++i) {
		// eine Warezeile ausgeben
		artikel *Ware = &Waren[i];
		printf("%s %s %s %s %.2f\n", Ware->ANR, Ware->MHD, Ware->Bezeichnung,
				Ware->VPE, Ware->Preis);
	}
}

/*
 Funktionsname : Rechnung_ausgeben
 Kurzbeschreibung der Aufgabe der Funktion:  die Rechnung auf dem Bildschirm ausgeben


 Eingaben  : rechnung , Zaehler
 Ausgaben  : void

 Autor     : Hamza Sliti

 Version   : 1

 Datum letzte Änderung  :18.12.2020

 */

void Rechnung_ausgeben(rechnungtype *rechnung, unsigned int *Zaehler) {
	float total = 0;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	printf(
			"____________________________________________________________________________________________________________________\n\n");
	// Datum und Uhrzeit ausgeben ,  Quelle :https://stackoverflow.com/questions/1442116/how-to-get-the-date-and-time-values-in-a-c-program
	printf(
			"                                           %d-%02d-%02d  %02d:%02d:%02d\n",
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
			tm.tm_sec);
	printf(
			"____________________________________________________________________________________________________________________\n");

	printf("%10s%20s%20s%20s%20s%10s", "Anzahl", "Bezeichnung", "ANR", "MHD",
			"VPE", "Price\n");

	for (int j = 0; j < *Zaehler; ++j) {
		//eine Rechnungszeile ausgeben ,  wenn die Anzahl > 0 ist
		if (rechnung[j].Anzahl > 0) {
			printf("%10d%20s%20s%20s%20s%10.2f\n", rechnung[j].Anzahl,
					rechnung[j].Bezeichnung, rechnung[j].ANR, rechnung[j].MHD,
					rechnung[j].VPE, rechnung[j].Preis);
			total = total + rechnung[j].Preis * rechnung[j].Anzahl;
		}
	}
	printf(
			"____________________________________________________________________________________________________________________\n");
	printf(
			"                                                                             Summe:%.2f            \n",
			total);

}

/*
 Funktionsname : Kombination_pruefer
 Kurzbeschreibung der Aufgabe der Funktion:  prüft ob die Kombination Artikelnummer, MHD vorhanden ist


 Eingaben  : tempMHD , Waren ,position
 Ausgaben  : richtig

 Autor     : Hamza Sliti

 Version   : 1

 Datum letzte Änderung  :18.12.2020

 */

int Kombination_pruefer(char* tempMHD, artikel *Waren, int *position) {
	int richtig = 0;
	// richtig = 1 , wenn die Kombination Artikelnummer, MHD    vorhanden ist
	if (strcmp(tempMHD, Waren[*position].MHD) == 0) {
		richtig = 1;

	} else
		richtig = 0;

	return richtig;
}

/*
 Funktionsname : Ware_hinzufuegen_entfernen
 Kurzbeschreibung der Aufgabe der Funktion: soll  eine Ware zur Rechnung hinzufügen oder von ihr entfernen


 Eingaben  : tempANR, tempMHD, tempAktion, tempAnzahl, rechnung, Zaehler,Waren, position
 Ausgaben  : void

 Autor     : Hamza Sliti

 Version   : 1

 Datum letzte Änderung  :18.12.2020

 */

void Ware_hinzufuegen_entfernen(char* tempANR, char* tempMHD, char* tempAktion,
		int *tempAnzahl, rechnungtype *rechnung, unsigned int *Zaehler,
		artikel *Waren, int *position) {

	//  Deklaration und Initialisierung
	char stop[2] = { "f" };
	char pause[2] = { "s" };
	char plus[2] = { "+" };
	char minus[2] = { "-" };
	int hinzufuegen = 0, entfernen = 0;
	int Suchergebnis = 0;
	int richtig = 0;
	int existent = 0;
	printf(
			"\n\n (+) eine Ware hinzufuegen \n (-) eine Ware  entfernen \n (s) Der aktuelle Rechnungsstand ausgeben \n (f) Die Rechnung abschließen und ausgeben\n ");
	scanf("%s", tempAktion);
	// wenn tempAktion = (+) , hinzufuegen nimmt 1 , der Benutzer will eine Ware hinzufuegen
	if (strcmp(tempAktion, plus) == 0) {
		hinzufuegen = 1;
	}
	// wenn tempAktion = (-) , entfernen nimmt 1 , der Benutzer will eine Ware entfernen
	if (strcmp(tempAktion, minus) == 0) {
		entfernen = 1;
	}
	// wenn tempAktion ungleich  (s) und (f)
	if ((strcmp(tempAktion, stop) != 0) && (strcmp(tempAktion, pause) != 0)) {
		printf(
				"\n\n(s) Der aktuelle Rechnungsstand ausgeben \n(f) Die Rechnung abschließen und ausgeben\n ");
		printf("\ngeben Sie bitte die Artikelnummer : ");
		scanf("%s", tempANR);

		// wenn tempANR ungleich  (s) und (f) , dh der Benutzer will eine Ware entfernen oder hinzufuegen
		if ((strcmp(tempANR, stop) != 0) && (strcmp(tempANR, pause) != 0)) {
			printf("\n\ngeben Sie bitte MHD : ");
			scanf("%s", tempMHD);
			printf("\n\ngeben Sie bitte die Anzahl: ");
			scanf("%d", tempAnzahl);
			// Enthält die Rechnung bereits einen Artikel dieser Art, muss die vorhandene Rechnungszeile aktualisiert oder gegebenenfalls gelöscht werden.
			for (unsigned int k = 0; k <= 50; k++) {
				if (strcmp(tempANR, rechnung[k].ANR) == 0) {
					existent = 1;
					if (hinzufuegen == 1) {
						rechnung[k].Anzahl = rechnung[k].Anzahl + *tempAnzahl;

					}
					if (entfernen == 1) {
						rechnung[k].Anzahl = rechnung[k].Anzahl - *tempAnzahl;

					}
				}

			}
			// Enthält die Rechnung bisher keinen Artikel dieser Art, muss eine Rechnungszeile hinzugefügt werden
			if (existent == 0) {

				if (entfernen == 1) {
					printf(" Enthält die Rechnung  keinen Artikel dieser Art");
				} else {
					// Ablage einer Rechnungszeile im Speicher.
					for (unsigned int i = 0; i <= 12; ++i) {

						// wenn  Artikel mit dieser ANR  gelistet
						if (strcmp(tempANR, Waren[i].ANR) == 0) {
							Suchergebnis++;
							*position = i;
							// richtig nimmt die Rückgabe von Kombination_pruefer  , wenn richtig = 1 , die Kombination Artikelnummer, MHD   ist vorhanden
							richtig = Kombination_pruefer(tempMHD, Waren,
									position);
							// Ablage einer Rechnungszeile im Speicher
							if (richtig == 1) {

								strcpy(rechnung[*Zaehler].Bezeichnung,
										Waren[i].Bezeichnung);
								strcpy(rechnung[*Zaehler].ANR, Waren[i].ANR);
								strcpy(rechnung[*Zaehler].MHD, Waren[i].MHD);
								strcpy(rechnung[*Zaehler].VPE, Waren[i].VPE);
								rechnung[*Zaehler].Preis = Waren[i].Preis;
								rechnung[*Zaehler].Anzahl = *tempAnzahl;
								*Zaehler += 1;

							} else {
								printf(
										"\n Artikel mit diesem MHD nicht gelistet ");
							}

						}
					}
					// Artikelnummer nicht in der Warenliste
					if ((Suchergebnis == 0) && (strcmp(tempANR, stop) != 0)) {
						printf("\n Bitte eine Richtige Ware eingeben ");
					}
				}
			}
		}
	}
}

int main() {

	//  Deklaration und Initialisierung

	artikel Waren[MAX_SIZE];
	int GroeSSe = Txt_einlesen(Waren);

	char tempANR[20];
	char tempMHD[20];
	char tempAktion[20];
	int tempAnzahl = 0;
	int position = 0;
	int Rechnungsnummer = 1;
	rechnungtype rechnung[13];
	char stop[2] = { "f" };
	char pause[2] = { "s" };
	unsigned int Zaehler = 0;

	// die Warenliste ausgeben

	printf("ANR MHD Bezeichnung VPE Preis/VPE :\n\n\n");

	Warenliste_ausgeben(Waren, GroeSSe);

	while (1) {
		// Aufruf der Funktion ,die eine Ware zur Rechnunghinzufügt oder von ihr entfernt
		Ware_hinzufuegen_entfernen(tempANR, tempMHD, tempAktion, &tempAnzahl,
				rechnung, &Zaehler, Waren, &position);

		// Die Rechnung ist auf Anforderung (Konsoleneingabe "f") abzuschließen
		if ((!strcmp(tempANR, stop)) || (!strcmp(tempAktion, stop))) {

			printf(
					"____________________________________________________________________________________________________________________\n\n");
			printf(
					"                                                 RECHNUNG  n° %d                                                          \n",
					Rechnungsnummer);
			Rechnungsnummer++;
			// Aufruf der Funktion , die die Rechnung auf dem Bildschirm ausgibt
			Rechnung_ausgeben(rechnung, &Zaehler);
			// Speicherbereich mit '0' auffüllen
			memset(rechnung, 0, sizeof rechnung);
		}

		// Der aktuelle Rechnungsstand ist auf Anforderung (Konsoleneingabe "s") auf dem Bildschirmausgegeben

		if ((!strcmp(tempANR, pause)) || (!strcmp(tempAktion, pause))) {
			printf(
					"____________________________________________________________________________________________________________________\n\n");
			printf(
					"                                Der aktuelle Rechnungsstand : RECHNUNG  n° %d                                                          \n",
					Rechnungsnummer);
			// Aufruf der Funktion , die die Rechnung auf dem Bildschirm ausgibt
			Rechnung_ausgeben(rechnung, &Zaehler);
		}

	}

	return 0;
}

