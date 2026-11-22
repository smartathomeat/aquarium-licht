# Home Assistant anschließen

1. Das Gerät über die [ESPHome-Integration](https://www.home-assistant.io/integrations/esphome/) hinzufügen. Dafür den eigenen API-Schlüssel verwenden.
2. Die vier Helfer aus [aquarium-helfer.yaml](aquarium-helfer.yaml) als [Konfigurationspaket](https://www.home-assistant.io/docs/configuration/packages/) laden oder in der Oberfläche mit denselben Entitäts-IDs anlegen. Bei vorhandenen Helfern diese weiterverwenden.
3. Neu angelegte Helfer lassen Automatik, Wolken und Mondlicht zunächst aus und starten bei 0 % Intensität. Sobald Home Assistant verbunden ist, übernimmt der ESP diese Werte. Eine passende Gesamtintensität setzen und dann `input_boolean.aquarium_automatik` einschalten. Wolken und Mondlicht nach Wunsch freigeben. Für Handbetrieb die Automatik ausschalten und die drei Lichter direkt einstellen.
4. Zur Anzeige ein eigenes Dashboard anlegen und [aquarium-dashboard.yaml](aquarium-dashboard.yaml) im Rohkonfigurationseditor einfügen. Weichen die durch Home Assistant erzeugten Entitäts-IDs ab, die Verweise im Dashboard entsprechend anpassen.

Voraussetzung für die Anzeige „Himmel“ sind aktuelle ESP-Entitäten. Sie zeichnet Sonne, Wolken und Mond passend zur Phase und zeigt die vom ESP gemeldete Uhrzeit. Der ESP selbst führt den Zeitplan aus. Eine zusätzliche Home-Assistant-Automation für die tägliche Lichtkurve ist überflüssig.
