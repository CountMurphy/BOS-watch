#ifndef _GPS_H_
#define _GPS_H_

#include <stdint.h>

void GPSInit();
void GPSPower(bool power);
uint8_t ReadChar();
void ParseNMEA(uint8_t *minutes, uint8_t *hours, uint8_t *seconds, uint8_t *day, uint8_t *month, uint8_t *year, uint32_t *lon,char *direction);
void GetGPRMC(uint8_t NMEACount, char *GPRMC);
void GetGPGGA(uint8_t NMEACount, char *GPGGA);
void GetCurrentLocation(char *lat, char *N_S,char*lon, char *E_W);
void GetGoogleReadyLocation(char *lat,char N_S,char *lon, char E_W, double *gLat,double *gLon);
bool SatFixStatus();
int8_t GetLocalTimeZoneOffset(uint32_t lon, char direction);
double CoordConvert(int coordWhole,int coordDecimal);
#endif
