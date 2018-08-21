
#include "Wire.h"
#include "PN532_I2C.h"
#include "emulatetag.h"
#include "NdefMessage.h"

PN532_I2C pn532i2c(Wire);
EmulateTag nfc(pn532i2c);

uint8_t ndefBuf[250];
NdefMessage message;
int messageSize;

uint8_t uid[3] = { 0x12, 0x34, 0x56 };

void setup()
{
  Serial.begin(115200);
  Serial.println("------- Emulate Tag --------");
  
  message = NdefMessage();
  message.addUriRecord("https://github.com/will127534/HITCON-Badge-2018");
  message.addUriRecord("hitcon://pair?v=18&a=808c2257d778e5f1340d9325116f5a7273b33f5d&k=ce1f843391af38a1a93cae8c6439754c&s=1f7fd22b-bbeb-dd95-98ac-b9b75e971974&c=256f3a074babbb0940dc1c2751eccf05e12df5c12737e1d8");
  messageSize = message.getEncodedSize();
  if (messageSize > sizeof(ndefBuf)) {
      Serial.println("ndefBuf is too small");
      while (1) { }
  }
  
  Serial.print("Ndef encoded message size: ");
  Serial.println(messageSize);

  message.encode(ndefBuf);
  
  // comment out this command for no ndef message
  nfc.setNdefFile(ndefBuf, messageSize);
  
  // uid must be 3 bytes!
  nfc.setUid(uid);
  
  nfc.init();
}

void loop(){
    nfc.emulate();
    nfc.init();
}