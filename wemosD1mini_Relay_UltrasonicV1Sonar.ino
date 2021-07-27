#include <ESP8266WiFi.h>

RF_MODE(RF_DISABLED);

//static const uint8_t D9   = 3;
//static const uint8_t D10  = 1;

#define RelayPin  D1
#define SonarPin  D2

#define GreenLED  D5
#define BlueLED   D6
#define RedLED    D7

void setup() {
  WiFi.disconnect(true); delay(1);
  WiFi.mode(WIFI_OFF); delay(1);
  WiFi.forceSleepBegin(); delay(1);
  
//  Serial.begin(115200);
//  delay(10);
  pinMode(RelayPin, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(RedLED, OUTPUT);
  pinMode(BlueLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  long duration, cm;
  int presents = 0;

  //  sonar code
  for (int i = 0; i < 3; i++)
  {
    pinMode(SonarPin, OUTPUT);
    digitalWrite(SonarPin, LOW);
    delayMicroseconds(2);
    digitalWrite(SonarPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(SonarPin, LOW);
  
    // The same pin is used to read the signal from the PING))): a HIGH pulse
    // whose duration is the time (in microseconds) from the sending of the ping
    // to the reception of its echo off of an object.
    pinMode(SonarPin, INPUT);
    duration = pulseIn(SonarPin, HIGH);
  
    // convert the time into a distance
    cm = microsecondsToCentimeters(duration);
    if (cm < 25 && cm > 0)
    {
      presents++;
    }
    delay(50);
  }

//  Serial.print(cm);
//  Serial.print("cm");
//  Serial.println();

  if (presents > 0)
  {
//    Serial.println("Object detected: turn the heater on");
    //  relay code
//    digitalWrite(RedLED, HIGH);
    digitalWrite(RelayPin, HIGH); // Turn the Heater on by making the Relay voltage HIGH
    delay(10000);                 // Heat for ten seconds
    digitalWrite(RelayPin, LOW);  // Turn the Heater off by making the Relay voltage LOW
//    digitalWrite(RedLED, LOW);

//    Serial.println("deep sleep");
    ESP.deepSleep(50000000);
  }
  else
  {
//    Serial.println("Object not found");

//    digitalWrite(BlueLED, HIGH);
//    delay(1000);
//    digitalWrite(BlueLED, LOW);

//    Serial.println("deep deep sleep");
    ESP.deepSleep(60000000);
  }
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
