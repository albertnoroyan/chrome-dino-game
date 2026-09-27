# ESP32-S3 Dino Runner Game

A standalone, retro-style arcade game built on the ESP32-S3 microcontroller. Features custom pixel graphics, responsive debounced button controls, delta-rendered LCD graphics to minimize screen flicker, dynamic tone-based audio effects, and status LED indicators.

Features

    Non-blocking Visual & Audio Feedback: Real-time jump sounds, victory jingles, and loss alerts via a piezo buzzer.

    Flawless LCD Delta-Rendering: Only updates altered characters on the 1602 I2C LCD, eliminating display ghosting and I2C bus bottlenecking.

    Dedicated LED Indicators: Win/Loss status indicators driven safely with current-limiting resistors.

    Hardware Noise Debouncing: Software-filtered input checking for clean button presses.

How to Play

    Power On: Connect your ESP32-S3 to power via USB.

    Start Screen: The LCD displays the main title banner: DINO RUNNER - Press Start!. Press the jump button to begin.

    Controls: Press the Jump Button to make the Dino jump over oncoming small and large cacti. Each successfully cleared obstacle increments your score counter on the top row.

    Game Over: Hitting a cactus triggers the Game Over sequence, playing a loss melody and flashing the Red LED. Press the button to restart.

    Winning the Game: Reach a score of 250 to trigger the victory trophy sequence, accompanied by a fanfare jingle and a flashing Green LED.

Hardware Requirements

    1x ESP32-S3 Development Board

    1x 1602 LCD Display with I2C Module (Address 0x27 or 0x3F)

    1x Push Button (Standard momentary switch)

    1x Piezo Buzzer (Passive or Active)

    1x Green LED (Win indicator)

    1x Red LED (Game Over indicator)

    2x Resistors 220 Ohm to 1k Ohm (Mandatory for LED protection)

    1x Breadboard & Jumper Wires

Hardware Wiring & Pinout

Note: Always use current-limiting resistors in series with the LEDs to prevent drawing excessive current from the GPIO pins.

    LCD SDA: GPIO 8

    LCD SCL: GPIO 9

    Jump Button: GPIO 17 (Connected to GND)

    Piezo Buzzer: GPIO 4 (Positive terminal to pin, negative to GND)

    Green LED: GPIO 16 (Anode through 220 Ohm resistor to GPIO 16)

    Red LED: GPIO 15 (Anode through 220 Ohm resistor to GPIO 15)

    Common GND: All negative leads tied to ESP32 GND rail

How to Build & Flash
1. Hardware Assembly

    Connect the I2C LCD (VCC to 5V, GND to GND, SDA to GPIO 8, SCL to GPIO 9).

    Wire the Push Button between GPIO 17 and GND.

    Wire the Buzzer positive leg to GPIO 4 and negative leg to GND.

    Connect the Green LED positive leg through a 220 Ohm resistor to GPIO 16 and the short lead to GND.

    Connect the Red LED positive leg through a 220 Ohm resistor to GPIO 15 and the short lead to GND.

2. Software Setup

    Download and install Arduino IDE.

    Add ESP32 board support to Arduino IDE via Boards Manager (search for esp32 by Espressif Systems).

    Go to Sketch > Include Library > Manage Libraries... and install LiquidCrystal_I2C (by Frank de Brabander).

3. Uploading Code

    Select your board under Tools > Board > ESP32 Arduino > ESP32S3 Dev Module.

    Select your connected USB serial port under Tools > Port.

    Copy the project source code into your sketch.

    Click the Upload button.

License

This project is open-source and available under the MIT License.
