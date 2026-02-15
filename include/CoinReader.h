#ifndef COINREADER_H
#define COINREADER_H

/**
 * Abstract interface for reading coin/credit input from arcade hardware
 * Supports multiple backends: keyboard simulation (dev), serial pulse acceptor (production)
 */
class CoinReader {
public:
    virtual ~CoinReader() = default;
    
    /**
     * Check if a coin has been inserted since last check
     * Non-blocking call, safe for main game loop
     * @return true if new coin detected, false otherwise
     */
    virtual bool checkForCoin() = 0;
    
    /**
     * Get current credit count
     * @return number of credits available
     */
    virtual int getCredits() const = 0;
    
    /**
     * Add credits manually (for testing/operator mode)
     * @param count number of credits to add
     */
    virtual void addCredits(int count) = 0;
    
    /**
     * Decrement credits by one
     * @return true if credit was decremented, false if no credits available
     */
    virtual bool useCredit() = 0;
    
    /**
     * Reset credit count to zero
     */
    virtual void resetCredits() = 0;
};

/**
 * Keyboard-based coin reader for development/testing
 * Press 'C' key to insert a coin (quarter)
 */
class KeyboardCoinReader : public CoinReader {
private:
    int credits;
    bool coinKeyWasPressed;  // Track key state to prevent holding
    
public:
    KeyboardCoinReader();
    ~KeyboardCoinReader() override = default;
    
    bool checkForCoin() override;
    int getCredits() const override;
    void addCredits(int count) override;
    bool useCredit() override;
    void resetCredits() override;
};

#ifndef __APPLE__
/**
 * Serial-based coin reader for production arcade hardware
 * Reads pulse signals from coin acceptor via USB-Serial or direct serial port
 * Compatible with standard pulse-output coin mechanisms (Coin Controls, Suzo-Happ, etc.)
 */
class SerialCoinReader : public CoinReader {
private:
    int credits;
    void* serialPort;  // Opaque pointer to sp_port (libserialport)
    bool isConnected;
    
    bool initializePort(const char* portName);
    void closePort();
    
public:
    /**
     * @param portName Serial port path (e.g., "/dev/ttyUSB0" on Linux, "/dev/cu.usbserial" on Mac)
     */
    SerialCoinReader(const char* portName = "/dev/ttyUSB0");
    ~SerialCoinReader() override;
    
    bool checkForCoin() override;
    int getCredits() const override;
    void addCredits(int count) override;
    bool useCredit() override;
    void resetCredits() override;
    
    bool isPortConnected() const { return isConnected; }
};
#endif // __APPLE__

#endif // COINREADER_H
