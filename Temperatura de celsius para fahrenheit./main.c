/*---------------------------------------------------------------
 Projeto: Mudar a unidade de medida da temperatura de celsius para 
 fahrenheit.
 OBS: Na tela do console coloquei pra exibir tanto o valor em Celsius 
 e o valor convertido para Fahrenheit.
 Autor: Wasley dos Santos
 Ultima atualização: 15/01/2025 
 ---------------------------------------------------------------*/
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Definições
#define ADC_TEMPERATURE_CHANNEL 4   // Canal ADC que corresponde ao sensor de temperatura interno

// Função para converter o valor lido do ADC para temperatura em graus Celsius
float adc_to_temperature(uint16_t adc_value) {
    const float conversion_factor = 3.3f / (1 << 12);  // Conversão de 12 bits (0-4095) para 0-3.3V
    float voltage = adc_value * conversion_factor;     // Converte o valor ADC para tensão
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;  // Converte a tensão para temperatura em Celsius
    return temperature;
}

float celsius_to_fahrenheit(float celsius) {
    return (celsius * 1.8f) + 32.0f;  // Converte Celsius para Fahrenheit
}

int main() {
    stdio_init_all();
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(ADC_TEMPERATURE_CHANNEL);

    while (true) {
        uint16_t adc_value = adc_read();
        float temperature_celsius = adc_to_temperature(adc_value);  // Converte ADC para Celsius
        float temperature_fahrenheit = celsius_to_fahrenheit(temperature_celsius);  // Converte Celsius para Fahrenheit
        printf("Temperatura Celsius: %.2f°C Fahrenheit: %.2f°F\n", temperature_celsius, temperature_fahrenheit);
        //Imprime a temperatura em Celsius e fahrenheit
        sleep_ms(2000); //delay de 2 segundos
    }

    return 0;
}
