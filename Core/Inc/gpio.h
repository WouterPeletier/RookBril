#ifndef __GPIO_H__
#define __GPIO_H__

#include <stm32f4xx.h>

void gpio_write(GPIO_TypeDef* port, uint8_t pin, uint8_t level);
uint8_t gpio_read(GPIO_TypeDef* port, uint8_t pin);
void gpio_toggle(GPIO_TypeDef* port, uint8_t pin);

typedef struct {
    GPIO_TypeDef* Port;
    uint8_t Pin;
    uint8_t Mode;
    uint8_t Type;
    uint8_t Pull;
    uint8_t Speed;
    uint8_t Alternate;
} gpio_opt_t;

void init_gpio_options(gpio_opt_t options);
void init_gpio(GPIO_TypeDef* port, uint8_t pin, uint8_t mode, uint8_t alternate, uint8_t type, uint8_t pull, uint8_t speed);

#define GPIO_0                  (0x00)
#define GPIO_1                  (0x01)
#define GPIO_2                  (0x02)
#define GPIO_3                  (0x03)
#define GPIO_4                  (0x04)
#define GPIO_5                  (0x05)
#define GPIO_6                  (0x06)
#define GPIO_7                  (0x07)
#define GPIO_8                  (0x08)
#define GPIO_9                  (0x09)
#define GPIO_10                 (0x0A)
#define GPIO_11                 (0x0B)
#define GPIO_12                 (0x0C)
#define GPIO_13                 (0x0D)
#define GPIO_14                 (0x0E)
#define GPIO_15                 (0x0F)


#define GPIO_OUTPUT_LOW         (0x00)
#define GPIO_OUTPUT_HIGH        (0x01)

#define GPIO_MODER_INPUT        (0x00)
#define GPIO_MODER_OUTPUT       (0x01)
#define GPIO_MODER_ALT          (0x02)
#define GPIO_MODER_ANALOG       (0x03)

#define GPIO_OTYPER_PUSHPULL    (0x00)
#define GPIO_OTYPER_OPENDRAIN   (0x01)

#define GPIO_PULL_NONE          (0x00)
#define GPIO_PULL_UP            (0x01)
#define GPIO_PULL_DOWN          (0x02)

#define GPIO_OSPEEDR_LOW        (0x00)
#define GPIO_OSPEEDR_MED        (0x01)
#define GPIO_OSPEEDR_HIGH       (0x02)
#define GPIO_OSPEEDR_VERYHIGH   (0x03)

#define GPIO_ALTFUNC_0          (0x00)
#define GPIO_ALTFUNC_1          (0x01)
#define GPIO_ALTFUNC_2          (0x02)
#define GPIO_ALTFUNC_3          (0x03)
#define GPIO_ALTFUNC_4          (0x04)
#define GPIO_ALTFUNC_5          (0x05)
#define GPIO_ALTFUNC_6          (0x06)
#define GPIO_ALTFUNC_7          (0x07)
#define GPIO_ALTFUNC_8          (0x08)
#define GPIO_ALTFUNC_9          (0x09)
#define GPIO_ALTFUNC_10         (0x0A)
#define GPIO_ALTFUNC_11         (0x0B)
#define GPIO_ALTFUNC_12         (0x0C)
#define GPIO_ALTFUNC_13         (0x0D)
#define GPIO_ALTFUNC_14         (0x0E)
#define GPIO_ALTFUNC_15         (0x0F)

#endif /* __GPIO_H__ */
