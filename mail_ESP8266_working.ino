#include <ESP8266WiFi.h>
#include <rBase64.h>

#define ENABLE_SERIAL_DBG
#define SENSOR_PING
#define PUSH_BUTTON

const char* SSID = "your-sidd"; // wifi ssid
const char* PASS = "your-wifi-password"; // ssid password
const char* user_base64 = "yourEmail@yahoo.com";
const char* user_password_base64 = "yourEmail_password";
const char* from_email = "MAIL From:<FromEmail@yahoo.com>";
const char* to_email = "RCPT To:<yourEmail@yahoo.com>";  // 
const char* SMTP_server = "smtp.mail.yahoo.com";//SMTP server addreaa
char* mail_subject = "Subject: Esp8266 button test\r\n";//email subject
const int SMTP_port = 465;
//rbase64.encode("

byte sendEmail(const char *mesaj);
byte eRcv(WiFiClientSecure client);
void efail(WiFiClientSecure client);

#ifdef SENSOR_PING
#define TRIGGER 5
#define ECHO    4
long duration, distance;
#endif

#ifdef PUSH_BUTTON
/******************************************Button_setup***************************************************************/
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 14;  // the number of the pushbutton pin D0
const int ledPin = LED_BUILTIN;  // the number of the LED pin

// Variables will change:
int ledState = HIGH;  // the current state of the output pin
int buttonState;  // the current reading from the input pin
int lastButtonState = LOW;  // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;  // the debounce time; increase if the output flickers
/*********************************************************************************************************/
#endif
char flag_once = 0;
char message_contet[50];
void setup()
{
	Serial.begin(115200);
	delay(10);
	Serial.println("");
	Serial.print("Connecting To ");
	WiFi.begin(SSID, PASS);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("");

	Serial.println("WiFi Connected");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

#ifdef SENSOR_PING
	// NodeMCU Pin D1 > TRIGGER | Pin D2 > ECHO
	pinMode(TRIGGER, OUTPUT);  //TRIGGER
	pinMode(ECHO, INPUT);  //ECHO
	//pinMode(BUILTIN_LED, OUTPUT);//BUILTIN_LED
#endif

#ifdef PUSH_BUTTON
	/*******ButtonSetup******************/
	pinMode(buttonPin, INPUT);
	pinMode(ledPin, OUTPUT);

	// set initial LED state
	digitalWrite(ledPin, ledState);
	/*******ButtonSetup******************/
#endif
	if (sendEmail("Board Reboot"))
	{
#ifdef ENABLE_SERIAL_DBG
		Serial.println(F("Email sent"));
#endif
	}
	else
	{
#ifdef ENABLE_SERIAL_DBG
		Serial.println(F("Email failed"));
#endif
	}
}

void loop()
{
	//delay(100);
#ifdef PUSH_BUTTON
	/*******ButtonSetup******************/
	// read the state of the switch into a local variable:
	int reading = digitalRead(buttonPin);

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH),  and you've waited
	// long enough since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (reading != lastButtonState)
	{
		// reset the debouncing timer
		lastDebounceTime = millis();
	}

	if ((millis() - lastDebounceTime) > debounceDelay)
	{
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != buttonState)
		{
			buttonState = reading;

			// only toggle the LED if the new button state is HIGH
			if (buttonState == LOW)
			{
				ledState = !ledState;

				if (sendEmail(message_contet))
				{
#ifdef ENABLE_SERIAL_DBG
					Serial.println(F("Email sent"));
#endif
				}
				else
				{
#ifdef ENABLE_SERIAL_DBG
					Serial.println(F("Email failed"));
#endif
				}
			}
		}
	}

// set the LED:
	digitalWrite(ledPin, ledState);

// save the reading.  Next time through the loop,
// it'll be the lastButtonState:
	lastButtonState = reading;
	/*******ButtonSetup******************/
#endif
#ifdef SENSOR_PING

	digitalWrite(TRIGGER, LOW);
	delayMicroseconds(2);

	digitalWrite(TRIGGER, HIGH);
	delayMicroseconds(10);

	digitalWrite(TRIGGER, LOW);
	duration = pulseIn(ECHO, HIGH);
	distance = (duration / 2) / 29.1;
	if (distance < 200 && distance >= 4)
	{
		flag_once = 1;
	}

	if (distance > 200)
	{
		distance = 200;
	}
	if (distance < 4)
	{
		distance = 0;
		if (flag_once == 1)
		{
			flag_once = 0;

			if (sendEmail("Too Close!!!!!!!!!!!!!!!!!!!"))
			{
#ifdef ENABLE_SERIAL_DBG
				Serial.println(F("Email sent"));
#endif
			}
			else
			{
#ifdef ENABLE_SERIAL_DBG
				Serial.println(F("Email failed"));
#endif
			}
		}
	}
	Serial.print(distance);
	Serial.println(" Centimeter:");
	delay(1000);
	sprintf(message_contet, "Last distance was %d", distance);
#endif
}

byte sendEmail(const char *mesaj)
{
	WiFiClientSecure client;

	if (client.connect(SMTP_server, SMTP_port) == 1)
	{
#ifdef ENABLE_SERIAL_DBG
		Serial.println(F("connected"));
#endif
	}
	else
	{
#ifdef ENABLE_SERIAL_DBG
		Serial.println(F("connection failed"));
#endif
		return 0;
	}
	if (!eRcv(client))
		return 0;
#ifdef ENABLE_SERIAL_DBG
	Serial.println(F("--- Sending EHLO"));
#endif
	client.println("EHLO 1.2.3.4");
	if (!eRcv(client))
		return 0;
#ifdef ENABLE_SERIAL_DBG
	Serial.println(F("--- Sending login"));
#endif
	client.println("AUTH LOGIN");
	if (!eRcv(client))
		return 0;
#ifdef ENABLE_SERIAL_DBG
	Serial.println(F("--- Sending User base64"));
#endif
	client.println(rbase64.encode(user_base64));
	if (!eRcv(client))
		return 0;
#ifdef ENABLE_SERIAL_DBG
	Serial.println(F("--- Sending Password base64"));
#endif
	client.println(rbase64.encode(user_password_base64));
	if (!eRcv(client))
		return 0;
#ifdef ENABLE_SERIAL_DBG
	Serial.println(F("--- Sending From"));
#endif
	client.println(from_email);
	if (!eRcv(client))
		return 0; //
#ifdef ENABLE_SERIAL_DBG
	Serial.println(F("--- Sending To"));
#endif
	client.println(to_email);
	if (!eRcv(client))
		return 0; //
#ifdef ENABLE_SERIAL_DBG
	Serial.println(F("--- Sending DATA"));
#endif
	client.println(F("DATA"));
	if (!eRcv(client))
		return 0;
//client.println(F("Subject: Esp8266 email test\r\n"));   //
	client.println((mail_subject));   //
	client.println(mesaj); //
	client.println(F(".")); //
	if (!eRcv(client))
		return 0;
#ifdef ENABLE_SERIAL_DBG
	Serial.println(F("--- Sending QUIT"));
#endif
	client.println(F("QUIT"));
	if (!eRcv(client))
		return 0;
	client.stop();
#ifdef ENABLE_SERIAL_DBG
	Serial.println(F("disconnected"));
#endif
	return 1;
}

byte eRcv(WiFiClientSecure client)
{
	byte respCode;
	byte thisByte;
	int loopCount = 0;

	while (!client.available())
	{
		delay(1);
		loopCount++;
		// if nothing received for 10 seconds, timeout
		if (loopCount > 10000)
		{
			client.stop();
#ifdef ENABLE_SERIAL_DBG
			Serial.println(F("\r\nTimeout"));
#endif
			return 0;
		}
	}

	respCode = client.peek();
	while (client.available())
	{
		thisByte = client.read();
#ifdef ENABLE_SERIAL_DBG
		Serial.write(thisByte);
#endif
	}

	if (respCode >= '4')
	{
		efail(client);
		return 0;
	}
	return 1;
}

void efail(WiFiClientSecure client)
{
	byte thisByte = 0;
	int loopCount = 0;

	client.println(F("QUIT"));

	while (!client.available())
	{
		delay(1);
		loopCount++;
		// if nothing received for 10 seconds, timeout
		if (loopCount > 10000)
		{
			client.stop();
#ifdef ENABLE_SERIAL_DBG
			Serial.println(F("\r\nTimeout"));
#endif
			return;
		}
	}

	while (client.available())
	{
		thisByte = client.read();
#ifdef ENABLE_SERIAL_DBG
		Serial.write(thisByte);
#endif
	}

	client.stop();
#ifdef ENABLE_SERIAL_DBG
	Serial.println(F("disconnected"));
#endif
}

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
