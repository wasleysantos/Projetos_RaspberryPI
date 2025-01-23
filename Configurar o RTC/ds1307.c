#include "hardware/i2c.h"  
#include "stdio.h"  
#include "ds1307.h"  

#define DS1307_I2C_ADDRESS 0x68  

// Função para converter um inteiro para BCD  
uint8_t int_to_bcd(int decimal) {  
    return ((decimal / 10) << 4) | (decimal % 10);  
}  

// Função para converter valores de BCD para decimal  
uint8_t bcd_to_decimal(uint8_t bcd) {  
    return ((bcd >> 4) * 10) + (bcd & 0x0F);  
}  

// Função para configurar o tempo no DS1307  
void configure_rtc_time(uint8_t seconds, uint8_t minutes, uint8_t hours, uint8_t day, uint8_t date, uint8_t month, uint8_t year) {  
    uint8_t time_data[7];  

    // Converte os valores para BCD  
    time_data[0] = int_to_bcd(seconds);  
    time_data[1] = int_to_bcd(minutes);  
    time_data[2] = int_to_bcd(hours);  
    time_data[3] = int_to_bcd(day);  
    time_data[4] = int_to_bcd(date);  
    time_data[5] = int_to_bcd(month);  
    time_data[6] = int_to_bcd(year - 2000); // O ano deve ser armazenado como 0-99  

    // Envia os dados para o DS1307  
    i2c_write_blocking(i2c0, DS1307_I2C_ADDRESS, (uint8_t[]){0x00}, 1, true); // Endereço do registrador  
    i2c_write_blocking(i2c0, DS1307_I2C_ADDRESS, time_data, 7, false);  
}  

// Função para ler o tempo do DS1307  
void read_time_ds1307() {  
    uint8_t time_data[7];  

    // Envia o endereço do registrador 0 (segundos) do DS1307  
    i2c_write_blocking(i2c0, DS1307_I2C_ADDRESS, (uint8_t[]){0x00}, 1, true);  

    // Lê os 7 bytes (segundos, minutos, horas, dia, data, mês, ano)  
    i2c_read_blocking(i2c0, DS1307_I2C_ADDRESS, time_data, 7, false);  

    // Converte e exibe os valores lidos  
    uint8_t seconds = bcd_to_decimal(time_data[0] & 0x7F);  // Máscara para remover bit de controle  
    uint8_t minutes = bcd_to_decimal(time_data[1] & 0x7F);  
    uint8_t hours = bcd_to_decimal(time_data[2] & 0x3F);  
    uint8_t day = bcd_to_decimal(time_data[3] & 0x07);      // Dia da semana (1-7)  
    uint8_t date = bcd_to_decimal(time_data[4] & 0x3F);  
    uint8_t month = bcd_to_decimal(time_data[5]);           // Remover a máscara  
    uint8_t year = bcd_to_decimal(time_data[6]) - 1806;     // O ano é armazenado como 0-99  

    printf("Hora: %02d:%02d:%02d\n", hours, minutes, seconds);  
    printf("Data: %02d/%02d/%02d\n", date, month, year);  
}