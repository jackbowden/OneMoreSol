#include "CoinReader.h"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

#ifndef __APPLE__
#include <libserialport.h>
#endif

// ============================================================================
// KeyboardCoinReader Implementation (Development/Testing)
// ============================================================================

KeyboardCoinReader::KeyboardCoinReader() 
    : credits(0), coinKeyWasPressed(false) {
    std::cout << "[CoinReader] Keyboard mode enabled - Press 'C' to insert quarter" << std::endl;
}

bool KeyboardCoinReader::checkForCoin() {
    bool coinKeyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
    
    // Edge detection: only trigger on key press, not hold
    if (coinKeyPressed && !coinKeyWasPressed) {
        credits++;
        coinKeyWasPressed = true;
        std::cout << "[CoinReader] COIN INSERTED! Credits: " << credits << std::endl;
        return true;
    }
    
    // Track key release
    if (!coinKeyPressed) {
        coinKeyWasPressed = false;
    }
    
    return false;
}

int KeyboardCoinReader::getCredits() const {
    return credits;
}

void KeyboardCoinReader::addCredits(int count) {
    credits += count;
    std::cout << "[CoinReader] Added " << count << " credits. Total: " << credits << std::endl;
}

bool KeyboardCoinReader::useCredit() {
    if (credits > 0) {
        credits--;
        std::cout << "[CoinReader] Credit used. Remaining: " << credits << std::endl;
        return true;
    }
    return false;
}

void KeyboardCoinReader::resetCredits() {
    credits = 0;
    std::cout << "[CoinReader] Credits reset to 0" << std::endl;
}

// ============================================================================
// SerialCoinReader Implementation (Production Hardware)
// ============================================================================

#ifndef __APPLE__

SerialCoinReader::SerialCoinReader(const char* portName) 
    : credits(0), serialPort(nullptr), isConnected(false) {
    if (initializePort(portName)) {
        std::cout << "[CoinReader] Serial mode enabled on " << portName << std::endl;
    } else {
        std::cerr << "[CoinReader] WARNING: Failed to open serial port " << portName << std::endl;
        std::cerr << "[CoinReader] Coin acceptor will not function!" << std::endl;
    }
}

SerialCoinReader::~SerialCoinReader() {
    closePort();
}

bool SerialCoinReader::initializePort(const char* portName) {
    sp_port** portPtr = reinterpret_cast<sp_port**>(&serialPort);
    
    // Get port by name
    enum sp_return result = sp_get_port_by_name(portName, portPtr);
    if (result != SP_OK) {
        std::cerr << "[CoinReader] Failed to find port: " << portName << std::endl;
        return false;
    }
    
    // Open port
    result = sp_open(*portPtr, SP_MODE_READ);
    if (result != SP_OK) {
        std::cerr << "[CoinReader] Failed to open port" << std::endl;
        sp_free_port(*portPtr);
        serialPort = nullptr;
        return false;
    }
    
    // Configure port: 9600 baud, 8N1 (standard for pulse acceptors)
    sp_set_baudrate(*portPtr, 9600);
    sp_set_bits(*portPtr, 8);
    sp_set_parity(*portPtr, SP_PARITY_NONE);
    sp_set_stopbits(*portPtr, 1);
    sp_set_flowcontrol(*portPtr, SP_FLOWCONTROL_NONE);
    
    // Set non-blocking read with minimal timeout
    isConnected = true;
    return true;
}

void SerialCoinReader::closePort() {
    if (serialPort != nullptr) {
        sp_port* port = *reinterpret_cast<sp_port**>(&serialPort);
        sp_close(port);
        sp_free_port(port);
        serialPort = nullptr;
        isConnected = false;
    }
}

bool SerialCoinReader::checkForCoin() {
    if (!isConnected) {
        return false;
    }
    
    sp_port* port = *reinterpret_cast<sp_port**>(&serialPort);
    unsigned char buffer[32];
    
    // Non-blocking read with 1ms timeout (won't slow down game loop)
    int bytesRead = sp_nonblocking_read(port, buffer, sizeof(buffer));
    
    if (bytesRead > 0) {
        // Simple protocol: any byte received = 1 coin inserted
        // For pulse acceptors wired to send a byte per pulse
        // More sophisticated: check for specific byte values (0x01, 0x05, etc.)
        for (int i = 0; i < bytesRead; i++) {
            if (buffer[i] == 0x01 || buffer[i] == 'Q') {  // 0x01 = pulse, 'Q' = quarter signal
                credits++;
                std::cout << "[CoinReader] COIN INSERTED! Credits: " << credits << std::endl;
                return true;
            }
        }
    }
    
    return false;
}

int SerialCoinReader::getCredits() const {
    return credits;
}

void SerialCoinReader::addCredits(int count) {
    credits += count;
    std::cout << "[CoinReader] Added " << count << " credits. Total: " << credits << std::endl;
}

bool SerialCoinReader::useCredit() {
    if (credits > 0) {
        credits--;
        std::cout << "[CoinReader] Credit used. Remaining: " << credits << std::endl;
        return true;
    }
    return false;
}

void SerialCoinReader::resetCredits() {
    credits = 0;
    std::cout << "[CoinReader] Credits reset to 0" << std::endl;
}

#endif // __APPLE__
