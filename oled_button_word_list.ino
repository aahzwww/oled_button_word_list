#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
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

unsigned long StartTime;
// how long each word is displayed on the screen
unsigned long Interval = 8000; 

unsigned long CurrentTime; // in setup and listWords
unsigned long ElapsedTime;
// pointer to a word array, the display and points logic assume 10 words
char *WORD[] = {"they","are","some","one","my","love","page","them","down","she"};

int x = 0;
int First = 0;
int Score = 0;

const int buttonPin = 2;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  pinMode(buttonPin, INPUT);
  
  countDown();
}

void loop() {
CurrentTime = millis();
ElapsedTime = CurrentTime - StartTime;

buttonState = digitalRead(buttonPin);

if (First == 0){
  listWords(x);
  First++;
  StartTime = millis();
}

if (  ElapsedTime > Interval && x < 10){
  listWords(x);
  x++;
  StartTime = millis();
}


//button press increases score and pauses for 0.3 of a seconds to avoid the multiple presses registering
if ( buttonState == HIGH) {
  Score++;
  x++;
  StartTime = millis();
  delay(300);
  listWords(x);
  

}

if (x == 10 ){
  showScore();
}


}


void listWords(int index){  
  
  display.clearDisplay();

  display.setTextSize(4); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, 20);
  display.println(WORD[index]);
  display.display();      // Show initial text
  
  

}


void countDown(void){  
  
int i;  

  for(i=5;i>0;i--){
    display.clearDisplay();

    display.setTextSize(6); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(50, 18);
    display.println(i);
    display.display();      // Show initial text
    delay(1000);
  }
StartTime = millis();
}


//display the score as a number then call listStars
void showScore(void){  
  

    display.clearDisplay();

    display.setTextSize(6); // Draw 2X-scale text
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(50, 18);
    display.println(Score);
    display.display();      // Show initial text
    delay(2000);
    listStars(Score);
}

//display the score as stars
void showStars(int Final_Score){

int s;
int column;
int row;
  
display.clearDisplay();
for (s = 0; s < Final_Score ; s++){
  column = s;
  row = 1;
  if (s > 4){
    column -= 5;
    row = 2;
  }
  display.drawBitmap(
    20 + LOGO_WIDTH * column,
    15 + LOGO_HEIGHT * row,
    logo_bmp, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();  
  delay(500);
  }
  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
  x++;
}
