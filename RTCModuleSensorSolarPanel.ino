#include <RTClib.h>

#include <Arduino.h>
// #include <WiFi.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
// #include <DS323x_Generic.h>
// #include <Timezone_Generic.h>

// #include <defines.h>

// DS323x rtc;
RTC_DS3231 rtc;

// TimeChangeRule myDST = { "EDT", Second, Sun, Mar, 2, -240 };  //Daylight time = UTC - 4 hours
// TimeChangeRule mySTD = { "EST", First, Sun, Nov, 2, -300 };   //Standard time = UTC - 5 hours
// Timezone *myTZ;

// TimeChangeRule *tcr;  //pointer to the time change rule, use to get TZ abbrev

const int D0_pin = 16;
const int D7_pin = 13;
const int D8_pin = 15;
const int D4_pin = 2;


const int pin_buzzer = D8_pin;

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
  // Wire.begin(5, 4);
  
  while(!rtc.begin()){
    Serial.println("Sensor RTC tidak ditemukan");
    delay(1000);
    Serial.flush();
  }
  
  rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));
  // rtc.adjust(DateTime(2023, 9, 4, 9,0,0));
  rtc.disableAlarm(1);
  rtc.disableAlarm(2);
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);

  rtc.setAlarm1(
    DateTime(
    rtc.now().day(),
    rtc.now().month(),
    rtc.now().year(),
    22,
    0,
    0),
    DS3231_A1_Hour
  );
    rtc.setAlarm2(
    DateTime(
    rtc.now().day(),
    rtc.now().month(),
    rtc.now().year(),
    22,
    30,
    0),
    DS3231_A2_Hour
  );  
  // rtc.enableAlarm(rtc.MATCH_HHMMSS);
}


void loop() {
  // put your main code here, to run repeatedly:
  if(rtc.alarmFired(1) || rtc.alarmFired(2)) {
    if(rtc.now().hour() >= 22) {
      Serial.println("Sudah lebih dari jam 10.");
    }
    digitalWrite(pin_buzzer, HIGH);
    delay(5000);
    digitalWrite(pin_buzzer, LOW);
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
    rtc.clearAlarm(3);
  }

  // if(rtc.checkAlarm1()) {

  // }

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
  // DateTime now = rtc.now();
  // time_t utc = now.get_time_t();
  // time_t local = myTZ->toLocal(utc, &tcr);

  // printDateTime(utc, "UTC");
  // printDateTime(local, tcr->abbrev);

  DateTime now = rtc.now();       //Menampilkan RTC pada variable now

  Serial.print("Tanggal : ");
  Serial.print(now.day());        //Menampilkan Tanggal
  Serial.print("/");
  Serial.print(now.month());      //Menampilkan Bulan
  Serial.print("/");
  Serial.print(now.year());       //Menampilkan Tahun
  Serial.print(" ");
  
  Serial.print("Jam : ");
  Serial.print(now.hour());       //Menampilkan Jam
  Serial.print(":");
  Serial.print(now.minute());     //Menampilkan Menit
  Serial.print(":");
  Serial.print(now.second());     //Menampilkan Detik
  Serial.println();

  delay(1000);
}
