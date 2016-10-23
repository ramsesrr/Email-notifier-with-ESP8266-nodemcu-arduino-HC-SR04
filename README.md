# Email-notifier-with-ESP8266-nodemcu-arduino-HC-SR04
Email Alarm notifier with ESP8266 WiFi module nodemcu/arduino The board sends emails when a the trigger condition is meet. The code can be configured to send emails on any desired condition(over temperature when using temperature sensors,digital/analog trigger on an input) The code is written in C using arduino environment. The sender email address can be a gmail,yahoo,hotmail......

Additional info...

/*

 Option 1 (recommended): Use mail.yahoo.com or the Yahoo Mail app for Android and iOS to more securely access your account.
 Option 2 (not recommended): If you still want to use an app that uses less secure sign in to your Yahoo account:

 Click here: https://login.yahoo.com/account/security#other-apps
 Turn on "Allow apps that use less secure sign in"
 Go back to your existing email application and sign in to your Yahoo account again.

 Using apps that don't meet Yahoo's recommended security standards may leave your account more vulnerable or less secure.

 static const uint8_t D0   = 16;
 static const uint8_t D1   = 5;
 static const uint8_t D2   = 4;
 static const uint8_t D3   = 0;
 static const uint8_t D4   = 2;
 static const uint8_t D5   = 14;
 static const uint8_t D6   = 12;
 static const uint8_t D7   = 13;
 static const uint8_t D8   = 15;
 static const uint8_t D9   = 3;
 static const uint8_t D10  = 1;

 #define D0 16
 #define D1 5 // I2C Bus SCL (clock)
 #define D2 4 // I2C Bus SDA (data)
 #define D3 0
 #define D4 2 // Same as "LED_BUILTIN", but inverted logic
 #define D5 14 // SPI Bus SCK (clock)
 #define D6 12 // SPI Bus MISO
 #define D7 13 // SPI Bus MOSI
 #define D8 15 // SPI Bus SS (CS)
 #define D9 3 // RX0 (Serial console)
 #define D10 1 // TX0 (Serial console)

 */

