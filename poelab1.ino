// sets variables and initial values
int led1 = 6;
// 8 and 12 don't support analog output
int led2 = 9;
int led3 = 10;
int led4 = 11;
int buttonPin = 2;

int allLeds[] = {led1,led2,led3,led4};
int numLeds = sizeof(allLeds)/sizeof(int);
int numStates = 4;

int buttonState = 0;
int lastButtonState = 0;
int lightState = 0;

//variables to set up for flashing and bouncing
long prevMillis = 0;
int state = HIGH;
int litLed = 1;
boolean bounceReverse = true;
//will translate 1-
int ledTranslate = led2 - 1;

double blinkScale = 0;
int lightMin = 20;

//initializes pins
void setup() {
  Serial.begin(9600);
  Serial.println("Set up!");
  pinMode(led1, OUTPUT);     
  pinMode(led2, OUTPUT); 
  pinMode(led3, OUTPUT); 
  pinMode(led4, OUTPUT);
  pinMode(buttonPin, INPUT);

}

// updates light state per light
void loop() {
  //reads in analog input from the infared sensor
  int sensorValue = analogRead(A0);
  //controls amount of light and speed
  blinkScale = sensorValue/25;

  // reads button signal
  buttonState = digitalRead(buttonPin);
  // if button state is the same as before, ignore
  if (buttonState != lastButtonState) {
    // if button pressed
    if (buttonState == HIGH) {
      // rotates through light states
      lightState = (lightState + 1) % numStates;
      //turns off all lights so next step can happen
      writeAllLights(LOW);
      // prints the light states for debugging
      Serial.println(lightState);
      // resetting the states for flash and bounce mode
      litLed = 1;
      state = HIGH;
      bounceReverse = true;
    }
  }
  // resets button state
  lastButtonState = buttonState;
  
    // executes all lights on function
  if (lightState == 1) {
    writeAllLights(HIGH);

  }
  // executes blink lights function
  if (lightState == 2) {
    blinkLights(2000/blinkScale);
  }
  //executes bounce light function
  if (lightState == 3) {
    bounceLights(2000/blinkScale);
  }
  
}
// blink light function
void blinkLights(double interval) {
  // keeps track of current time through millis function
  unsigned long currMillis = millis();
  // checks if some amount of time has passed since blink
   if (currMillis - prevMillis > interval) {
      //if so, changes the recorded previous time value
      prevMillis = currMillis;
      // checks state and changes it to the other one
      if (state == LOW)
        state = HIGH;
      else
        state = LOW;
      // does it for each led
      for (int i = 0; i < (sizeof(allLeds)/sizeof(int)); i++) {
        writeLight(allLeds[i], state);
      }
    }
}
// bounce lights function
void bounceLights(double interval) {
 // keeps track of current time through millis function
 unsigned long currMillis = millis(); 
 // checks if some amount of time has passed since blink
 if (currMillis - prevMillis > interval) {
   //if so, changes the recorded previous time value
   prevMillis = currMillis;
   //led1 is on pin 6 which must be handled separately
   if (litLed == 0) {
     writeLight(led1, LOW);
   }
   //otherwise turn off appropriate led
   else {
     writeLight(litLed + ledTranslate, LOW);
   }
   //if bounce reverse enacted
   if (bounceReverse) {
    //lit led number moves over by one negatively
     litLed--;
   }
   else {
    //lit led number moves over by one positively
     litLed++;
   }
   // change bounce reverse mode if it's at the end of the row
   if (litLed % (numLeds-1) == 0) {
     bounceReverse = !bounceReverse;
   }
   //handling led1 separately
   if (litLed == 0) {
     writeLight(led1, HIGH);
   }
   //otherwise light appropriate led
   else {
     writeLight(litLed + ledTranslate, HIGH);
   }
  }
}
// function that writes all the lights
void writeAllLights(int tempState) {
  //goes through each light
 for (int i = 0; i < (sizeof(allLeds)/sizeof(int)); i++) {
    //if the lights are all off, go through each one and turn off
    if (tempState == LOW) {
      digitalWrite(allLeds[i], tempState);
    }
    else {
      // turns lights on accordingly with the blinkscale
      analogWrite(allLeds[i], lightMin + (10 * blinkScale));
    }
  }
}
// writes only one light at a time, but same as above
void writeLight(int led, int tempState) {
  if (tempState == LOW) {
    digitalWrite(led, tempState);
  }
  else {
    analogWrite(led, lightMin + 10 * blinkScale);
  }
}

