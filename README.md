# Elapsed Time Counter for ESP-01 (ESP8266)
This is a simple elapsed time counter designed to count seconds when the power is on.

# Features
- Counts elapsed seconds since the first power on.
- Creates WiFi access point (AP). Network name is TimeCounter, no password.
- When connected to the AP the counter value can be read from http://192.168.4.1.
- The counter value is saved to the SPI flash every 10 seconds to make it persistent.
- The data is stored using LittleFS which support wear leveling and bad blocks detection.
