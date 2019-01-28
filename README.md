![enter image description here](https://github.com/christophe94700/WS2812FX-fr/raw/master/WS2812FX_logo.png)

Origine [https://github.com/kitesurfer1404/WS2812FX](https://github.com/kitesurfer1404/WS2812FX "https://github.com/kitesurfer1404/WS2812FX")
Version en français- French Version

Mon exemple permet d'avoir:
- un serveur web.
- commande vocal avec Alexa.
- 6 alarmes de mise en marche programmée du bandeau LED.
- Minuteur d'arrêt.
- Configuration via la liaison série.
- Configuration via point d'accès lors de la première mise en route.
- Mise à jour OTA

Configuration depuis la liaison série et du client web. Plus d'information sur mon site www.caron.ws rubrique ESP8266.

## Compilation:
Compilation avec le SDK 2.5.0 Bêta 3
Pour l'utilisation avec Alexa, j'utilise le serveur web de la version 2.4.2 avec modification du fichier Parsing.cpp

WS2812FX - Plus d'effet pour vos LEDs!
======================================

Cette bibliothèque offre une variété d'effets pour les LEDs WS2811 / WS2812 / NeoPixel en remplacement de la bibliothèque Adafruit NeoPixel avec des fonctionnalités supplémentaires.

## Chargement des fichiers Binaires:
Utiliser un utilitaire pour flasher notre ESP8266. Voici les indication sur mon site:
https://caron.ws/diy-cartes-microcontroleurs/mise-a-jour-du-firmware/

Vesrion compiler avec SDK2.5.0:
Pour une version 4M avec 1M en spiffs voici les adresses mémoires pour les deux fichiers Binaires:
esp8266_web_Alexa.ino.bin Adresse 0x00000
esp8266_web_Alexa.spiffs.bin Adresse 0x300000

Vesrion compiler avec SDK2.4.2:
Pour une version 1M avec 64K en spiffs voici les adresses mémoires pour les deux fichiers Binaires:
esp8266_web_Alexa.ino.bin Adresse 0x00000
esp8266_web_Alexa.spiffs.bin Adresse 0xEB000
 
 
 ![enter image description here](https://github.com/christophe94700/WS2812FX-fr/raw/master/Flash.png)
