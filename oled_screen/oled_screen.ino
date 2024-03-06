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

uint8_t buttonState;
uint8_t tempo;
uint8_t idA;
uint8_t idB;
uint8_t idC;
uint8_t idD;

uint8_t oldOledState;
uint8_t oledState = 0;
uint8_t stepsA;
uint8_t stepsB;
uint8_t stepsC;
uint8_t stepsD;

boolean newData = false;

const unsigned char stopIcon [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e,
	0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e,
	0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x1e, 0x1e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char playIcon [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x0f, 0x80, 0x00, 0x0f, 0xc0, 0x00, 0x0f,
	0xe0, 0x00, 0x0f, 0xf0, 0x00, 0x0f, 0xf8, 0x00, 0x0f, 0xfc, 0x00, 0x0f, 0xfc, 0x00, 0x0f, 0xf8,
	0x00, 0x0f, 0xf0, 0x00, 0x0f, 0xe0, 0x00, 0x0f, 0xc0, 0x00, 0x0f, 0x80, 0x00, 0x0f, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char alpha [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0xa0, 0x00, 0x02, 0x60, 0x00, 0x04, 0x20, 0x00, 0x04, 0x20, 0x00, 0x04, 0x20, 0x00, 0x02, 0x68,
	0x00, 0x01, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char beta [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x01, 0x20, 0x00, 0x02, 0x20, 0x00, 0x02,
	0x20, 0x00, 0x02, 0xc0, 0x00, 0x02, 0x20, 0x00, 0x02, 0x10, 0x00, 0x02, 0x10, 0x00, 0x03, 0x10,
	0x00, 0x02, 0xe0, 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char delta [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00,
	0x90, 0x00, 0x00, 0x80, 0x00, 0x00, 0x40, 0x00, 0x00, 0xf0, 0x00, 0x01, 0x10, 0x00, 0x02, 0x08,
	0x00, 0x02, 0x08, 0x00, 0x02, 0x08, 0x00, 0x01, 0x10, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char gamma [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x10, 0x00, 0x01,
	0x10, 0x00, 0x01, 0x10, 0x00, 0x00, 0xa0, 0x00, 0x00, 0xa0, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x40,
	0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

char *arrA[3] = { "Kick ", "Bongo", "Hat  "};  // Values in arrays need to be the same length or characters from
char *arrB[3] = { "Snare", "Rim  ", "Conga" }; // previous values will stay drawn on screen after change
char *arrC[3] = { "Hihat  ", "PercHat", "Cymbal " };
char *arrD[3] = { "Clap   ", "Cowbell", "Tamb   " };

void parseData() {
  char * strtokIindex;
  if (oledState == 0) {
    strtokIindex = strtok(tempChars, ",");
    oledState = atoi(strtokIindex);

    strtokIindex = strtok(NULL, ",");
    buttonState = atoi(strtokIindex);

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
  } else {
    strtokIindex = strtok(tempChars, ",");
    oledState = atoi(strtokIindex);

    strtokIindex = strtok(NULL, ",");
    stepsA = atoi(strtokIindex);

    strtokIindex = strtok(NULL, ",");
    stepsB = atoi(strtokIindex);

    strtokIindex = strtok(NULL, ",");
    stepsC = atoi(strtokIindex);

    strtokIindex = strtok(NULL, ",");
    stepsD = atoi(strtokIindex);
  }
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

void drawPlayScreen() {
  oled.setTextSize(1);

  oled.setCursor(15, 5);
  oled.drawBitmap(0, 0, alpha, 19, 18, WHITE);
  oled.print(":");
  oled.println(arrA[idA]);
  oled.display();

  oled.setCursor(75, 5);
  oled.drawBitmap(62, 0, beta, 19, 18, WHITE);
  oled.print(":");
  oled.println(arrB[idB]);
  oled.display();

  oled.setCursor(16, 50);
  oled.drawBitmap(0, 45, gamma, 19, 18, WHITE);
  oled.print(":");
  oled.println(arrC[idC]);
  oled.display();

  oled.setCursor(77, 50);
  oled.drawBitmap(62, 45, delta, 19, 18, WHITE);
  oled.print(":");
  oled.println(arrD[idD]);
  oled.display();

  if (buttonState == 0) {
    oled.fillRect(8, 19, 19, 19, BLACK); // So the icons don't overlap when chaging play states
    oled.drawBitmap(10, 20, playIcon, 19, 18, WHITE);
    oled.display();
  } else {
    oled.fillRect(8, 19, 19, 19, BLACK);
    oled.drawBitmap(10, 20, stopIcon, 19, 18, WHITE);
    oled.display();
  }

  oled.setTextSize(2);
  if (tempo < 100) {
    oled.setCursor(35, 22);
    oled.print(" ");
    oled.print(tempo);
    oled.print(" BPM");
    oled.display();
  } else {
    oled.setCursor(35, 22);
    oled.print(tempo);
    oled.print(" BPM");
    oled.display();
  }
}

void drawSetSteps() {
  oled.setTextSize(2);

  oled.setCursor(15, 0);
  oled.drawBitmap(0, 0, alpha, 19, 18, WHITE);
  oled.print(":");
  oled.print(stepsA);
  oled.println(" ");
  oled.display();

  oled.setCursor(77, 0);
  oled.drawBitmap(62, 0, beta, 19, 18, WHITE);
  oled.print(":");
  oled.print(stepsB);
  oled.println(" ");
  oled.display();

  oled.setCursor(15, 48);
  oled.drawBitmap(0, 45, gamma, 19, 18, WHITE);
  oled.print(":");
  oled.print(stepsC);
  oled.println(" ");
  oled.display();

  oled.setCursor(77, 48);
  oled.drawBitmap(62, 45, delta, 19, 18, WHITE);
  oled.print(":");
  oled.print(stepsD);
  oled.println(" ");
  oled.display();

  oled.setCursor(8, 25);
  oled.print("MAX STEPS");
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
  oldOledState = oledState;
}

void loop() {
  recvWithStartEndMarkers();
  if (newData == true) {
    strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
    parseData();
    //showParsedData();
    newData = false;
  }
  if (oledState == 0) {
    drawPlayScreen();
  } else {
    drawSetSteps();
  }

  if (oldOledState != oledState) { // Clear screen at state change
    oled.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
  }
  oldOledState = oledState;
}