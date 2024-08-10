/*
Author: L3nny_P34s4n7
Email: contact@leonardovichi.com
Date: 2024-08-10
License: MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

CONNECTIONS
-----------
SSD1306 OLED:
SDA > SDA or A4
SCL > SCL or A5

PN532 RFID NFC
SDA > SDA or A4
SCL > SCL or A5

 */

#include <Wire.h>
#include <Adafruit_PN532.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET    -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  // Endereço I2C padrão para a tela OLED

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Definindo os pinos utilizados para I2C
#define SDA_PIN A4
#define SCL_PIN A5

// Inicializando o PN532
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Inicializando NFC...");

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("Falha na alocação do display SSD1306"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("Não encontrou a placa PN53x");
    display.println("NFC não encontrado");
    display.display();
    while (1); // parar se não encontrar o módulo
  }

  Serial.print("Encontrou o chip PN5"); 
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Versão do firmware: ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);

  nfc.SAMConfig(); // Configura o módulo para operações normais
  Serial.println("Esperando por um cartão NFC...");
  display.clearDisplay();
  display.println("Esperando por um cartão NFC...");
  display.display();
}

void loop(void) {
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer para armazenar o UID
  uint8_t uidLength;                        // Comprimento do UID
  uint8_t data[16];                         // Buffer para armazenar dados do bloco
  uint8_t key[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };  // Chave padrão

  // Tentando ler o cartão NFC
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength)) {
    Serial.println("Cartão NFC encontrado!");
    display.clearDisplay();
    display.println("Cartão NFC encontrado!");

    // Imprimindo o UID
    Serial.print("UID: ");
    display.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(uid[i], HEX);
      Serial.print(" ");
      display.print(uid[i], HEX);
      display.print(" ");
    }
    Serial.println();
    display.println();

    // Tentativa de autenticação do bloco 1
    if (nfc.mifareclassic_AuthenticateBlock(uid, uidLength, 1, 0, key)) {
      Serial.println("Autenticação bem-sucedida.");
      display.println("Autenticação bem-sucedida.");

      // Tentando ler os dados gravados (bloco 1)
      if (nfc.mifareclassic_ReadDataBlock(1, data)) {
        Serial.print("Dados gravados (bloco 1): ");
        display.print("Bloco 1: ");
        for (int i = 0; i < 16; i++) {
          Serial.print(data[i], HEX);
          Serial.print(" ");
          display.print(data[i], HEX);
          display.print(" ");
        }
        Serial.println();
        display.println();
      } else {
        Serial.println("Falha na leitura do bloco 1.");
        display.println("Falha na leitura do bloco 1.");
      }
    } else {
      Serial.println("Falha na autenticação.");
      display.println("Falha na autenticação.");
    }

    display.display();
    Serial.println("----- Fim das Informações do Cartão -----\n");
    delay(2000);
  } else {
    Serial.println("Aguardando um cartão...");
    display.clearDisplay();
    display.println("Aguardando um cartão...");
    display.display();
    delay(1000);
  }
}
