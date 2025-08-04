// Blynk configuration
#define BLYNK_TEMPLATE_ID "TMPL3NkxJm3ng"
#define BLYNK_TEMPLATE_NAME "FINAL PROJECT"
#define BLYNK_AUTH_TOKEN "rs8_TPtLncu-jVIA7jjrADz6wEXeLvYQ"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <DHT.h>

// Define the pulse sensor settings
const int pulsePin = 32; // the pulse sensor pin
const int ledPin = 33; // the LED pin
int pulseValue; // the pulse sensor value
int bpm; // the heart rate in beats per minute
char ssid[] = "Anime";
char pass[] = "animesh3";
BlynkTimer timer;

// I2C LCD settings
LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the I2C LCD address and dimensions (16x2)

// Ultrasonic Sensor Configuration
#define TRIGGER_PIN 2  // GPIO2
#define ECHO_PIN 4     // GPIO4
#define MAX_DISTANCE 500 // Maximum distance we want to measure (in centimeters)

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

const int pirPin = 13; // PIR sensor output connected to GPIO 13
bool motionDetected = false;

// DHT sensor configuration
#define DHTPIN 27 // Connect Out pin to D2 in NODE MCU
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Initialize the I2C LCD
  lcd.init();
  lcd.backlight();

  // Connect to WiFi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  while (!Blynk.connected()) {
    Serial.println("Connecting to Blynk...");
    delay(1000);
  }
  Serial.println("Connected to Blynk");

  // Set up the pulse sensor
  pinMode(pulsePin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Ultrasonic Sensor Initialization
  lcd.setCursor(0, 0);
  lcd.print("Urine level:"); // Set the cursor to the third row

  dht.begin();
  timer.setInterval(1000L, sendSensor);
}

void loop() {
  // Simulate a pulse rate in the desired range (e.g., between 65 and 100 BPM)
  bpm = random(65, 100);  // Generates a random BPM value between 65 and 100

  // Display the simulated BPM value and take actions based on it
  Serial.print("Pulse rate: ");
  Serial.print(bpm);
  Serial.println(" BPM");
  lcd.setCursor(5, 2);
  lcd.print("Pulse rate: ");
  lcd.print(bpm);
  delay(1000);
  Blynk.virtualWrite(V0, bpm);

  int bpm = random(65, 100);{
    // Take action for a low simulated pulse rate
    // You can add your specific actions here.
  
    // Take action for a high simulated pulse rate
    // You can add your specific actions here.
  }

  // Ultrasonic Sensor Measurement
  unsigned int distance = sonar.ping_cm();
  Blynk.virtualWrite(V1, distance);
  lcd.setCursor(12, 0);
  lcd.print(distance);
  lcd.print(" cm      ");

  // PIR Motion Detection
  int motionState = digitalRead(pirPin);
  Blynk.virtualWrite(V2, motionDetected);

  if (motionState == HIGH) {
    if (!motionDetected) {
      motionDetected = true;
      lcd.setCursor(0, 1);
      lcd.print("Motion Detected!");
    }
  } else {
    if (motionDetected) {
      motionDetected = false;
      lcd.setCursor(0, 1);
      lcd.print("Motion Stopped!");
    }
  }

  // Read the pulse sensor value
  pulseValue = analogRead(pulsePin);
    {
    Blynk.virtualWrite(V0, bpm);
  }

  // Run the Blynk loop
  Blynk.run();
  timer.run();
}

void sendSensor() {
  float t = dht.readTemperature();

  if (isnan(t)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V3, t);
  Serial.print("Temperature : ");
  Serial.print(t);
  Serial.println(" C");

  // Display temperature on the LCD
  lcd.setCursor(0, 3);
  lcd.print("Temperature: ");
  lcd.print(t);
  lcd.print("*C");
}