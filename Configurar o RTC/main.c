/*-------------------------------------------------------------------------
 Projeto: Configurar o RTC para 24/09/2024 – 13:27:00 e em seguida, 
 realizar a leitura do mesmo a cada 5 segundos, e imprimir na tela 
 do console (Serial USB) o valor lido. 
 Autor: Wasley dos Santos
 Ultima atualização: 15/01/2025 
 -------------------------------------------------------------------------*/

#include "hardware/i2c.h"  
#include "pico/stdlib.h"  
#include "ds1307.h"  

// Função para configurar a UART (para saída de texto no console serial)  
void setup_uart() {  
    // Configura a UART0 para 115200 bauds  
    uart_init(uart0, 115200);  
    
    // Define os pinos para a UART0 (Pico utiliza GPIO 0 e 1 por padrão)  
    gpio_set_function(8, GPIO_FUNC_UART);  // RX  
    gpio_set_function(9, GPIO_FUNC_UART);  // TX  
}  

// Função principal  
int main() {  
    stdio_init_all();  
    setup_uart();  // Configura a UART para saída de texto  

    // Inicializa a I2C com frequência de 100 kHz  
    i2c_init(i2c0, 100 * 1000);  

    // Configura GPIO 8 (SDA) e GPIO 9 (SCL) para I2C  
    gpio_set_function(8, GPIO_FUNC_I2C);  
    gpio_set_function(9, GPIO_FUNC_I2C);  
    gpio_pull_up(8);  
    gpio_pull_up(9);  

    // Configura a data e hora para 24/09/2024 13:27:00  
    configure_rtc_time(0, 00, 27, 13, 24, 24, 9);   

    // Leitura do tempo do DS1307 e exibição no console  
    printf("Data e hora configuradas:\n");  
    read_time_ds1307();  // Chama a função para ler e exibir a data e hora  

    // Loop para leitura contínua do tempo do DS1307  
    while (true) {  
        sleep_ms(5000);      // Aguarda 5 segundos antes de ler novamente  
        read_time_ds1307();  // Lê e exibe o tempo do DS1307  
    }  
}