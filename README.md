# Setup
To setup bulb communication go to `lib/backend/core` and check the README file \
Next, add a `network-credentials.h` file to the `include` folder with `NETWORK_SSID` and `NETWORK_KEY` (same network as your bulb) \
Then, checkout `SmartBulbAdapater.h` and define `bulbIp` to match yours

## Please note!
In recent firmware updates, TP-Link has switched to a KLAP protocol. \
Please enable "Third-party Compatability" in the third-party services settings to allow for legacy handshake communication

<img width="718" height="593" alt="image" src="https://github.com/user-attachments/assets/c250c4a2-47b9-468c-99a2-cacda6939259" /> \
Breadboard design using:
- **an ESP32-S3** (Feel free to use any board with I2C support, 3.3v and at least 5 GPIO pins) 
- **Thru-hole 5-way Navigation switch** \(id 504)
- **OLED Display Module** (UI was designed with a 0.96" 128x64 size. A larger screen would work, just make sure to adjust screen dimension parameters)
