# KridaSetu 🏛️🎲

### Traditional Games. Modern Experience.

KridaSetu is an interactive, technology-enabled traditional play system designed to bring Indian traditional games, physical play and cultural storytelling into a screen-free experience.

The idea is simple: **use technology to make traditional play more engaging, not to replace the play itself.**

---

## 🎯 Problem

Traditional Indian games encourage physical movement, social interaction, counting, planning and decision-making while carrying cultural and historical value.

However, many traditional games are becoming less familiar to younger generations as screen-based entertainment becomes more common.

At the same time, many existing smart gaming products depend on smartphones/tablets or are built around a single game.

KridaSetu aims to bridge this gap by combining the **physical and social nature of traditional games** with useful technology.

---

## 💡 Solution

KridaSetu uses an **Active Core + Passive Game Board** architecture.

A reusable smart core contains the main electronics, sensing, audio and game-control system. Different traditional game boards can be used with the same core.

The child continues to physically:

- Roll dice
- Move game pieces
- Count positions
- Make decisions
- Follow rules
- Interact with other players

Technology works in the background to detect interactions and provide feedback.

---

## 🧠 Key Innovation — Dynamic Spatial Game Mapping

The same sensing hardware can be interpreted differently for different games through firmware.

Hall-effect sensors detect magnetic game-piece movements through the playing surface. The ESP32 processes these sensor inputs according to the selected game.

This creates a reusable platform where the **hardware remains largely the same while the game mapping and logic can change through firmware**.

The concept is intended to support traditional games such as:

- Pachisi
- Chaupar
- Moksha Patam
- Chowka Bara
- Navakankari
- Puli Meka

---

## 👀 Screen-Free Interaction

KridaSetu is designed as an **eyes-up play experience**.

Instead of requiring a phone or tablet during gameplay, the system can provide feedback through:

- 360° ambient LED lighting
- Audio feedback
- Game instructions
- Regional-language narration
- Traditional storytelling and music

This keeps children's attention on the physical board and on the people playing with them.

---

## ⚙️ Prototype

The current ESP32 firmware demonstrates the core hardware-control layer of the KridaSetu prototype.

### Current prototype features

- ESP32-based control
- Hall-effect sensor detection
- Multiplexer-based sensor scanning
- Game identification
- Magnetic piece detection
- WS2812B LED feedback
- DFPlayer Mini audio feedback
- Confirmation-button interaction
- Serial sensor monitoring
- Offline operation

### Current game identification

The prototype firmware currently identifies:

| Game ID | Game |
|---:|---|
| 1 | Vaikuntapalli |
| 2 | Pachisi |
| 3 | Puli Meka |

The firmware can be extended with additional game mappings and rules.

---

## 🔌 Hardware

The prototype uses readily available components:

- **ESP32** – main controller
- **Hall-effect sensors** – magnetic piece detection
- **Multiplexers** – read multiple sensor channels
- **WS2812B LEDs** – visual feedback
- **DFPlayer Mini** – audio playback
- **Speaker** – audio output
- **Push button** – confirmation input
- **Magnetic game pieces** – physical interaction
- **Interchangeable game boards** – traditional gameplay

The current BOM-based target for the smart core is approximately **₹1,300**, subject to final component selection and production scale.

---

## 🔄 How It Works

```text
Traditional Game Board
          ↓
     Game Identification
          ↓
         ESP32
          ↓
     Multiplexer
          ↓
     Hall Sensors
          ↓
   Piece Detection
          ↓
    Game Processing
       ↙       ↘
     LEDs      Audio
Basic flow
Select and place a traditional game board.
The system identifies the selected game.
Children play using physical pieces and dice.
Hall sensors detect magnetic piece movement.
ESP32 processes the sensor information.
LEDs and audio provide feedback.
🔐 Privacy-Conscious Design

KridaSetu is designed to operate without requiring cameras, microphones or internet connectivity for basic gameplay.

The current prototype processes sensor information locally on the ESP32.

Future gameplay analytics can also be designed around locally processed sensor events instead of collecting images or recordings of children.

🏛️ Cultural & Educational Value

KridaSetu is designed around Indian traditional games and cultural heritage.

Regional audio, storytelling and traditional music can be integrated to make gameplay more culturally engaging while preserving the physical nature of the games.

The concept also supports the broader direction of NEP 2020 towards play-based and toy-based learning and the use of Indian and local cultural resources.

🌱 Future Scope

Future development can include:

Additional traditional Indian games
Expanded dynamic game mappings
Complete game-specific rule validation
Regional-language audio
Traditional storytelling content
Gameplay analytics
More game-board designs
School and Anganwadi deployment
📹 Prototype Demonstration

Working Prototype Video:
[Add Google Drive Video Link]

The video demonstrates the physical prototype and its hardware interaction.

💻 Source Code

ESP32 Firmware:
[Add GitHub Repository Link]

The repository contains the Arduino/ESP32 firmware used for:

Hall sensor scanning
Game identification
LED control
Audio control
Piece detection
Basic gameplay interaction
🏆 Project Information

Project: KridaSetu
Category: Hardware
Theme: Toys & Games
SIH 2026 Problem Statement: Development of unique toys and games based on civilization, history and culture
    Game Processing
       ↙       ↘
     LEDs      Audio
