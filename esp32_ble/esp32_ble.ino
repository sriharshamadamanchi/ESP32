#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEClient.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SERVICE_UUID "0000fee7-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID "0000fec6-0000-1000-8000-00805f9b34fb"

BLEClient* pClient = NULL;
static boolean performOperation = false;
static boolean connected = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice = NULL;
String command;

WiFiClientSecure wifiClientSecure;

const char* ssid = "JioFiber-8XN93";
const char* password = "mvss@1116";

const char* accessToken = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJwYXJ0bmVySWQiOjIsImRldmljZU1hYyI6IkVBOkRGOjYyOkVBOkJFOkE1Iiwicm9sZSI6IkFETUlOIiwiaWF0IjoxNzE3NDg3NDU5LCJleHAiOjE3MTc1NzM4NTl9.KErNU06bo_1DqaTS04dqYba04_AcMI7-Z4dstC_NRg0";

const char* awsEndpoint = "a3oibjuw7emgs5-ats.iot.us-east-1.amazonaws.com";

const char* certificate =

  "-----BEGIN CERTIFICATE-----\n"
  "MIIDWTCCAkGgAwIBAgIUGfnQd9htO/xS/S9B1sJWurSWPwUwDQYJKoZIhvcNAQEL\n"
  "BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"
  "SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MDYwNDA2MjQz\n"
  "MloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"
  "ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALwuJSrAziI0AzjWzmfQ\n"
  "YJA+Labjeu4Yx6imNt+HwQz3w+qL/Otf/oJQNABUM/c7Uw7Cv2Cb1XkA1aakEMtM\n"
  "yFRuV+78opElNv0eiRPMngC0+3WXbK5L53B+p595wg7L28iY/ojeO2a06OdLqaHo\n"
  "fghHkZVfFvFImqVhLrWO8e8hKKSER9IMuPKjbYMYb1Uwpyg8/xjm+7bcWxyCv+GC\n"
  "ojOUaFY83QD5YecBeshP6KZh8W3JAKS4+VaQ3yqUcA6CrloLVDwU+GKWonNE6cL9\n"
  "m8fZpEkZTmnW4AYwdoJiEp/eCD6InEE4LGvbipbOXQf7fRYcW1d9r2tBckO5m7F6\n"
  "QOsCAwEAAaNgMF4wHwYDVR0jBBgwFoAUvYPWYof3Q3+rEDwtvGRmwna5go0wHQYD\n"
  "VR0OBBYEFE+UM5vLFqAfJPzBbxSYwZ240NEqMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"
  "AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQDCAslGwY+Gfp5tLmzrp8wwtgSp\n"
  "idEYJKl5+sfOVFfbBfv40KSdXsm0p+OO2R/T8ADeOXzdIzEBntc0/R8CHKHMMYUi\n"
  "Q/SfDu8Xu+JrE05hYnZn/OTBL5i9NCvy/0OMi2SJuMOSBYcZt6/TTIkUp25C/E1d\n"
  "23q/zmFatqc8H1vjteULBwr8L86JQ5TFZ/NlbCVGZkNvzzxUTVX7QS1TytXNO29H\n"
  "wtXcPDXiiQjfKO/D8SgxVTNVxvNM+2YRebtsosDqwf+SXhV19DUk+8hfQZfJyfAw\n"
  "T019wYWzIiZfq27d+65QWlCdA+VOwbTUDtaVwqcEv99ot58uvrp+tFouj9CP\n"
  "-----END CERTIFICATE-----\n";

const char* privateKey =

  "-----BEGIN RSA PRIVATE KEY-----\n"
  "MIIEowIBAAKCAQEAvC4lKsDOIjQDONbOZ9BgkD4tpuN67hjHqKY234fBDPfD6ov8\n"
  "61/+glA0AFQz9ztTDsK/YJvVeQDVpqQQy0zIVG5X7vyikSU2/R6JE8yeALT7dZds\n"
  "rkvncH6nn3nCDsvbyJj+iN47ZrTo50upoeh+CEeRlV8W8UiapWEutY7x7yEopIRH\n"
  "0gy48qNtgxhvVTCnKDz/GOb7ttxbHIK/4YKiM5RoVjzdAPlh5wF6yE/opmHxbckA\n"
  "pLj5VpDfKpRwDoKuWgtUPBT4Ypaic0Tpwv2bx9mkSRlOadbgBjB2gmISn94IPoic\n"
  "QTgsa9uKls5dB/t9FhxbV32va0FyQ7mbsXpA6wIDAQABAoIBAEFrDVKqij6LtF8L\n"
  "FeAzKY6bfwVYaYDiLm6rscHNkMgwTfR4y+kLiw/ET/+ZRiN5hStJenxcUD7rklqH\n"
  "CKuUOsRXQ8x6GQ26UT53YhH2/SqjwBMmHAsPqhqKfT1Md7DrH7HBtiLglufilmDq\n"
  "BcOabk30i2DRGMmygshhW3qUgfiuBYNAspJmA7cSh0R5NQ9WIFGnEgnpKCKEosnI\n"
  "kZiPrk9w18dkvZeYdCwQXYqTp+So+NdbFuwI6uz5XKcBNkc9SI/AdzCMyZXwHhwa\n"
  "qtpc9rBi61sxnaSY7ZNHbICrbm/HD83qCs6/frg/j1jF5ZLKcTIjMYtWquQsCapR\n"
  "GdL+6MkCgYEA+NRKlCyAZybwC3Sqyw7IamYfw9o41UYd+/nRj/aYyq8KmODY9SrC\n"
  "gwjyzVL5airWKAcfwQTpRCROfcJbdD97V+OJJum3fmOdyggG2r7PqAKd7ZRnMDEf\n"
  "lVx5qLUvRzfVB/f34MEIgo7huvolspoTJI1n8aH0YiyrvLrwEhvOm0UCgYEAwZps\n"
  "RIoixjcvk1+EMtCDJCoGRID+305c2NgRHqVP0GdYJI+N2febyaiN9+8mZZQ+yfhr\n"
  "zfR385rSjF8FfrsRANoei0q/QS0lXRit86H8ekFsQg8aw9JptpkSqhGPzCVI+ZFY\n"
  "fGNQ/E7SBBraSDFMS1A8udGrhn74bzOPuW2pFm8CgYAlJNOJMOdt/oMAwsaoDynd\n"
  "sCb3voCTXacmMRGWUvJV9zE2xwG7/0kP9GsCOWuf7IsRaJbV4lzL3juVTEkInl1U\n"
  "LlQP6f2JMWehe/dB+g/XfaSDjY6lw/959Vb31ywajnwPd3WaKFaT3q5VkffEpHlN\n"
  "EIq+qDVp3k84mhMyMNJznQKBgQDA+5w2ifd12+apoYmWWXCPqPBMcsUcdPFqGZ6f\n"
  "DKvNkmYzM4FzpzT8Q+8YGKQOl0Irc1yKlMjI5ChCUimZFQEy9zmBIYYZgtX+wa4L\n"
  "8AlLGFHAW2wTjHdh053eNtk+5l+infY1L463oh4Ih3fUhIeHfzi5BT5UN9aOijgW\n"
  "3y+wMwKBgGipvfto2QQCCycyJ/LpHrEJ+GRzjq6j60vzHAlCvWjeNtWWYPVJeZ9Q\n"
  "Nn1xFHCo2fL/UPfTzi9pYlgLAMZrbKE5eHRq2U+3bq3www5MQTCOjhj8BbCVEVXG\n"
  "XEJs/tiWmCeSwd7ZI1ZN1LRskSGpfRmEdsSbE3Vfvo8BZ5QrWRcq\n"
  "-----END RSA PRIVATE KEY-----\n";

const char* rootCA =

  "-----BEGIN CERTIFICATE-----\n"
  "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n"
  "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"
  "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n"
  "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"
  "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"
  "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"
  "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"
  "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"
  "VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"
  "93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"
  "jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n"
  "AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n"
  "A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n"
  "U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n"
  "N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n"
  "o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n"
  "5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n"
  "rqXRfboQnoZsG4q5WTP468SQvvG5\n"
  "-----END CERTIFICATE-----\n";

void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wifiClientSecure);

const char* publish_topic = "publish/esp32_ble";
const char* subscribe_topic = "subscribe/esp32_ble";

class ClientCallbacks : public BLEClientCallbacks {
  void onConnect(BLEClient* pClient) {
    connected = true;
    Serial.println(F("Client Connected *****"));
  }

  void onDisconnect(BLEClient* pClient) {
    Serial.println(F("Client Disconnected *****"));
  }
};

class AdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.println(F(advertisedDevice.toString().c_str()));

    if (advertisedDevice.getAddress().toString() == "ea:df:62:ea:be:a5") {
      Serial.println(F("Found smart bolt device"));
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
    }
  }
};

bool connectToServer() {
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new ClientCallbacks());
  Serial.println(" - Created client");

  if (pClient->connect(myDevice)) {
    Serial.println(F(" - Connected to device"));
    connected = true;
  } else {
    Serial.println(F(" - Failed to connect to device"));
  }

  return connected;
}

void connectToWiFi() {
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(F("Connecting to WiFi..."));
  }
  Serial.print(F(", WiFi connected, IP address: "));
  Serial.println(WiFi.localIP());
}

void hexStringToByteArray(const String& hexString, uint8_t* byteArray, size_t arraySize) {
  for (size_t i = 0; i < arraySize; i++) {
    byteArray[i] = (uint8_t)strtol(hexString.substring(i * 2, i * 2 + 2).c_str(), NULL, 16);
  }
}

void connectAWSIOTCore() {
  wifiClientSecure.setCACert(rootCA);
  wifiClientSecure.setCertificate(certificate);
  wifiClientSecure.setPrivateKey(privateKey);
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("Starting ESP32 BLE..."));
  connectToWiFi();
  connectAWSIOTCore();
  BLEDevice::init("BLE Initialized");
}

void scan() {
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new AdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(10, false);
}

void executeBLE() {
  connectToServer();

  if (connected && !command.isEmpty()) {
    if (pClient->isConnected()) {
      BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
      if (pRemoteService != nullptr) {
        pRemoteCharacteristic = pRemoteService->getCharacteristic(CHARACTERISTIC_UUID);
        if (pRemoteCharacteristic != nullptr) {
          size_t commandLength = command.length();
          size_t arraySize = commandLength / 2;
          uint8_t data[arraySize];
          hexStringToByteArray(command, data, arraySize);

          if (pRemoteCharacteristic->canWrite()) {
            int maxLength = 20;
            int dataLength = sizeof(data);

            if (dataLength <= maxLength) {
              pRemoteCharacteristic->writeValue(data, dataLength);
            } else {
              for (int i = 0; i < dataLength; i += maxLength) {
                int chunkSize = min(maxLength, dataLength - i);
                pRemoteCharacteristic->writeValue(data + i, chunkSize);
                delay(200);
              }
            }
            Serial.println(F("Written to characteristic"));
          } else {
            Serial.println(F("Characteristic not writable"));
          }
        } else {
          Serial.println(F("Characteristic not found"));
        }
      } else {
        Serial.println(F("Service not found"));
      }
      connected = false;
    }
  }
}

void reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("Reconnecting to WiFi..."));
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    unsigned long startAttemptTime = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(100);
    }

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println(F("Failed to reconnect to WiFi"));
    } else {
      Serial.println(F("Reconnected to WiFi"));
    }
  }
}


void loop() {

  reconnectWiFi();

  pubSubCheckConnect();

  if (performOperation && myDevice != NULL) {
    performOperation = false;
    executeBLE();
  }
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on ");
  Serial.print(topic);
  Serial.print(": ");

  String payloadString;

  for (int i = 0; i < length; i++) {
    payloadString += (char)payload[i];
  }

  Serial.println(payloadString);

  myDevice = NULL;
  command = payloadString;
  performOperation = true;

  scan();
}

static int count = 0;

void pubSubCheckConnect() {
  if (WiFi.status() == WL_CONNECTED && !pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: ");
    Serial.println(awsEndpoint);
    while (!pubSubClient.connected()) {
      Serial.print("Current count: ");
      Serial.println(count);
      if (count > 2) {
        ESP.restart();
      }
      count++;
      pubSubClient.connect("ESP32 BLE");
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe(subscribe_topic);
  }
  pubSubClient.loop();
}