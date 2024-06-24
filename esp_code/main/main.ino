#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.

const char* ssid = "your-network-ssid";
const char* password = "your-network-password";
const char* mqtt_server = "192.168.1.10";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)

// defining output pins
#define P21 D0
#define P03 D1
#define P02 D2
#define P13 D4

char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void handle_pin_change(char * topic, String value) {
  String pin = String(topic).substring(20);
  int pinNum = pin.toInt();
  switch(pinNum) {
    case 21:
      if (value.toInt() == 1) {
        digitalWrite(P21, LOW);
      }
      else {
        digitalWrite(P21, HIGH);
      }
      break;
    case 3:
      if (value.toInt() == 1) {
        digitalWrite(P03, LOW);
      }
      else {
        digitalWrite(P03, HIGH);
      }
      break;
    case 2:
      if (value.toInt() == 1) {
        digitalWrite(P02, LOW);
      }
      else {
        digitalWrite(P02, HIGH);
      }
      break;
    case 13:
      if (value.toInt() == 1) {
        digitalWrite(P13, LOW);
      }
      else {
        digitalWrite(P13, HIGH);
      }
      break;
    default:
      break;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String msg = "";
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg += (char)payload[i];
  }
  Serial.println();
  handle_pin_change(topic, msg);

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    // Attempt to connect
    // change client-id, username and password in the following line if needed
    if (client.connect("espclient-id-25", "board01", "esp8266")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("board-state/val", "hello world");
      // ... and resubscribe
      client.subscribe("board-state/board01/21");
      client.subscribe("board-state/board01/3");
      client.subscribe("board-state/board01/13");
      client.subscribe("board-state/board01/2");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(P21, OUTPUT);
  pinMode(P02, OUTPUT);
  pinMode(P03, OUTPUT);
  pinMode(P13, OUTPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(&callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
