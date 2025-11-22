# Funktionsumfang #

## Menüs und Funktionen (Stand Feb. 2021) {#menüs-und-funktionen-stand-feb.-2021 .western}

### Datenbank Menü

**Start** - Mit dieser Auswahl kommt man zurück zur Anzeige des Logos mit der Summe aller Verträge.

**Wechseln\...** - Führt zu einer Dialogfolge, mit der einen neue Datenbank angelegt oder zu einer
existierenden Datenbank gewechselt werden kann.

**Kopie / Anonymisierte Kopie anlegen\...** - Führt zu einer Dialogfolge, mit der man die momentan
geöffnete Datenbank in eine neue Datei kopieren kann. Im Falle einer "anonymisierten Kopie" werden
die persönlichen Daten der Kreditgeber:innen durch "neutrale" Daten überschrieben.

**Projektkonfiguration ändern\...** - Dieser Menüpunkt startet eine Dialogfolge zur Eingabe von
Projektdaten wie Adresse oder Geschäftsführern etc. Diese Informationen werden auch bei der
Erstellung von Briefen verwendet.

**Ausgabeverzeichnis festlegen\...** - Führt zu einer Dialogfolge, mit der ein Verzeichnis
ausgewählt werden kann. Dort werden bei verschiedenen Aktionen CSV und PDF Dateien erzeugt (Briefe,
Listen etc.)

### Kreditgeber

**Liste** - zeigt eine Liste aller DK Geber:innen (= Kreditor:innen) an. Die Liste kann mit der
Filterfunktion (Eingabefeld über der Liste) eingeschränkt werden. Sortierung der Liste erfolgt über
die Felder der Kopfzeile.

**Kontext Menü der Zeilen in der Liste**

- Bearbeiten bestehender Kontakte
- Löschen eines Kontakts (sofern es keine Verträge zu dem Kontakt gibt)
- Anzeige aller Verträge des ausgewählten Kontakts

**Neu anlegen\...** - Führt zur Dialogfolge zum Anlegen eines neuen Kontakts (mit oder ohne Vertrag)

### Verträge

**Liste anzeigen** - zeigt die Liste aller Verträge an. Die Liste kann durch Klick auf die
Kopfzeilenfelder sortiert werden. Bei der Auswahl eines Vertrages wird in der unteren Hälfte des
Fensters alle Buchungen zu diesem Vertrag angezeigt.

Auch hier kann durch Eingabe eines Suchbegriffes im "Filter" Feld die Liste eingeschränkt
werden. Der Filter bezieht sich auf die Spalten "KreditorIn" und "Vertragskennung".

**Kontext Menü in der Liste**

- für aktive Verträge
  - **Vertrag kündigen**: Eingabe des geplanten Vertragsende Datums
  - **Vertrag beenden**: Eingabe des Beendigungsdatums, Berechnung des abschließenden Zinses
  - **Ein- und Auszahlungen**: um den Wert des Vertrages unter der Laufzeit zu erhöhen oder zu
    verringern.
  - **Anmerkung ändern**
  - **Vertragskennung ändern**
  - **Vertragsdatum ändern**
  - **Kündigungsfrist und Vertragsende ändern**
  - **letzte Buchung löschen**
  - **(vor der zweiten Buchung) Datum Geldeingang ändern**
- für inaktive Verträge - also Verträge ohne Zahlungseingang
  - **Vertrag aktivieren**: Eingabe des Datums des Geldeingangs (markiert den Beginn der
    Zinszahlung)
  - **Vertrag löschen**: Löscht den Vertrag

**Beendete V. anzeigen** - Ähnlich wie die Liste der Verträge gibt es hier die Liste der *beendeten*
Verträge. Bei der Auswahl eines Vertrages wird in der unteren Hälfte des Fensters alle Buchungen zu
diesem Vertrag angezeigt.

**Neu anlegen\...** - Genau wie das Menü "Kreditgeber" > "Neu" kann mit dieser Funktion ein neuer
Vertrag und ggf. ein neue neue Kreditgeberin eingegeben werden.

**Jahreszinsberechnung** - Für den Jahresabschluss: berechnet Zinsen, passt Wert der Verträge bei
thesaurierenden Verträgen an. Ausgabe in CSV Listen und zukünftig auch Briefe im PDF Format.

**Anlagen verwalten** - Diese Option wechselt zu einer Listenansicht der konfigurierten Geldanlagen
an. Jeder Vertrag kann einer Geldanlage zugeordnet werden. Die Übersicht erlaubt es die Anzahl und
das Volumen der Verträge mit gleichen Eigenschaften im Blick zu behalten.

**Übersicht** - Ausgabe diverser Übersichten / Statistiken zu den Krediten

**Verlauf** - Ausgabe einer Statistik mit der man den Verlauf des Kreditvolumens anzeigen kann

**Listen drucken** - Ausgaben diverser Listen

## Stand der Dinge / Ausblick

### Aktuelle Entwicklung

- Kurzfristig:
  - Verzögerte Zinszahlung bis \[Datum\] (z.B. \"bis Baubeginn\")
- Mittelfristig:
  - Briefdruck (PDF Erzeugung) mit Projekt- und Empfängerspezifischen Anpassungen an Text und Layout.
