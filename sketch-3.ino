#include <AdafruitIO_WiFi.h>
#include <ESP32Servo.h>

// Adafruit IO credentials
#define IO_USERNAME "saadiiiii"
#define IO_KEY "aio_HZVo36L1teIkLoXKudiBNbMGmGUI"

// Wi-Fi credentials
#define WIFI_SSID "saadswifi"
#define WIFI_PASS "12345678"

// Create Adafruit IO instance
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Define Adafruit IO feed
AdafruitIO_Feed *statusFeed = io.feed("status");

// Servo settings
Servo myServo;
#define SERVO_PIN 15 // Change to the GPIO pin you connect to the servo

void setup() {
  Serial.begin(115200);

  // Attach servo to pin
  myServo.attach(SERVO_PIN);

  // Set servo to the initial (closed) position
  myServo.write(0);

  Serial.print("Connecting to Adafruit IO...");
  io.connect();

  // Wait for Adafruit IO connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected to Adafruit IO!");

  // Set up a subscription to the status feed
  statusFeed->onMessage(handleStatus);
  statusFeed->get(); // Get the initial status feed value
}

void loop() {
  io.run(); // Maintain Adafruit IO connection
}

// Handle incoming data from the status feed
void handleStatus(AdafruitIO_Data *data) {
  int statusCode = data->toInt();
  Serial.println("Received Status: " + String(statusCode));

  if (statusCode == 200) {
    // Open the door (rotate servo)
    Serial.println("Access granted: Opening door...");
    myServo.write(0); // Rotate servo to open position
    delay(6000);       // Wait 3 seconds (door remains open)

    // Close the door (return servo to initial position)
    Serial.println("Closing door...");
    myServo.write(80);
  } else {
    Serial.println("Access denied: Door remains closed.");
  }
}
