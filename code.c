# ESP32-With-XY-MD02-Sensor-Temperature-Humidity-RS485-Modbus-Protocol

#include <ModbusMaster.h>

#define MAX485_DE 5   // Pin สำหรับ DE
#define MAX485_RE 18  // Pin สำหรับ RE

// Slave ID ของ XY-MD02
static uint8_t xymdSlaveAddr = 0x01;

// กำหนดพอร์ตสำหรับ RS485
HardwareSerial chat(1); // UART1 สำหรับ RS485

ModbusMaster node; // สร้างอ็อบเจ็กต์ ModbusMaster

float temperature = 0; // เก็บค่าอุณหภูมิ
float humidity = 0;    // เก็บค่าความชื้น

// ฟังก์ชันสำหรับควบคุมการส่งข้อมูล (DE/RE)
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

  // ตั้งค่าพิน
  pinMode(MAX485_RE, OUTPUT);
  pinMode(MAX485_DE, OUTPUT);
  digitalWrite(MAX485_RE, LOW);
  digitalWrite(MAX485_DE, LOW);

  // เริ่มต้นการสื่อสาร RS485
  chat.begin(9600, SERIAL_8N1, 16, 17); // TX=16, RX=17
  node.begin(xymdSlaveAddr, chat);

  // ตั้งค่าฟังก์ชันควบคุมการส่งข้อมูล
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
}

void loop() {
  uint8_t result;

  // อ่านค่าอุณหภูมิ (Temperature) จาก Register 0x0001
  result = node.readInputRegisters(0x0001, 1); // อ่าน 1 Word จาก Register 0x0001
  if (result == node.ku8MBSuccess) {
    temperature = node.getResponseBuffer(0) / 100.0; // แปลงค่าอุณหภูมิ
    Serial.print("Temperature: ");
    Serial.print(String(temperature, 1)); // แสดงผล 1 ตำแหน่งทศนิยม
    Serial.println(" °C");
  } else {
    Serial.print("Failed to read Temperature. Error: ");
    Serial.println(result);
  }

  // อ่านค่าความชื้น (Humidity) จาก Register 0x0002
  result = node.readInputRegisters(0x0002, 1); // อ่าน 1 Word จาก Register 0x0002
  if (result == node.ku8MBSuccess) {
    humidity = node.getResponseBuffer(0) / 100.0; // แปลงค่าความชื้น
    Serial.print("Humidity: ");
    Serial.print(String(humidity, 1)); // แสดงผล 1 ตำแหน่งทศนิยม
    Serial.println(" %");
  } else {
    Serial.print("Failed to read Humidity. Error: ");
    Serial.println(result);
  }

  delay(2000); // หน่วงเวลา 2 วินาที
}
