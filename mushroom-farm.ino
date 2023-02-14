
#include <DHT.h>

#define DHT_PIN 7
#define RELAY_PIN1 8
#define RELAY_PIN2 9
#define RELAY_PIN3 10
#define RELAY_PIN4 11
#define DHTTYPE DHT22

DHT dht(DHT_PIN, DHT22);

float temp;
float hum;
bool humidifying;
bool heating;
bool prevHeat;
int cooldown = 0;

void setup()
{
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  pinMode(RELAY_PIN4, OUTPUT);

  Serial.begin(9600); // Initiate serial monitor
  dht.begin();        // Initiate DHT sensor
}

void loop()
{
  delay(2000);

  temp = dht.readTemperature(false); // Read temperature of DHT and store it in to variable (temp). FALSE reads in celsius, leave empty for farenheit
  hum = dht.readHumidity();          // Read humidity of DHTand store it in variable (hum).

  Serial.print("Temperature: "); // Print text "Temperature: " in to serial monitor
  Serial.print(temp);            // Print variable (temperature value) in to serial port.
  Serial.print("\t");            // tab
  Serial.print("Humidity: ");    // Print text "Humidity: " in to serial monitor
  Serial.print(hum);             // Print variable (temperature value) in to serial port. ln for line break
  Serial.print("\t");            // tab
  Serial.print("Humidifying: "); // Humidifying
  Serial.print(humidifying);     // on/off
  Serial.print("\t");            // tab
  Serial.print("Heating: ");     // Heating
  Serial.print(heating);         // on/off
  Serial.print("\t");            // tab
  Serial.print(prevHeat);        // Heating cycle
  Serial.print("\t");            // tab
  Serial.print(cooldown);        // Cooldown counter
  Serial.println();

  // Humidifying logic
  int upperHum = 85;
  int lowerHum = 80;

  if (isnan(hum))
  {
    humidifying = true;
  }
  else if (hum >= upperHum && humidifying)
  {
    humidifying = false;
  }
  else if (hum <= lowerHum && !humidifying)
  {
    humidifying = true;
  }

  // Heating logic
  int upperHeat = 16;
  int lowerHeat = 15;
  if (isnan(temp))
  {
    heating = false;
  }
  else if (temp <= lowerHeat && !heating)
  {
    heating = true;
  }
  else if (temp >= upperHeat && heating)
  {
    heating = false;
  }

  // Write to outputs
  if (humidifying || heating || cooldown > 0)
  {
    // We only have one fan currently, so we put this on too for heating
    digitalWrite(RELAY_PIN1, HIGH); // Humidity ON
  }
  else
  {
    digitalWrite(RELAY_PIN1, LOW); // Humidity OFF
  }
  if (heating && !prevHeat)
  {
    prevHeat = true; // Heating can only be on max 2sec, to prevent everything from melting
    cooldown = 5;
    digitalWrite(RELAY_PIN2, LOW); // Heating ON
  }
  else
  {
    if (cooldown > 0)
      cooldown--;
    prevHeat = false;
    digitalWrite(RELAY_PIN2, HIGH); // Heating OFF
  }
}