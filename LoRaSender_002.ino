/* Heltec Automation send communication test example
 *
 * Function:
 * 1. Send data from a esp32 device over hardware 
 *  
 * Description:
 * 
 * HelTec AutoMation, Chengdu, China
 * 成都惠利特自动化科技有限公司
 * www.heltec.org
 *
 * this project also realess in GitHub:
 * https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
 * */

#include "LoRaWan_APP.h"
#include "Arduino.h"

#include "HT_SSD1306Wire.h"
#include "Arduino.h"




const float S_analog = 1023.0;
int co, nh3;
float txNo2;

#define RF_FREQUENCY 868000000  // Hz

#define TX_OUTPUT_POWER 5  // dBm

#define LORA_BANDWIDTH 0         // [0: 125 kHz, \
                                 //  1: 250 kHz, \
                                 //  2: 500 kHz, \
                                 //  3: Reserved]
#define LORA_SPREADING_FACTOR 7  // [SF7..SF12]
#define LORA_CODINGRATE 1        // [1: 4/5, \
                                 //  2: 4/6, \
                                 //  3: 4/7, \
                                 //  4: 4/8]
#define LORA_PREAMBLE_LENGTH 8   // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT 0    // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false


#define RX_TIMEOUT_VALUE 1000
#define BUFFER_SIZE 30  // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

//double txNumber;

bool lora_idle = true;

static RadioEvents_t RadioEvents;
void OnTxDone(void);
void OnTxTimeout(void);


//rotate only for GEOMETRY_128_64
static SSD1306Wire display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);  // addr , freq , i2c group , resolution , rst

void VextON(void) {
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void)  //Vext default OFF
{
  pinMode(Vext, OUTPUT);
  digitalWrite(Vext, HIGH);
}

void setup() {

  Serial.begin(9600);
  Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);

  //txNumber = 0;

  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;

  Radio.Init(&RadioEvents);
  Radio.SetChannel(RF_FREQUENCY);
  Radio.SetTxConfig(MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, 3000);


  VextON();
  delay(100);

  display.init();
  display.clear();
  display.display();

  display.setContrast(255);

  /*
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.clear();
  display.display();
  display.screenRotate(ANGLE_180_DEGREE);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 32-16/2, "ROTATE_180");
  display.display();
  delay(2000);
  */

}



void loop() {


  /*
  if (lora_idle == true) {
    delay(1000);
    txNumber += 0.01;
    sprintf(txpacket, "Hello world number %0.2f", txNumber);  //start a package

    Serial.printf("\r\nsending packet \"%s\" , length %d\r\n", txpacket, strlen(txpacket));

    Radio.Send((uint8_t *)txpacket, strlen(txpacket));  //send the package out
    lora_idle = false;
  }
*/

  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.clear();
  display.display();
  display.screenRotate(ANGLE_180_DEGREE);
  display.setFont(ArialMT_Plain_10);
  //display.drawString(64, 32-16/2, txpacket);
  display.drawString(54, 32-10/2, txpacket);
  display.display();


  co = map(analogRead(14), 0, S_analog, 1, 1000);               // Calcolo Monossido di Carbonio
  nh3 = map(analogRead(13), 0, S_analog, 1, 500);               // Calcolo Ammoniaca
  txNo2 = (map(analogRead(12), 0, S_analog, 5, 1000)) / 100.0;  // Calcolo Diossido di Azoto

  if (lora_idle == true) {
    sprintf(txpacket, "Pacote enviado %0.2f", txNo2);  //start a package

    Serial.printf("\r\nsending packet \"%s\" , length %d\r\n", txpacket, strlen(txpacket));

    Radio.Send((uint8_t *)txpacket, strlen(txpacket));  //send the package out
    //lora_idle = false;
  }
  Radio.IrqProcess();

  delay(5000);
}

void OnTxDone(void) {
  Serial.println("TX done......");
  lora_idle = true;
}

void OnTxTimeout(void) {
  Radio.Sleep();
  Serial.println("TX Timeout......");
  lora_idle = true;
}