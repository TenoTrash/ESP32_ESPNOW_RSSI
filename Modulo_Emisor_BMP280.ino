/*********
  Marcelo Ferrarotti 2021
  Proyecto de emisor y receptor para tres entradas lógicas (para contacto seco) en emisor y tres salidas lógicas de estado en receptor
  Usando protocolo ESP NOW de Espressif
  MODULO EMISOR con BMP280 por I2C
*********/

#include <esp_now.h>
#include <WiFi.h>

#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>

#define SEALEVELPRESSURE_HPA (1025.00) //Presión atmosferica promedio de Buenos Aires

Adafruit_BMP280 bmp; // I2C
float temperature;
float humidity;
float pressure;

#define conector_1  18
#define conector_2  19
#define conector_3  5

// MAC Address del ESP32 placa B "receptor"
uint8_t broadcastAddress[] = {0xB4, 0xE6, 0x2D, 0xC1, 0x59, 0x0D};

// Variable to add info about peer
esp_now_peer_info_t peerInfo;

// Estructura de datos para enviar
typedef struct struct_message {
  float temp;
  float pres;
  bool pulso;
  bool entrada_1;
  bool entrada_2;
  bool entrada_3;
} struct_message;

struct_message paquete_datos;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  pinMode(conector_1, INPUT);
  pinMode(conector_2, INPUT);
  pinMode(conector_3, INPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // Set values to send
  getReadings();
  paquete_datos.pulso = 1;
  paquete_datos.entrada_1 = digitalRead(conector_1);
  paquete_datos.entrada_2 = digitalRead(conector_2);
  paquete_datos.entrada_3 = digitalRead(conector_3);
  paquete_datos.temp = temperature;
  paquete_datos.pres = pressure;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &paquete_datos, sizeof(paquete_datos));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(3000);
}

void getReadings() {
  temperature = bmp.readTemperature();
  pressure = (bmp.readPressure() / 100.0F);
}
