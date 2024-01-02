/*
* Mole Control by Markus Klingler (grafMakulaDer2te)
*/
//Gamebuino Meta Version

#include <Gamebuino-Compat.h>

#define SCR_CLK 13;
#define SCR_DIN 11;
#define SCR_DC A2;
#define SCR_CS A1;
#define SCR_RST A0;

#define BTN_UP_PIN 9;
#define BTN_RIGHT_PIN 7;
#define BTN_DOWN_PIN 6;
#define BTN_LEFT_PIN 8;
#define BTN_A_PIN 4;
#define BTN_B_PIN 2;
#define BTN_C_PIN A3;

#define BuzzerPin 3;

Gamebuino gB;

const byte titleScreenImage[] PROGMEM = {24,20,
B00001111,B11111111,B00000000,
B00001111,B11111111,B00000000,
B00110000,B00000000,B11000000,
B00110000,B00000000,B11000000,
B11111111,B11111111,B11110000,
B11111111,B11111111,B11110000,
B11111111,B11111111,B11110000,
B11111111,B11111111,B11110000,
B11111111,B00001111,B11110000,
B11111111,B00001111,B11110000,
B11000000,B00000000,B00110000,
B11000000,B00000000,B00110000,
B11001111,B00001111,B00110000,
B11001111,B00001111,B00110000,
B11000000,B11110000,B00110000,
B11000000,B11110000,B00110000,
B11110000,B00000000,B11110000,
B11110000,B00000000,B11110000,
B00001111,B11111111,B00000000,
B00001111,B11111111,B00000000,
};

const byte holeIcon[] PROGMEM = {
  16,
  10,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00111111,
  B00000000,
  B11111111,
  B11000000,
  B11111111,
  B11000000,
  B00111111,
  B00000000,
};

const byte moleIcon[] PROGMEM = {
  16,
  10,
  B00111111,
  B00000000,
  B01000000,
  B10000000,
  B11111111,
  B11000000,
  B11111111,
  B11000000,
  B11110011,
  B11000000,
  B10000000,
  B01000000,
  B10110011,
  B01000000,
  B10001100,
  B01000000,
  B11000000,
  B11000000,
  B00111111,
  B00000000,
};

#define debounceDelay 100
long debounceNextMillis = 0;

extern const uint8_t font3x5[];
extern const uint8_t font5x7[];

#define initScreenFont font5x7
#define initScreenText "Mole Control"

#define gamepad_holesStartX 5
#define gamepad_holesStartY 8
#define gamepad_holesStepX 15
#define gamepad_holesStepY 12

#define gamepad_hammerOffsetX -1
#define gamepad_hammerOffsetY -1
#define gamepad_hammerWidth 12
#define gamepad_hammerHight 13

#define gamepad_font font3x5
#define gamepad_scoreX 50
#define gamepad_scoreStringY 0
#define gamepad_scoreStringText "Score:"
#define gamepad_scoreY 6
#define gamepad_levelStringY 12
#define gamepad_levelStringText "Level:"
#define gamepad_levelY 18
#define gamepad_lifesStringY 24
#define gamepad_lifesStringText "Lifes:"
#define gamepad_lifesY 30

#define gameover_font font5x7
#define gameover_x 15
#define gameover_y1 17
#define gameover_text1 "Game"
#define gameover_y2 27
#define gameover_text2 "Over"
#define gameover_squareStartX 13
#define gameover_squareStartY 15
#define gameover_squareWidth 27
#define gameover_squareHeight 21

int8_t hammerX = 0;
int8_t hammerY = 0;

int16_t score = 0;
int8_t lifes = 11;

uint16_t level = 1;
#define level_initalScoreLevelUp 5
#define level_addScoreLevelUp 0.5
int16_t level_scoreLevelUpNow = level_initalScoreLevelUp;
int16_t level_scoreSinceLastLevel = 0;

#define level_initialMoleTimeoutTime 5000
#define level_endMoleTimoutTime 500
#define level_substMoleTimeoutTime 200
int16_t level_moleTimeoutTimeNow = level_initialMoleTimeoutTime;

#define level_initalMinTimeNextMole 2500
#define level_endMinTimeNextMole 300
#define level_substMinTimeNextMole 300
int16_t level_nowMinTimeNextMole = level_initalMinTimeNextMole;

#define level_initalMaxTimeNextMole 5000
#define level_endMaxTimeNextMole 500
#define level_substMaxTimeNextMole 100
int16_t level_nowMaxTimeNextMole = level_initalMaxTimeNextMole;

int32_t level_timeNextMole = level_nowMaxTimeNextMole;

int32_t moleTimeoutMillis[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

uint32_t rand_counter = 0;

void setup() {
  //for debuging
  //Serial.begin(115200);

  gB.begin();
  gB.display.setFont(initScreenFont);
  gB.titleScreen(F(initScreenText),titleScreenImage);
  gB.pickRandomSeed();
  gB.battery.show = false;  //hide the battery indicator
  delay(200);
}

void loop() {
  if ((millis() > debounceNextMillis)) {
    bool btnPressed = false;
    if (gB.buttons.pressed(BTN_DOWN)) {
      hammerY++;
      if (hammerY > 2) hammerY = 2;
      btnPressed = true;
      rand_counter -= 5678;
    }
    if (gB.buttons.pressed(BTN_UP)) {
      hammerY--;
      if (hammerY < 0) hammerY = 0;
      btnPressed = true;
      rand_counter += 4234;
    };
    if (gB.buttons.pressed(BTN_LEFT)) {
      hammerX--;
      if (hammerX < 0) hammerX = 0;
      btnPressed = true;
      rand_counter += ~rand_counter;
    };
    if (gB.buttons.pressed(BTN_RIGHT)) {
      hammerX++;
      if (hammerX > 2) hammerX = 2;
      btnPressed = true;
      rand_counter += -1324;
      rand_counter = ~rand_counter;
    };
    if (gB.buttons.pressed(BTN_A)) {
      clickMole();
      btnPressed = true;
      rand_counter += 1234;
    };
    if (gB.buttons.pressed(BTN_C)) {  //restart
      gB.display.setFont(initScreenFont);
      gB.titleScreen(F(initScreenText),titleScreenImage);
      gB.pickRandomSeed();
      gB.battery.show = false;  //hide the battery indicator
      delay(200);

      score = 0;
      level = 1;
      lifes = 10;

      level_scoreLevelUpNow = level_initalScoreLevelUp;
      level_nowMinTimeNextMole = level_initalMinTimeNextMole + millis();
      level_nowMaxTimeNextMole = level_initalMaxTimeNextMole + millis();
      level_moleTimeoutTimeNow = level_initialMoleTimeoutTime + millis();
      level_scoreSinceLastLevel = 0;

      for (int8_t it = 0; it < 9; it++) {
        moleTimeoutMillis[it] = 0;
      }

      rand_counter *= 3 + 1;
    }

    if (btnPressed) debounceNextMillis = millis() + 200;
  }

  if (gB.update()) {
    drawGamePad();
  }

  if (lifes > 0) {
    checkTimoutMoles();
    randGenMoles();
  } else gameOver();
  rand_counter++;
}

void drawGamePad() {
  for (uint8_t ix = 0; ix < 3; ix++) {
    for (uint8_t iy = 0; iy < 3; iy++) {

      //test for mole
      //calc x and y
      int8_t xNow = gamepad_holesStartX + (ix * gamepad_holesStepX);
      int8_t yNow = gamepad_holesStartY + (iy * gamepad_holesStepY);

      //draw moles and holes
      if (moleTimeoutMillis[(3 * iy) + ix] > 0) gB.display.drawBitmap(xNow, yNow, moleIcon);
      else gB.display.drawBitmap(xNow, yNow, holeIcon);

      //draw hammer
      if ((ix == hammerX) && (iy == hammerY)) {
        gB.display.drawRect(xNow + gamepad_hammerOffsetX, yNow + gamepad_hammerOffsetY, gamepad_hammerWidth, gamepad_hammerHight);
      }

      //display score and lives
      gB.display.setFont(gamepad_font);
      gB.display.cursorX = gamepad_scoreX;
      gB.display.cursorY = gamepad_scoreStringY;
      gB.display.print(gamepad_scoreStringText);
      gB.display.cursorX = gamepad_scoreX;
      gB.display.cursorY = gamepad_scoreY;
      gB.display.print(score);
      gB.display.cursorX = gamepad_scoreX;
      gB.display.cursorY = gamepad_levelStringY;
      gB.display.print(gamepad_levelStringText);
      gB.display.cursorX = gamepad_scoreX;
      gB.display.cursorY = gamepad_levelY;
      gB.display.print(level);
      gB.display.cursorX = gamepad_scoreX;
      gB.display.cursorY = gamepad_lifesStringY;
      gB.display.print(gamepad_lifesStringText);
      gB.display.cursorX = gamepad_scoreX;
      gB.display.cursorY = gamepad_lifesY;
      gB.display.print(lifes);
    }
  }
}

void checkTimoutMoles() {
  int32_t millisNow = millis();
  for (int8_t it = 0; it < 9; it++) {
    if ((moleTimeoutMillis[it] < millisNow) && (moleTimeoutMillis[it] > 0)) {
      if (lifes > 0) lifes--;
      moleTimeoutMillis[it] = 0;
    }
  }
}

int8_t positive_modulo(uint8_t i, int8_t n) {
  return (i % n + n) % n;
}

void randGenMoles() {
  if (millis() > level_timeNextMole) {
    int16_t timeInterval = level_nowMaxTimeNextMole - level_nowMinTimeNextMole;
    int16_t randTimeInterval = rand_counter % timeInterval;
    level_timeNextMole = millis() + constrain((level_nowMinTimeNextMole), level_nowMinTimeNextMole, level_nowMaxTimeNextMole);
    if (!moleTimeoutMillis[rand_counter % 9]) moleTimeoutMillis[rand_counter % 9] = millis() + level_moleTimeoutTimeNow;
  }
}

void clickMole() {
  if (moleTimeoutMillis[(3 * hammerY) + hammerX] > 0) {
    moleTimeoutMillis[(3 * hammerY) + hammerX] = 0;
    score++;
    level_scoreSinceLastLevel++;
    setLevel();
  } else {
    if (lifes > 0) lifes--;
    moleTimeoutMillis[(3 * hammerY) + hammerX] = 0;
  }
}

void setLevel() {
  if (level_scoreSinceLastLevel >= level_scoreLevelUpNow) {
    level++;
    level_scoreSinceLastLevel = 0;
    level_scoreLevelUpNow = level_initalScoreLevelUp + int16_t((double)level * level_addScoreLevelUp);

    level_nowMinTimeNextMole -= level_substMinTimeNextMole;
    if (level_nowMinTimeNextMole < level_endMinTimeNextMole) level_nowMinTimeNextMole = level_endMinTimeNextMole;
    level_nowMaxTimeNextMole -= level_substMaxTimeNextMole;
    if (level_nowMaxTimeNextMole < level_endMaxTimeNextMole) level_nowMaxTimeNextMole = level_endMaxTimeNextMole;
    if (level_nowMaxTimeNextMole < level_nowMinTimeNextMole) level_nowMaxTimeNextMole = level_nowMinTimeNextMole;  //let max never be smaler than min
    level_moleTimeoutTimeNow -= level_substMoleTimeoutTime;
    if (level_moleTimeoutTimeNow < level_endMoleTimoutTime) level_moleTimeoutTimeNow = level_endMoleTimoutTime;
  }
}

void gameOver() {
  gB.display.setColor(GRAY);
  gB.display.fillRect(gameover_squareStartX, gameover_squareStartY, gameover_squareWidth, gameover_squareHeight);
  gB.display.setColor(BLACK);
  gB.display.drawRect(gameover_squareStartX, gameover_squareStartY, gameover_squareWidth, gameover_squareHeight);
  gB.display.cursorX = gameover_x;
  gB.display.cursorY = gameover_y1;
  gB.display.setFont(gameover_font);
  gB.display.print(gameover_text1);
  gB.display.cursorX = gameover_x;
  gB.display.cursorY = gameover_y2;
  gB.display.print(gameover_text2);
}
