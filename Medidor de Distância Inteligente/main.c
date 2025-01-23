/*---------------------------------------------------------------
 Projeto: Medidor de Distância Inteligente com Alerta Sonoro e Visual
 Autor: Wasley dos Santos
 Última atualização: 19/01/2025

 Descrição:
 Este projeto utiliza um microcontrolador Raspberry Pi Pico W para medir 
 a distância de objetos usando um sensor ultrassônico HC-SR04 e exibir 
 as informações em um display LCD 16x2 via I2C. O sistema também possui 
 alertas visuais (LEDs) e sonoros (buzzer) para indicar a proximidade de 
 objetos, sendo útil para aplicações como prevenção de colisões ou 
 sistemas de monitoramento.
 ---------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

// Definições do LCD
const int LCD_CLEARDISPLAY = 0x01;
const int LCD_RETURNHOME = 0x02;
const int LCD_ENTRYMODESET = 0x04;
const int LCD_DISPLAYCONTROL = 0x08;
const int LCD_FUNCTIONSET = 0x20;
const int LCD_SETDDRAMADDR = 0x80;

const int LCD_ENTRYLEFT = 0x02;
const int LCD_DISPLAYON = 0x04;
const int LCD_2LINE = 0x08;
const int LCD_BACKLIGHT = 0x08;
const int LCD_ENABLE_BIT = 0x04;

static int addr = 0x27; // Endereço I2C do LCD

#define LCD_CHARACTER  1
#define LCD_COMMAND    0

#define TRIG_PIN 15
#define ECHO_PIN 14

#define LED_1 21
#define LED_2 20
#define LED_3 19
#define LED_4 18

#define BUZZER_PIN 22

#define TIMEOUT 30000

#define DISTANCIA_1M 100
#define DISTANCIA_2M 200
#define DISTANCIA_3M 300
#define DISTANCIA_4M 400
#define DISTANCIA_5CM 5

void i2c_write_byte(uint8_t val) {
    #ifdef i2c_default
    i2c_write_blocking(i2c_default, addr, &val, 1, false);
    #endif
}

void lcd_toggle_enable(uint8_t val) {
    sleep_us(600);
    i2c_write_byte(val | LCD_ENABLE_BIT);
    sleep_us(600);
    i2c_write_byte(val & ~LCD_ENABLE_BIT);
    sleep_us(600);
}

void lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = mode | (val & 0xF0) | LCD_BACKLIGHT;
    uint8_t low = mode | ((val << 4) & 0xF0) | LCD_BACKLIGHT;

    i2c_write_byte(high);
    lcd_toggle_enable(high);
    i2c_write_byte(low);
    lcd_toggle_enable(low);
}

void lcd_clear(void) {
    lcd_send_byte(LCD_CLEARDISPLAY, LCD_COMMAND);
}

void lcd_set_cursor(int line, int position) {
    int val = (line == 0) ? 0x80 + position : 0xC0 + position;
    lcd_send_byte(val, LCD_COMMAND);
}

void lcd_char(char val) {
    lcd_send_byte(val, LCD_CHARACTER);
}

void lcd_string(const char *s) {
    while (*s) {
        lcd_char(*s++);
    }
}

void lcd_init() {
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x03, LCD_COMMAND);
    lcd_send_byte(0x02, LCD_COMMAND);

    lcd_send_byte(LCD_ENTRYMODESET | LCD_ENTRYLEFT, LCD_COMMAND);
    lcd_send_byte(LCD_FUNCTIONSET | LCD_2LINE, LCD_COMMAND);
    lcd_send_byte(LCD_DISPLAYCONTROL | LCD_DISPLAYON, LCD_COMMAND);
    lcd_clear();
}

void init_hcsr04() {
    gpio_init(TRIG_PIN);
    gpio_set_dir(TRIG_PIN, GPIO_OUT);
    gpio_init(ECHO_PIN);
    gpio_set_dir(ECHO_PIN, GPIO_IN);
}

float get_distance() {
    gpio_put(TRIG_PIN, 0);
    sleep_us(2);
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    uint64_t start_time = time_us_64();
    while (gpio_get(ECHO_PIN) == 0) {
        if (time_us_64() - start_time > TIMEOUT) {
            return -1;
        }
    }
    start_time = time_us_64();

    while (gpio_get(ECHO_PIN) == 1) {
        if (time_us_64() - start_time > TIMEOUT) {
            return -1;
        }
    }
    uint64_t end_time = time_us_64();

    return (float)(end_time - start_time) * 0.0343 / 2;
}

void controlar_leds(int leds) {
    gpio_put(LED_1, leds & 0x01);
    gpio_put(LED_2, leds & 0x02);
    gpio_put(LED_3, leds & 0x04);
    gpio_put(LED_4, leds & 0x08);
}

void avaliar_distancia(float distance) {
    char mensagem[16];
    lcd_clear();

    if (distance <= DISTANCIA_5CM) {
        sprintf(mensagem, "BATEUUU");
        lcd_set_cursor(0, 0);
        lcd_string(mensagem);
        controlar_leds(0x0F);
        sleep_ms(50);
        controlar_leds(0x00);
        gpio_put(BUZZER_PIN, 1);  // Liga o buzzer
        sleep_ms(50);            // Mantém ligado por 500ms
        gpio_put(BUZZER_PIN, 0);  // Desliga o buzzer
    } else if (distance > DISTANCIA_5CM && distance <= DISTANCIA_1M) {
        sprintf(mensagem, "Muito proximo");
        lcd_set_cursor(0, 0);
        lcd_string(mensagem);
        sprintf(mensagem, "%.2f cm", distance);
        lcd_set_cursor(1, 0);
        lcd_string(mensagem);
        controlar_leds(0x0F);
        gpio_put(BUZZER_PIN, 1);  // Liga o buzzer
        sleep_ms(1000);            // Mantém ligado por 500ms
        gpio_put(BUZZER_PIN, 0);  // Desliga o buzzer
    } else if (distance <= DISTANCIA_2M) {
        sprintf(mensagem, "Proximo");
        lcd_set_cursor(0, 0);
        lcd_string(mensagem);
        sprintf(mensagem, "%.2f cm", distance);
        lcd_set_cursor(1, 0);
        lcd_string(mensagem);
        controlar_leds(0x07);
    } else if (distance <= DISTANCIA_3M) {
        sprintf(mensagem, "Medio");
        lcd_set_cursor(0, 0);
        lcd_string(mensagem);
        sprintf(mensagem, "%.2f cm", distance);
        lcd_set_cursor(1, 0);
        lcd_string(mensagem);
        controlar_leds(0x03);
    } else if (distance <= DISTANCIA_4M) {
        sprintf(mensagem, "Distante");
        lcd_set_cursor(0, 0);
        lcd_string(mensagem);
        sprintf(mensagem, "%.2f cm", distance);
        lcd_set_cursor(1, 0);
        lcd_string(mensagem);
        controlar_leds(0x01);
    } else {
        sprintf(mensagem, "Muito longe");
        lcd_set_cursor(0, 0);
        lcd_string(mensagem);
        controlar_leds(0x00);
    }
}

int main() {
    stdio_init_all();

    init_hcsr04();

    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    #if !defined(i2c_default)
        #warning i2c/lcd_1602_i2c example requires a board with I2C pins
    #else
        i2c_init(i2c_default, 100 * 1000);
        gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
        gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
        gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
        gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
        bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

        lcd_init();

        gpio_init(LED_1);
        gpio_set_dir(LED_1, GPIO_OUT);
        gpio_init(LED_2);
        gpio_set_dir(LED_2, GPIO_OUT);
        gpio_init(LED_3);
        gpio_set_dir(LED_3, GPIO_OUT);
        gpio_init(LED_4);
        gpio_set_dir(LED_4, GPIO_OUT);

        while (1) {
            float distance = get_distance();
            avaliar_distancia(distance);
            sleep_ms(100);
        }
    #endif

    return 0;
}
