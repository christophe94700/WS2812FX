// QUICKFIX...See https://github.com/esp8266/Arduino/issues/263
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define WIFI_TIMEOUT 10000                  // Vérification du WIFI toute les xxxx ms.
#define WIFIAP_TIMEOUT 1800000              // Temps de connexion en mode AP
#define WIFI_RESET 5                        // Reset du WIFI toute les xxxx minutes Valeur Max 255mn. Vauleur multiple de WIFI_TIMEOUT
#define HTTP_PORT 80                        // Port de connexion pour le serveur HTTP

// Adresse EEPROM
#define ADRESS_LED_LUM 0                    // Adresse pour le stockage dans EEPROM de la Luminosité (0)
#define ADRESS_LED_MOD 1                    // Adresse pour le stockage dans EEPROM du Mode (1)
#define ADRESS_LED_VIT 2                    // Adresse pour le stockage dans EEPROM de la vitesse sur 2 mots (2 et 3)
#define ADRESS_LED_COL 4                    // Adresse pour le stockage dans EEPROM de la couleur sur 4 mots (4-7)
#define ADRESS_WIFI 8                       // Adresse de départ paramètres WIFI Mot de passe et SSID (Fin ADRESS_WIFI + 96 mots)8-103
#define ADRESS_RESEAU 104                   // Adresse de départ paramètres Reseau DHCP=0 Fixe=1, IP passerelle,Fin IP statique (Fin ADRESS_RESEAU + 6 mots)104-109
#define ADRESS_NLED 110                     // Adresse de départ paramètres nombres de LED (Fin ADRESS_NLED + 2 mots)110-111
#define ADRESS_ON_OFF 112                   // Adresse mémorisation Marche/Arrêt du bandeau LED (112)
#define ADRESS_MINUTEUR 113                 // Adresse mémorisation Marche/Arrêt du bandeau LED (Fin ADRESS_MINUTEUR + 2 mots)113-114
#define ADRESS_GMT  115                     // Paramètre GMT heure été/hivers 1 mot (115)
#define ADRESS_TIMER 116                    // Timer on/off (116)
#define ADRESS_PIN_LED 117                  // PIN LED (117)
#define ADRESS_CUSTOM_S1 118                // Paramètre Custom Effet 1e Segment Pramètres: Mode:1 mot,Couleur: 4 mots, Vitesse: 2 mots,Sens : 1 mot total: 8 Bytes 118-125
#define ADRESS_CUSTOM_S2 126                // Paramètre Custom Effet 2e Segment Pramètres: Mode:1 mot,Couleur: 4 mots, Vitesse: 2 mots,Sens : 1 mot total: 8 Bytes 126-133
#define ADRESS_CUSTOM_S3 134                // Paramètre Custom Effet 3e Segment Pramètres: Mode:1 mot,Couleur: 4 mots, Vitesse: 2 mots,Sens : 1 mot total: 8 Bytes 134-141
#define ADRESS_LED_COL_ALEXA 142            // Adresse pour le stockage dans EEPROM de la couleur pour Alexa sur 4 mots (4-7) 142-145

#define ADRESS_NOM_ALEXA 200                // Paramètre Nom du périphérique pour Alexa 32 mots 200-231
#define ADRESS_PASSWORD 232                // Paramètre Nom du périphérique pour OTA et accès paramètres 32 mots 232-263

#define ADRESS_AL0 317                      // Adresse mémorisation paramètres alarme0 13 Bytes ID1,ID2,ID3,ID4,ID5,Heure,Minute,On/Off, Jour/toujours,couleur,mode,puissance,timer
#define ADRESS_AL1 330                      // Adresse mémorisation paramètres alarme1 13 Bytes ID1,ID2,ID3,ID4,ID5,Heure,Minute,On/Off, Jour/toujours,couleur,mode,puissance,timer
#define ADRESS_AL2 343                      // Adresse mémorisation paramètres alarme2 13 Bytes ID1,ID2,ID3,ID4,ID5,Heure,Minute,On/Off, Jour/toujours,couleur,mode,puissance,timer 
#define ADRESS_AL3 356                      // Adresse mémorisation paramètres alarme3 13 Bytes ID1,ID2,ID3,ID4,ID5,Heure,Minute,On/Off, Jour/toujours,couleur,mode,puissance,timer
#define ADRESS_AL4 369                      // Adresse mémorisation paramètres alarme4 13 Bytes ID1,ID2,ID3,ID4,ID5,Heure,Minute,On/Off, Jour/toujours,couleur,mode,puissance,timer
#define ADRESS_AL5 382                      // Adresse mémorisation paramètres alarme5 13 Bytes ID1,ID2,ID3,ID4,ID5,Heure,Minute,On/Off, Jour/toujours,couleur,mode,puissance,timer - Fin 394

#define ADRESS_EEPROM_INIT 400              // Adresse mémorisation de l'initialisation de EEPROM

// Paramètrages
#define ADRESS_CUSTOMB 8                    // Nombres de mots par segment
#define ADRESS_ALB 13                        // Nombres de mots par alarme
#define NB_ALARME 6                         // Nombres d'alarme



// Valeur par défaut EEPROM
#define DEF_LED_LUM 255                    // Valeur par défaut EEPROM de la Luminosité
#define DEF_LED_MOD 0                      // Valeur par défaut EEPROM du Fixe
#define DEF_LED_VIT 3000                   // Valeur par défaut EEPROM de la vitesse 
#define DEF_LED_COL HOTWHITE               // Valeur par défaut EEPROM de la couleur 

#define DEF_NLED 10                     // Valeur par défaut EEPROM Nombre de LED
#define DEF_ON_OFF 0                   // Valeur par défaut EEPROM Marche/Arrêt du bandeau LED 112
#define DEF_MINUTEUR 1                 // Valeur par défaut EEPROM Minuteur
#define DEF_GMT  0                     // Paramètre GMT heure été/hivers 
#define DEF_PASSWORD "admin"          // Paramètre mot de passe par défaut


#define S_REASON_FOR_RESTART "\nRaison du démarrage: (%d) getResetReason: %s\n\n"
#define S_POWER_ON           "Mise sous tension."
#define S_HARDWARE_WD        "Déclenchement du chien de garde matériel."
#define S_EXCEPTION_OCCURED  "Occurence d'une exception.\n"
#define S_SOFTWARE_WD        "Déclenchement du chien de garde logiciel."
#define S_RESTART_OR_RESET   "Fonction restart() ou reset() invoquée."
#define S_ESP_WOKE           "Réveil du ESP après un profond sommeil."
#define S_EXTERNAL_RESET     "Réinitialisation externe du système."
#define S_UNKNOWN_REASON     "Pour une raison inconnue."

#define S_RST_RESTART "\n\nRedémarrage avec ESP.restart()"
#define S_RST_RESET "\n\nRedémarrage avec ESP.reset()"
#define S_RST_HWDT "\n\nRedémarrage avec déclenchement du chien de garde matériel (attendre 6 secondes)"
#define S_RST_SWDT "\n\nRedémarrage avec déclenchement du chien de garde logiciel (attendre 3 seconds)"
#define S_RST_EXCP_0 "\n\nRedémarrage en vertu de l'exception 0: IllegalInstructionCause (division par 0)"
#define S_RST_EXCP_3 "\n\nRedémarrage en vertu de l'exception 3: LoadStoreErrorCause"
