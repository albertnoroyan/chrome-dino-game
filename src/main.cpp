#include <Arduino.h>
#include "Wire.h"
#include "LiquidCrystal_I2C.h"

// I2C LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Game Variables
int score = 0;
int last_score = -1;
int spawn_timer = 0;
int win_timer = 0;
int winning_score = 250;
bool game_over_flag = false;
bool game_won_flag = false;
bool winning_sequence = false;
bool last_win_seq = false;
bool is_jumping = false;

// Pins
const int jumpButton = 17;
const int buzzerPin = 4; 
const int greenLedPin = 16;
const int redLedPin = 15;

// Dino Sprites (Left & Right Leg)
byte dino_l[8] = {
  B00000111, B00000101, B00000111, B00010110,
  B00011111, B00011110, B00001110, B00000100
};

byte dino_r[8] = {
  B00000111, B00000101, B00000111, B00010110,
  B00011111, B00011110, B00001110, B00000010
};

// Cactus Sprites
byte cactus_small[8] = {
  B00000100, B00000101, B00010101, B00010101,
  B00010111, B00011100, B00000100, B00000000
};

byte cactus_big[8] = {
  B00000000, B00000100, B00000101, B00010101,
  B00010110, B00001100, B00000100, B00000100
};

// Cup Sprite
byte cup_sprite[8] = {
  B11111, B11111, B01110, B01110,
  B00100, B00100, B01110, B11111
};

// Game World Arrays
char world[32];
char prev_world[32];
byte dino_sprite = 0;

// Sound Functions (Includes noTone to clear electrical noise)
void playJumpSound() {
  tone(buzzerPin, 250, 40);
}

void playGameOverSound() {
  tone(buzzerPin, 300, 150);
  delay(150);
  tone(buzzerPin, 200, 150);
  delay(150);
  tone(buzzerPin, 120, 300);
  delay(300);
  noTone(buzzerPin);
}

void playWinSound() {
  tone(buzzerPin, 800, 100);
  delay(120);
  tone(buzzerPin, 1000, 100);
  delay(120);
  tone(buzzerPin, 1200, 100);
  delay(120);
  tone(buzzerPin, 1500, 300);
  delay(300);
  noTone(buzzerPin);
}

// Noise-Filtered Button Read (Debounced)
bool get_button() {
  if (digitalRead(jumpButton) == LOW) {
    delay(10); // Noise filter delay
    return (digitalRead(jumpButton) == LOW);
  }
  return false; 
}

void reset_game() {
  score = 0;
  last_score = -1;
  spawn_timer = 0;
  win_timer = 0;
  game_over_flag = false;
  game_won_flag = false;
  winning_sequence = false;
  last_win_seq = false;
  is_jumping = false;

  for (int i = 0; i < 32; i++) {
    world[i] = 32;
    prev_world[i] = 255;
  }

  lcd.clear();
}

bool scroll_world() {
  delay(100); 

  if (score >= winning_score && !winning_sequence) {
    winning_sequence = true;
    for (int i = 0; i < 32; i++) {
      if (world[i] == 2 || world[i] == 3) {
        world[i] = 32; 
      }
    }
  }

  for (int i = 16; i < 31; i++) {
    world[i] = world[i + 1];
  }
  world[31] = 32;

  for (int i = 0; i < 15; i++) {
    world[i] = world[i + 1];
  }
  world[15] = 32;

  if (winning_sequence) {
    win_timer++;
    if (win_timer == 18) {
      world[31] = 4; 
    }
  } else {
    spawn_timer++;
    if (spawn_timer > 4) {
      if (random(0, 3) == 0) {
        world[31] = random(2, 4); 
        spawn_timer = 0;
      }
    }
  }

  if (world[17] == 4 || world[16] == 4) {
    game_won_flag = true;
    return false;
  }

  if (!is_jumping && (world[17] == 2 || world[17] == 3)) return true;
  if (is_jumping && (world[1] == 2 || world[1] == 3)) return true;

  return false;
}

void render_screen() {
  dino_sprite = (dino_sprite == 0) ? 1 : 0;

  for (int i = 0; i < 6; i++) {
    char expected_char = (i == 1 && is_jumping) ? dino_sprite : world[i];
    if (expected_char != prev_world[i]) {
      lcd.setCursor(i, 0);
      lcd.write(byte(expected_char));
      prev_world[i] = expected_char;
    }
  }

  if (score != last_score || winning_sequence != last_win_seq) {
    lcd.setCursor(6, 0);
    if (!winning_sequence) {
      lcd.print("Score:");
      if (score < 10) lcd.print("   ");
      else if (score < 100) lcd.print("  ");
      else if (score < 1000) lcd.print(" ");
      lcd.print(score);
    } else {
      lcd.print("          ");
    }
    last_score = score;
    last_win_seq = winning_sequence;
  }

  for (int i = 16; i < 32; i++) {
    char expected_char = (i == 17 && !is_jumping) ? dino_sprite : world[i];
    if (expected_char != prev_world[i]) {
      lcd.setCursor(i - 16, 1);
      lcd.write(byte(expected_char));
      prev_world[i] = expected_char;
    }
  }
}

void handle_game_over() {
  digitalWrite(redLedPin, HIGH);   // Red ON
  playGameOverSound();

  delay(300);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("GAME OVER!");
  lcd.setCursor(3, 1);
  lcd.print("Score: ");
  lcd.print(score);

  delay(1500);
  lcd.setCursor(0, 1);
  lcd.print("   Try again!   ");

  while (!get_button()); 
  delay(200);
  digitalWrite(redLedPin, LOW);
  reset_game();
}

void handle_win() {
  digitalWrite(greenLedPin, HIGH);
  playWinSound();

  delay(300);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("YOU WIN!");
  lcd.setCursor(3, 1);
  
  delay(2000); 
  lcd.setCursor(0, 1);
  lcd.print(" Press to Play! ");

  while (!get_button()); 
  delay(200);
  digitalWrite(greenLedPin, LOW);
  reset_game();
}

void show_start_screen() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("DINO RUNNER");
  lcd.setCursor(1, 1);
  lcd.write(byte(0));
  lcd.write(byte(3));
  lcd.print(" Press Start!");

  while (!get_button());
  delay(200);
  reset_game();
}

void setup() {
  pinMode(jumpButton, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, LOW);
  randomSeed(analogRead(1));

  Wire.begin(8, 9);
  Wire.setClock(100000); 
  Wire.setTimeOut(1000);

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, dino_l);
  lcd.createChar(1, dino_r);
  lcd.createChar(2, cactus_small);
  lcd.createChar(3, cactus_big);
  lcd.createChar(4, cup_sprite);

  show_start_screen();
}

void loop() {
  if (game_over_flag) {
    handle_game_over();
    return;
  }

  if (game_won_flag) {
    handle_win();
    return;
  }

  if (get_button() && !is_jumping && !winning_sequence) {
    is_jumping = true;
    playJumpSound(); 

    for (int jump_frame = 0; jump_frame < 4; jump_frame++) {
      if (scroll_world()) {
        game_over_flag = true;
        break;
      }
      if (game_won_flag) break; 

      if (!winning_sequence) score++; 
      render_screen();
    }
    is_jumping = false;
  }

  if (scroll_world()) {
    game_over_flag = true;
  } else {
    if (!winning_sequence) score++; 
    render_screen();
  }
}