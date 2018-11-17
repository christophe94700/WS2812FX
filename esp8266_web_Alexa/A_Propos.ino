/*
  WS2812FX Webinterface.

  Harm Aldick - 2016
  FEATURES
      Webinterface with mode, color, speed and brightness selectors


  LICENSE

  The MIT License (MIT)

  Copyright (c) 2016  Harm Aldick

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.

  CHANGELOG
  2016-11-26 initial version
  2018-01-06 added custom effects list option and auto-cycle feature

*/

/* Christophe Caron www.caron.ws
   Sauvegarde dans EEPROM paramètres bandeau LED V1.1.0 19/01/2018
   Sauvegarde dans EEPROM paramètres WIFI V1.2.0 19/01/2018
   Sauvegarde dans EEPROM paramètres Réseau V1.2.1 21/01/2018
   Ajout des information sur ESP01 V1.2.2 21/01/2018
   Modification du client Web V1.3.0 21/01/2018
   Utilisation de la bibliothèque FS pour le client web, ajout de jscolor dans la mémoire EEPROM V1.4.0 21/01/2018
   Sauvegarde dans EEPROM paramètres nombre de led V1.5.0 21/01/2018
   Mémorisation du marche/arrêt du bandeau LED V1.5.1 22/01/2018
   Modification du client web V1.5.2 23/01/2018
   Ajout gestion de la date et de l'heure avec serveur NTP V1.6.0 26/10/2018
   Modification du client web V1.6.1 27/01/2018
   Ajout de la fonction minuteur V1.6.2 28/01/2018
   Ajout reset wifi et ping V1.6.3 29/01/2018
   Ajout calendrier mise en marche et arrêt V1.6.4 18/02/2018
   Ajout du mode AP si pas de connexion au réseau pour configuration des paramètres réseau et LED. Amélioration du programme V1.6.5 11/03/2018
   Ajout page pour affichage des paramètres + changement Heure d'été/hiver + amélioration gestion des alarmes + modes custom V1.6.6 04/04/2018
   Ajout page pour modification du Custom Effet + Amélioration du programme gestion des alarmes V1.6.7 19/04/2018
   Ajout commande vocale Avec Alexa Amazon SDK 2.4.1 Modification du fichier Parsing.cpp (!isEncoded||(0==_currentArgCount)){ // @20180124OF01: Workarround for Alexa Bug V1.6.8 05/07/2018
   Séparation du programme en plusieurs fichiers V1.6.9 19/08/2018 
   SDK 2.4.2 Modification du fichier Parsing.cpp (!isEncoded||(0==_currentArgCount)){ // @20180124OF01: Workarround for Alexa Bug
   Ajout d'un timer d'arrêt pour les alarmes + Corrections de Bug Mineur + Initialisation des valeurs dans EEPROM
   Amélioration du programme gestion du Wifi,Ajout d'un temps maximun en mode AP et ajout de la puissance de luminosité dans les alarmes V1.7.0 24/08/2018
   Amélioration gestion des alarmes, de la minuterie. Ajout de la selection alarme weekend et semaine. Modifier la valeur du nombre d'alarme dans TimeAlarms_h=> dtNBR_ALARMS 40 
   Ajout information cause du reboot https://sigmdel.ca/michel/index_fr.html V1.8.0 16/09/2018
   Ajout fonction OTA et correction bug mineur V1.9.0 28/10/2018
   Ajout fonction mDNS, mot passe pour mise à jour "admin" et accès aux paramètres. 
   Améliorations mineurs et passage à EEPROM 1M avec 64KO pour SPIFFS pour mise à jour via OTA V1.9.1 03/11/2018 
   Ajout Initialisation EEPROM avec paramètres par défaut apres mise à jour OTA et configuration de la broche de sortie pour la LED V1.9.2 14/11/2018
   Ajout de la configuration de la couleur pour commande vocal Alexa V1.9.3 17/11/2018 
*/
