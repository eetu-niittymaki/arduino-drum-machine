#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width,  in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars];   

int buttonState;
int tempo;
int idA;
int idB;
int idC;
int idD;

boolean newData = false;

const unsigned char stopIcon [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 
	0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 
	0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char playIcon [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x3e, 0x00, 0x3f, 0x00, 0x3f, 0x80, 
	0x3f, 0xc0, 0x3f, 0xe0, 0x3f, 0xf0, 0x3f, 0xf8, 0x3f, 0xf8, 0x3f, 0xf8, 0x3f, 0xf8, 0x3f, 0xf0, 
	0x3f, 0xe0, 0x3f, 0xc0, 0x3f, 0x80, 0x3f, 0x00, 0x3e, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00
};

char *arrA[3] = { "Kick ", "Bongo", "Hat  "};  // Values in arrays need to be the same length or characters from
char *arrB[3] = { "Snare", "Rim  ", "Conga" }; // previous values will stay drawn on screen after change
char *arrC[3] = { "Hihat  ", "PercHat", "Cymbal " };
char *arrD[3] = { "Clap   ", "Cowbell", "Tamb   " };

void parseData() {
    char * strtokIindex; 

    strtokIindex = strtok(tempChars, ",");  // This is correct, don't change     
    buttonState = atoi(strtokIindex); // 
 
    strtokIindex = strtok(NULL, ","); 
    tempo = atoi(strtokIindex);     
    
    strtokIindex = strtok(NULL, ",");
    idA = atoi(strtokIindex);    
  
    strtokIindex = strtok(NULL, ",");
    idB = atoi(strtokIindex); 

    strtokIindex = strtok(NULL, ",");
    idC = atoi(strtokIindex); 

    strtokIindex = strtok(NULL, ",");
    idD = atoi(strtokIindex);
}

void showParsedData() {
   Serial.println(buttonState);
   Serial.println(tempo);
   Serial.println(idA);
   Serial.println(idB);
   Serial.println(idC);
   Serial.println(idD);
}

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte index = 0;
    char startMarker = '[';
    char endMarker = ']';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();
        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[index] = rc;
                index++;
                if (index >= numChars) {
                    index = numChars - 1;
                }
            } else {
                receivedChars[index] = '\0'; // terminate the string
                recvInProgress = false;
                index = 0;
                newData = true;
            }
        } else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

void draw() {
  oled.setTextSize(1);

  oled.setCursor(5, 0);
  oled.print("A: ");
  oled.println(arrA[idA]);
  oled.display();

  oled.setCursor(65, 0);
  oled.print("B: ");
  oled.println(arrB[idB]);
  oled.display();
  
  oled.setCursor(5, 50);
  oled.print("C: ");
  oled.println(arrC[idC]);
  oled.display();

  oled.setCursor(65, 50);
  oled.print("D: ");
  oled.println(arrD[idD]);
  oled.display();

  if (buttonState == 1) {
    oled.drawBitmap(8, 15, playIcon, 15, 25, WHITE);
    oled.display();
  } else {
    oled.drawBitmap(8, 18, stopIcon, 19, 18, WHITE);
    oled.display();
  }

  oled.setTextSize(2);
  oled.setCursor(30, 20);
  oled.print(tempo);
  oled.print(" BPM");
  oled.display();
}

void setup() {
  Serial.begin(9600);
  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  //delay(1000);
  oled.clearDisplay();
  oled.setTextColor(WHITE, BLACK);
  oled.setTextSize(2);
}

void loop() {
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    showParsedData();
    newData = false;
  }
  draw();
}