# AirConMote

- This is a firmware writed for ESP32/ESP8266 module.
- It give WiFi and MQTT support on some Daikin Air Conditionner that have the S31 module connector.
- It is easy to assemble, you will only need an ESP32 module, and a 5-wire JST EH cable that must be soldered to the ESP32 module. The ESP32 module will be programmed using an USB câble.
- This project is not a Daikin official module, it is also not a copy, it does not cover exacly the same features/goals.

![AirConMote](/media/airconmote-760.png?raw=true "AirConMote")

![Complete module](/media/module.jpg?raw=true "Complete module")

## Warning 🚨

This project is an **experimental** Wifi Interface for some Daikin Air Conditionner, it should **not** be used in production.

**I AM NOT RESPONSIBLE FOR ANY DAMAGES ON YOUR AIR CONDITIONNER, HEALTH, ELECTRIC SHOCK, FIRE OR WHAT EVER GETS DAMAGED IF YOU USE THIS PROJECT! USE IT AT YOUR OWN RISK!**

This code is "experimental" for various reasons:
- I don't have any documentation of the Daikin connector.
- It is not tested rigorously.
- It can suffer from significant changes (breaking changes) without further notice.

**IMPORTANT: REMOVE MAIN POWER OF THE AIR CONDITIONNER BEFORE CONNECTING THINGS ON IT TO AVOID ELECTRIC SHOCK !!!**

**Use it at your own risk!** ☠️

## Features

Provides many of the features required for IoT :

* Web interface that allow full control of the Air Conditionner
* Configurable WiFi - Network scanner and WiFi configuration screen
* MQTT - Connection to an MQTT broker for automation and monitoring
* Remote Firmware Updates - Firmware replacement using OTA update or upload via UI
* Security - Protected RESTful endpoints and a secured user interface

## Getting Started

### Prerequisites

You will need the following before you can get started.

* [PlatformIO](https://platformio.org/) - IDE for development
* [Node.js](https://nodejs.org) - For building the interface with npm

### Code base
This project is based on the excellent [esp8266-react](https://github.com/rjwats/esp8266-react) project.
The Readme here is a part of the original, you may find more detailed documentation on the [esp8266-react](https://github.com/rjwats/esp8266-react) project.


### Building and uploading the firmware

Pull the project and open it in PlatformIO. PlatformIO should download the ESP8266 platform and the project library dependencies automatically.

The project structure is as follows:

Resource                         | Description
-------------------------------- | ----------------------------------------------------------------------
[interface/](interface)          | React based front end
[lib/framework/](lib/framework)  | C++ back end for the ESP8266/ESP32 device
[src/](src)                      | The main.cpp and demo project to get you started
[scripts/](scripts)              | Scripts that build the React interface as part of the platformio build
[platformio.ini](platformio.ini) | PlatformIO project configuration file

### Building the firmware

Once the platform and libraries are downloaded the back end should successfully build within PlatformIO. 

The firmware may be built by pressing the "Build" button:

![build](/media/build.png?raw=true "build")

Alternatively type the run command:

```bash
platformio run
```

#### Uploading the firmware

The project is configured to upload over a serial connection by default. You can change this to use OTA updates by uncommenting the relevant lines in ['platformio.ini'](platformio.ini). 

The firmware may be uploaded to the device by pressing the "Upload" button:

![uploadfw](/media/uploadfw.png?raw=true "uploadfw")

Alternatively run the 'upload' target:

```bash
platformio run -t upload
```

#### Connector
The Air Conditionner must have an "S21" connector. This is a 5 pins one.

Here are photos of the connector on a daikin ATXC35C :

![S21](/media/S21-small.jpg?raw=true "S21")


Pinout :

| S21 | Daikin | ESP32 Pin |
|-----|:---:|----------:|
| 1   | 5V  | VCC       |
| 2   | TX  | IO16      |
| 3   | RX  | IO17      |
| 4   | 12V |           |
| 5   | GND | GND       |


![S21 zoom](/media/S21-zoom.jpg?raw=true "S21 zoom")

![ESP32 Pinout](/media/esp-pinout.jpg?raw=true "ESP32 pinout")

### Building & uploading the interface

The interface has been configured with create-react-app and react-app-rewired so the build can customized for the target device. The large artefacts are gzipped and source maps and service worker are excluded from the production build. This reduces the production build to around ~150k, which easily fits on the device.

The interface will be automatically built by PlatformIO before it builds the firmware. The project can be configured to serve the interface from either PROGMEM or the filesystem as your project requires. The default configuration is to serve the content from PROGMEM, serving from the filesystem requires an additional upload step which is [documented below](#serving-the-interface-from-the-filesystem).

### Developing the interface locally

UI development is an iterative process so it's best to run a development server locally during interface development (using `npm start`). This can be accomplished by deploying the backend to a device and configuring the interface to point to it:

![Development Server](/media/devserver.png?raw=true "Development Server")

The following steps can get you up and running for local interface development:

- Deploy firmware to device
- [Configuring the dev proxy](#configuring-the-dev-proxy) with device's IP in interface/package.json
- [Start the development server](#starting-the-development-server) with "npm start"
- Develop interface locally

#### Configuring the dev proxy

The interface has a development environment which is enabled when running the development server using `npm start`. The [package.json](interface/package.json) file defines the location of the services which the development server will proxy. This is defined by the "proxy" propery, which will need to be change to the the IP address or hostname of the device running the firmware.

```json
"proxy": "http://192.168.0.77"
```

> **Tip**: You must restart the development server for changes to the proxy location to come into effect. Note that the proxy is configured to handle http and WebSocket connections from this location, see [setupProxy.js](interface/src/setupProxy.js) for details.

#### Starting the development server

Change to the [interface](interface) directory with your bash shell (or Git Bash) and use the standard commands you would with any react app built with create-react-app:

```bash
cd interface
```

Install the npm dependencies, if required and start the development server:

```bash
npm install
npm start
```
> **Tip**: You can (optionally) speed up the build by commenting out the call to build_interface.py under "extra scripts" during local development. This will prevent the npm process from building the production release every time the firmware is compiled significantly decreasing the build time.

## Factory settings

The framework has built-in factory settings which act as default values for the various configurable services where settings are not saved on the file system. These settings can be overridden using the build flags defined in [factory_settings.ini](factory_settings.ini).

Customize the settings as you see fit, for example you might configure your home WiFi network as the factory default:

```ini
  -D FACTORY_WIFI_SSID=\"My Awesome WiFi Network\"
  -D FACTORY_WIFI_PASSWORD=\"secret\"
  -D FACTORY_WIFI_HOSTNAME=\"awesome_light_controller\"
```

### Default access point settings

By default, the factory settings configure the device to bring up an access point on start up which can be used to configure the device:

* SSID: ESP8266-React
* Password: esp-react

### Security settings and user credentials

By default, the factory settings configure two user accounts with the following credentials: 

Username | Password
-------- | --------
admin    | admin
guest    | guest

It is recommended that you change the user credentials from their defaults better protect your device. You can do this in the user interface, or by modifying [factory_settings.ini](factory_settings.ini) as mentioned above.

### Customizing the factory time zone setting

Changing factory time zone setting is a common requirement. This requires a little effort because the time zone name and POSIX format are stored as separate values for the moment. The time zone names and POSIX formats are contained in the UI code in [TZ.tsx](interface/src/ntp/TZ.tsx). Take the appropriate pair of values from there, for example, for Los Angeles you would use:

```ini
  -D FACTORY_NTP_TIME_ZONE_LABEL=\"America/Los_Angeles\"
  -D FACTORY_NTP_TIME_ZONE_FORMAT=\"PST8PDT,M3.2.0,M11.1.0\"
```

## Libraries Used

* [esp8266-react](https://github.com/rjwats/esp8266-react)
* [React](https://reactjs.org/)
* [Material-UI](https://material-ui.com/)
* [notistack](https://github.com/iamhosseindhv/notistack)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
* [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
* [AsyncMqttClient](https://github.com/marvinroger/async-mqtt-client)
