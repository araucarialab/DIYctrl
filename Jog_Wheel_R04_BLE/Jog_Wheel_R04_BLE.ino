#include <Rotary.h>
#include <BleKeyboard.h>

Rotary rotary = Rotary(27, 26);

BleKeyboard bleKeyboard;

int counter = 0;

void setup() {
//  pinMode(14, INPUT_PULLUP); 
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
}

void loop() {
  
unsigned char result = rotary.process();  

  if(bleKeyboard.isConnected()) {
        if(digitalRead(14)==LOW)
    {
    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('k');
    delay(120);
    bleKeyboard.releaseAll();
    delay(200);
  }

  

  if (result == DIR_CW) {
    counter++;
    bleKeyboard.write(KEY_DOWN_ARROW);
  } 
  
  else if (result == DIR_CCW) {
    counter--;
    bleKeyboard.write(KEY_UP_ARROW);
    }
  }


//  Serial.println("Waiting 5 seconds...");
//  delay(5000);
}

void turning() { 
    if(digitalRead(14)==LOW)
  {
    bleKeyboard.press(KEY_LEFT_GUI);
    bleKeyboard.press('k');
    delay(120);
    bleKeyboard.releaseAll();
    delay(200);
  }

  unsigned char result = rotary.process();

  if (result == DIR_CW) {
    counter++;
    bleKeyboard.write(KEY_RIGHT_ARROW);
  } 
  
  else if (result == DIR_CCW) {
    counter--;
    bleKeyboard.write(KEY_LEFT_ARROW);
  }

}
