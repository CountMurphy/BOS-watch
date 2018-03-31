#include <stdint.h>
#include<stdio.h>
#include<math.h>
#include "fsl_gpio_driver.h"
#include "fsl_uart_driver.h"

#include "gps.h"
#include "pins.h"

static const gpio_output_pin_user_config_t GPSEnable={
    .pinName=GPIO_MAKE_PIN(GPIOC_IDX,4),
    .config.outputLogic=1
};

uart_state_t g_uartState;

static const gpio_input_pin_user_config_t SatFix={
    .pinName=GPIO_MAKE_PIN(GPIOC_IDX,10)
};

uart_user_config_t g_uartConfig= {
    .baudRate = 9600u,
    .parityMode = kUartParityDisabled,
    .stopBitCount = kUartOneStopBit,
    .bitCountPerChar = kUart8BitsPerChar,
};



void GPSInit()
{
    UART_DRV_Init(2u, &g_uartState, &g_uartConfig);
}


void GPSPower(bool power)
{
    GPIO_DRV_OutputPinInit(&GPSEnable);
    GPIO_DRV_InputPinInit(&SatFix);
    GPIO_DRV_WritePinOutput(GPSEnable.pinName,power?0:1);
}

uint8_t ReadChar()
{
    uint8_t rxChar;
    UART_DRV_ReceiveDataBlocking(2u, &rxChar, 1u,1000u);
    return rxChar;
}

void ParseNMEA(uint8_t *minutes,uint8_t *hours,uint8_t *seconds,uint8_t *day,uint8_t *month,uint8_t *year,uint32_t *lon,char *direction)
{
    uint8_t NMEACount = 100;
    char GPRMC[NMEACount];
    char GPGGA[NMEACount];
    GetGPGGA(NMEACount,GPGGA);
    GetGPRMC(NMEACount,GPRMC);

    char hourChar[2];
    hourChar[0]=GPGGA[7];
    hourChar[1]=GPGGA[8];
    int temp;
    sscanf(hourChar, "%d", &temp);
    *hours=(uint8_t)temp;

    char minChar[2];
    minChar[0]=GPGGA[9];
    minChar[1]=GPGGA[10];
    sscanf(minChar, "%d", &temp);
    *minutes=(uint8_t)temp;

    char secChar[2];
    secChar[0]=GPGGA[11];
    secChar[1]=GPGGA[12];
    sscanf(secChar, "%d", &temp);
    *seconds=(uint8_t)temp;

    char dayChar[2];
    dayChar[0]=GPRMC[56];
    dayChar[1]=GPRMC[57];
    sscanf(dayChar, "%d", &temp);
    *day=(uint8_t)temp;

    char monChar[2];
    monChar[0]=GPRMC[58];
    monChar[1]=GPRMC[59];
    sscanf(monChar, "%d", &temp);
    *month=(uint8_t)temp;

    char yearChar[2];
    yearChar[0]=GPRMC[60];
    yearChar[1]=GPRMC[61];
    sscanf(yearChar, "%d", &temp);
    *year=(uint8_t)temp;

    char lonChar[5];
    lonChar[0]=GPRMC[32];
    lonChar[1]=GPRMC[33];
    lonChar[2]=GPRMC[34];
    lonChar[3]=GPRMC[35];
    lonChar[4]=GPRMC[36];
    sscanf(lonChar, "%d", &temp);
    *lon=(uint32_t)temp;

    *direction=GPRMC[43];
}

void GetGPRMC(uint8_t NMEACount, char *GPRMC)
{
    bool parse=true;
    while(parse)
    {
        if(ReadChar()=='$')
        {
            GPRMC[0]='$';
            for(int i=1;i<NMEACount;i++)
            {
                //Verify string read
                if(i==6)
                {
                    if(GPRMC[4]!='M' || GPRMC[5]!='C')
                        break;
                }
                GPRMC[i]=ReadChar();
                if(GPRMC[i]=='\n')
                {
                    parse=false;
                    break;
                }
            }
        }
    }
}


void GetGPGGA(uint8_t NMEACount, char *GPGGA)
{
    bool parse=true;
    while(parse)
    {
        if(ReadChar()=='$')
        {
            GPGGA[0]='$';
            for(int i=1;i<NMEACount;i++)
            {
                //Verify string read
                if(i==6)
                {
                    if(GPGGA[4]!='G' || GPGGA[5]!='A')
                        break;
                }
                GPGGA[i]=ReadChar();
                if(GPGGA[i]=='\n')
                {
                    parse=false;
                    break;
                }
            }
        }
    }
}

bool SatFixStatus()
{
    return GPIO_DRV_ReadPinInput(SatFix.pinName);
}

void ParseNMEAMOCK(uint8_t *minutes,uint8_t *hours,uint8_t *seconds,uint8_t *day,uint8_t *month,uint8_t *year,uint32_t *lon,char *direction)
{
    char GPRMC[100] = "\002GPRMC,193811.000,A,3000.0000,N,09000.0000,W,0.60,60.68,210418,,,A*4B\r\n";
    char GPGGA[100] = "\002GPGGA,203433.000,3000.0000,N,09000.0000,W,1,04,3.21,171.0,M,-22.6,M,,*5B\r\n";

    char hourChar[2];
    hourChar[0]=GPGGA[7];
    hourChar[1]=GPGGA[8];
    int temp;
    sscanf(hourChar, "%d", &temp);
    *hours=(uint8_t)temp;

    char minChar[2];
    minChar[0]=GPGGA[9];
    minChar[1]=GPGGA[10];
    sscanf(minChar, "%d", &temp);
    *minutes=(uint8_t)temp;

    char secChar[2];
    secChar[0]=GPGGA[11];
    secChar[1]=GPGGA[12];
    sscanf(secChar, "%d", &temp);
    *seconds=(uint8_t)temp;

    char dayChar[2];
    dayChar[0]=GPRMC[56];
    dayChar[1]=GPRMC[57];
    sscanf(dayChar, "%d", &temp);
    *day=(uint8_t)temp;

    char monChar[2];
    monChar[0]=GPRMC[58];
    monChar[1]=GPRMC[59];
    sscanf(monChar, "%d", &temp);
    *month=(uint8_t)temp;

    char yearChar[2];
    yearChar[0]=GPRMC[60];
    yearChar[1]=GPRMC[61];
    sscanf(yearChar, "%d", &temp);
    *year=(uint8_t)temp;

    char lonChar[5];
    lonChar[0]=GPRMC[32];
    lonChar[1]=GPRMC[33];
    lonChar[2]=GPRMC[34];
    lonChar[3]=GPRMC[35];
    lonChar[4]=GPRMC[36];
    sscanf(lonChar, "%d", &temp);
    *lon=(uint32_t)temp;

    *direction=GPRMC[43];
}

void GetCurrentLocation(char *lat, char *N_S,char*lon, char *E_W)
{
    char GPRMC[100];
    GetGPRMC(100,GPRMC);
    lon[0]=GPRMC[32];
    lon[1]=GPRMC[33];
    lon[2]=GPRMC[34];
    lon[3]=GPRMC[35];
    lon[4]=GPRMC[36];
    lon[5]=GPRMC[37];
    lon[6]=GPRMC[38];
    lon[7]=GPRMC[39];
    lon[8]=GPRMC[40];
    lon[9]=GPRMC[41];
    *E_W= GPRMC[43];

    lat[0]=GPRMC[20];
    lat[1]=GPRMC[21];
    lat[2]=GPRMC[22];
    lat[3]=GPRMC[23];
    lat[4]=GPRMC[24];
    lat[5]=GPRMC[25];
    lat[6]=GPRMC[26];
    lat[7]=GPRMC[27];
    lat[8]=GPRMC[28];
    *N_S=GPRMC[30];
}

void GetGoogleReadyLocation(char *lat,char N_S,char *lon, char E_W, double *gLat,double *gLon)
{

    //dumb ass work around as sscanf wont work with a float...
    int latWhole;
    int latDecimal;
    char latDecChar[4]={lat[5],lat[6],lat[7],lat[8]};
    sscanf(lat,"%d",&latWhole);
    sscanf(latDecChar,"%d",&latDecimal);

    int lonWhole;
    int lonDecimal;
    char lonDecChar[4]={lon[6],lon[7],lon[8],lon[9]};
    sscanf(lon,"%d",&lonWhole);
    sscanf(lonDecChar,"%d",&lonDecimal);

    *gLat=CoordConvert(latWhole,latDecimal);
    *gLon=CoordConvert(lonWhole,lonDecimal);

    if(N_S=='S')
    {
        *gLat*=-1;
    }
    if(E_W=='W')
    {
        *gLon*=-1;
    }
}

double CoordConvert(int coordWhole,int coordDecimal)
{
    double coorF=coordWhole+((float)coordDecimal*.0001);
    int firstDigits=(int)coorF/100;
    return firstDigits+((coorF-((double)firstDigits*100))/60);
}

int8_t GetLocalTimeZoneOffset(uint32_t lon, char direction)
{
    //9736
    //97+(36/100)
    float numToRound=(lon/100)+((float)(lon%100)/60);
    double integral;
    double fractional = modf((double)numToRound, &integral);
    lon=(uint32_t)numToRound;

    if((uint32_t)(fractional*10)>=5)
    {
        lon++;
    }

    int8_t offset=0;

    if (lon <=165 && lon >=158)
        offset=11;
    if (lon <=157 && lon >=136)
        offset=10;
    if (lon <=135 && lon >=128)
        offset=9;
    if (lon<=127 && lon>=113)
        offset=8;
    if (lon<=112 && lon>=105)
        offset=7;
    if (lon<=104 && lon >=90)
        offset=6;
    if (lon <=89 && lon >= 67)
        offset=5;
    if (lon <=65 && lon >=55)
        offset=4;
    if (lon <=54 && lon>=35)
        offset=3;
    if (lon <=34 && lon >=21)
        offset=2;
    if (lon <=20 && lon >=12)
        offset=1;
    if (lon <=11 && lon >=0)
        offset=0;

  if (direction=='W')
    offset*=(-1);
  return offset;
}