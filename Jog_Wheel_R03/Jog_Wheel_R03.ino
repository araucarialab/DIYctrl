#include <Rotary.h>
#include <Keyboard.h>

Rotary rotary = Rotary(3, 4);

int counter = 0;

void setup() {
  pinMode(5, INPUT_PULLUP); 
}

void loop() {
  unsigned char result = rotary.process();
  
  if(digitalRead(5)==LOW)
  {
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('k');
    delay(120);
    Keyboard.releaseAll();
    delay(200);
  }

  if (result == DIR_CW) {
    counter++;
    Keyboard.write(KEY_RIGHT_ARROW);
  } 
  
  else if (result == DIR_CCW) {
    counter--;
    Keyboard.write(KEY_LEFT_ARROW);
  }
}
