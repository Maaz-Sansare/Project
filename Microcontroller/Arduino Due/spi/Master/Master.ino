#include <SPI.h>

void setup() {

  pinMode(SS, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  Serial.begin(9600);
}

byte var = 4;
byte chartosend[] = "Hello Slave";
byte rec[sizeof(chartosend)];

void loop (void) {
   digitalWrite(SS, LOW); // enable Slave Select
   
   for (int i = 0 ; i < sizeof(chartosend) ; i++) {
      rec[i] = SPI.transfer (chartosend[i]);
   }
   
   digitalWrite(SS, HIGH); // disable Slave Select
   
   Serial.println("\n new data \n");
   for (int i = 0 ; i < 11 ; i++) {
      Serial.println(char(rec[i]));
   }
   delay(500);
}
