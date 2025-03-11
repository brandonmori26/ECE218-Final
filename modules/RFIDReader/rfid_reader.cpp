#include "mbed.h"
#include "MFRC522.h"

// Nucleo Pin for MFRC522 reset (pick another D pin if you need D8)
#define MF_RESET    p15
#define SPI_MOSI    p5
#define SPI_MISO    p6
#define SPI_SCK     p7
#define SPI_CS      

DigitalOut LedGreen(LED1);

// External function declaration
extern void pcSerialComStringWrite(const char* buffer);

// Global variable to track card detection status
bool cardDetected = false;

MFRC522 RfChip(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_CS, MF_RESET);

void RFID_detected(void) {
  char buffer[100]; // Buffer for formatting strings

  sprintf(buffer, "starting...\r\n");
  pcSerialComStringWrite(buffer);

  // Init. RC522 Chip
  RfChip.PCD_Init();

  sprintf(buffer, "init passed\r\n");
  pcSerialComStringWrite(buffer);

  // Reset card detection status at the beginning
  cardDetected = false;

  while (true) {
    LedGreen = 1;

    // Look for new cards
    if (!RfChip.PICC_IsNewCardPresent())
    {
      wait_ms(500);
      continue;
    }

    // Select one of the cards
    if (!RfChip.PICC_ReadCardSerial())
    {
      wait_ms(500);
      sprintf(buffer, "card read\r\n");
      pcSerialComStringWrite(buffer);
      continue;
    }

    LedGreen = 0;

    // Print Card UID
    sprintf(buffer, "Card UID:");
    pcSerialComStringWrite(buffer);
    
    for (uint8_t i = 0; i < RfChip.uid.size; i++)
    {
      sprintf(buffer, " %X02", RfChip.uid.uidByte[i]);
      pcSerialComStringWrite(buffer);
    }
    
    sprintf(buffer, "\n\r");
    pcSerialComStringWrite(buffer);

    // Print Card type
    uint8_t piccType = RfChip.PICC_GetType(RfChip.uid.sak);
    sprintf(buffer, "PICC Type: %s \n\r", RfChip.PICC_GetTypeName(piccType));
    pcSerialComStringWrite(buffer);
    
    // Update global card detection status
    cardDetected = true;
    wait_ms(1000);
    return; // Exit the function when a card is detected
  }
}
