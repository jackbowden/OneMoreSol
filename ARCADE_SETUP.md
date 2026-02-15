# Arcade Quarter-Operated Credit System

This document describes the arcade coin acceptor integration for One More Sol.

## Features

- **Insert Coin to Start**: Classic arcade attract mode requiring a quarter to begin
- **Continue Countdown**: 10-second countdown after death to insert coin and continue
- **Credit Tracking**: Visual display of available credits
- **Hardware Support**: Serial coin acceptor integration for production cabinets

## Development (macOS)

### Requirements
- **SFML 2.x** (NOT SFML 3.x)
- CMake 3.16+
- C++17 compiler

### Install SFML 2
```bash
# If you have SFML 3 installed, remove it first
brew uninstall sfml

# Install SFML 2
brew install sfml@2

# Link SFML 2
brew link sfml@2 --force
```

### Build
```bash
cd build
cmake ..
make
./main
```

### Testing Credits

Press **'C'** key to insert a coin (quarter) during development.

**Game Flow:**
1. Game starts at "INSERT COIN" screen
2. Press 'C' to add a credit
3. Press Space to start game (consumes 1 credit)
4. When you die, 10-second continue countdown begins
5. Press 'C' during countdown to continue at same level (consumes 1 credit)
6. If countdown expires with no credits, returns to "INSERT COIN" and resets to level 1

## Production (Linux Arcade Cabinet)

### Hardware Setup

**Recommended Coin Acceptor:**
- Coin Controls NRI G-13 (~$25) - industry standard mechanical quarter acceptor
- Alternative: Suzo-Happ comparable model

**Connection Options:**

**A) USB-Serial Adapter (Recommended for portability)**
- FTDI USB-to-TTL serial cable (~$10)
- Wire coin acceptor pulse output to serial RX pin
- Configure acceptor for 1 pulse per quarter

**B) GPIO (Raspberry Pi or PC/104 boards)**
- Direct connection to GPIO pins
- Use voltage divider if acceptor outputs 12V (GPIO needs 3.3V)
- Requires modifying `CoinReader.cpp` to use GPIO instead of serial

### Software Setup

**1. Install Dependencies**
```bash
# Debian/Ubuntu
sudo apt-get install libsfml-dev libserialport-dev cmake build-essential

# Arch Linux
sudo pacman -S sfml libserialport cmake gcc
```

**2. Configure Serial Port**

Edit [src/CoinReader.cpp](src/CoinReader.cpp) line 94 to match your hardware:
```cpp
SerialCoinReader(const char* portName = "/dev/ttyUSB0");
```

Common serial port names:
- `/dev/ttyUSB0` - USB-serial adapter
- `/dev/ttyS0` - Built-in COM port
- `/dev/ttyACM0` - Arduino/microcontroller

**3. Build for Production**
```bash
mkdir build
cd build
cmake ..
make
```

**4. Test Hardware**

Run the game and insert a physical quarter - you should see console output:
```
[CoinReader] Serial mode enabled on /dev/ttyUSB0
[CoinReader] COIN INSERTED! Credits: 1
```

**5. Deploy**

Copy `main` executable and `../assets/` folder to arcade cabinet. Run:
```bash
./main
```

For kiosk/fullscreen mode, launch with window manager settings or:
```bash
# Example for X11 fullscreen
export DISPLAY=:0
./main &
wmctrl -r "One More Sol" -b add,fullscreen
```

### Hardware Wiring

**USB-Serial Connection:**
```
Coin Acceptor          FTDI Adapter
  Pulse Out  --------> RX
  Ground     --------> GND
  +12V       --------> (external power supply)
```

**Serial Protocol:**
- Pulse acceptor sends 0x01 byte per quarter inserted
- Baud rate: 9600, 8N1 (8 data bits, no parity, 1 stop bit)
- Non-blocking reads won't slow game loop

### Troubleshooting

**"Failed to open serial port"**
- Check permissions: `sudo usermod -a -G dialout $USER` (logout/login after)
- Verify device: `ls -l /dev/ttyUSB*`
- Test with: `screen /dev/ttyUSB0 9600`

**Coin inserted but no credit**
- Check console output for `[CoinReader]` messages
- Verify acceptor is powered (12V DC typically)
- Test acceptor with multimeter - pulse output should go high when coin inserted
- Check byte value in [CoinReader.cpp](src/CoinReader.cpp#L148) matches your hardware

**Multiple credits per coin**
- Acceptor may be sending multiple pulses
- Adjust DIP switches on acceptor for single pulse mode
- Add debouncing in code if needed

## Credit Pricing

Current implementation: **1 credit = 1 quarter ($0.25)**

To change pricing, modify the credit logic in [GameStateManager.cpp](src/GameStateManager.cpp):
- Single credit start, multiple credits to continue
- Multiple credits to start (2 quarters = $0.50)
- Add "ADD CREDITS" option at start screen for pre-loading

Average playtime: ~5-15 minutes per credit depending on skill.

## Code Architecture

### Key Files

- **[include/CoinReader.h](include/CoinReader.h)** - Abstract coin reader interface
- **[src/CoinReader.cpp](src/CoinReader.cpp)** - Keyboard (dev) and serial (production) implementations
- **[include/GameStateManager.h](include/GameStateManager.h)** - Added credit tracking and new game states
- **[src/GameStateManager.cpp](src/GameStateManager.cpp)** - InsertCoin and Continue views
- **[src/main.cpp](src/main.cpp)** - Updated flow to start with InsertCoin state
- **[CMakeLists.txt](CMakeLists.txt)** - Added libserialport dependency

### State Machine

```
InsertCoin (attract mode)
    ↓ (credit inserted, Space pressed)
Play (gameplay)
    ↓ (player dies)
Continue (10s countdown)
    ↓ (credit inserted)
Play (resume at same level)
    ↓ (countdown expires, no credits)
InsertCoin (reset to level 1)
```

### Platform Differences

macOS (Development):
- Uses `KeyboardCoinReader` - 'C' key simulates quarters
- No libserialport dependency
- Compiled with `-D__APPLE__`

Linux (Production):
- Uses `SerialCoinReader` - reads actual coin acceptor
- Requires libserialport-dev
- Reads from `/dev/ttyUSB0` or configured serial port

## Future Enhancements

Potential additions (not implemented):
- High score persistence (save top scores to file)
- Operator menu (press F1-F5 to add free credits, view earnings stats)
- Bill acceptor support (accept $1, $5 bills)
- Attract mode gameplay demo (AI playing in background)
- Audit log (track total credits inserted, games played, revenue)
- Time-based play option (1 quarter = 5 minutes)
- MDB protocol support (vending machine standard, more complex hardware)

## Support

For hardware recommendations or integration questions, document your:
- Coin acceptor model
- Connection method (USB, serial, GPIO)
- Console output from `[CoinReader]` logs
- Acceptor manual/datasheet if available
