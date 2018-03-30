// Title: Smart Home  
// Editor: David Jeong
// 2018-01-30
// Pin placement
// OLED : I2C
// NeoPixel : D8
// HC-SR04 : echo D12, trig D13
// DHT11 : D7
// BUZZER : D2
// FanMotor : INA D5, INB D6
// SG90 : D3
// Bluetooth : RX D10, TX D11


// Wiring
// 1. OLED: SCL, SDA
// Library: https://github.com/adafruit/Adafruit-GFX-Library
// Library: https://github.com/adafruit/Adafruit_SSD1306
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2
#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };


// 2. Neopixel: D8
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN            8
// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


// 3. HC-SR04: D13, D12
int trigPin = 13;    // trigPin을 13번에 저장
int echoPin = 12;    // echPin을 12번에 저장
long duration, distance;       //시간과 거리를 설정합니다


// 4. DHT11: D7
// Library: https://github.com/adafruit/DHT-sensor-library
// Library: https://github.com/adafruit/Adafruit_Sensor
#include "DHT.h"
#define DHTPIN 7     // DHT11이 연결된 핀
#define DHTTYPE DHT11   // DHT 11, DHT시리즈중 11을 선택합니다.
DHT dht(DHTPIN, DHTTYPE);
float h; // 습도 humidity
float t; // 온도 temperature




// 5. Buzzer: D2
int BUZZER = 2;

// 6. Fan motor: D5, D6
int INA = 5;
int INB = 6;

// 7. SG90: D3
#include<Servo.h>
Servo MOTOR;

// 8. Bluetooth: D10, D11

#include<SoftwareSerial.h>
SoftwareSerial BTSerial(10, 11);  

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTSerial.begin(9600);
  HCSR04();
  dht.begin();
  OLED_SETUP();
  FANMOTOR_SETUP();
  BUZZER_SETUP();
  BUZZER_OFF();
  NEOPIXEL_SETUP();
}

void loop() {
  if (BTSerial.available()){
    char data = BTSerial.read();
    Serial.write(data);

     if(data == '1')
     {
      NEOPIXEL_WHITE_ON();
     }
     if(data == '2')
     {
      NEOPIXEL_OFF();
     }
     if(data == '3')
     {
      FANMOTOR_ROT();
     }
     if(data == '4')
     {
      FANMOTOR_STOP();
     }
     if(data == '5')
     {
      DOOR_OPEN();
     }
     if(data == '6')
     {
      DOOR_CLOSE();
     }
     if(data == '7')
     {
      OLED_ON();
     }
     if(data == '8')
     {
      OLED_OFF();
     }
     if(data == '9')
     {
      delay(100);
      getdht11();
      BTSerial.print(t); BTSerial.print(" C   ");
      BTSerial.print(h); BTSerial.println(" %   ");
     }
     if(data == 'A')
     {
      delay(100);
      getdistance();
      BTSerial.print(distance); BTSerial.print(" cm   ");
     }
  }


 
  
  // put your main code here, to run repeatedly:
  //NEOPIXEL_WHITE_ON();
  //NEOPIXEL_OFF();
 //BUZZER_ON();
 //BUZZER_OFF();
 
 getdht11();
 //BTSerial.flush();
 //BTSerial.print(t); BTSerial.println(" C   ");
 //BTSerial.print(h); BTSerial.print(" %   ");
 //BTSerial.flush();
 
 //FANMOTOR_ROT();
 //FANMOTOR_STOP();
 
 //DOOR_OPEN();
 //DOOR_CLOSE();
 

}


void NEOPIXEL_SETUP()
{
    pixels.begin(); // This initializes the NeoPixel library.
}
void NEOPIXEL_RED_ON()
{
   pixels.setPixelColor(0, pixels.Color(255,0,0)); // Moderately bright green color.
   pixels.show(); // This sends the updated pixel color to the hardware.
}
void NEOPIXEL_GREEN_ON()
{
   pixels.setPixelColor(0, pixels.Color(0,255,0)); // Moderately bright green color.
   pixels.show(); // This sends the updated pixel color to the hardware.
}
void NEOPIXEL_BLUE_ON()
{
   pixels.setPixelColor(0, pixels.Color(0,0,255)); // Moderately bright green color.
   pixels.show(); // This sends the updated pixel color to the hardware.
}
void NEOPIXEL_WHITE_ON()
{
   pixels.setPixelColor(0, pixels.Color(255,255,255)); // Moderately bright green color.
   pixels.show(); // This sends the updated pixel color to the hardware.
}
void NEOPIXEL_OFF()
{
   pixels.setPixelColor(0, pixels.Color(0,0,0)); // Moderately bright green color.
   pixels.show(); // This sends the updated pixel color to the hardware.
}

void FANMOTOR_SETUP()
{
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
}
void FANMOTOR_ROT()
{
  digitalWrite(INA,HIGH);
  digitalWrite(INB, LOW);
}
void FANMOTOR_STOP()
{
  digitalWrite(INA, LOW);
  digitalWrite(INB, LOW);
}
void BUZZER_SETUP()
{
  pinMode(BUZZER, OUTPUT);
}
void BUZZER_ON()
{
  digitalWrite(BUZZER, HIGH);
}
void BUZZER_OFF()
{
  digitalWrite(BUZZER, LOW);
}
void HCSR04()
{
  pinMode(trigPin, OUTPUT);   //trigPic을 출력모드로
  pinMode(echoPin, INPUT);    //echoPin을 입력모드로
}
void getdistance()
{
  digitalWrite(trigPin, LOW);    // trigPin이 low 신호를 주면
  delayMicroseconds(2);          // 2 만큼 지연합니다
  digitalWrite(trigPin, HIGH);   // trigPin이 high 신호를 주면
  delayMicroseconds(10);         // 10 만큼 지연합니다
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);  // duration에 밑의 공식을 대입합니다
  distance = (duration/2) / 29.1;     // 초음파가 갔다가 돌아오기 때문에 2배의 값을 얻습니다 그렇기에 거리/2를 합니다.
}


void DOOR_OPEN()
{
  MOTOR.attach(3);
  MOTOR.write(60);
  delay(100);
  MOTOR.attach(13);
}
void DOOR_CLOSE()
{
  MOTOR.attach(3);
  MOTOR.write(120);
  delay(100);
  MOTOR.attach(13);
}

void getdht11()
{
  h = dht.readHumidity();// 습도를 측정합니다.
  t = dht.readTemperature();// 온도를 측정합니다.
}
void OLED_SETUP()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
}
void OLED_ON()
{
  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();


  // draw scrolling text
  testscrolltext();
  delay(1000);
  display.clearDisplay();


  // miniature bitmap display
  //display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
  //display.display();
  //delay(1);



  // draw a bitmap icon and 'animate' movement
  //testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}

void OLED_OFF()
{
  display.display();
  display.clearDisplay();
}

void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
    
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
    }
    display.display();
    delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, BLACK);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > display.height()) {
        icons[f][XPOS] = random(display.width());
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
   }
}


void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
  delay(1);
}



void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.clearDisplay();
  display.println("Mechasolution");
  display.display();
  delay(1);
 
//  display.startscrollright(0x00, 0x0F);
//  delay(2000);
//  display.stopscroll();
//  delay(1000);
//  display.startscrollleft(0x00, 0x0F);
//  delay(2000);
//  display.stopscroll();
//  delay(1000);    
//  display.startscrolldiagright(0x00, 0x07);
//  delay(2000);
//  display.startscrolldiagleft(0x00, 0x07);
//  delay(2000);
//  display.stopscroll();
}

