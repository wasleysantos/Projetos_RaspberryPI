/*-----------------------------------------------------------------------
 Projeto:  O LED vermelho deve ser acionado com um PWM de 1kHz.
 O duty cycle deve ser iniciado em 5% e atualizado a cada 2 segundos 
 em incrementos de 5%. Quando atingir o valor máximo, deve retornar a 5%. 
 O LED azul deve ser acionado com um PWM de 10kHz.
 Autor: Wasley dos Santos
 Ultima atualização: 13/01/2025 
 OBS: O processo de duty cycle demora 38 segundos, os leds se apagam 
 e acendem novamente em 5%. Led Vermelho 1 Khz e Azul 10 Khz
 -----------------------------------------------------------------------*/

#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Pinos dos LEDs
#define LED_VERMELHO 12 // Pino do LED vermelho
#define LED_AZUL 11     // Pino do LED azul

// Configurações do PWM
#define PERIODO_VERMELHO 1000 // 1 kHz para o LED vermelho
#define PERIODO_AZUL 100      // 10 kHz para o LED azul

#define DIVISOR_VERMELHO 125.0 
#define DIVISOR_AZUL 125.0

// Variáveis para o duty cycle do LED vermelho E LED azul
uint16_t duty_cycle_vermelho = 5; // Inicia com 5%
uint16_t duty_cycle_azul = 5; // Inicia com 5%
const uint16_t duty_cycle_max = 100; // Máximo de 100%
const uint16_t duty_cycle_increment = 5; // Incremento de 5%

void configurar_pwm(uint pino, uint16_t periodo, float divisor, uint16_t duty) {
    uint slice = pwm_gpio_to_slice_num(pino);
    gpio_set_function(pino, GPIO_FUNC_PWM);
    pwm_set_clkdiv(slice, divisor);
    pwm_set_wrap(slice, periodo);
    pwm_set_gpio_level(pino, (duty * periodo) / 100);
    pwm_set_enabled(slice, true);
}

int main() {
    

    configurar_pwm(LED_AZUL, PERIODO_AZUL, DIVISOR_AZUL, duty_cycle_azul);
    configurar_pwm(LED_VERMELHO, PERIODO_VERMELHO, DIVISOR_VERMELHO, duty_cycle_vermelho);
     

    // Loop infinito para atualizar o duty cycle do LED vermelho e LED azul
    while (true) {
        // Atualiza o duty cycle do LED vermelho e azul
        configurar_pwm(LED_VERMELHO, PERIODO_VERMELHO, DIVISOR_VERMELHO, duty_cycle_vermelho);
        configurar_pwm(LED_AZUL, PERIODO_AZUL, DIVISOR_AZUL, duty_cycle_azul);

        // Incrementa o duty cycle
        duty_cycle_vermelho += duty_cycle_increment;
        duty_cycle_azul += duty_cycle_increment;

        // Se o duty cycle exceder o máximo, reinicia e apaga o LED
        if ((duty_cycle_vermelho & duty_cycle_azul)> duty_cycle_max) {
            duty_cycle_vermelho = 0;
            duty_cycle_azul = 0; // Apaga o LED
            configurar_pwm(LED_VERMELHO, PERIODO_VERMELHO, DIVISOR_VERMELHO, duty_cycle_vermelho);
            configurar_pwm(LED_AZUL, PERIODO_AZUL, DIVISOR_AZUL, duty_cycle_azul);
            sleep_ms(500); // Aguarda meio segundo com o LED apagado
            duty_cycle_vermelho = 5; // Retorna a 5%
            duty_cycle_azul = 5; // Retorna a 5%
        }

        // Aguarda 2 segundos
        sleep_ms(2000);
    }

    return 0;
}