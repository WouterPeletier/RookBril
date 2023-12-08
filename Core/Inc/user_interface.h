/*
 * user_interface.h
 *
 *  Created on: Dec 8, 2023
 *      Author: gino
 */

#ifndef INC_USER_INTERFACE_H_
#define INC_USER_INTERFACE_H_

#define settings_font Font_11x18
#define max_string_length 10 // = SSD1306_WIDTH / settings_font.FontWidth - 1
#define settings_count 3

/*void display_menu(uint8_t n);
void display_setting(uint8_t n);*/

struct setting_struct {
  char name[max_string_length + 1];
  char display_string[max_string_length + 1];

  int32_t value_max;
  int32_t value_min;
  int32_t* value_ptr;
  int val_digit_count;

  void (*fun_ptr)(void);
};

enum inputs{CW, CCW, PB}; // input kan; een clockwise rotatie-, counter-clockwise rotatie- of een push button signaal zijn

bool iterate_UI(enum inputs input);

void init_settings(void);

#endif /* INC_USER_INTERFACE_H_ */
