# Microcontroller Chrome Dino Game 🦖

A hardware adaptation of the classic offline Chrome Dinosaur game built with an ESP32-S3 microcontroller, push button, I2C LCD, buzzer, and status LEDs(green and red).

## 🎮 How to Play

* **Start Game:** Press the push button.
* **Jump:** Tap the button to jump over cactuses.
* **Win Condition:** Reach a score of 250 to unlock the victory sequence!
* **Game Over:** Hitting a cactus triggers the red LED and loss sound, then resets the game.

## 🛠️ Hardware Requirements

* ESP32-S3 Microcontroller
* 1602 LCD Display (I2C Module)
* Push Button(for jumping)
* Piezo Buzzer(for sound)
* Green LED (Win indicator)
* Red LED (Game Over indicator)
* Resistors (220Ω) for LEDs
* Breadboard & Jumper Wires


## 🔌 Pin Wiring Summary

* **LCD SDA:** GPIO 8
* **LCD SCL:** GPIO 9
* **Push Button:** GPIO 17
* **Buzzer:** GPIO 4
* **Green LED:** GPIO 16
* **Red LED:** GPIO 15

## 🚀 How to Build & Flash

1. Open code in VS Code / PlatformIO.
2. Connect hardware based on pin assignments.
3. Upload to the board and start playing!