# wisp32-watch

Open-source firmware that turns a **Waveshare ESP32-S3-Touch-AMOLED-2.06** into a touch smart wearable inference device with a built-in web dashboard and an integrated Gemini real-time voice AI assistant.

**Project site:** [wisp32-watch.web.app](https://wisp32-watch.web.app)

Created by Jose AVILES.

## What it does

- **Clock and weather API** — time and date on the AMOLED screen, synced over WiFi (NTP), with weather for a location you set.
- **Touch pages on the watch** — Settings, AI Assistant, Web Search, Translator, Voice Notes, and a QR code that opens the dashboard on your phone.
- **Real-Time Voice inference device** — talk to the watch and hear it answer. Works with OpenAI (Realtime API) and Google Gemini, using your own API key.
- **Local or Remote File Query with Military Grade Encryption** — Query your encrypted local files, remote files on your Wifi Network, Files in your Google Drive
- **Web dashboard** — the watch runs its own web server. From a browser on the same network you can manage files, a knowledge base, chat history, tasks, contacts, alarms and settings.
- **Email** — the watch can send email through your own SMTP account.
- **Encryption** — files on the SD card can be AES-encrypted with a password you choose.
- **Power** — double-tap to sleep or wake; hold the BOOT button to shut down.

## Hardware

| | |
|---|---|
| **Board** | Waveshare ESP32-S3-Touch-AMOLED-2.06 |
| **Display** | CO5300 QSPI AMOLED, 410×502 |
| **Touch** | FT3168 |
| **Storage** | SD card (SD_MMC, 1-bit mode) |
| **Power** | AXP2101 (battery level) |
| **Speaker** | ES8311 codec (I2S) |
| **Microphone** | ES7210 mic ADC (I2S) |

### Pin assignments

```
LCD (QSPI):        SDIO0=4  SDIO1=5  SDIO2=6  SDIO3=7  SCLK=11  CS=12  RESET=8
SD (SDMMC 1-bit):  CLK=2  CMD=1  DATA=3
```

## Building

Built with the **Arduino IDE** and the ESP32 Arduino core.

1. Download or clone this repository. The folder must be named `wisp32-watch`.
2. Open `wisp32-watch.ino` in the Arduino IDE.
3. Install the libraries listed below.
4. Apply the ArduinoWebsockets changes described below.
5. Select the board settings listed below.
6. Upload.

### Board settings

In the Arduino IDE, under **Tools**:

| Setting | Value |
|---|---|
| Board | `Waveshare ESP32-S3-Touch-AMOLED-2.06` |
| PSRAM | Enabled |
| USB CDC On Boot | Enabled |
| Partition Scheme | 16M Flash (3MB APP/9.9MB FATFS) |

The board must be the Waveshare entry, not `ESP32S3 Dev Module`. The sketch uses pin names (`TP_SDA`, `SDMMC_CLK`, `AMOLED_PWR_EN` and others) that only the Waveshare board entry defines. It is included in the `esp32` board package by Espressif.

### Libraries

From the Arduino Library Manager:

- `ArduinoJson`
- `ArduinoWebsockets` (version 0.5.4)
- `XPowersLib`
- `GFX Library for Arduino` (this is Arduino_GFX; it provides `Arduino_GFX_Library.h`)

Not in the Library Manager, so install it by hand from GitHub:

- `Arduino_DriveBus` ([Xk-w/Arduino_DriveBus](https://github.com/Xk-w/Arduino_DriveBus))

Everything else (`WiFi`, `HTTPClient`, `WebServer`, `ESPmDNS`, `SD_MMC`, `Preferences`, `ESP_I2S`, `mbedtls`, FreeRTOS) comes with the ESP32 core.

### ArduinoWebsockets changes

The sketch needs a modified copy of ArduinoWebsockets 0.5.4. Without it the build stops with `clearHeaders` not found.

1. Install `ArduinoWebsockets` 0.5.4.
2. Copy the `src` folder from [`library-patches/ArduinoWebsockets`](library-patches/ArduinoWebsockets) over the `src` folder of the installed library (usually `Documents/Arduino/libraries/ArduinoWebsockets`), replacing five files.

What the modified files change:

- `clearHeaders()` is added, so headers from one AI provider are not sent to another.
- A custom header with an empty value is not sent.
- Large messages are read completely instead of being cut off.
- The connection timeout is 8 seconds instead of 1.
- Secure connections do not verify the server's certificate when no certificate is supplied.
- The server's status line is printed to the serial port when connecting.

**Already included in this repository:**

- `es8311.c/.h`, `es8311_reg.h` — speaker codec driver
- `es7210_mini.c/.h` — microphone driver
- `qrcodegen.c/.h` — QR code generator

## First start

When the watch has no WiFi network saved, it creates its own hotspot called **`Watch-Setup`**. Connect to it to enter your WiFi details. After that, open the dashboard from a browser on the same network (the QR page on the watch shows the address).

API keys, passwords and WiFi details are entered through the watch or the dashboard. **No keys or passwords are stored in this source code** — the watch keeps them in its own flash memory and on the SD card.

## A note on memory

This board has very little internal memory (SRAM), and the AI assistant's secure connection uses most of it. The comment at the top of `wisp32-watch.ino` explains the one rule to follow when changing the code: lists of `String`s must be stored in PSRAM, not as plain global arrays. Read it before adding new global variables.

## License

This project is licensed under the **GNU General Public License v3.0** — see [`LICENSE`](LICENSE).

Third-party code included here keeps its own license:

- `qrcodegen.c` / `qrcodegen.h` — MIT, © Project Nayuki
- `es8311.c` / `es8311.h` / `es8311_reg.h` — Apache-2.0, © Espressif Systems
- `es7210_mini.c` / `es7210_mini.h` — written for this project; register map and start-up sequence derived from Espressif Systems' ES7210 driver (Apache-2.0)
- `library-patches/ArduinoWebsockets` — modified files from ArduinoWebsockets 0.5.4, GPL-3.0, © Gil Maimon

Thank you to Espressif Systems, Project Nayuki and Gil Maimon for their work.
