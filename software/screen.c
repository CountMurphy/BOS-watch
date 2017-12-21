#include "screen.h"

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
            OSA_TimeDelay(100);
            GPIO_DRV_WritePinOutput(spiAccept.pinName,0);
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
    case 'g':
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
        //m an M.....just do a double 'n'
    case 'l':
        retVal=0b00110000;
        break;
    case 'n':
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
    }
    return retVal;
}

uint8_t addDot(char digit)
{
    uint8_t dot= 0x80;
    return  digit^dot;
}
