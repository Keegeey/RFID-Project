#include <Adafruit_PN532.h>

#define SCK (13)
#define MOSI (11)
#define SS (10)
#define MISO (12)



void setup() {
  // put your setup code here, to run once:
  Adafruit_PN532 nfc(SCK, MISO, MOSI, SS);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Working");
  delay(1000);
}
