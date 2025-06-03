#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Servo.h>

// ===== configuration WiFi and Bot =====
const char* ssid = "your connaction network/wifi";
const char* password = "your password";

#define BOTtoken "your token bot"  
#define CHAT_ID "chat id API"    

// ===== Objek =====
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
Servo myServo;

int servoPin = 2;  
int posisiKunci = 0;     
int posisiBuka = 100;     

unsigned long lastTimeBotRan;

void setup() {
  Serial.begin(115200);
  myServo.attach(servoPin);

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nTerhubung ke WiFi");

  // Trust certificate for Telegram
  client.setInsecure(); 

  myServo.write(posisiKunci);  // Start with locked
  delay(300);
}

void loop() {
  if (millis() - lastTimeBotRan > 1000) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("Pesan Diterima");
      for (int i = 0; i < numNewMessages; i++) {
        String text = bot.messages[i].text;
        String chat_id = bot.messages[i].chat_id;
        String from_name = bot.messages[i].from_name;

        if (from_name == "") from_name = "Pengguna";

        if (text == "/buka") {
          myServo.write(posisiBuka);
          bot.sendMessage(chat_id, "Pintu dibuka!", "");
        } else if (text == "/tutup") {
          myServo.write(posisiKunci);
          bot.sendMessage(chat_id, "Pintu dikunci!", "");
        } else {
          bot.sendMessage(chat_id, "ketik perintah seperti ini: /buka atau /tutup.", "");
        }
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
