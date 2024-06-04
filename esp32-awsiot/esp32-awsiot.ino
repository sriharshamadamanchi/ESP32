#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <WiFi.h>

const char* ssid = "Harsha";
const char* password = "Harsha@123";

const char* awsEndpoint = "agqq2gyows7fg-ats.iot.us-east-1.amazonaws.com";

const char* certificate_pem_crt = \

"-----BEGIN CERTIFICATE-----\n" \
  "MIIDWjCCAkKgAwIBAgIVAIjyZZfKmJ7jbv5+bm1OXkQuRGdhMA0GCSqGSIb3DQEB\n" \
  "CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n" \
  "IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNDAyMTcxOTM4\n" \
  "MDFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n" \
  "dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCsscWYlAaI6meUMkF6\n" \
  "kxONoo8Dv27LBsBRo/gGGwKEVOgwmct7IdTMvb7EoJBXmLXrIsPFE++0eMeNbm9D\n" \
  "EPI8GxPMFarK+mtk39L4Xg0117DigekwAJBsBiUYT2qc43noFlQtpoa76OfY2rgP\n" \
  "VBB2k5wVpBPQg7bLaScfM0hCd+HgO7fa7hnw8n+tTGYxXvg119lBaHPBAjAmdVP9\n" \
  "aNjY5h1P960nLG/f6cQzWpP4fW+tqw/oNgrItsGcMDeTpj8qoZQ4rfacmrXPUVpD\n" \
  "TUujBQKfRvFqeiKQU7DeEXUo/FAS3TQUhyQ8FsFq2510UcMtNePBwitwqa0jjc5E\n" \
  "A0rrAgMBAAGjYDBeMB8GA1UdIwQYMBaAFKgRU6JfC+npbfgkwDgR5LAAjuy2MB0G\n" \
  "A1UdDgQWBBS+EbL/0opeT4H9Ht5aprO0ByU0rTAMBgNVHRMBAf8EAjAAMA4GA1Ud\n" \
  "DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAGOUZQp8ocOqSGbrRr51nXkkE\n" \
  "H+OTJ4OrGRW+y6eteeosArldiN+9oe4wVLuByyRFF5efMdGfFiNpC4Mke8f/LkvZ\n" \
  "6CTlDxyySlDuIE4PrRhsgG8KXbIcFUTx65K7Fuxwg3ZTKxuWNHe/7DAEYb95wJXW\n" \
  "gFWKLNiokhZPkD2TtncYDnEbykeKDyWrnvhv/6POm126K3j1Xl9+cU/7ij6oFGnU\n" \
  "4I5SWP9a7Egrhwcvaii+B18zSjUFBwIhhnHLZA3FKROdaZHIxFtDDoxocT7P5ET0\n" \
  "okOTmFC5CRCCyZT+jk58vQ6PRdeZ4kcpxRPbIr68PmSs2tfxqStVZgyJuNivtw==\n" \
  "-----END CERTIFICATE-----\n";

const char* private_pem_key = \

"-----BEGIN RSA PRIVATE KEY-----\n" \
  "MIIEowIBAAKCAQEArLHFmJQGiOpnlDJBepMTjaKPA79uywbAUaP4BhsChFToMJnL\n" \
  "eyHUzL2+xKCQV5i16yLDxRPvtHjHjW5vQxDyPBsTzBWqyvprZN/S+F4NNdew4oHp\n" \
  "MACQbAYlGE9qnON56BZULaaGu+jn2Nq4D1QQdpOcFaQT0IO2y2knHzNIQnfh4Du3\n" \
  "2u4Z8PJ/rUxmMV74NdfZQWhzwQIwJnVT/WjY2OYdT/etJyxv3+nEM1qT+H1vrasP\n" \
  "6DYKyLbBnDA3k6Y/KqGUOK32nJq1z1FaQ01LowUCn0bxanoikFOw3hF1KPxQEt00\n" \
  "FIckPBbBatuddFHDLTXjwcIrcKmtI43ORANK6wIDAQABAoIBAAnYOINyLM0xT6ZL\n" \
  "FyB9seepFK4KA67fO6dryX/Pml0g2furSMZEL3Vce7uJy7Cyq+wzlyVs9YU8PWP6\n" \
  "n60bEjQK9x3zPkFD9UtS1xwBgI9JNHQ1I63w2krOLt8r58/5x8FLRJE1pQihRlY+\n" \
  "Ya++AxI/Z+5z5A1G9Oyzmd9VdREU3GhDcyX7ep8DFuIGG+t5egVhLeU4vV79E016\n" \
  "id1HNuG0Ifxh6APgSN+Z1UgZ6gde7EiZydS9QEf/zwljzqjT+df0qp/fRDS58pAR\n" \
  "k7fYIPR0aKnMrDSxNU80legr0U+GgWh7XcWNiFlkI+HF/Vqo4ENjVNDxskmoKEo1\n" \
  "Qs3AMIECgYEA4x2T4xJdDfShDgYx5mQvun4ZWhJq27THMBmqdLd/m150XNRSk7LV\n" \
  "FZu3DaQ2V9lR9MmCe2AG6rHqdwq6PqRDpK3sgW+WeJ0qQJvvgYr+vwIZ+YtE6+Am\n" \
  "AP0KmIU8YU4zJn155/dbcUv5LJ2jDN+oelRCxThax/PHJVluFKllseMCgYEAwqha\n" \
  "TdVEzveKmVHLJ2+fyiFS1ELRj9f8WcbF+8p/KAlQD2K1yqsjf4zrqdGGwuLOCf+z\n" \
  "GmOE2HdFgU3W/zIVIIWUjdCi/6SRZbapZg2PLgcPaa46nu/rSqkVN0Rud2TwkUJJ\n" \
  "Grb7mlf2TJW5/rhqCyi81eJPmG529CrFeue0MVkCgYAF+JIZxzLRGhxd3QXU0895\n" \
  "bLVH6/IJQrB18EbN+MEPGaWfhymAnpQBlG10m1ueZaVE9UNL8RNdej360TpiSTlK\n" \
  "DBSKtp9THnas6iya0EewR+Lc769pMCRvpDdyGRakTEUF/wiEc7lguimb5a0SupKY\n" \
  "XLzAqKGiXiNfNQGy4108FwKBgAzfcUlgZzKAsEu/Wv36pciKN54N5SnsEKYnPQSP\n" \
  "uaT+KKs7zeXfN3PB7/vE/XJ56UJEBKYkGq7uBEyuxx8NQtRpwQ830u+xBGSDleRH\n" \
  "1UYn9V75xKqqFxdS4zJKdjuzEy2oAomfvDfSxVj/NdOggnLGeOZ0FutvsrIm+fRM\n" \
  "yl/RAoGBAMZC/5UqYorPmqaRK1Vo2KwtWPC6PNPG5LyfugXG44jWuig2+x4GAsBr\n" \
  "z0M7cKEovknbYE4yWXOtw4wZiU5xk+lRvIVbmAoVaRol0VBXLWJ8XVbdkv5BQkQa\n" \
  "iJlUr+eRLqRHDNhBkCo9CV+VLaUdX5rFDz0k0wzGG3SAL2RzXobl\n" \
  "-----END RSA PRIVATE KEY-----\n";

const char* rootCA = \
"-----BEGIN CERTIFICATE-----\n" \
  "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n" 
  "ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n" \
  "b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n" \
  "MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n" \
  "b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n" \
  "ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n" \
  "9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n" \
  "IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n" \
  "VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n" \
  "93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n" \
  "jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n" \
  "AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n" \
  "A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n" \
  "U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n" \
  "N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n" \
  "o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n" \
  "5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n" \
  "rqXRfboQnoZsG4q5WTP468SQvvG5\n" \
  "-----END CERTIFICATE-----";

WiFiClientSecure wiFiClient;
void msgReceived(char* topic, byte* payload, unsigned int len);
PubSubClient pubSubClient(awsEndpoint, 8883, msgReceived, wiFiClient); 

const char* publish_topic = "publish/esp32";
const char* subscribe_topic = "subscribe/esp32";

void setup() {
  Serial.begin(9600); delay(50); Serial.println("");
  Serial.println("ESP32 AWS IoT");
  Serial.printf("SDK version: %s\n", ESP.getSdkVersion());

  Serial.print("Connecting to "); 
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());

  wiFiClient.setCACert(rootCA);
  wiFiClient.setCertificate(certificate_pem_crt);
  wiFiClient.setPrivateKey(private_pem_key);
}

unsigned long lastPublish;
int msgCount;

void loop() {

  pubSubCheckConnect();

   //If you need to increase buffer size, you need to change MQTT_MAX_PACKET_SIZE in PubSubClient.h
   char fakeData[128];

  float var1 =  random(55,77); //fake number range, adjust as you like
  float var2 =  random(77,99);
  
  //long float format, takes up a lot of the allocated char buffer
  // sprintf(fakeData,  "{\"uptime\":%lu,\"temp\":%f,\"humid\":%f}", millis() / 1000, var1, var2);
  
  //shorter float format, saves space, makes AWS services slightly easier
  sprintf(fakeData,  "{\"uptime\":%lu,\"temp\":%.0f,\"humid\":%.0f}", millis() / 1000, var1, var2);
 


  if (millis() - lastPublish > 10000) {
  boolean rc = pubSubClient.publish(publish_topic, fakeData);
    Serial.print("Published, rc="); Serial.print( (rc ? "OK: " : "FAILED: ") );
    Serial.println(fakeData);
    lastPublish = millis();

    
  }
}

void msgReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void pubSubCheckConnect() {
  if (!pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(awsEndpoint);
    while (!pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect("ESPthingXXXX");
      delay(1000);
    }
    Serial.println(" connected");
    pubSubClient.subscribe(subscribe_topic);
  }
  pubSubClient.loop();
}
