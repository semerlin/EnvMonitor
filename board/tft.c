#include "tft.h"
#include "stm32f10x_cfg.h"
#include "pinconfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "pic.h"


/* lcd width and height definition */
#define WIDTH   (240)
#define HEIGHT  (320)

/* data/command select pin definition */
static uint8 dc_group;
static uint8 dc_pin;
static uint8 cs_group;
static uint8 cs_pin;

/* DR register definition, for reach max spi speed */
volatile uint32 *DR_REG = (volatile uint32 *)(0x4001300c);

/**
 * @brief choose command
 */
__INLINE static void chooseCommand(void)
{
    GPIO_ResetPin((GPIO_Group)dc_group, dc_pin);
}

/**
 * @brief choose data
 */
__INLINE static void chooseData(void)
{
    GPIO_SetPin((GPIO_Group)dc_group, dc_pin);
}

/**
 * @brief choose lcd
 */
__INLINE static void chooseLcd(void)
{
    GPIO_ResetPin((GPIO_Group)cs_group, cs_pin);
}

/**
 * @brief do not choose lcd
 */
__INLINE static void releaseLcd(void)
{
    GPIO_SetPin((GPIO_Group)cs_group, cs_pin);
}



static void lcdWriteReg(__in uint8 reg)
{
    chooseLcd();
    chooseCommand();
    SPI_WriteReadDataSync(SPI1, reg);
    releaseLcd();
}

static void lcdWriteData(__in uint8 data)
{
    chooseLcd();
    chooseData();
    SPI_WriteReadDataSync(SPI1, data);
    releaseLcd();
}

static void lcdWriteData16(__in uint16 data)
{
    chooseLcd();
    chooseData();
    SPI_WriteReadDataSync(SPI1, data >> 8);
    SPI_WriteReadDataSync(SPI1, data & 0xff);
    releaseLcd();
}

static void lcdWriteRawData16(__in uint16 data)
{
    SPI_WriteReadDataSync(SPI1, data >> 8);
    SPI_WriteReadDataSync(SPI1, data & 0xff);
}

static void lcdWriteRAM(void)
{
    chooseLcd();
    chooseCommand();
    SPI_WriteReadDataSync(SPI1, 0x2c);
    releaseLcd(); 
}
/* Private defines */
#define ILI9341_RESET				0x01
#define ILI9341_SLEEP_OUT			0x11
#define ILI9341_GAMMA				0x26
#define ILI9341_DISPLAY_OFF			0x28
#define ILI9341_DISPLAY_ON			0x29
#define ILI9341_COLUMN_ADDR			0x2A
#define ILI9341_PAGE_ADDR			0x2B
#define ILI9341_GRAM				0x2C
#define ILI9341_MAC					0x36
#define ILI9341_PIXEL_FORMAT		0x3A
#define ILI9341_WRITE_CONTINUE		0x3C //dacus important for speed
#define ILI9341_WDB					0x51
#define ILI9341_WCD					0x53
#define ILI9341_RGB_INTERFACE		0xB0
#define ILI9341_FRC					0xB1
#define ILI9341_BPC					0xB5
#define ILI9341_DFC					0xB6
#define ILI9341_POWER1				0xC0
#define ILI9341_POWER2				0xC1
#define ILI9341_VCOM1				0xC5
#define ILI9341_VCOM2				0xC7
#define ILI9341_POWERA				0xCB
#define ILI9341_POWERB				0xCF
#define ILI9341_PGAMMA				0xE0
#define ILI9341_NGAMMA				0xE1
#define ILI9341_DTCA				0xE8
#define ILI9341_DTCB				0xEA
#define ILI9341_POWER_SEQ			0xED
#define ILI9341_3GAMMA_EN			0xF2
#define ILI9341_INTERFACE			0xF6
#define ILI9341_PRC					0xF

/**
 * @brief init lcd
 */
void lcdInit(void)
{
    /* lcd backlight on */
    pinSet("lcd_bkl");
    
    /* init spi1 */
    SPI_Config config;
    config.mainMode = SPI_MainMode_Master;
    config.subMode = SPI_SubMode_Full_Duplex;
    config.clock = SPI_Clk_Divided_2;
    config.polarity = SPI_Polarity_Low;
    config.phase = SPI_Phase_FirstClk;
    config.length = SPI_DataLength_8Bits;
    config.format = SPI_DataFormat_MSB;
    config.nssMode = SPI_NSS_SOFTWARE;
    config.useCrc = FALSE;
    
    SPI_Setup(SPI1, &config);
    SPI_Enable(SPI1, TRUE);
    
    /* get data/command pin */
    getPinInfo("lcd_dc", &dc_group, &dc_pin);
    
    /* get cs pin*/
    getPinInfo("spi1_nss", &cs_group, &cs_pin);
    
    /* init lcd registers */
    pinReset("lcd_rst");
    vTaskDelay(20 / portTICK_PERIOD_MS); //delay 20ms
   	pinSet("lcd_rst");	 
 	vTaskDelay(20 / portTICK_PERIOD_MS); //delay 20ms

    /* register init */
    lcdWriteReg(0xcb);  
    lcdWriteData(0x39); 
    lcdWriteData(0x2c); 
    lcdWriteData(0x00); 
    lcdWriteData(0x34); 
    lcdWriteData(0x02); 

    lcdWriteReg(0xcf);  
    lcdWriteData(0x00); 
    lcdWriteData(0Xc1); 
    lcdWriteData(0X30); 

    lcdWriteReg(0xe8);  
    lcdWriteData(0x85); 
    lcdWriteData(0x00); 
    lcdWriteData(0x78); 

    lcdWriteReg(0xea);  
    lcdWriteData(0x00);
    lcdWriteData(0x00);

    lcdWriteReg(0xed);  
    lcdWriteData(0x64); 
    lcdWriteData(0x03); 
    lcdWriteData(0X12); 
    lcdWriteData(0X81); 

    lcdWriteReg(0xf7);  
    lcdWriteData(0x20); 

    lcdWriteReg(0xc0);    //Power control 
    lcdWriteData(0x23);   //VRH[5:0] 

    lcdWriteReg(0xc1);    //Power control 
    lcdWriteData(0x10);   //SAP[2:0];BT[3:0] 

    lcdWriteReg(0xc5);    //VCM control 
    lcdWriteData(0x3e); //
    lcdWriteData(0x28); 

    lcdWriteReg(0xc7);    //VCM control2 
    lcdWriteData(0x86);  //--

    lcdWriteReg(0x36);    // Memory Access Control 
    lcdWriteData(0x48); //C8 //48 68ÊúÆÁ//28 E8 ºáÆÁ

    lcdWriteReg(0x3a);    
    lcdWriteData(0x55); 

    lcdWriteReg(0xb1);    
    lcdWriteData(0x00);  
    lcdWriteData(0x18); 

    lcdWriteReg(0xb6);    // Display Function Control 
    lcdWriteData(0x08); 
    lcdWriteData(0x82);
    lcdWriteData(0x27);  

    lcdWriteReg(0xf2);    // 3Gamma Function Disable 
    lcdWriteData(0x00); 

    lcdWriteReg(0x26);    //Gamma curve selected 
    lcdWriteData(0x01); 

    lcdWriteReg(0xe0);    //Set Gamma 
    lcdWriteData(0x0f); 
    lcdWriteData(0x31); 
    lcdWriteData(0x2b); 
    lcdWriteData(0x0c); 
    lcdWriteData(0x0e); 
    lcdWriteData(0x08); 
    lcdWriteData(0x4e); 
    lcdWriteData(0xf1); 
    lcdWriteData(0x37); 
    lcdWriteData(0x07); 
    lcdWriteData(0x10); 
    lcdWriteData(0x03); 
    lcdWriteData(0x0e); 
    lcdWriteData(0x09); 
    lcdWriteData(0x00); 

    lcdWriteReg(0Xe1);    //Set Gamma 
    lcdWriteData(0x00); 
    lcdWriteData(0x0e); 
    lcdWriteData(0x14); 
    lcdWriteData(0x03); 
    lcdWriteData(0x11); 
    lcdWriteData(0x07); 
    lcdWriteData(0x31); 
    lcdWriteData(0xc1); 
    lcdWriteData(0x48); 
    lcdWriteData(0x08); 
    lcdWriteData(0x0f); 
    lcdWriteData(0x0c); 
    lcdWriteData(0x31); 
    lcdWriteData(0x36); 
    lcdWriteData(0x0f); 

    lcdWriteReg(0x11);    //Exit Sleep 
    vTaskDelay(120 / portTICK_PERIOD_MS);
            
    lcdWriteReg(0x29);    //Display on 
    lcdWriteReg(0x2c); 

    lcdClearScreen(255, 255, 255);
}


/**
 * @brief control lcd backlight
 * @param on or off flag
 */
void lcdBklOn(__in BOOL flag)
{
    if(flag)
        pinSet("lcd_bkl");
    else
        pinReset("lcd_bkl");
}


void lcdSetCursor(__in uint16 xPos, __in uint16 yPos)
{
    lcdWriteReg(0x2a);
	lcdWriteData(xPos >> 8); 
	lcdWriteData(xPos & 0xff);	 
	lcdWriteReg(0x2b); 
	lcdWriteData(yPos >> 8); 
	lcdWriteData(yPos & 0xff);
}

/**
 * RED: 5bits
 * GREEN: 6bits
 * BLUE: 5bits
 */
void lcdClearScreen(__in uint8 red, uint8 green, uint8 blue)
{
	uint32 totalpoint = WIDTH * HEIGHT;
	lcdSetCursor(0, 0);
	lcdWriteRAM();
    uint16 color = (red & 0x1f);
    color <<= 6;
    color |= (green & 0x3f);
    color <<= 5;
    color |= (blue & 0x1f);
    chooseLcd();
    chooseData();
	for(uint32 index = 0; index < totalpoint; index++)
	{
        *DR_REG = (color >> 8);
        __ASM("nop");
        __ASM("nop");
        __ASM("nop");
        *DR_REG = (color & 0xff);
        __ASM("nop");
        __ASM("nop");
        __ASM("nop");
	}
    releaseLcd();
}

void showimage(uint16 x,uint16 y) 
{  
    uint32 totalpoint = WIDTH * HEIGHT;
	lcdSetCursor(0, 0);
	lcdWriteRAM();
    chooseLcd();
    chooseData();
	for(uint32 index = 0; index < totalpoint; index++)
	{
        *DR_REG = gImage_pic[index * 2 +1];
        __ASM("nop");
        __ASM("nop");
        __ASM("nop");
        *DR_REG = gImage_pic[index * 2];
        __ASM("nop");
        __ASM("nop");
        __ASM("nop");
	}
    releaseLcd();
}



