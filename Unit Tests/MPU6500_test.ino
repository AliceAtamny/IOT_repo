#include <Wire.h>

#define MPU_ADDR 0x68

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  // Check sensor identity
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x75);

  if (Wire.endTransmission(false) != 0 ||
      Wire.requestFrom(MPU_ADDR, 1) != 1) {
    Serial.println("Sensor not found!");
    while (true) delay(100);
  }

  int id = Wire.read();

  if (id != 0x70) {
    Serial.print("Unexpected sensor ID: ");
    Serial.println(id, HEX);
    while (true) delay(100);
  }

  // Wake up sensor
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0x00);

  if (Wire.endTransmission() != 0) {
    Serial.println("Initialization failed");
    while (true) delay(100);
  }

  delay(100);
  Serial.println("MPU6500 FOUND");
}

int16_t readValue() {
  uint8_t high = Wire.read();
  uint8_t low = Wire.read();

  return (int16_t)((high << 8) | low);
}

void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);

  if (Wire.endTransmission(false) != 0 ||
      Wire.requestFrom(MPU_ADDR, 14) != 14) {
    Serial.println("Read failed");
    delay(500);
    return;
  }

  int16_t ax = readValue();
  int16_t ay = readValue();
  int16_t az = readValue();

  readValue(); // Skip temperature

  int16_t gx = readValue();
  int16_t gy = readValue();
  int16_t gz = readValue();

  Serial.printf(
    "Accel: X=%d Y=%d Z=%d\n",
    ax, ay, az
  );

  Serial.printf(
    "Gyro: X=%d Y=%d Z=%d\n\n",
    gx, gy, gz
  );

  delay(200);
}