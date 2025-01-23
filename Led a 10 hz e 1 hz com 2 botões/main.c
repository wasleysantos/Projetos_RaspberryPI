/*---------------------------------------------------------------
 Projeto: Quando o botão A for pressionado 5 vezes, o LED deve 
 piscar por 10 segundos na frequência de 10 Hz.
 O botão B permite alterar a frequência do LED para 1 Hz.
 (OBS: Aperte o botão B para mudar frequencia para 1hz antes de 
 apertar o botão A, ou durante o LED piscando a 10 Hz.)
 Autor: Wasley dos Santos
 Ultima atualização: 19/01/2025 
 ---------------------------------------------------------------*/

#include <stdio.h>
#include "pico/stdlib.h"

#define LED_GREEN 11
#define BTN_A 5
#define BTN_B 6

typedef enum {
    ESTADO_INATIVO,       // Estado de espera ou inatividade
    ESTADO_ALTERAR_FREQ,  // Estado para alterar a frequência do LED
    ESTADO_CONTAR_PRESS,  // Estado para contar as pressões do botão
    ESTADO_PISCAR_LED     // Estado para piscar o LED
} Estado;

int main()
{
    stdio_init_all();

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, 0); // Garante que o LED comece apagado

    gpio_init(BTN_A);
    gpio_set_dir(BTN_A, GPIO_IN);
    gpio_pull_down(BTN_A); // Habilita o resistor de pull-down interno para o botão A

    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_down(BTN_B); // Habilita o resistor de pull-down interno para o botão B

    int contador = 0;
    int frequencia_hz = 10; // Frequência padrão do LED
    Estado estado_atual = ESTADO_INATIVO;

    // Loop principal
    while (1)
    {
        switch (estado_atual)
        {
            case ESTADO_INATIVO:
                // Verifica se o botão B foi pressionado
                if (gpio_get(BTN_B) == 1)
                {
                    sleep_ms(150); // Debounce

                    if (gpio_get(BTN_B) == 1)
                    {
                        frequencia_hz = 1; // Altera a frequência para 1 Hz
                        printf("Botão B pressionado: frequência alterada para 1 Hz\n");
                        while (gpio_get(BTN_B) == 1); // Aguarda a liberação do botão
                    }
                }

                // Verifica se o botão A foi pressionado
                if (gpio_get(BTN_A) == 1)
                {
                    sleep_ms(150); // Debounce

                    if (gpio_get(BTN_A) == 1)
                    {
                        contador++;
                        printf("Botão A pressionado: %dx\n", contador);
                        while (gpio_get(BTN_A) == 1); // Aguarda a liberação do botão
                    }
                }

                if (contador >= 5)
                {
                    estado_atual = ESTADO_PISCAR_LED;
                }
                break;

            case ESTADO_PISCAR_LED:
                printf("LED piscando na frequência de %d Hz\n", frequencia_hz);

                // Calcula os tempos de delay com base na frequência
                int delay_ms = 1000 / (frequencia_hz * 2);

                // Pisca o LED por 10 segundos
                for (int i = 0; i < (10 * frequencia_hz); i++)
                {
                    gpio_put(LED_GREEN, 1); // Acende o LED
                    sleep_ms(delay_ms);
                    gpio_put(LED_GREEN, 0); // Apaga o LED
                    sleep_ms(delay_ms);
                    if (gpio_get(BTN_B) == 1){
                        for (int i = 0; i <=5; i++) { 
                          printf("Frequência alterada para 1 Hz\n"); 
                          gpio_put(LED_GREEN, 1); // Acende o LED  
                          sleep_ms(1000); // Mantém o LED ligado por 1 segundo (1000 ms)  
                          gpio_put(LED_GREEN, 0); // Apaga o LED  
                          sleep_ms(1000); // Mantém o LED desligado por 1 segundo (1000 ms)  
                      }
                    }
                }

                contador = 0; // Reseta o contador após piscar o LED
                frequencia_hz = 10; // Reseta a frequência padrão
                estado_atual = ESTADO_INATIVO;
                break;

            default:
                estado_atual = ESTADO_INATIVO;
                break;
        }

        sleep_ms(100); // Delay para evitar uso excessivo da CPU
    }

    return 0;
}