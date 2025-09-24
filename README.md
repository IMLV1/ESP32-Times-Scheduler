# ESP32 TimeSlot

This project is an **ESP32-based Time Slot Manager** that allows you to **add or remove time schedules** through **MQTT** and **Node-RED**, while storing all data persistently in the ESP32’s flash memory (**Preferences**).  

---

## Features
- Automatic Wi-Fi configuration with **WiFiManager**  
- Send and receive time data through **MQTT**  
- Stores scheduled times in **ESP32 Preferences (Flash memory)**  
- Add or remove time slots via **Node-RED**  
- Publishes status messages back to MQTT clients in real time  

---

## MQTT Topics

| Topic             | Description |
|--------------------|------------|
| `esp32/add`        | Send a JSON payload to **add a time slot** |
| `esp32/remove`     | Send a JSON payload to **remove a time slot** |
| `esp32/status`     | ESP32 publishes messages back with operation status (e.g., “Add time Success”, “Delete Success”) |

---

## JSON Format
```json
{
  "hour": 14,
  "minute": 30,
  "second": 0
}
```
> The numbers will be converted to a time string in the format `HH:MM:SS`.

---

## How It Works
1. Power on the ESP32.  
   - On the first boot it will start an AP named **`ESP32-Config`**.  
   - Connect to this AP and configure your Wi-Fi credentials.  
2. After Wi-Fi is set up, the ESP32 will attempt to connect to the configured **MQTT broker** (IP defined in the code).  
3. From **Node-RED** (or any MQTT client), send a JSON message to:
   - `esp32/add` to **add a new time slot**.
   - `esp32/remove` to **remove an existing time slot**.
4. ESP32 stores the time slots in **flash memory** and replies with status messages on `esp32/status`.

---

## Installation
1. Install **Arduino IDE** (or PlatformIO).  
2. Install the required libraries:
   - `WiFi.h` (comes with the ESP32 core)
   - `WiFiManager`
   - `PubSubClient`
   - `ArduinoJson`
   - `Preferences` (comes with the ESP32 core)
3. In the code, edit the MQTT broker IP:
   ```cpp
   #define mqtt_server "timekeeper.broker"
   ```
4. Upload the code to your ESP32 board.

---

## Notes
- Supports up to **8 time slots**.  
- If a duplicate time is added, it will be ignored and a `Duplicate time, ignore` message will be published.  
- When a time slot is removed, the remaining slots are automatically re-indexed.

---

## License
This project is released under the **MIT License**.  
You are free to use, modify, and distribute it.

---

**ESP32 TimeSlot** – Simple, persistent time scheduling over **MQTT + Node-RED**.
