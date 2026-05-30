# ESP8266-DHT11-Google-Sheets-Logger

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-ESP8266-orange.svg)](firmware/dht11_google_sheets/platformio.ini)

NodeMCU **ESP8266** firmware that reads a **DHT11** sensor and logs temperature and humidity to **Google Sheets** via [PushingBox](https://www.pushingbox.com/).
Based on the [Hackster.io tutorial](https://www.hackster.io/detox/transmit-esp8266-data-to-google-sheets-8fc617) by Stephen Borsay — modernized with PlatformIO, secrets management, and CI.

## How it works

```text
DHT11 → ESP8266 → PushingBox (HTTP) → Google Apps Script (HTTPS) → Google Sheet
```

PushingBox bridges HTTP from the ESP8266 to HTTPS required by Google Apps Script.

## Hardware

| Part | Notes |
|------|-------|
| NodeMCU ESP8266 | Tested with ESP-12E |
| DHT11 | Data pin → `D4` (GPIO2) |
| USB cable | Power + serial monitor |

Wire DHT11 VCC to 5V (`Vin`), GND to GND, data to `D4`.

## Setup

### 1. Google Sheet + Apps Script

1. Create a Google Sheet and copy its spreadsheet ID from the URL.
2. Open **Extensions → Apps Script** and paste [`google-apps-script/Code.gs`](google-apps-script/Code.gs).
3. Set `SPREADSHEET_ID` in the script.
4. **Deploy → New deployment → Web app** (execute as *me*, access *anyone*).
5. Save the deployed web app URL.

### 2. PushingBox

1. Create a [PushingBox](https://www.pushingbox.com/) account.
2. Add a **CustomURL** service pointing at your Apps Script web app URL.
3. Create a scenario with this GET query string:

```text
?humidityData=$humidityData$&celData=$celData$&fehrData=$fehrData$&hicData=$hicData$&hifData=$hifData$
```

4. Copy the **Device ID** for the firmware.

### 3. Firmware

```bash
git clone git@github.com:PiyushMishra318/ESP8266-DHT11-Google-Sheets-Logger.git
cd ESP8266-DHT11-Google-Sheets-Logger/firmware/dht11_google_sheets
cp src/secrets.example.h src/secrets.h
# edit secrets.h with WiFi SSID, password, and PushingBox device ID
pio run -t upload
pio device monitor
```

## Configuration

Copy `src/secrets.example.h` to `src/secrets.h`:

| Define | Description |
|--------|-------------|
| `WIFI_SSID` | WiFi network name |
| `WIFI_PASSWORD` | WiFi password |
| `PUSHINGBOX_DEVID` | PushingBox scenario device ID |

`secrets.h` is gitignored. Never commit credentials.

## Project layout

```text
firmware/dht11_google_sheets/
  platformio.ini
  src/main.cpp
  src/config.h
  src/secrets.example.h
google-apps-script/Code.gs
.github/workflows/ci.yml
```

## Development

```bash
cd firmware/dht11_google_sheets
pio run                 # build with secrets.h
pio run -e ci           # CI build with dummy credentials
```

## License

MIT © 2026 [Piyush Mishra](https://github.com/PiyushMishra318)

## Credits

- [Stephen Borsay — Transmit ESP8266 Data to Google Sheets](https://www.hackster.io/detox/transmit-esp8266-data-to-google-sheets-8fc617)
- Adafruit DHT library
