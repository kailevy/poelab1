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

long prevMillis = 0;
int state = HIGH;

int litLed = 1;
int ledTranslate = led2 - 1;
boolean bounceReverse = true;

double blinkScale = 0;
int lightMin = 50;

void setup() {
  Serial.begin(9600);
  Serial.println("Set up!");
  pinMode(led1, OUTPUT);     
  pinMode(led2, OUTPUT); 
  pinMode(led3, OUTPUT); 
  pinMode(led4, OUTPUT);
  pinMode(buttonPin, INPUT);

}

void loop() {
  int sensorValue = analogRead(A0);
  blinkScale = sensorValue/25;
  
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      lightState = (lightState + 1) % numStates;
      writeAllLights(LOW);
      
      Serial.println(lightState);
      // reset variables
      litLed = 1;
      state = HIGH;
      bounceReverse = true;
    }
  }
  
  lastButtonState = buttonState;
  
    
  if (lightState == 1) {
    writeAllLights(HIGH);

  }
  
  if (lightState == 2) {
    blinkLights(2000/blinkScale);
  }
  
  if (lightState == 3) {
    bounceLights(2000/blinkScale);
  }
  
}

void blinkLights(double interval) {
  unsigned long currMillis = millis();
  
   if (currMillis - prevMillis > interval) {
      prevMillis = currMillis;
      if (state == LOW)
        state = HIGH;
      else
        state = LOW;
      for (int i = 0; i < (sizeof(allLeds)/sizeof(int)); i++) {
        writeLight(allLeds[i], state);
      }
    }
}

void bounceLights(double interval) {
 unsigned long currMillis = millis(); 
 
 if (currMillis - prevMillis > interval) {
   prevMillis = currMillis;
   if (litLed == 0) {
     writeLight(led1, LOW);
   }
   else {
     writeLight(litLed + ledTranslate, LOW);
   }
   if (bounceReverse) {
     litLed--;
   }
   else {
     litLed++;
   }
   if (litLed % (numLeds-1) == 0) {
     bounceReverse = !bounceReverse;
   }
   if (litLed == 0) {
     writeLight(led1, HIGH);
   }
   else {
     writeLight(litLed + ledTranslate, HIGH);
   }
 }
}

void writeAllLights(int tempState) {
 for (int i = 0; i < (sizeof(allLeds)/sizeof(int)); i++) {
    if (tempState == LOW) {
      digitalWrite(allLeds[i], tempState);
    }
    else {
      analogWrite(allLeds[i], lightMin + (10 * blinkScale));
    }
  }
}

void writeLight(int led, int tempState) {
  if (tempState == LOW) {
    digitalWrite(led, tempState);
  }
  else {
    analogWrite(led, lightMin + 10 * blinkScale);
  }
}

