#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "DFRobotDFPlayerMini.h"

#define MUX_S0 25
#define MUX_S1 26
#define MUX_S2 27
#define MUX_S3 14

#define MUX1_SIG 32
#define MUX2_SIG 33

#define GAME_ID_0 34
#define GAME_ID_1 35
#define GAME_ID_2 36
#define GAME_ID_3 39

#define DFPLAYER_RX 16
#define DFPLAYER_TX 17

#define LED_PIN 18
#define NUM_LEDS 40

#define CONFIRM_BUTTON 4

int HALL_THRESHOLD = 2000;

const int MUX1_CHANNELS = 13;
const int MUX2_CHANNELS = 12;

const int mux1Channels[MUX1_CHANNELS] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
};

const int mux2Channels[MUX2_CHANNELS] = {
  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
};

int mux1Values[MUX1_CHANNELS];
int mux2Values[MUX2_CHANNELS];

bool mux1Occupied[MUX1_CHANNELS];
bool mux2Occupied[MUX2_CHANNELS];

bool previousMux1State[MUX1_CHANNELS];
bool previousMux2State[MUX2_CHANNELS];

HardwareSerial DFSerial(2);
DFRobotDFPlayerMini dfPlayer;

bool dfPlayerReady = false;

Adafruit_NeoPixel leds(
  NUM_LEDS,
  LED_PIN,
  NEO_GRB + NEO_KHZ800
);

enum GameType {
  GAME_UNKNOWN,
  GAME_VAIKUNTAPALLI,
  GAME_PACHISI,
  GAME_PULI_MEKA
};

GameType currentGame = GAME_UNKNOWN;

String gameName(GameType game) {
  switch (game) {
    case GAME_VAIKUNTAPALLI:
      return "VAIKUNTAPALLI";

    case GAME_PACHISI:
      return "PACHISI";

    case GAME_PULI_MEKA:
      return "PULI MEKA";

    default:
      return "UNKNOWN";
  }
}

void selectMuxChannel(int channel) {
  digitalWrite(MUX_S0, channel & 0x01);
  digitalWrite(MUX_S1, (channel >> 1) & 0x01);
  digitalWrite(MUX_S2, (channel >> 2) & 0x01);
  digitalWrite(MUX_S3, (channel >> 3) & 0x01);

  delayMicroseconds(100);
}

int readMux1(int channel) {
  selectMuxChannel(channel);
  delayMicroseconds(100);
  return analogRead(MUX1_SIG);
}

int readMux2(int channel) {
  selectMuxChannel(channel);
  delayMicroseconds(100);
  return analogRead(MUX2_SIG);
}

bool sensorDetected(int value) {
  return value > HALL_THRESHOLD;
}

void scanMux1() {
  for (int i = 0; i < MUX1_CHANNELS; i++) {
    int channel = mux1Channels[i];
    int value = readMux1(channel);

    mux1Values[i] = value;
    mux1Occupied[i] = sensorDetected(value);
  }
}

void scanMux2() {
  for (int i = 0; i < MUX2_CHANNELS; i++) {
    int channel = mux2Channels[i];
    int value = readMux2(channel);

    mux2Values[i] = value;
    mux2Occupied[i] = sensorDetected(value);
  }
}

void printSensorValues() {
  Serial.println();
  Serial.println("----------- SENSOR DATA -----------");

  Serial.println("MUX 1:");

  for (int i = 0; i < MUX1_CHANNELS; i++) {
    Serial.print("C");
    Serial.print(mux1Channels[i]);
    Serial.print(" = ");
    Serial.print(mux1Values[i]);
    Serial.print("  ");

    if (mux1Occupied[i])
      Serial.println("[DETECTED]");
    else
      Serial.println("[EMPTY]");
  }

  Serial.println();
  Serial.println("MUX 2:");

  for (int i = 0; i < MUX2_CHANNELS; i++) {
    Serial.print("C");
    Serial.print(mux2Channels[i]);
    Serial.print(" = ");
    Serial.print(mux2Values[i]);
    Serial.print("  ");

    if (mux2Occupied[i])
      Serial.println("[DETECTED]");
    else
      Serial.println("[EMPTY]");
  }

  Serial.println("-----------------------------------");
}

int readGameID() {
  int b0 = digitalRead(GAME_ID_0);
  int b1 = digitalRead(GAME_ID_1);
  int b2 = digitalRead(GAME_ID_2);
  int b3 = digitalRead(GAME_ID_3);

  return (b3 << 3) | (b2 << 2) | (b1 << 1) | b0;
}

GameType identifyGame() {
  int id = readGameID();

  Serial.print("GAME ID = ");
  Serial.println(id, BIN);

  switch (id) {
    case 1:
      return GAME_VAIKUNTAPALLI;

    case 2:
      return GAME_PACHISI;

    case 3:
      return GAME_PULI_MEKA;

    default:
      return GAME_UNKNOWN;
  }
}

void setAllLEDs(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, leds.Color(r, g, b));
  }

  leds.show();
}

void startupAnimation() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds.setPixelColor(i, leds.Color(0, 80, 40));
    leds.show();
    delay(20);
  }

  delay(200);
  setAllLEDs(0, 0, 0);
}

void successEffect() {
  setAllLEDs(0, 100, 30);
  delay(250);
  setAllLEDs(0, 0, 0);
}

void errorEffect() {
  setAllLEDs(120, 0, 0);
  delay(250);
  setAllLEDs(0, 0, 0);
}

void playAudio(int track) {
  if (!dfPlayerReady) {
    Serial.println("DFPlayer not available.");
    return;
  }

  Serial.print("Playing audio track: ");
  Serial.println(track);

  dfPlayer.playMp3Folder(track);
}

void playGameIntro(GameType game) {
  switch (game) {
    case GAME_VAIKUNTAPALLI:
      playAudio(1);
      break;

    case GAME_PACHISI:
      playAudio(2);
      break;

    case GAME_PULI_MEKA:
      playAudio(3);
      break;

    default:
      break;
  }
}

void detectSensorChanges() {
  for (int i = 0; i < MUX1_CHANNELS; i++) {
    bool current = mux1Occupied[i];
    bool previous = previousMux1State[i];

    if (current != previous) {
      Serial.print("MUX1 C");
      Serial.print(mux1Channels[i]);

      if (current) {
        Serial.println(" -> PIECE DETECTED");
        successEffect();
      } else {
        Serial.println(" -> PIECE REMOVED");
      }

      previousMux1State[i] = current;
    }
  }

  for (int i = 0; i < MUX2_CHANNELS; i++) {
    bool current = mux2Occupied[i];
    bool previous = previousMux2State[i];

    if (current != previous) {
      Serial.print("MUX2 C");
      Serial.print(mux2Channels[i]);

      if (current) {
        Serial.println(" -> PIECE DETECTED");
        successEffect();
      } else {
        Serial.println(" -> PIECE REMOVED");
      }

      previousMux2State[i] = current;
    }
  }
}

bool buttonPressed() {
  if (digitalRead(CONFIRM_BUTTON) == LOW) {
    delay(30);

    if (digitalRead(CONFIRM_BUTTON) == LOW) {
      return true;
    }
  }

  return false;
}

void waitForButtonRelease() {
  while (digitalRead(CONFIRM_BUTTON) == LOW) {
    delay(10);
  }
}

void initializeGame() {
  currentGame = identifyGame();

  Serial.println();
  Serial.println("==============================");
  Serial.print("SELECTED GAME: ");
  Serial.println(gameName(currentGame));
  Serial.println("==============================");

  if (currentGame == GAME_UNKNOWN) {
    Serial.println("No valid game selected.");
    setAllLEDs(100, 0, 0);
    delay(500);
    setAllLEDs(0, 0, 0);
    return;
  }

  setAllLEDs(0, 60, 30);
  delay(300);
  setAllLEDs(0, 0, 0);

  playGameIntro(currentGame);
}

void printSystemStatus() {
  Serial.println();
  Serial.println("================================");
  Serial.println("       KRIDASETU STATUS");
  Serial.println("================================");

  Serial.print("Game: ");
  Serial.println(gameName(currentGame));

  Serial.print("DFPlayer: ");

  if (dfPlayerReady)
    Serial.println("READY");
  else
    Serial.println("NOT AVAILABLE");

  Serial.println("Sensors: ACTIVE");
  Serial.println("WS2812B: ACTIVE");
  Serial.println("================================");
}

void setup() {
  Serial.begin(115200);

  delay(1000);

  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_S3, OUTPUT);

  pinMode(MUX1_SIG, INPUT);
  pinMode(MUX2_SIG, INPUT);

  pinMode(GAME_ID_0, INPUT);
  pinMode(GAME_ID_1, INPUT);
  pinMode(GAME_ID_2, INPUT);
  pinMode(GAME_ID_3, INPUT);

  pinMode(CONFIRM_BUTTON, INPUT_PULLUP);

  leds.begin();
  leds.setBrightness(80);
  leds.clear();
  leds.show();

  startupAnimation();

  DFSerial.begin(
    9600,
    SERIAL_8N1,
    DFPLAYER_RX,
    DFPLAYER_TX
  );

  Serial.println("Initializing DFPlayer...");

  if (dfPlayer.begin(DFSerial)) {
    dfPlayerReady = true;
    Serial.println("DFPlayer READY");
    dfPlayer.volume(20);
    delay(500);
  } else {
    dfPlayerReady = false;
    Serial.println("DFPlayer NOT FOUND");
    Serial.println("Continuing without audio...");
  }

  scanMux1();
  scanMux2();

  for (int i = 0; i < MUX1_CHANNELS; i++) {
    previousMux1State[i] = mux1Occupied[i];
  }

  for (int i = 0; i < MUX2_CHANNELS; i++) {
    previousMux2State[i] = mux2Occupied[i];
  }

  initializeGame();

  printSystemStatus();

  Serial.println();
  Serial.println("KRIDASETU SYSTEM READY.");
  Serial.println("Press CONFIRM button to scan.");
}

void loop() {
  scanMux1();
  scanMux2();

  detectSensorChanges();

  if (buttonPressed()) {
    Serial.println();
    Serial.println("========== TURN CONFIRMED ==========");

    GameType detectedGame = identifyGame();

    if (detectedGame != GAME_UNKNOWN) {
      currentGame = detectedGame;

      Serial.print("Active Game: ");
      Serial.println(gameName(currentGame));

      scanMux1();
      scanMux2();

      printSensorValues();

      playAudio(4);

      successEffect();
    } else {
      Serial.println("INVALID GAME ID");
      errorEffect();
    }

    waitForButtonRelease();
  }

  delay(20);
}
