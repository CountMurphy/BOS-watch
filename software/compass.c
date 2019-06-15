#include "fsl_i2c_hal.h"
#include "fsl_i2c_master_driver.h"
#include "compass.h"

#define DATA_LENGTH             6
#define BOARD_I2C_INSTANCE      0

// Buffer store data to send to slave
// Buffer store data to receive from slave
uint8_t rxBuff[DATA_LENGTH] = {0};

i2c_device_t compass=
{
    .address=0x32>>1,
    .baudRate_kbps=100
};

i2c_master_state_t compass_state;

void CompassInit()
{
    I2C_DRV_MasterInit(BOARD_I2C_INSTANCE, &compass_state);
    OSA_TimeDelay(1);
    StndBy();
}

uint16_t GetHeading()
{
    uint8_t txBuff[DATA_LENGTH] = {0};
   // Master sends 1 bytes CMD and data to slave
    txBuff[0]=0x50;

    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, txBuff, 1, 1000);

    // Delay to wait slave received data
    OSA_TimeDelay(1);


    // Master receives count byte data from slave
    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, rxBuff, 6, 1000);
    uint16_t heading=rxBuff[0]<<8 | rxBuff[1];
    return heading;
}

uint16_t GetPitch()
{
    uint8_t txBuff[DATA_LENGTH] = {0};
    txBuff[0]=0x55;

    //Get Tilt Data
    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, txBuff, 1, 1000);

    OSA_TimeDelay(1);


    I2C_DRV_MasterReceiveDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, rxBuff, 6, 1000);
    uint16_t pitch=rxBuff[0]<<8 | rxBuff[1];
    return pitch;
}

void StndBy()
{
    uint8_t txBuff[DATA_LENGTH] = {0};
    txBuff[0]=0x83;

    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, txBuff, 1, 1000);
}

void Resume()
{
    uint8_t txBuff[DATA_LENGTH] = {0};
    txBuff[0]=0x84;

    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, txBuff, 1, 1000);
    OSA_TimeDelay(1);
    txBuff[0]=0x75;

    I2C_DRV_MasterSendDataBlocking(BOARD_I2C_INSTANCE, &compass, NULL, 0, txBuff, 1, 1000);
}
