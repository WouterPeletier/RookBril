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

void display_menu(uint8_t n);
void display_setting(uint8_t n);
void init_settings(void);

#endif /* INC_USER_INTERFACE_H_ */
