#ifndef HTTPCLIENT_ESP32_AT_H_
#define HTTPCLIENT_ESP32_AT_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

class HttpClient_ESP32_AT
{
 private:
    // SoftwareSerial rx and tx pins
    uint32_t m_rxPin;
    uint32_t m_txPin;

    // AT command serial interface for ESP32
    Stream *m_serial; // base class of SoftwareSerial and HardwareSerial

 public:
    // Three types of the constructors:
    // - Create SoftwareSerial inside the constructor
    //   rxPin: Wire this to Tx Pin of ESP32
    //   txPin: Wire this to Rx Pin of ESP32
    HttpClient_ESP32_AT(uint32_t rxPin, uint32_t txPin, uint32_t baud = 115200);
    // - Construct from SoftwareSerial or HardwareSerial
    HttpClient_ESP32_AT(SoftwareSerial &serial);
    HttpClient_ESP32_AT(HardwareSerial &serial);

    // Destructor
    ~HttpClient_ESP32_AT();

 public:
    // Health check of the serial interface
    bool statusAT();

    // Health check of the WiFi connection
    bool statusWiFi();

    // Connect/Disconnect ESP32 to/from WiFi network
    bool connectAP(const String& ssid, const String& password);
    bool disconnectAP();

 private:
    // Clear rx buffer
    void rxClear();

    // Restart (Reset) ESP32
    bool restart();

    // Check the response for the last AT command is "OK"
    bool checkATResponse(String target = "OK", uint32_t timeout = 1000);
    bool checkATResponse(String *buf, String target = "OK", uint32_t timeout = 1000); // store the data into buffer

    // Get IPSTATUS of ESP32
    //   2: ESP32 station connected to an AP and has obtained IP
    //   3: ESP32 station created a TCP or UDP transmission
    //   4: the TCP or UDP transmission of ESP32 station disconnected
    //   5: ESP32 station did NOT connect to an AP
    uint8_t ipStatus();

 public:
    // HTTP GET and POST
    // - return true if successful, else error
    // - SSL/TLS is supported but command format are different from original library
    bool get(const String& protocol, const String& host, const String& path, uint32_t port = 80);
    bool post(const String& protocol, const String& host, const String& path, const String& body,
              const String& contentType = "application/x-www-form-urlencoded", uint32_t port = 80);

    // HTTP response status code of the last request
    //   -1: timeout error (request data was NOT sent)
    //    0: response parse error (request data sent SUCCESSFULLY, but response data was corrupted)
    //   else (>0): HTTP status code (200, 404, 500, 302,.. etc.)
    int responseStatusCode();

 private:
    // Create or Destroy TCP connection
    bool connectTcp(const String& host, uint32_t port);
    bool disconnectTcp();
    bool connectedTcp(); // true if TCP connection exists

 private:
    // Create or Destroy SSL connection
    bool connectSsl(const String& host, uint32_t port);
    bool disconnectSsl();
    bool connectedSsl(); // true if SSL connection exists

 private:
    // Common HTTP request interface for GET and POST
    bool sendRequest(const String& protocol, const String& method,
                     const String& host, uint32_t port, const String& path,
                     const String& contentType = "", const String& body = "");
    int m_responseStatusCode;
};

#endif // #ifndef HTTPCLIENT_ESP32_AT_H_
