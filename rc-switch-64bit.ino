/*
  Example for receiving Data from TFA Dostmann 30.3249.02 (thermo/hygrometer)
*/

#include "./RCSwitch.h"

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);
  Serial.println("RC-Switch-Recv-Test-64Bit:");
  mySwitch.enableReceive(digitalPinToInterrupt(3));
  mySwitch.setReceiveTolerance(30);
}

void loop() {
  if (mySwitch.available()) {
    uint8_t bitLength = mySwitch.getReceivedBitlength();
    if (bitLength > 39 && bitLength < 42) {
      uint8_t bitOffset = bitLength == 40 ? 0 : 1;
      uint64_t value = mySwitch.getReceivedValue();
      uint8_t id = value >> (bitOffset + 32);
      bool batteryLow = (value >> (bitOffset + 31) & 1);
      bool manualSend = (value >> (bitOffset + 30) & 1);
      uint8_t channel = (value >> (bitOffset + 28) & 3);
      float temperature = 0.1f * (value >> (bitOffset + 16) & 0xfff) - 50;
      uint8_t humidity = (value >> (bitOffset + 8) & 0xff);
      uint16_t unknownData =  value & (bitOffset ? 0x1ff : 0xff);

      Serial.print("Id: "); Serial.println(id);
      Serial.print("Battery: "); Serial.println(batteryLow ? "Low" : "OK");
      Serial.print("Sent: "); Serial.println(manualSend ? "Manual" : "Auto");
      Serial.print("Channel: "); Serial.println(channel + 1);
      Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
      Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
      Serial.print("Unknown Data: "); Serial.println(unknownData, DEC);
      Serial.println();
      mySwitch.resetAvailable();
    }
  }
}
