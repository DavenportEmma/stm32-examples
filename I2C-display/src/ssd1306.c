#include "ssd1306.h"
#include "stm32f722xx.h"
#include "i2c.h"


int command_ssd1306(uint8_t command) {
	uint8_t sequence[] = { 0x00, command };
	send_i2c(SSD1306_ADDR, sequence, 2);
}

int display_ssd1306(uint8_t* buffer) {
	command_ssd1306(SSD1306_PAGEADDR);
	command_ssd1306(0);
	command_ssd1306(0xFF);
	command_ssd1306(SSD1306_COLUMNADDR);
	command_ssd1306(0);
	command_ssd1306(WIDTH - 1);


}

int init_ssd1306() {
    if((I2C2->CR1 & I2C_CR1_PE) == 0) {
        return 1;
    }

	int flag = 0;
    // display initialisation sequence
	command_ssd1306(SSD1306_DISPLAYOFF);			// 0xAE
	command_ssd1306(SSD1306_SETDISPLAYCLOCKDIV);	// 0xD5
	command_ssd1306(0x80);							// the suggested ratio
	command_ssd1306(SSD1306_SETMULTIPLEX);			// 0xA8
	command_ssd1306(HEIGHT - 1);

	command_ssd1306(SSD1306_SETDISPLAYOFFSET);		// 0xD3
	command_ssd1306(0x0);							// no offset
	command_ssd1306(SSD1306_SETSTARTLINE | 0x0);	// line #0
	command_ssd1306(SSD1306_CHARGEPUMP);			// 0x8D
              
	// command_ssd1306((vccstate == SSD1306_EXTERNALVCC) ? 0x10 : 0x14);
	command_ssd1306(0x14);

	command_ssd1306(SSD1306_MEMORYMODE);			// 0x20
	command_ssd1306(0x00);							// horizontal addressing mode
	command_ssd1306(SSD1306_SEGREMAP | 0x1);		// col. addr 0 is mapped to SEG0
	command_ssd1306(SSD1306_COMSCANDEC);			// 0xC8scan from COM[N-1] to COM0

	command_ssd1306(SSD1306_SETCOMPINS);			//0xDA
	command_ssd1306(0x12);							// alternative COM pin config
											// disable COM Left/Right remap
	command_ssd1306(SSD1306_SETCONTRAST);			// 0x81
	// command_ssd1306((vccstate == SSD1306_EXTERNALVCC) ? 0x9F : 0xCF);
	command_ssd1306(0xCF);


  	command_ssd1306(SSD1306_SETPRECHARGE); // 0xd9
  	// command_ssd1306((vccstate == SSD1306_EXTERNALVCC) ? 0x22 : 0xF1);
  	command_ssd1306(0xF1);


  	command_ssd1306(SSD1306_SETVCOMDETECT);			// 0xDB
  	command_ssd1306(0x40);
  	command_ssd1306(SSD1306_DISPLAYALLON_RESUME);	// 0xA4
  	command_ssd1306(SSD1306_NORMALDISPLAY);			// 0xA6
  	command_ssd1306(SSD1306_DEACTIVATE_SCROLL);		
  	command_ssd1306(SSD1306_DISPLAYON);				// / Main screen turn on
	// command_ssd1306(SSD1306_DISPLAYALLON);
    
	return 0;
}