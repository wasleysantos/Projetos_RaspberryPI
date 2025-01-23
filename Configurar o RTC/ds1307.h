#ifndef DS1307_H  
#define DS1307_H  

// Prototipação da função de leitura do tempo  
void read_time_ds1307();  
void configure_rtc_time(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t day, uint8_t date, uint8_t month, uint8_t year);  

#endif