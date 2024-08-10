# Leitor NFC com Display OLED

Este projeto é um leitor NFC utilizando o módulo PN532 e um display OLED SSD1306. O código permite a leitura de cartões NFC compatíveis com o padrão MIFARE e exibe as informações no display OLED, além de imprimir os detalhes no monitor serial.

## Como Usar

1. **Clone o repositório:**

   ```bash
   git clone https://github.com/LeoVichi/NFC_reader_oled.git
   cd nfc-oled-leitor
   ```

2. **Configuração do Hardware:**

   Conecte os dispositivos ao microcontrolador usando I2C:
   - **SSD1306 OLED:**
     - SDA -> A4
     - SCL -> A5
   - **PN532 RFID NFC:**
     - SDA -> A4
     - SCL -> A5

3. **Bibliotecas Necessárias:**

   Certifique-se de que as seguintes bibliotecas estejam instaladas:
   - `Adafruit_PN532`
   - `Adafruit_GFX`
   - `Adafruit_SSD1306`
   - `Wire` (normalmente já incluída com o Arduino IDE)

   Estas bibliotecas podem ser instaladas através do Gerenciador de Bibliotecas da Arduino IDE.

4. **Carregue o Código:**

   Abra o arquivo `nfc_oled_leitor.ino` no Arduino IDE e carregue-o no microcontrolador.

5. **Funcionamento:**

   - O código inicializa o módulo PN532 e o display OLED.
   - Quando um cartão NFC é detectado, o UID e outras informações são exibidas no display OLED e no monitor serial.
   - Se a leitura falhar, mensagens de erro serão exibidas.

## Licença

Este projeto é licenciado sob os termos da licença MIT. Veja o arquivo [LICENSE](LICENSE) para mais detalhes.

## Autor

- **L3nny_P34s4n7**
- **Email:** contact@leonardovichi.com
