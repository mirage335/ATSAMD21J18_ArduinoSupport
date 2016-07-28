
_Problem_

The LED does not blink as expected when programmed by the Arduino IDE. Only programming with Atmel Studio 6 works.

Schematic provided as ATSAMD21J18_Minimal_Schematic.pdf. Example Atmel Studio "LED_TOGGLE1" project works. Example Arduino "sketch" does not work.

Arduino has substantial community-written libraries, and is an open-source solution. If we can make this particular chip, ATSAMD21J18, work with the Arduino IDE, functionality beyond Arduino Mega will be available as a single $6 chip. Notable functionality includes 48MHz clock, 32bit math, 256kB ROM, extensively reconfigurable SERCOM interfaces, fast analog IO, onboard high-accuracy clocks, and minimal PCB requirements.


_Debugging Procedure_

1) Created working LED toggle firmware in Atmel Studio 6, based on "port_pin_toggle_output_level(LED_0_PIN);".
2) Installed SAM-BA bootloader from Atmel, device became visible to 'lsusb' command under Linix/UNIX.
3) Uploaded essentially empty Arduino sketch successfully.
4) Carefully worked out and understood Arduino/ARM/SAM pin mapping and port manipulation functions.
5) Configured an example sketch to blink LED connected to PB07.
6) Successfully uploaded. No effect.
7) Successively added println statements, attempted to toggle other LEDs. No result.
8) Uploaded Arduino Zero bootloader. Not visible to 'lsusb'.
9) Compared Autonomo bootloader to offical Arduino Zero bootloader. No difference. Uploaded anyway. Not visible to 'lsusb'.
10) Uploaded Atmel SAM-BA bootloader, copied resultant fuse settings. Uploaded Autonomo bootloader. Set fuse settings copied down earlier, noticed some differences. Still not visible to 'lsusb'. Cannot upload Arduino code.
11) Uploaded Arduino blink sketch ".hex" file directly through Atmel Studio. Still, no LEDs light up.
12) Restored original Atmel Studio 6 LED blink code. Now, that does not work either!
13) Frustration. Uploaded Atmel SAM-BA bootloader. At least shows up through 'lsusb'.
14) After repeatedly switching firmwares, checking fuses, and finally loading an old precompiled '*.hex" file from the Atmel Studio file project, original LED blink works again.
15) Uploaded Atmel SAM-BA bootloader. Then rebuilt and executed original LED blink project, still using SAM-ICE. Still works.
16) Built and uploaded LED blink Arduino Zero sketch with Atmel Studio 7, through "Native USB Port", having uploaded SAM-BA bootloader. Connected to serial port. No data or debugging information.
17) Stripped out pin toggle code from sketch, leaving just Serial/SerialUSB statements. Still, no data received.

_Sketch_


#define PORT_GROUP PORTB
#define PORT_NUMBER 7

volatile uint32_t *setPin = &PORT->Group[PORT_GROUP].OUTSET.reg;  // Ptr to PortA Data Output Value Set register
volatile uint32_t *clrPin = &PORT->Group[PORT_GROUP].OUTCLR.reg;  // Ptr to PortA Data Output Value Clear register
volatile uint32_t *dirPin = &PORT->Group[PORT_GROUP].DIRSET.reg;  // Ptr to PortA Data Direction Set register

const uint32_t  PinMASK = (1ul << PORT_NUMBER);   // Generate bit mask, binary one PORT_NUMBER places to the left

// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize digital pin as an output.
  *dirPin = PinMASK;
}

// the loop function runs over and over again forever
void loop()
{
  *setPin = PinMASK;        // turn the LED on (HIGH is the voltage level)
  delay(250);              // wait for a second
  *clrPin = PinMASK;        // turn the LED off by making the voltage LOW
  delay(250);              // wait for a second
  SerialUSB.println("cycle");
  Serial.println("cycle");
}


_PIN_

Pin numbers are enumerated by an array in local code.
~/.arduino15/packages/arduino/hardware/samd/1.6.4/variants/arduino_zero

Position in the g_APinDescription array is the pin number. For example, for the Arduino Zero, the first entry is pin 0, and corresponds to PA11 as described in the Atmel datasheet.
//{ PORTA, 11, PIO_SERCOM, (PIN_ATTR_DIGITAL), No_ADC_Channel, NOT_ON_PWM, NOT_ON_TIMER, EXTERNAL_INT_11 }, // RX: SERCOM0/PAD[3]
References for the arduino pinout confirm this first pin, index 0, is indeed used as an RX pin, and indeed connected to chip pin labeled PA11.


_PORT_

Read/write pin code, including digitalWrite(), is defined in local code.
~/.arduino15/packages/arduino/hardware/samd/1.6.4/cores/arduino

For writing a digital pin, the digitalWrite() function ultimately relys on one line to actually set the pin high.
//PORT->Group[g_APinDescription[ulPin].ulPort].OUTSET.reg = (1ul << g_APinDescription[ulPin].ulPin) ;

From here, some basic substitutions are illuminating.
PORT->Group[g_APinDescription[ulPin].ulPort].OUTSET.reg = (1ul << g_APinDescription[ulPin].ulPin) ;
PORT->Group[g_APinDescription[0].ulPort].OUTSET.reg = (1ul << g_APinDescription[ulPin].ulPin) ;
PORT->Group[PORTA].OUTSET.reg = (1ul << g_APinDescription[ulPin].ulPin) ;

PORT->Group[PORTA].OUTSET.reg = (1ul << g_APinDescription[ulPin].ulPin) ;
PORT->Group[PORTA].OUTSET.reg = (1ul << g_APinDescription[0].ulPin) ;
PORT->Group[PORTA].OUTSET.reg = (1ul << 11) ;	//Assuming the second ulPin refers to hardware port/pin designator PA11.

These substitutions are confirmed by a post on arduino forums - https://forum.arduino.cc/index.php?topic=334073.0 .

//volatile uint32_t *setPin = &PORT->Group[g_APinDescription[PIN].ulPort].OUTSET.reg;
//const uint32_t  PinMASK = (1ul << g_APinDescription[PIN].ulPin);
//*setPin = PinMASK;

//volatile uint32_t *setPin = &PORT->Group[PORTA].OUTSET.reg;
//const uint32_t  PinMASK = (1ul << 11);
//*setPin = PinMASK;


See Atmel datasheet, and arduino forums - https://forum.arduino.cc/index.php?topic=334073.0 .

-Example-

//From https://forum.arduino.cc/index.php?topic=334073.0 .

#include <Arduino.h>

#define PIN 10

#ifdef _VARIANT_ARDUINO_ZERO_
volatile uint32_t *setPin = &PORT->Group[g_APinDescription[PIN].ulPort].OUTSET.reg;
volatile uint32_t *clrPin = &PORT->Group[g_APinDescription[PIN].ulPort].OUTCLR.reg;

const uint32_t  PinMASK = (1ul << g_APinDescription[PIN].ulPin);
#endif

void setup() {
  pinMode(PIN, OUTPUT);
}

void loop() {
  while(1) {
#ifdef _VARIANT_ARDUINO_ZERO_
  *setPin = PinMASK;
  *clrPin = PinMASK;
#else
    digitalWrite(PIN, HIGH);
    digitalWrite(PIN, LOW);
#endif
  }
}

-Example-
#define PORT_NUMBER 17    // Digital pin 13 is port pin PA17

volatile uint32_t *setPin = &PORT->Group[PORTA].OUTSET.reg;  // Ptr to PortA Data Output Value Set register
volatile uint32_t *clrPin = &PORT->Group[PORTA].OUTCLR.reg;  // Ptr to PortA Data Output Value Clear register
volatile uint32_t *dirPin = &PORT->Group[PORTA].DIRSET.reg;  // Ptr to PortA Data Direction Set register

const uint32_t  PinMASK = (1ul << PORT_NUMBER);   // Generate bit mask, binary one 17 places to the left

// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize digital pin 13 (PA17) as an output.
  *dirPin = PinMASK;
}

// the loop function runs over and over again forever
void loop()
{
  *setPin = PinMASK;        // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  *clrPin = PinMASK;        // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}

_SERCOM_
//SPIClass SPI1 (&sercom1, 12, 13, 11, SPI_PAD_0_SCK_1, SERCOM_RX_PAD_3);

As documented by https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/creating-a-new-spi .

_Reset_
Double-tap on a reset pin immediately after startup may be necessary to re-enter bootloader in case of bad code locking up the MCU.

_Bootloader_
As described by the Femtoduino USB GitHub readme, after the relevant bootloader hex file has been extracted, it can be flashed through the following GUI sequence.
Atmel Studio 6 --> Tools --> Device Programmer --> Memories

Assert "Erase Flash before programming" and "Verify Flash after programming" before clicking "Program".

The "SAM-BA Bootloader for SAM D21", "Application Note", also mentions this process, along with a file AT07175.zip, containing the necessary HEX files. This package is available under a different name, from the Atmel website, under the SAMD21J18 documents list.

This procedure ultimately proved successful.

Autonomo is apparently a SAMD21J18 based Arduino Zero clone, with Arduino support. Apparently, it ships with an identical bootloader to the actual Arduino Zero, despite using a slightly different chip. Strangely though, unlike the official Atmel SAM-BA bootloader, neither this nor the Arduino Zero bootloader result in any USB functionality as seen by 'lsusb', virtual serial port, or attempted sketch upload. Setting fuses to those set along with Atmel SAM-BA bootloader programming did not bring USB functionality online with Arduino/Autonomo bootloader.

_Connectivity_
SAM-BA bootloader explictly states PA25/PA24 must be connected to USB D+/D-. Sparkfun breakout for similar chip also meets this specification. Arduino zero schematic for another similar chip also meets this specification.

_Clock_
Some SAMD21 breakoutboards seem to include a dedicated crystal oscillator. Since the SAMD21J18 includes a "high-accuracy" RTC clock internally, this is not desired.

_Factory_
As stated in the "SAM-BA Bootloader for SAM D21", "Application Note", these chips are NOT factory programmed with the SAM-BA bootloader.

_Datasheet_
ATSAMD21J18/ATSAMD21G18 - https://cdn.sparkfun.com/datasheets/Dev/Arduino/Boards/Atmel-42181-SAM-D21_Datasheet.pdf

_Comparable_
SparkFun SAMD21 Breakout - https://github.com/sparkfun/SAMD21_Dev_Breakout
SparkFun SAMD21 Breakout Design/Firmware - https://github.com/sparkfun/SAMD21_Dev_Breakout
Arduino Zero - https://www.arduino.cc/en/Main/ArduinoBoardZero
Arduino Zero Cores/Bootloaders - https://github.com/arduino/ArduinoCore-samd

_Reference_
https://hackaday.io/project/6276-arduino-zero-sam-d20d21-breakout-board
https://www.sparkfun.com/products/13672
https://github.com/sparkfun/SAMD21_Dev_Breakout/tree/master/Firmware
https://github.com/femtoduino/femto-usb

https://github.com/arduino/ArduinoCore-samd/tree/master/bootloaders/zero
https://github.com/arduino/ArduinoCore-samd

https://github.com/arduino/ArduinoCore-samd/issues/7

https://forum.arduino.cc/index.php?topic=382407.0

https://hackaday.io/project/7996-mini-sam

https://hackaday.io/project/6062-jack

https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/muxing-it-up

https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/creating-a-new-spi

https://github.com/arduino/ArduinoCore-samd

https://github.com/OpenROV/openrov-arduino-cores

http://forum.arduino.cc/index.php?topic=129868.15

https://hackaday.io/project/7576-sam-dev

https://forum.arduino.cc/index.php?topic=334073.0

https://cdn-learn.adafruit.com/assets/assets/000/030/929/medium800/adafruit_products_2995_pinout_v1_0.png?1457306311

http://forum.sodaq.com/t/use-arduino-ide-1-6-6-for-autonomo/25
http://downloads.sodaq.net/package_sodaq_index.json

