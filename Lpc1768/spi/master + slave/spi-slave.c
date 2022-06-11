/*
	Baremetal Lpc1768 Spi Master + Slave
*/
/* To change Slave to Master change line no. 45 
   Set SPCR bit 5 to 1 for Master mode 
   Clear SPCR bit 5 for Slave mode */
/* Paste this code in any IDE */
#include "lpc17xx.h"
#include <stdint.h>
 
// Port 0
#define SPI_1_MISO_PIN       17   
#define SPI_1_MOSI_PIN       18  
#define SPI_1_SCK_PIN        15  
#define SPI_1_CS_PIN         16  
#define MAX_SLAVE            5 

uint16_t rx_data2[7];
uint16_t tx_data2[] = {'H','e','l','l','o','\r','\n'};
uint16_t checker[1];

static uint32_t spi_Pclk(void)
{
    uint32_t Pclk,spiPclk = 0;

    Pclk = (LPC_SC->PCLKSEL0 >> 16) & 0x03;

    switch( Pclk )
    {
    case 0x00:
        spiPclk = SystemCoreClock/4;        
        break;
    case 0x01:
        spiPclk = SystemCoreClock;
        break;
    case 0x02:
        spiPclk = SystemCoreClock/2;
        break;
    case 0x03:
        spiPclk = SystemCoreClock/8;
        break;
    }

    return spiPclk;
}

void spi_init(){
    LPC_SC->PCONP |= (1 << 8); // Enable clock
    LPC_SPI->SPCR &= ~(1 << 5); // Clear bit 5 to select slave mode

    // set alternate function 
    LPC_PINCON->PINSEL1 |= (0x3UL << 2);	
    LPC_PINCON->PINSEL1 |= (0x3UL << 4);
    LPC_PINCON->PINSEL0 |= (0x3UL << 30);
    if(!(LPC_SPI->SPCR & (0x20))){	
        LPC_PINCON->PINSEL1 |= (0x3UL << 0);	
    }

    //Set Direction
    if(!(LPC_SPI->SPCR & (0x20))){	
        LPC_GPIO0->FIODIR |=  (1 << SPI_1_MISO_PIN);	// Output
        LPC_GPIO0->FIODIR &= ~(1 << SPI_1_MOSI_PIN);    // Input
        LPC_GPIO0->FIODIR &= ~(1 << SPI_1_SCK_PIN);     // Input
        LPC_GPIO0->FIODIR &= ~(1 << SPI_1_CS_PIN);      // Input
    }
    else{
        LPC_GPIO0->FIODIR &= ~(1 << SPI_1_MISO_PIN);	// Input
        LPC_GPIO0->FIODIR |=  (1 << SPI_1_MOSI_PIN);    // Output
        LPC_GPIO0->FIODIR |=  (1 << SPI_1_SCK_PIN);     // Output
    }
    LPC_SC->PCLKSEL0 |= (1 << 16);
    // Set clock
    if((LPC_SPI->SPCR & (0x20))){

        uint32_t Pclk = spi_Pclk();
        uint32_t prescale, temp, br = 1000000;
        prescale = 8;
        while (1){
	    	temp = br * prescale;
	    	if (temp >= Pclk){
	    		break;
	    	}
	    	prescale += 2;
	    	if(prescale >= 254){
	    		break;
	    	}
	    }
        LPC_SPI->SPCCR = prescale;
    }

    // Setup clock polarity and phase
    LPC_SPI->SPCR &= ~(1 << 3); // CPHA 1
    LPC_SPI->SPCR &= ~(1 << 4); // CPOL 1

    // Set data frame size
    LPC_SPI->SPCR |= (8 << 8); // 8 bit

    // Set bit order
    LPC_SPI->SPCR &= ~(1 << 6); // MSB

    // Dummy read
    uint32_t Dummy;
    Dummy = LPC_SPI->SPSR;
    Dummy = LPC_SPI->SPDR;
    (void)Dummy;

}

void spi_read(uint16_t* rx_data, uint8_t size){
    
    uint8_t ReadDone = 0 ;
    while(ReadDone < size){ 
        while(!(LPC_SPI->SPSR & (1 << 7)));   
        rx_data[ReadDone] = LPC_SPI->SPDR;
        ReadDone++;    
    }

}

void spi_write(const uint16_t* tx_data, uint8_t size){
    
    uint8_t WriteDone = 0 ;
    while(WriteDone < size){
        LPC_SPI->SPDR = tx_data[WriteDone];
        while(!(LPC_SPI->SPSR & (1 << 7)));   
        WriteDone++;    
    }

}

void spi_exchange(uint16_t tx_data, uint16_t* rx_data){
   
    LPC_SPI->SPDR = tx_data;
    while(!(LPC_SPI->SPSR & (1 << 7))); 
    *rx_data = LPC_SPI->SPDR;
   
}

int main(){

    SystemInit(); // for Keil
    spi_init();
    /*Below code can be used if lpc1768 is in slave*/
    while(1){
        spi_read(checker,1);
        if(checker[0] == 0x4){
            //spi_read(rx_data2,7);
            //spi_write(tx_data2,7);
            for(uint8_t i = 0 ; i < 7 ; i++){
                spi_exchange(tx_data2[i],&rx_data2[i]);
            }
            
        }
    }

    /*Below code can be used if lpc1768 is in master mode*/
    /*LPC_GPIO0->FIODIR |=  (1 << 16);    // Output
    while(1){
        LPC_GPIO0->FIOCLR |=  (1 << 16);    // Slave active
        //spi_read(rx_data2,7);
        //spi_write(tx_data2,7);
        for(uint8_t i = 0 ; i < 7 ; i++){
            spi_exchange(tx_data2[i],&rx_data2[i]);
        }
        LPC_GPIO0->FIOSET |=  (1 << 16);    // Slave diable
    }*/
}
