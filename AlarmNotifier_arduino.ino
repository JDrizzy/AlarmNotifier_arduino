#include <ESP8266WiFi.h>
#include <SmsGlobal.h>

const char *wifi_ssid = "<wifi-ssid>";
const char *wifi_password = "<wifi-password>";

const char *sms_global_user = "<user>";
const char *sms_global_password = "<password>";
const char *sms_to[] = {"0411111111", "0411111112"};

SmsGlobal smsGlobal(sms_global_user, sms_global_password);

const int LED_PIN = D5;
const int ALARM_PIN = D6;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");
  connect_to_wifi();

  pinMode(LED_PIN, OUTPUT);
  pinMode(ALARM_PIN, INPUT_PULLUP);
}

void loop() {
  if (alarm_is_active()) {
    Serial.println("Alarm is active");
    send_notification();
    for(int i = 0; i <= 5; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
      delay(1000);
    }
    Serial.println("Exiting...");
    ESP.deepSleep(6e8); // 10 minutes
  }
}

void connect_to_wifi() {
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

bool alarm_is_active() {
  if (digitalRead(ALARM_PIN) == LOW) {
    delay(500);
    // Double check just to make sure
    if (digitalRead(ALARM_PIN) == LOW) {
      return true;
    }
  }
  return false;
}

void send_notification() {
  Serial.println("Ssending SMS notification now");
  for (int i = 0; i < sizeof(sms_to) / sizeof(sms_to[0]); i++)
  {
    Serial.print("Send SMS to: ");
    Serial.println(sms_to[i]);
    smsGlobal.send("Home", sms_to[i], "House alarm is active");
  }
}
