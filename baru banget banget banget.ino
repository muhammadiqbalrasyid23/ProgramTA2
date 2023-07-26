#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ThingESP.h>

ThingESP8266 thing("XORA", "TugasAkhir", "23032000");

#define relay1 5 //pin d1
#define relay2 4 //pin d2
#define relay3 0 //pin d3
#define relay4 2 //pin d4
#define FLAME_PIN 14 //pin D5
#define MQ2_PIN A0 //pin A0 untuk sensor MQ2
#define MOTION_SENSOR_PIN D6 //pin D6 untuk sensor gerak HC-SR501
#define BUZZER_PIN 13 //pin D7 untuk buzzer

//siapkan variabel untuk menampung URL
String url;

//siapkan variabel unutk wificlient
WiFiClient client;

unsigned long previousMillis = 0;
const long interval = 6000;
unsigned long delayMillis = 10000;  // Waktu jeda sebelum membaca sensor PIR (misalnya 10000 untuk 10 detik)
// Variabel untuk kondisi deteksi api
String kondisi = "";

void setup() {
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(FLAME_PIN, INPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(MOTION_SENSOR_PIN, INPUT); // Set pin sebagai input untuk sensor gerak
  pinMode(BUZZER_PIN, OUTPUT); // Set pin buzzer sebagai output
  thing.SetWiFi("Iqbal", "asujancuk");
  thing.initDevice();
}

String HandleResponse(String query) {
  // Penanganan perintah berdasarkan nilai query
  if (query == "hidupkan semua stop kontak") {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    Serial.println("semua stop kontak sudah dinyalakan");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "semua stop kontak sudah dinyalakan";
  } else if (query == "matikan semua stop kontak") {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    Serial.println("semua stop kontak sudah dimatikan");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "semua stop kontak sudah dimatikan";
  } else if (query == "hidupkan 1") {
    digitalWrite(relay1, LOW);
    Serial.println("Baik, dihidupkan");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "Baik, dihidupkan";
  } else if (query == "matikan 1") {
    digitalWrite(relay1, HIGH);
    Serial.println("Baik, dimatikan");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "Baik, dimatikan";
  } else if (query == "hidupkan 2") {
    digitalWrite(relay2, LOW);
    Serial.println("Baik, dihidupkan");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "Baik, dihidupkan";
  } else if (query == "matikan 2") {
    digitalWrite(relay2, HIGH);
    Serial.println("Baik, dimatikan");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "Baik, dimatikan";
  } else if (query == "hidupkan 3") {
    digitalWrite(relay3, LOW);
    Serial.println("Baik, dihidupkan");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "Baik, dihidupkan";
  } else if (query == "matikan 3") {
    digitalWrite(relay3, HIGH);
    Serial.println("Baik, dimatikan");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "Baik, dimatikan";
  } else if (query == "hidupkan 4") {
    digitalWrite(relay4, LOW);
    Serial.println("Baik, dihidupkan");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "Baik, dihidupkan";
  } else if (query == "matikan 4") {
    digitalWrite(relay4, HIGH);
    Serial.println("Baik, dimatikan");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "Baik, dimatikan";
  } else if (query == "cek 1") {
    Serial.println(digitalRead(relay1) ? "stop kontak mati" : "stop kontak hidup");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return digitalRead(relay1) ? "stop kontak mati" : "stop kontak hidup";
  } else if (query == "cek 2") {
    Serial.println(digitalRead(relay2) ? "stop kontak mati" : "stop kontak hidup");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return digitalRead(relay2) ? "stop kontak mati" : "stop kontak hidup";
  } else if (query == "cek 3") {
    Serial.println(digitalRead(relay3) ? "stop kontak mati" : "stop kontak hidup");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return digitalRead(relay3) ? "stop kontak mati" : "stop kontak hidup";
  } else if (query == "cek 4") {
    Serial.println(digitalRead(relay4) ? "stop kontak mati" : "stop kontak hidup");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return digitalRead(relay4) ? "stop kontak mati" : "stop kontak hidup";
  } else if (query == "makasih ya") {
    Serial.println("Baik sama-sama");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "Baik sama-sama";
  } else {
    Serial.println("bahasa tidak dimengerti");
    delay(delayMillis); // Jeda sebelum membaca sensor PIR
    return "bahasa tidak dimengerti";
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    int api = digitalRead(FLAME_PIN);
    int gasValue = analogRead(MQ2_PIN);

    static unsigned long motionDisabledMillis = 0; // Waktu terakhir sensor gerak dinonaktifkan
    const unsigned long motionDisableDuration = 10000; // Durasi nonaktif sensor gerak dalam milidetik (10 detik)

    if (api == 0 || gasValue > 65) {
      // Deteksi api atau gas
      digitalWrite(relay1, HIGH);
      digitalWrite(relay2, HIGH);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, HIGH);
      Serial.println("Ada Titik Api");
      kirim_wa("Bahaya!!!");

      motionDisabledMillis = currentMillis; // Menyimpan waktu saat sensor gerak dinonaktifkan
    } else {
      // Tidak ada deteksi bahaya
      if (currentMillis - motionDisabledMillis >= motionDisableDuration) {
        // Mengecek apakah sudah melewati durasi nonaktif sensor gerak
        int motion = digitalRead(MOTION_SENSOR_PIN); // Baca status sensor gerak
        if (motion == HIGH) {
          // Sensor gerak mendeteksi gerakan
          digitalWrite(relay1, LOW);
          digitalWrite(relay2, LOW);
          digitalWrite(relay3, LOW);
          digitalWrite(relay4, LOW);
          kondisi = "Ada Gerakan";
        } else {
          // Tidak ada gerakan terdeteksi oleh sensor gerak
          digitalWrite(relay1, HIGH);
          digitalWrite(relay2, HIGH);
          digitalWrite(relay3, HIGH);
          digitalWrite(relay4, HIGH);
          kondisi = "Tidak Ada Gerakan";
        }
      }
    }

    Serial.print("Api: ");
    Serial.print(api);
    Serial.print(" Gas: ");
    Serial.print(gasValue);
    Serial.print(" Gerakan: ");
    int motion = digitalRead(MOTION_SENSOR_PIN); // Baca status sensor gerak
    Serial.println(motion);
    float voltage = gasValue * (3.3 / 1023.0); // Mengonversi nilai analog menjadi tegangan
    float ppm = (0.4 * voltage + 0.1) * 1000; // Mengonversi tegangan menjadi ppm gas (sesuaikan dengan karakteristik sensor MQ2 Anda)

    Serial.print("PPM Gas: ");
    Serial.println(ppm);
  }
  thing.Handle();
}

void kirim_wa(String pesan) {
  url = "http://api.callmebot.com/whatsapp.php?phone=6285156285679&text=" + urlencode(pesan) + "&apikey=1924971";
  //kirim pesan
  postData();
}

void postData() {
  //siapkan variabel untuk menampung status pesan terkirim atau tidak
  int httpCode;
  //siapkan variabel untuk protokol http yang akan terkoneksi ke server callmebot.com
  HTTPClient http;
  //eksekusi link URL
  http.begin(client, url);
  httpCode = http.POST(url);
  //uji nilai variabel httpcode
  if (httpCode == 200) {
    Serial.println("Notifikasi WhatsApp Berhasil Terkirim");
  } else {
    Serial.println("Notifikasi WhatsApp Gagal Terkirim");
  }
  http.end();
}

String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0, code1, code2;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    // jika ada spasi kosong maka ganti dengan tanda +
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      code2 = '\0';
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield();
  }
  Serial.println(encodedString);
  return encodedString;
}
