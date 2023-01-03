#include "esp_master_spi.h"

SPIClass *initialize_spi(uint8_t spi_num){
    SPIClass * spi = NULL;
if(spi_num==2){
    #define MOSI 13
    #define MISO 12
    #define SS 15
    #define SCK 14
    spi = new SPIClass(HSPI);
}
else{
    #define MOSI 23
    #define MISO 19
    #define SS 5
    #define SCK 18
    spi = new SPIClass(VSPI);
    spi ->begin();
    pinMode(spi->pinSS(),OUTPUT);
}
    spi ->begin();
    pinMode(spi->pinSS(),OUTPUT);
    return spi;
}


SPISettings set_spi_settings(uint32_t clkf, uint8_t spi_mode, char first_bit){
    uint8_t bit;
  SPISettings mysettings = SPISettings();
  if (first_bit == 'M'){
    bit = MSBFIRST;
  }
    else
        bit = LSBFIRST;

  
  switch(spi_mode){
    case 1: mysettings = SPISettings(clkf,bit,SPI_MODE1);break;
    case 2: mysettings = SPISettings(clkf,bit,SPI_MODE2);break;
    case 3: mysettings = SPISettings(clkf,bit,SPI_MODE3);break;
    default: mysettings = SPISettings(clkf,bit,SPI_MODE0);break;
  }
  return mysettings;
}


void spi_send(SPIClass *spi,uint8_t data, uint32_t clkf, uint8_t spi_mode, char first_bit){
    //Creating a SPISettings object containing the SPI transfer settings to be used in the SPI transaction
  uint8_t bit;
  SPISettings mysettings = SPISettings();
  if (first_bit == 'M'){
    bit = MSBFIRST;
  }
    else
        bit = LSBFIRST;

  
  switch(spi_mode){
    case 1: mysettings = SPISettings(clkf,bit,SPI_MODE1);break;
    case 2: mysettings = SPISettings(clkf,bit,SPI_MODE2);break;
    case 3: mysettings = SPISettings(clkf,bit,SPI_MODE3);break;
    default: mysettings = SPISettings(clkf,bit,SPI_MODE0);break;
  }
  //Initializing an SPI transaction using the defined SPI settings
  spi->beginTransaction(mysettings);
  //Driving the SS pin low
  digitalWrite(spi->pinSS(),LOW);
  //transfering the data
  spi->transfer(data);
  //Driving the SS pin high to end the transaction
  digitalWrite(spi->pinSS(),HIGH);
  //ending the SPI transaction
  spi->endTransaction();
}


void spi_send(SPIClass *spi,uint8_t data, SPISettings mysettings){
 //Initializing an SPI transaction using the defined SPI settings
  spi->beginTransaction(mysettings);
  //Driving the SS pin low
  digitalWrite(spi->pinSS(),LOW);
  //transfering the data
  spi->transfer(data);
  //Driving the SS pin high to end the transaction
  digitalWrite(spi->pinSS(),HIGH);
  //ending the SPI transaction
  spi->endTransaction();
}


uint8_t spi_receive(SPIClass *spi, uint32_t clkf, uint8_t spi_mode, char first_bit){
  //Creating a SPISettings object containing the SPI transfer settings to be used in the SPI transaction
  uint8_t bit;
  SPISettings mysettings = SPISettings();
  if (first_bit == 'M'){
    bit = MSBFIRST;
  }
    else
        bit = LSBFIRST;

  
  switch(spi_mode){
    case 1: mysettings = SPISettings(clkf,bit,SPI_MODE1);break;
    case 2: mysettings = SPISettings(clkf,bit,SPI_MODE2);break;
    case 3: mysettings = SPISettings(clkf,bit,SPI_MODE3);break;
    default: mysettings = SPISettings(clkf,bit,SPI_MODE0);break;
  }
  //Initializing an SPI transaction using the defined SPI settings
  spi->beginTransaction(mysettings);
  digitalWrite(spi->pinSS(),LOW);
  //sending a dummy byte. Usually, after the transaction. The content of the slave register and the
  // master register are switched. Therefore, we can store the returned content from the transfer.
  uint8_t result = spi->transfer(0x0F);
  digitalWrite(spi->pinSS(),HIGH);
  spi->endTransaction();
  return result;
}


uint8_t spi_receive(SPIClass *spi, SPISettings mysettings){
  //initialize the transaction with the defined SPI settings
  spi->beginTransaction(mysettings);
  digitalWrite(spi->pinSS(),LOW);
  //sending a dummy byte. Usually, after the transaction. The content of the slave register and the
  // master register are switched. Therefore, we can store the returned content from the transfer.
  uint8_t result = spi->transfer(0x0F);
  digitalWrite(spi->pinSS(),HIGH);
  spi->endTransaction();
  return result;
}
