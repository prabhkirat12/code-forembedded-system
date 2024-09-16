#include <math.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
 
int soil = A0;        // Pin connected to soil moisture sensor
int relay = 3;        // Pin connected to relay
int ntc = A2;         // Pin connected to temperature sensor
int ledPin = 8;       // Pin connected to LED
 
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize the I2C LCD
 
void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(soil, INPUT);
  pinMode(relay, OUTPUT);
  pinMode(ntc, INPUT);
  pinMode(ledPin, OUTPUT); // Set LED pin as OUTPUT
  digitalWrite(relay, HIGH); // Initially turn off the relay
  digitalWrite(ledPin, LOW); // Initially turn off the LED
  Serial.begin(9600);
 
  lcd.setCursor(0, 0);
  lcd.print("Moisture:");
  lcd.setCursor(0, 1);
  lcd.print("Temp:");
}
 
void loop() {
  // Read soil moisture sensor
  int soilData = analogRead(soil);
  Serial.print("Soil DATA: ");
  Serial.println(soilData);
 
  // Control relay based on soil moisture
  if (soilData < 200) {
    digitalWrite(relay, LOW);  // Turn ON the relay when soil is dry
  } else {
    digitalWrite(relay, HIGH); // Turn OFF the relay when soil is moist
  }
 
  // Read temperature from analog sensor using the Thermistor function
  int raw_value = analogRead(ntc);
  double temperature = Thermistor(raw_value);
 
  // Output of the measured temperature
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
 
  // Update LCD display
  lcd.setCursor(10, 0);
  lcd.print("       "); // Clear the previous value
  lcd.setCursor(10, 0);
  lcd.print(soilData);
 
  lcd.setCursor(6, 1);
  lcd.print("       "); // Clear the previous value
  lcd.setCursor(6, 1);
  lcd.print(temperature, 1); // Display temperature with 1 decimal place
 
  // Control the LED based on temperature
  if (temperature > 20) {
    digitalWrite(ledPin, HIGH); // Turn ON the LED
    delay(500); // Wait for 0.5 seconds
    digitalWrite(ledPin, LOW);  // Turn OFF the LED
    delay(500); // Wait for 0.5 seconds
  } else {
    digitalWrite(ledPin, LOW);  // Ensure the LED is OFF if temperature is <= 20°C
  }
 
  delay(1000); // Wait 1 seconds before next reading
}
 
// Function to calculate temperature from thermistor
double Thermistor(int RawADC) {
  double resistance = 10000.0 * ((1023.0 / RawADC) - 1.0);
  double tempK = 0.955 / (0.001129148 + (0.000234125 * log(resistance))
   + (0.0000000876741 * pow(log(resistance), 3)));
  double tempC = tempK - 273.15; // Convert Kelvin to Celsius
  return tempC;
}
 