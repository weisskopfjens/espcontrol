# ESPControl reference

## Build in webpages

http://node_ip/config
Main configuration page

http://node_ip/filemanager
Filemanager for spiffs filesystem

http://node_ip/update
Update page

http://node_ip/json?infos
General infos about the nodes as json data

http://node_ip/json?vars
All global lua variables as json data

http://node_ip/json?nodes
All neigbour nodes with name and ip as json data

http://node_ip/control?cmd=...
Sends an lua command (...) to the node

http://node_ip/control?luaerror
Print an Errortext if there is an error in the lua script



## LUA functions reference
---
## Core functions

#### pinMode(integer pin, integer mode)
> INPUT             0x00
> INPUT_PULLUP      0x02
> INPUT_PULLDOWN_16 0x04 // PULLDOWN only possible for pin16
> OUTPUT            0x01

#### digitalWrite(integer pin, integer state)

#### digitalRead(integer pin)

#### analogWrite(integer pin, integer value)

#### delay(integer ms)

#### print(string output)

#### millis(integer ms)

#### tostring(number int)
> convert a number to a string

#### tonumber(string str)
> convert a string to a number

#### reboot()
> Reboot the node

#### saveCfg()
> Save the config to filesystem

#### readCfg()
> Read the config from filesystem

#### now()
> Return the the current time in minutes.

#### time()
> Return the current time as string.

#### sunrise()
> Return time of the sunrise in minutes since midnight.

#### sunset()
> Return time of the sunset in minutes since midnight.

#### newNode(string name, integer id, string ip)
> Add a new node to the internal table of neigbour nodes

#### wifiDisconnect()
> Disconnects WiFi

#### minOfDay()
> Return the elapsed minutes since midnight.

#### sendMail(string host, integer port, string user, string pass, string from, string to, string subject, string message)
> Send a mail over SMTP

#### udpBroadcast(string message, integer port)
> Send a UDP broadcast

#### udpSend(string ip, string message, integer port)
> Send a UDP paket to an ip

#### httpGet(string data)
> Send a http get to URL. data = URL.

#### getNode(integer nodeid)

#### configWiFi

#### attachCounter(integer counternumber, integer pin, integer type, integer pullup)
> * **counternumber**
  * a number 0-3 for the counter id
* **type**
  * RISING  1
  * FALLING 2
  * CHANGE  3
* **pullup**
  * INPUT 0
  * INPUT_PULLUP  2

#### getCounter(integer counternumber)

#### resetCounter(integer counternumber)

#### deleteFile(string filename)
> delete a file

#### format()
> formating the spiffs filesystem

#### renameFile(string oldfilename, string newfilename)
> rename a file

#### round(number value)
> rounds up a float value

#### floor(number value)
> rounds down a float value

#### ceil(number value)

#### abs(integer value)
> return a absulut value (-1 = 1)

#### strcmp(string str1, string str2)
> compares two strings

#### strstr(string str, string needle)

#### deepSleep(integer u_sec)
> Set ESP in deep sleep mode until u_sec expired.
> Connect RST pin with D0 pin (GPIO 16)
---
## plugin_bme280

#### initBME280(integer sda,integer scl)
> Initialize the bme280 plugin. You must provide a SDA Pin and SCL pin.

#### getBME280values()
> Return temperature[1,number], humidity[2,number] and pressure[3,number]
---
## plugin_dht

#### initDHT(integer pin)
> Initialize the DHT plugin. You must provide a pin.

#### getDHTvalues()
> Return temperature[1,number], humidity[2,number]*
---
## plugin_ir

#### initIR(integer rxPin, integer txPin)
> Initialize the IR plugin. You must provide a pin for the receiver (rx) and a pin for the tranceiver (tx

#### getIRValue()
> Return the last received IR command. Return 0 if nothing received

#### IRsend(integer type, integer data, integer nbits)
> Send IR data
---
## plugin_ledstrip

#### initLED(integer redPin, integer greenPin, interger bluePin)
> Initialize the RGB LED strip plugin. You must provide a pin for red, green and blue channel.

#### changeLEDcolor(integer hue, integer saturation, integer value)
> Change the color.

#### getAnimationSpeed()
> Return the animation speed [1,integer]

#### setAnimationSpeed(integer animationspeed)

#### setLEDcolor(integer hue, integer saturation, integer value)

#### LEDon()

#### LEDoff()

#### toggleLED()

#### setLEDMode(integer animationmode)

#### getLEDMode()
> Return the animation mode as Integer [1,integer]

#### isLEDOn()
> Return if the strip is on [1,integer]

#### getLEDcolor()
> Return hue[1,integer], saturation[2,integer], value[3,integer]
---
## plugin_ssd1306
A plugin to control a ssd1306 oled display over i2c bus.

#### initSSD1306(integer sda, integer scl)
> Initialize the bme280 plugin. You must provide a SDA Pin and SCL pin.

#### ssd1306drawString(integer xpos, integer ypos, string message)

#### ssd1306clear()

#### ssd1306display()

#### ssd1306font(integer fontsize, integer alignment)
>* **fontsize**
  * 0 = 10
  * 1 = 16
  * 2 = 24
* **alignment**
  * 0 = left
  * 1 = center
  * 2 = right
---
## plugin_mqtt

#### initMQTT(string ip, integer port)
> Initialize the MQTT plugin. A server ip and a port must be provide.

#### mqttSubscribe(string topic, integer qos)

#### mqttPublish(string topic, integer qos, integer retain,string message)

#### mqttSetWill(string topic, integer qos, integer retain,string message)

## plugin_WS2812

#### initWS2812(integer ledcount, integer ledpin)

#### WS2812setMode(integer mode)

#### WS2812setSpeed(integer speed)

#### WS2812chgSpeed(integer speed)

#### WS2812setBrightness(integer brightness)

#### WS2812chgBrightness(integer brightness)

#### WS2812control(integer value)

#### WS2812getStatus()
> return 4 integer values
> * 1. brightness
> * 2. speed
> * 3. mode
> * 4. is running
