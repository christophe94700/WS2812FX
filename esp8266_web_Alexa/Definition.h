// QUICKFIX...See https://github.com/esp8266/Arduino/issues/263
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define WIFI_TIMEOUT 60000                  // Vérification du WIFI toute les xxxx ms.
#define WIFI_RESET 5                        // Reset du WIFI toute les xxxx minutes Valeur Max 255mn. Vauleur multiple de WIFI_TIMEOUT
#define HTTP_PORT 80                        // Port de connexion pour le serveur HTTP

#define LED_PIN 2                           // 0 = GPIO0, 2=GPIO2
#define LED_MAX 1000                        // Nombres de LED MAX 

// Adresse EEPROM
#define ADRESS_LED_LUM 0                    // Adresse pour le stockage dans EEPROM de la Luminosité
#define ADRESS_LED_MOD 1                    // Adresse pour le stockage dans EEPROM du Mode
#define ADRESS_LED_VIT 2                    // Adresse pour le stockage dans EEPROM de la vitesse sur 2 mots (2 et 3)
#define ADRESS_LED_COL 4                    // Adresse pour le stockage dans EEPROM de la couleur sur 4 mots (4-7)

#define ADRESS_WIFI 8                       // Adresse de départ paramètres WIFI Mot de passe et SSID (Fin ADRESS_WIFI + 96 mots)8-103
#define ADRESS_RESEAU 104                   // Adresse de départ paramètres Reseau DHCP=0 Fixe=1, IP passerelle,Fin IP statique (Fin ADRESS_RESEAU + 6 mots)104-109
#define ADRESS_NLED 110                     // Adresse de départ paramètres nombres de LED (Fin ADRESS_NLED + 2 mots)110-111
#define ADRESS_ON_OFF 112                   // Adresse mémorisation Marche/Arrêt du bandeau LED 112
#define ADRESS_MINUTEUR 113                 // Adresse mémorisation Marche/Arrêt du bandeau LED 113 (Fin ADRESS_MINUTEUR + 2 mots)113-116
#define ADRESS_GMT  117                     // Paramètre GMT heure été/hivers 1 mot


#define ADRESS_CUSTOM_S1 118                // Paramètre Custom Effet 1e Segment Pramètres: Mode:1 mot,Couleur: 4 mots, Vitesse: 2 mots,Sens : 1 mot total: 8 Bytes 118-125
#define ADRESS_CUSTOM_S2 126                // Paramètre Custom Effet 2e Segment Pramètres: Mode:1 mot,Couleur: 4 mots, Vitesse: 2 mots,Sens : 1 mot total: 8 Bytes 126-133
#define ADRESS_CUSTOM_S3 134                // Paramètre Custom Effet 3e Segment Pramètres: Mode:1 mot,Couleur: 4 mots, Vitesse: 2 mots,Sens : 1 mot total: 8 Bytes 134-141

#define ADRESS_CUSTOMB 8                    // Nombres de mots par segment

#define ADRESS_NOM_ALEXA 200                // Paramètre Nom du périphérique pour Alexa 32 mots 

#define ADRESS_AL0 317                      // Adresse mémorisation paramètres alarme0 8 Bytes ID,Heure,Minute,On/Off, Jour/toujours,couleur,mode,timer
#define ADRESS_AL1 325                      // Adresse mémorisation paramètres alarme1 8 Bytes ID,Heure,Minute,On/Off, Jour/toujours,couleur,mode,timer 
#define ADRESS_AL2 333                      // Adresse mémorisation paramètres alarme2 8 Bytes ID,Heure,Minute,On/Off, Jour/toujours,couleur,mode,timer 
#define ADRESS_AL3 341                      // Adresse mémorisation paramètres alarme3 8 Bytes ID,Heure,Minute,On/Off, Jour/toujours,couleur,mode,timer
#define ADRESS_AL4 349                      // Adresse mémorisation paramètres alarme4 8 Bytes ID,Heure,Minute,On/Off, Jour/toujours,couleur,mode,timer
#define ADRESS_AL5 357                      // Adresse mémorisation paramètres alarme4 8 Bytes ID,Heure,Minute,On/Off, Jour/toujours,couleur,mode,timer - Fin 364

#define ADRESS_ALB 8                        // Nombres de mots par alarme
#define NB_ALARME 6                         // Nombres d'alarme
