#define REDUCED_MODES // sketch is too big for Arduino w/32k flash, so invoke reduced modes
#include <WS2812FX_fr.h>

#define LED_COUNT 13 // Nombres de LED sur le bandeau
#define LED_PIN 2

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

String cmd = "";               // String to store incoming serial commands
boolean cmd_complete = false;  // whether the command string is complete


void setup() {
  Serial.begin(115200);
  ws2812fx.init();
  ws2812fx.setBrightness(30);
  ws2812fx.setSpeed(1000);
  ws2812fx.setColor(0x007BFF);
  ws2812fx.setMode(FX_MODE_STATIC);
  ws2812fx.start();

  printModes();
  printUsage();
}

void loop() {
  ws2812fx.service();

  // On Atmega32U4 based boards (leonardo, micro) serialEvent is not called
  // automatically when data arrive on the serial RX. We need to do it ourself
  #if defined(__AVR_ATmega32U4__) || defined(ESP8266)
  serialEvent();
  #endif

  if(cmd_complete) {
    process_command();
  }
}

/*
 * Checks received command and calls corresponding functions.
 */
void process_command() {
  if(cmd == "b+") { 
    ws2812fx.increaseBrightness(25);
    Serial.print("Increased brightness by 25 to: ");
    Serial.println(ws2812fx.getBrightness());
  }

  if(cmd == "b-") {
    ws2812fx.decreaseBrightness(25); 
    Serial.print("Decreased brightness by 25 to: ");
    Serial.println(ws2812fx.getBrightness());
  }

  if(cmd.startsWith("b ")) { 
    uint8_t b = (uint8_t) cmd.substring(2, cmd.length()).toInt();
    ws2812fx.setBrightness(b);
    Serial.print("Set brightness to: ");
    Serial.println(ws2812fx.getBrightness());
  }

  if(cmd == "s+") { 
//  ws2812fx.increaseSpeed(10);
    ws2812fx.setSpeed(ws2812fx.getSpeed() * 1.2);
    Serial.print("Increased speed by 10 to: ");
    Serial.println(ws2812fx.getSpeed());
  }

  if(cmd == "s-") {
//  ws2812fx.decreaseSpeed(10);
    ws2812fx.setSpeed(ws2812fx.getSpeed() * 0.8);
    Serial.print("Decreased speed by 10 to: ");
    Serial.println(ws2812fx.getSpeed());
  }

  if(cmd.startsWith("s ")) {
    uint16_t s = (uint16_t) cmd.substring(2, cmd.length()).toInt();
    ws2812fx.setSpeed(s); 
    Serial.print("Set speed to: ");
    Serial.println(ws2812fx.getSpeed());
  }

  if(cmd.startsWith("m ")) { 
    uint8_t m = (uint8_t) cmd.substring(2, cmd.length()).toInt();
    ws2812fx.setMode(m);
    Serial.print("Set mode to: ");
    Serial.print(ws2812fx.getMode());
    Serial.print(" - ");
    Serial.println(ws2812fx.getModeName(ws2812fx.getMode()));
  }

  if(cmd.startsWith("c ")) { 
    uint32_t c = (uint32_t) strtol(&cmd.substring(2, cmd.length())[0], NULL, 16);
    ws2812fx.setColor(c); 
    Serial.print("Set color to: ");
    Serial.print("0x");
    if(ws2812fx.getColor() < 0x100000) { Serial.print("0"); }
    if(ws2812fx.getColor() < 0x010000) { Serial.print("0"); }
    if(ws2812fx.getColor() < 0x001000) { Serial.print("0"); }
    if(ws2812fx.getColor() < 0x000100) { Serial.print("0"); }
    if(ws2812fx.getColor() < 0x000010) { Serial.print("0"); }
    Serial.println(ws2812fx.getColor(), HEX);
  }

  cmd = "";              // reset the commandstring
  cmd_complete = false;  // reset command complete
}

/*
 * Prints a usage menu.
 */
void printUsage() {
  Serial.println("Usage:");
  Serial.println();
  Serial.println("m <n> \t : select mode <n>");
  Serial.println();
  Serial.println("b+ \t : increase brightness");
  Serial.println("b- \t : decrease brightness");
  Serial.println("b <n> \t : set brightness to <n>");
  Serial.println();
  Serial.println("s+ \t : increase speed");
  Serial.println("s- \t : decrease speed");
  Serial.println("s <n> \t : set speed to <n>");
  Serial.println();
  Serial.println("c 0x007BFF \t : set color to 0x007BFF");
  Serial.println();
  Serial.println();
  Serial.println("Have a nice day.");
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println();
}


/*
 * Prints all available WS2812FX blinken modes.
 */
void printModes() {
  Serial.println("Supporting the following modes: ");
  Serial.println();
  for(int i=0; i < ws2812fx.getModeCount(); i++) {
    Serial.print(i);
    Serial.print("\t");
    Serial.println(ws2812fx.getModeName(i));
  }
  Serial.println();
}


/*
 * Reads new input from serial to cmd string. Command is completed on \n
 */
void serialEvent() {
  while(Serial.available()) {
    char inChar = (char) Serial.read();
    if(inChar == '\n') {
      cmd_complete = true;
    } else {
      cmd += inChar;
    }
  }
}
