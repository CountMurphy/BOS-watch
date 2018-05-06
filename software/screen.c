#include "fsl_gpio_driver.h"
#include "screen.h"
#include "switch.h"


static const gpio_output_pin_user_config_t displayOutput={
    .pinName=GPIO_MAKE_PIN(GPIOB_IDX,18),
    .config.outputLogic=0
};

static const gpio_output_pin_user_config_t spiAccept={
    .pinName=GPIO_MAKE_PIN(GPIOD_IDX,4),
    .config.outputLogic=0
};

spi_master_user_config_t userConfig =
{
    .bitsPerSec = 4000000, /* 4 MHz */
    .polarity = kSpiClockPolarity_ActiveHigh,
    .phase = kSpiClockPhase_FirstEdge,
    .direction = kSpiMsbFirst,
    .bitCount = kSpi8BitMode,
};


void ScreenInit()
{
    GPIO_DRV_OutputPinInit(&spiAccept);
    GPIO_DRV_OutputPinInit(&displayOutput);
    SPI_DRV_MasterInit(SPI_MASTER_INSTANCE, &spiMasterState);
    SPI_DRV_MasterConfigureBus(SPI_MASTER_INSTANCE,&userConfig,&calculatedBaudRate);
}


static spi_status_t SPI_Transfer(const uint8_t *tx, uint8_t *rx, size_t count)
{
    spi_status_t rc;

    rc = SPI_DRV_MasterTransfer(1, NULL, tx, rx, count);
    if (rc != kStatus_SPI_Success) {
        return rc;
    }
    int i, timeout = (count + 127) / 128 + 1;
    for (i = 0; i < timeout; ++i) {
        rc = SPI_DRV_MasterGetTransferStatus(1, NULL);
        if (rc == kStatus_SPI_Success) {
           return rc;
        }
    }
    return rc;
}



void spiLatch()
{
    //have shift registers accept current values
    GPIO_DRV_WritePinOutput(spiAccept.pinName,1);
    GPIO_DRV_WritePinOutput(spiAccept.pinName,0);
}

void rawWriteToScreen(uint8_t bytes[])
{
    uint8_t rx;
    SPI_Transfer(bytes,&rx,2);
    spiLatch();
}

void powerDisplay(bool isEnabled)
{
    GPIO_DRV_WritePinOutput(displayOutput.pinName,isEnabled?0:1);
}

void multiplex(char msg[], bool dots[], int count)
{

    uint8_t digits;
    if(count==0)
    {
        digits=0xFF;
    }else{
        digits=center(count);
    }

    uint8_t digitMask=0x01;
    uint8_t  rx[2];

    int countIndex=0;
    for(int i=0;i<8;i++)
    {
        //get current digit
        uint8_t digit=digits & digitMask;

        uint8_t shiftedDigit=digit>>i;
        uint8_t charData= dots[countIndex] ==true ? addDot(dictionary(msg[countIndex])) : dictionary(msg[countIndex]);
        uint8_t spiData[2] = {digitMask^0xFF,shiftedDigit== 0x00? charData :0x00};
        if(shiftedDigit == 0x00)
        {
            countIndex++;
        }

        SPI_Transfer(spiData,rx,2);

        spiLatch();
        digitMask=digitMask<<1;
        OSA_TimeDelay(1);

    }
}

void Scroll(char *word, bool dots[],uint8_t count)
{
	for(int i=0;i<count+8;i++)
	{
		uint8_t digit=0x80>>i;

		//multiplex loop
		for(int waitCount=0;waitCount<3001;waitCount++)
		{
			int ii=i;
			int newCount=0;
			while(ii>-1)
			{
                if(InterruptTriggered())
                {
                    return;
                }
                if(i>=8)
                {
                    uint8_t data[2]={(0x80>>ii)^0xFF, dictionary(newCount>count? '\0':word[newCount])};
			    	rawWriteToScreen(data);
                }
                else
                {
                    uint8_t data[2]={(digit<<newCount)^0xFF, dictionary(newCount>count? '\0':word[newCount])};
				    rawWriteToScreen(data);
                }
				newCount++;
				ii--;
			}
		}

	}  
}

uint8_t dictionary(char digit)
{
    uint8_t retVal;
    switch(digit)
    {
    case '0':
    case 'O':
        retVal=0b00111111;
        break;
    case '1':
    case 'I':
        retVal=0b00000110;
        break;
    case '2':
        retVal=0b01011011;
        break;
    case '3':
        retVal=0b01001111;
        break;
    case '4':
        retVal=0b01100110;
        break;
    case '5':
    case 's':
    case 'S':
        retVal=0b01101101;
        break;
    case '6':
    case 'G':
        retVal=0b01111101;
        break;
    case '7':
        retVal=0b00000111;
        break;
    case '8':
        retVal=0b01111111;
        break;
    case '9':
    case 'q':
    case 'Q':
        retVal=0b01100111;
        break;
    case 'A':
    case 'a':
        retVal=0b01110111;
        break;
    case 'b':
    case 'B':
        retVal=0b01111100;
        break;
    case 'c':
        retVal=0b01011000;
        break;
    case 'C':
        retVal=0b00111001;
        break;
    case 'D':
    case 'd':
        retVal=0b01011110;
        break;
    case 'e':
    case 'E':
        retVal=0b01111001;
        break;
    case 'f':
    case 'F':
        retVal=0b01110001;
        break;
    case 'g':
        retVal=0b01101111;
        break;
    case 'H':
        retVal=0b01110110;
        break;
    case 'h':
        retVal=0b01110100;
        break;
    case 'i':
        retVal=0b00000100;
        break;
    case 'j':
    case 'J':
        retVal=0b00001110;
        break;
    case 'k':
    case 'K':
        retVal=0b01111010;
        break;
    case 'L':
        retVal=0b00111000;
        break;
        //m an M.....just do a double 'n', or an 'rn'
    case 'l':
        retVal=0b00110000;
        break;
    case 'n':
    case 'N':
        retVal=0b01010100;
        break;
    case 'o':
        retVal=0b01011100;
        break;
    case 'P':
    case 'p':
        retVal=0b01110011;
        break;
    case 'r':
        retVal=0b01010000;
        break;
    case 'R':
        retVal=0b00110011;
        break;
    case 't':
    case 'T':
        retVal=0b01111000;
        break;
    case 'U':
        retVal=0b00111110;
        break;
    case 'v':
    case 'V':
    case 'u':
        retVal=0b00011100;
        break;
    case 'W':
    case 'w':
        retVal=0b01111110;
        break;
    case 'y':
    case 'Y':
        retVal=0b01101110;
        break;
    case '\0':
        retVal=0x00;
        break;
    case '-':
        retVal=0b01000000;
        break;
    case '.':
        retVal=0b10000000;
        break;
    }
    return retVal;
}

uint8_t addDot(char digit)
{
    uint8_t dot= 0x80;
    return  digit|dot;
}


uint8_t center(int count)
{
    uint8_t mask=0xFF;
    uint8_t retval=0x00;
    uint8_t right=0x08;
    uint8_t left=0x10;
    bool isOdd=(count%2)!=0;
    for(int i=1;i<=(count/2);i++)
    {
        uint8_t shiftedL=left<<(i-1);
        retval=retval|shiftedL;
        if(i<=count)
        {
            uint8_t shiftedR=right>>(i-1);
            retval=retval|shiftedR;
        }
    }
    if(isOdd)
    {
      retval=retval|(right>>count/2);
    }
    return reverseBin(retval^mask);
}

uint8_t reverseBin(uint8_t data)
{
    uint8_t output = 0;
    uint8_t n = sizeof(data) << 3;
    uint8_t i = 0;

    for (i = 0; i < n; i++)
        if ((data>> i) & 0x1)
            output |=  (0x1 << (n - 1 - i));

    return output;
}

