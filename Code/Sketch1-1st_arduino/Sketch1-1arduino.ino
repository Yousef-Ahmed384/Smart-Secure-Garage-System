//////////////////////////////////////////////////////////////////// SKETCH 1 ////////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------> SKETCH FOR ARDUINO 1 (ENTER ARDUINO) <-------------------------------------------------------//
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); //1 ->Address by Hexa 2->cols 3->rows

// Pin Definitions
#define IR_ENTRY    7
#define IR_parking1 3
#define IR_parking2 A1
#define BUZZER_PIN  6
#define SERVO_PIN  5
#define RGB_redD   2
#define OPEN_POS   180
#define CLOSE_POS 90
#define darkThreShold  950
const int ldrPins[] = {A3, A2};
const int RGB_RED[] = {2, 10, 12};
const int RGB_GREEN[] = {8, 9, 11, 13};


bool entryDetected = false;
int freeSlots = 7; // Total parking slots
Servo gateServo;
char receivedData = '\0'; //Still not Work : (



// Functions Initilizations declaration
void startSerial();
void startPins();
void startServo();
void startLCD();
void showWelcomeMessage();

// Operational Functions declaration
void checkEntry();
void CheckParking();
bool ReallyCheck(int IR);
void CheckLdrParking();
void controlGate();
void updateDisplay();
void soundBuzzer(int numOfBeeps, int time);
void openGate(int closePos, int openPos, int tim);
void closeGate(int closePos, int openPos, int tim);
void showParkingFullMessage();
void FlameSensorRecievedData();
bool bluetoothSerial();

void setup() {
  startSerial();
  startLCD();
  startPins();
  startServo();
  showWelcomeMessage();
}

void loop() {      
  checkEntry();
  controlGate();
  CheckParking();
  CheckLdrParking();
  FlameSensorRecievedData();
  updateDisplay();
}

// Initialization Functions
void startSerial() {
  Serial.begin(9600);   // For debugging
  Wire.begin();
}

void startLCD() {
  lcd.init();
  lcd.backlight();
}

void startPins() {
 pinMode(IR_ENTRY, INPUT);
 pinMode(IR_parking1, INPUT);
 pinMode(IR_parking2, INPUT);

  for (int i = 0; i < 2; i++){
	  pinMode(ldrPins[i], INPUT);
  }

  for (int i = 0; i < 3; i++){
    pinMode(RGB_RED[i], OUTPUT);
    digitalWrite(RGB_RED[i], LOW);
  }
  for (int i = 0; i < 4; i++){
	  pinMode(RGB_GREEN[i], OUTPUT);
    digitalWrite(RGB_GREEN[i], HIGH);
  }
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

void startServo() {
  gateServo.attach(SERVO_PIN);
  gateServo.write(CLOSE_POS);
}


void showWelcomeMessage() {
  lcd.setCursor(0, 0);
  lcd.print("  Welcome to  ");
  lcd.setCursor(0, 1);
  lcd.print("Smart Parking");
  delay(2000);
  lcd.clear();
}

// Operational Functions
bool ReallyCheck(int IR){
  if (digitalRead(IR) == LOW)
  {
    delay(25);
    if (digitalRead(IR) == LOW)
      return true;
    else
      return false;
  }
  else
    return false;
}

void checkEntry() {
  if (ReallyCheck(IR_ENTRY) && !entryDetected) {
    entryDetected = true;
    if (freeSlots > 0) {
      // Show message first
      lcd.setCursor(0, 0);
      lcd.print("    Welcome   ");
      lcd.setCursor(0, 1);
      lcd.print("You can Enter :)");
      // Open gate and update state
      openGate(CLOSE_POS, OPEN_POS, 15);
      freeSlots--;
      Serial.println("Vehicle entered. Slots left: " + String(freeSlots));
      updateDisplay();
      // Keep message visible while gate is open
      delay(300);  // Give time to see message
      controlGate();
      
      // Don't close gate here - let controlGate() handle it
    }
    else {
      entryDetected = false;
      showParkingFullMessage();
      // No state change since entry wasn't allowed
    }
  }
}

void CheckParking(){
  if(ReallyCheck(IR_parking1)) { 
    Serial.println("There is a car parking in 1st place");
    digitalWrite(RGB_RED[2], HIGH);
    digitalWrite(RGB_GREEN[2], LOW);
    
  }
  else {
    digitalWrite(RGB_RED[2], LOW);
    digitalWrite(RGB_GREEN[2], HIGH);
  }
  int IR_Park2 = analogRead(IR_parking2);
  if(IR_Park2 < 100) { 
    Serial.println("There is a car parking in the 2nd place");
    digitalWrite(RGB_GREEN[3], LOW);
    
  }
  else {
    digitalWrite(RGB_GREEN[3], HIGH);
  }
  
}
void CheckLdrParking() {
  int ldr1 = analogRead(ldrPins[0]);
  int ldr2 = analogRead(ldrPins[1]);
  
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
}



void FlameSensorRecievedData(){
  if(Serial.available()){
    String recievedFlame = Serial.readStringUntil('\n');
    if (recievedFlame.equals("FIRE = 1")){
      gateServo.write(CLOSE_POS);
      soundBuzzer(3, 200);
      lcd.setCursor(0, 0);
      lcd.print("There is a FIRE");
      lcd.setCursor(0, 1);
      lcd.print("Don't Enter");
    }
    else if (recievedFlame.equals("FIRE = 0")){
      controlGate();
      CheckLdrParking();
      lcd.clear();
      updateDisplay();
      
    }
  }
}

void controlGate() {
  if (entryDetected) {
    delay (3000);     // Allow vehicle to pass
    closeGate(CLOSE_POS, OPEN_POS, 15);
    entryDetected = false;
    Serial.println("Gate closed");
    lcd.clear(); // Debugg
  }
}

void updateDisplay() {
    lcd.setCursor(0, 0);
    lcd.print("Available Slots:");
    lcd.setCursor(0, 1);
    lcd.print("      ");
    if(Serial.available()){

    }
    lcd.print(freeSlots);
  
}

void showParkingFullMessage() {
  lcd.setCursor(0, 0);
  lcd.print("  PARKING FULL  ");
  lcd.setCursor(0, 1);
  lcd.print("  NO FREE SLOTS  ");
  soundBuzzer(3, 300);
  delay(1000);
  lcd.clear();

}

void openGate(int closePos, int openPos, int tim) {
  gateServo.attach(SERVO_PIN);
  for (int i = closePos; i <= openPos; i++) {
    gateServo.write(i);
    delay(tim);
  }
  soundBuzzer(1, 100);
}

void closeGate(int closePos, int openPos, int tim) {
  for (int i = openPos; i >= closePos; i--) {
    gateServo.write(i);
    delay(tim);
  }

  soundBuzzer(2, 200);
  gateServo.detach();
}



void soundBuzzer(int numOfBeeps, int time) {
  for (int i = 0; i < numOfBeeps; i++)
  {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(time);
    digitalWrite(BUZZER_PIN, LOW);
    delay(time);

  }
  
}

// bluetooht Function
bool bluetoothSerial() {
  if (Serial.available()) {
    Serial.println("Enter the first Condition");
    receivedData = Serial.read();
    Serial.println(receivedData);
    //receivedData.trim();
    if (receivedData == 'c') {
      Serial.println("connected");
      receivedData = '\0'; 
      return true;
    }
      
  }
  receivedData = '\0';
  return false;
}
