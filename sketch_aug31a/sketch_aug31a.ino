#include <Arduino.h>
// #include <WiFi.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <DS323x_Generic.h>
#include <Timezone_Generic.h>
// #include <defines.h>

DS323x rtc;
TimeChangeRule myDST = { "EDT", Second, Sun, Mar, 2, -240 };  //Daylight time = UTC - 4 hours
TimeChangeRule mySTD = { "EST", First, Sun, Nov, 2, -300 };   //Standard time = UTC - 5 hours
Timezone *myTZ;

TimeChangeRule *tcr;  //pointer to the time change rule, use to get TZ abbrev

const int D0_pin = 16;
const int D7_pin = 13;
const int D8_pin = 12;
const int D4_pin = 2;


const int pin_buzzer = D4_pin;
const int pin_rtc_scl = D8_pin;
const int pin_rtc_sda = D7_pin;

void printDateTime(time_t t, const char *tz) {
  char buf[32];
  char m[4];  // temporary storage for month string (DateStrings.cpp uses shared buffer)
  strcpy(m, monthShortStr(month(t)));
  sprintf(buf, "%.2d:%.2d:%.2d %s %.2d %s %d %s",
          hour(t), minute(t), second(t), dayShortStr(weekday(t)), day(t), m, year(t), tz);
  Serial.println(buf);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // rtc.begin();
  // rtc.setHourMode(CLOCK_H12);
  pinMode(pin_buzzer, OUTPUT);

  //koneksi ke wifi lain
  const char *ssid = "Micon";
  const char *password = "lilseven";
  WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.print("Mengkoneksi kan ke wifi:");
  //   Serial.println(ssid);
  // }

  myTZ = new Timezone(myDST, mySTD);
}


void loop() {
  // put your main code here, to run repeatedly:


  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == '1') {
      //buzzer bunyi
      digitalWrite(pin_buzzer, HIGH);
      Serial.println("Buzzer Nyala.");
    }
    if (command == '2') {
      //buzzer mati
      digitalWrite(pin_buzzer, LOW);
      Serial.println("Buzzer Mati.");
    }
  }
  Serial.println("Awaiting...");
  // delay(200);
  DateTime now = rtc.now();
  time_t utc = now.get_time_t();
  time_t local = myTZ->toLocal(utc, &tcr);

  printDateTime(utc, "UTC");
  printDateTime(local, tcr->abbrev);

  delay(10000);
}
