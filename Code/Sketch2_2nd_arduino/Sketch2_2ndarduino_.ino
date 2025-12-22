//---------------------------------------->>>>>>>> SKETCH 2 <<<<<<<<<----------------------------------------------------

///////////////////////////////////////////  SKETCH FOR ARDUINO 2 (EXIT ARDUINO)  //////////////////////////////////////////////////
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MFRC522.h>
#include <SPI.h>
#include <Servo.h>

// --> Pins Declaration <--//
#define flameSensor_pin A0
#define button_pin      6
#define IRSensor_pin   A5
#define servo2_pin      A4
#define openPos         180
#define closePos        90
#define darkThreShold  1000
#define RST_IN_CARD     9  // RFID
#define SS_IN_CARD      10  // RFID
#define buzzer2_pin     2
const int ldrPins[] = {A3, A2, A1};
const int RGB_RED[] = {3, 6};
const int RGB_GREEN[] = {4, 5, 7};


//--> variables declaration <--//
//SoftwareSerial arduinocom(0, 45);  // to make the data transfer <--
MFRC522 rfid(SS_IN_CARD, RST_IN_CARD);
byte nuidReader[4];
Servo exitGateServo;
int buttonState = 1;
bool exitDetected = false;
int freeSlots = 7; // as when the car exit the slots should increase  dec. in entering and inc. in exiting
//--> Functions declaration <--//

// > setup functions
void StartSerial();
void StartRFID();
void StartPins();
void StartServo();

// > operational functions
void FlameDetector(int flameThreShold, int flameVal);
void ButtonPress(int stateOfTheButton);
void CheckCarExistance();
bool ReallyCheck(int IR);
void ControlGate();
void CheckExit();
void soundBuzzer();
void OpenGate(int cp, int op, int tim);
void CloseGate(int cp, int op, int tim);
bool scanningTheCard();
void resetRFID();



void setup() {

  StartSerial();
  StartRFID();
  StartPins();
  StartServo();
}

void loop() {
  int flameValue = analogRead(flameSensor_pin);
  buttonState = digitalRead(button_pin);
  FlameDetector(50, flameValue);
  ButtonPress(buttonState);
  CheckCarExistance();
  CheckExit();
  ControlGate();
  delay(50);
}

// --> initialized functions definition <-- //
void StartSerial() {
  Serial.begin(9600);  // For debugging & transfering the data
  Wire.begin();
}


void StartPins() {
  // > input pins
  pinMode(flameSensor_pin, INPUT);
  pinMode(IRSensor_pin, INPUT);
  
  for (int i = 0; i < 3; i++){
	  pinMode(ldrPins[i], INPUT);
  }

  for (int i = 0; i < 3; i++){
	  pinMode(RGB_GREEN[i], OUTPUT);
    //digitalWrite(RGB_GREEN[i], HIGH);
  }
  for (int i = 0; i < 2; i++){
    pinMode(RGB_RED[i], OUTPUT);
    //digitalWrite(RGB_RED[i], LOW);
  }
  CheckCarExistance()  // if there is a car parking while opening the garage of connecting it

  pinMode(button_pin, INPUT);
  pinMode(buzzer2_pin, OUTPUT);

  digitalWrite(buzzer2_pin, LOW);

}

void StartServo() {
  exitGateServo.attach(servo2_pin);
  exitGateServo.write(closePos);
}

void StartRFID() {
  SPI.begin();
  rfid.PCD_Init();
  Serial.println("The RFID is ready to scan...");
}

// --> sensor reading operations <-- //
bool ReallyCheck(int IR) {
  const int ThreShold = 250;
  if (analogRead(IR) < ThreShold) {
    delay(25);
    if (analogRead(IR) < ThreShold)
      return true;
    else
      return false;
  } else
    return false;
}

void CheckExit() {
  if (ReallyCheck(IRSensor_pin) && !exitDetected) {
    Serial.println("IR Exit Triggered");
    //if (freeSlots < 7)
      if(scanningTheCard()){
        exitDetected = true;
        OpenGate(closePos, openPos, 15);
        Serial.println("The servo get the command");
        Serial.print("A car was left");
        freeSlots = min(7, freeSlots + 1);
        Serial.println("Vehicle exited. Slots left: " + String(freeSlots));
        delay(300);
        ControlGate();
      }
      
    
  }
}

//------------------------------------------------- >> operational function definition << --------------------------------------------------------------//
// --> open & close gate operations <-- //
void OpenGate(int cp, int op, int tim) {
  //exitGateServo.write(servo2_pin);
  for (int i = cp; i <= op; i++) {
    exitGateServo.write(i);
    delay(tim);
  }
    soundBuzzer(2, 200);
}

void CloseGate(int cp, int op, int tim) {
  for (int i = op; i>= cp; i--) {
    exitGateServo.write(i);
    delay(tim);
  }
    soundBuzzer(1, 200);
    //exitGateServo.detach();
}

void ControlGate() {
  if (exitDetected) {
    delay (3000);     // Allow vehicle to pass
    CloseGate(closePos, openPos, 15);
    exitDetected = false;
    Serial.println("Gate closed");
  }
}
// --> ldr function <-- //
void CheckCarExistance() {
  int ldr1 = analogRead(ldrPins[0]);
  int ldr2 = analogRead(ldrPins[1]);
  int ldr3 = analogRead(ldrPins[2]);
  
  // For slot 1
  if(ldr1 > darkThreShold){ 
    Serial.println("Car in1");
    digitalWrite(RGB_GREEN[0], LOW);
    digitalWrite(RGB_RED[0], HIGH);   
  } else { // No car
    digitalWrite(RGB_GREEN[0], HIGH);  
    digitalWrite(RGB_RED[0], LOW);   
  }
  
  if(ldr2 > darkThreShold){ 
    Serial.println("Car in 2");
    digitalWrite(RGB_GREEN[1], LOW); 
    digitalWrite(RGB_RED[1], HIGH);    
  } else { 
    digitalWrite(RGB_GREEN[1], HIGH);  
    digitalWrite(RGB_RED[1], LOW);   
  }
  if(ldr3 > darkThreShold){ 
    Serial.println("Car in 3");
    digitalWrite(RGB_GREEN[2], LOW); 
  } else { 
    digitalWrite(RGB_GREEN[2], HIGH);  
  }
}

// --> flame function <-- //
void FlameDetector(int flameThreShold, int flameVal) {
  static bool isFlamed = false;
  if (flameVal >= flameThreShold) {
    if (!isFlamed) { // First detection
      Serial.println("FIRE = 1");
      isFlamed = true;
      exitGateServo.write(openPos);
      OpenGate(closePos, openPos, 20);
      soundBuzzer(8, 150);
    }
  }
  else {
    if (isFlamed) {           // Fire just extinguished
      Serial.println("FIRE = 0");
      isFlamed = false;
      digitalWrite(buzzer2_pin, LOW);
      ControlGate();
      CloseGate(closePos, openPos, 15);
    }
  }
}
// -->  Button operational function def <-- //
void ButtonPress(int buttonState) {
  static unsigned long lastPressTime = 0;
  const unsigned long debounceDelay = 200;
  
  if (buttonState == LOW && (millis() - lastPressTime) > debounceDelay) {
    lastPressTime = millis();
    // Emergency close gate
    if(exitDetected) {
      CloseGate(closePos, openPos, 15);
      exitDetected = false;
      soundBuzzer(3, 200);
    }
  }
}
// --> Buzzer to open the gate and closing it while exiting <-- //
void soundBuzzer(int numOfBeeps, int time) {
  for (int i = 0; i < numOfBeeps; i++)
  {
    digitalWrite(buzzer2_pin, HIGH);
    delay(time);
    digitalWrite(buzzer2_pin, LOW);
    delay(time);

  }
  
}
// --> RFID operational functions definition <-- //
bool scanningTheCard() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return false;
  }
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI && piccType != MFRC522::PICC_TYPE_MIFARE_1K && piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    resetRFID();
    return false;
  }
  for (byte i = 0; i < 4; i++) {
    nuidReader[i] = rfid.uid.uidByte[i];
  }
  if (nuidReader[0] == 0x76 && nuidReader[1] == 0xCF && nuidReader[2] == 0xC8 && nuidReader[3] == 0xE9) {
    resetRFID();
    delay(500);  // Prevent multiple reads
    return true;
  } else {
    resetRFID();
    return false;
  }
}

void resetRFID() {
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
