## Raspberry Pi home server

### Introduction

This project is an implementation of an mqtt client for an IoT project.
The project uses technologies such as firebase firestore, Eclipse Mosquitto broker, paho-mqtt client, etc and front-end is developed using svelte and tailwindcss

#### Hardware used
- Raspberry Pi 3 B+
- ESP8266 Nodemcu module
- 8 Channel 12v Relay board
- Power supply 12v & 5v


### Setup

#### 1. Install MQTT broker on Raspberry Pi

Follow the guide on [Eclipse Mosquitto](https://mosquitto.org/download/) to see installation methods.

#### 2. Clone this repo

On the RPi run:
```
git clone "https://github.com/nirmal15mathew/rpi-home-server"
```

> [!NOTE]
> In the server.py you can give custom ip address, port, username passwords and other configurations


### 3. Run server

Run the following command inside the folder 

```
mosquitto -c mosquitto.conf -v
```

This will give the output port and updates in terminal

Open up a different terminal session and run

```
python server.py
```
This should start the server and connect to mqtt broker

### 4. Flashing NodeMCU

Download main.ino from esp_code/main and upload it into nodemcu. make sure to update the ssid and password to your local network as well as the ip address for your raspberry pi

> [!IMPORTANT]
> Also give username and password (if set in the mqtt broker)

Once started, Open serial monitor in arduino ide to see status and confirm connection