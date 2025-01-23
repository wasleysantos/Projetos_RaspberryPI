/*----------------------------------------------------------------------
 Projeto: Elabore um código utilizando a interfaces UART0 e conecte 
 os fios TX e RX atribuídos à essa interface entre. Essa estrutura 
 envia dados e recebe os dados na mesma interface, apenas para verificar 
 seu funcionamento. Utilize a função scanf da biblioteca stdio para 
 enviar via console um dado à placa, em seguida, transmita da UART0 
 para a UART1, e por fim, transmita o dado recebido para o console 
 utilizando o printf.
 Autor: Wasley dos Santos
 Ultima atualização: 15/01/2025 
 -----------------------------------------------------------------------*/
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include <stdio.h>

#define UART0_ID uart0
#define UART1_ID uart1
#define BAUD_RATE 115200

#define UART0_TX_PIN 0
#define UART0_RX_PIN 1
#define UART1_TX_PIN 4
#define UART1_RX_PIN 5

int main() {
    stdio_init_all();
    uart_init(UART0_ID, BAUD_RATE);
    uart_init(UART1_ID, BAUD_RATE);

    // Configura os pinos para UART0
    gpio_set_function(UART0_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART0_RX_PIN, GPIO_FUNC_UART);

    // Configura os pinos para UART1
    gpio_set_function(UART1_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART1_RX_PIN, GPIO_FUNC_UART);

    // Buffer para armazenar a mensagem
    char mensagem[100];

    // Recebe a mensagem via console
    printf("Digite mensagem para enviar via UART 0:");
    scanf("%99s", mensagem);

    // Envia a mensagem pela UART0
    uart_puts(UART0_ID, mensagem);

    // Aguarda e verifica se há dados disponíveis na UART1
    sleep_ms(300); // Dá um tempo para a transmissão
    if (uart_is_readable(UART1_ID)) {
        char recebido[100];
        int i = 0;

        // Lê a mensagem recebida da UART1
        while (uart_is_readable(UART1_ID) && i < sizeof(recebido) - 1) {
            recebido[i++] = uart_getc(UART1_ID);
        }
        recebido[i] = '\0'; // Finaliza a string recebida
        sleep_ms(300);
        // Imprime a mensagem recebida no console
        printf("\nMensagem recebida da UART1: %s\n",mensagem, recebido);
       
        
        
    } else {
        printf("Nenhuma mensagem recebida da UART1.\n");
    } 
  getchar();
    
}
