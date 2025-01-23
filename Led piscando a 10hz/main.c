/*---------------------------------------------------------------
 Projeto: Quando o botão A for pressionado 5 vezes, o LED deve 
 ser piscar por 10 segundos na frequência de 10 Hz.
 Autor: Wasley dos Santos
 Ultima atualização: 16/01/2025 
 ---------------------------------------------------------------*/

#include <stdio.h>  
#include "pico/stdlib.h"  

#define LED_GREEN 11  
#define BTN_A 5 

int main()   
{   stdio_init_all();
    gpio_init(LED_GREEN);  
    gpio_set_dir(LED_GREEN, GPIO_OUT);  
    gpio_put(LED_GREEN, 0); // Garante que o LED comece apagado  

    gpio_init(BTN_A);  
    gpio_set_dir(BTN_A, GPIO_IN);  

    int contador = 0;   

    // Loop
    while (1)  
    {  
        // Verifica se o botão está pressionado  
        if (gpio_get(BTN_A) == 0)   
        {  
            // Um pequeno delay para evitar múltiplas contagens quando apertar o botão  
            sleep_ms(100);  

            // Incrementa o contador se o botão ainda estiver pressionado  
            if (gpio_get(BTN_A) == 0)   
            {  
                printf("Botão A pressionado: %dx\n", contador); 
                contador++;  
                 
                
                // Aguarda a liberação do botão  
                while (gpio_get(BTN_A) == 0);  
            }  
        }  

        // Acende o LED se o botão for pressionado 5 vezes  
        if (contador >= 5)  
        {  
            printf("Botão A pressionado: 5x\n");
            printf("LED piscando na frequência de 10 HZ\n");
            // Pisca o LED por 10 segundos a 10 Hz (100 ms ON, 100 ms OFF)  
            for (int i = 0; i < 100; i++) // 10 segundos = 100 ciclos de 100 ms  
            {  
                gpio_put(LED_GREEN, 1); // Acende o LED  
                sleep_ms(50); // Para 10 Hz, 50 ms ON (100 ms total) 
                gpio_put(LED_GREEN, 0); // Apaga o LED  
                sleep_ms(50); // Para 10 Hz, 50 ms ON (100 ms total)
                
            }  
            contador = 0;  
        }  
        else  
        {  
            gpio_put(LED_GREEN, 0); // Mantém o LED apagado  
        }  

        sleep_ms(1000);   
    }  

    return 0;  
}