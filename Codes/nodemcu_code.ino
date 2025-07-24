#include <ESP8266WiFi.h>

const char* ssid = "xxxxxx"; // Change to your WiFi SSID
const char* password = "xxxxxx"; // Change to your WiFi password
const String event = "Alert"; // This should match the event name you set up in IFTTT
const String IFTTT_KEY = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // Your IFTTT Webhooks key

WiFiClient client;

void sendIFTTTNotification() {
  Serial.println("Sending IFTTT request...");

  // Create the URL for the IFTTT Maker Webhooks
  String url = "/trigger/" + event + "/json/with/key/" + IFTTT_KEY;

  // Connect to the IFTTT server
  if (client.connect("maker.ifttt.com", 80)) {
    // Send a POST request to the IFTTT Maker Webhooks
    client.println("POST " + url + " HTTP/1.1");
    client.println("Host: maker.ifttt.com");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(2);
    client.println();
    client.println("{}");

    // Wait for the server's response
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
    }

    client.stop();
  } else {
    Serial.println("Error connecting to IFTTT server");
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');

    if (data.startsWith("Speed:")) {
      String speedValue = data.substring(6);
      Serial.print("Received Speed: ");
      Serial.println(speedValue);

      float speed = speedValue.toFloat();

      if (speed > 10.0) {
        // If speed is greater than 10, send an IFTTT notification
        sendIFTTTNotification();
      }
    }

    if (data.startsWith("Direction:")) {
      String directionValue = data.substring(10);
      Serial.print("Received Direction: ");
      Serial.println(directionValue);
    }
  }
}
