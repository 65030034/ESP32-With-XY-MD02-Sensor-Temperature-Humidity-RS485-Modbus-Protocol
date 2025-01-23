

#include <ModbusMaster.h>

#define MAX485_DE 5   
#define MAX485_RE 18  

static uint8_t xymdSlaveAddr = 0x01;

HardwareSerial chat(1); 

ModbusMaster node; 

float temperature = 0;
float humidity = 0;   

void preTransmission() {
  digitalWrite(MAX485_RE, HIGH);
  digitalWrite(MAX485_DE, HIGH);
  delay(1);
}

void postTransmission() {
  delay(3);
  digitalWrite(MAX485_RE, LOW);
  digitalWrite(MAX485_DE, LOW);
}

void setup() {
  Serial.begin(115200); // Debugging
  Serial.println("XY-MD02 Sensor via Modbus RS485");

  pinMode(MAX485_RE, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_RE, LOW);
  digitalWrite(MAX485_DE, LOW);

  chat.begin(9600, SERIAL_8N1, 16, 17); // TX=16, RX=17
  node.begin(xymdSlaveAddr, chat);

  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop() {
  uint8_t result;

  result = node.readInputRegisters(0x0001, 1); 
  if (result == node.ku8MBSuccess) {
    temperature = node.getResponseBuffer(0) / 100.0;
    Serial.print("Temperature: ");
    Serial.print(String(temperature, 1)); 
    Serial.println(" °C");
  } else {
    Serial.print("Failed to read Temperature. Error: ");
    Serial.println(result);
  }

  result = node.readInputRegisters(0x0002, 1); 
  if (result == node.ku8MBSuccess) {
    humidity = node.getResponseBuffer(0) / 100.0; 
    Serial.print("Humidity: ");
    Serial.print(String(humidity, 1)); 
    Serial.println(" %");
  } else {
    Serial.print("Failed to read Humidity. Error: ");
    Serial.println(result);
  }

  delay(2000); 
}
