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

struct setting_struct { // elke instelling word in zo'n struct gedefinieerd
  char name[max_string_length + 1]; // naam van de instelling
  char display_string[max_string_length + 1]; // string die in het menu wordt geplaatst (naam + value waarde)

  int32_t value_max; // maximum waarde van de instelling
  int32_t value_min; // minimum waarde van de instelling
  int32_t* value_ptr; // pointer die wijst naar de value van de instelling
  int val_digit_count; // het aantal cijfers in de huidige waarde van de instelling

  void (*fun_ptr)(bool* bool_ptr); // eventuele pointer naar een functie van de instelling
  // je kan alleen een instelbare waarde hebben of alleen een functie die wordt uitgevoerd bij deze instelling.
};

enum inputs{CW, CCW, PB}; // input kan; een clockwise rotatie-, counter-clockwise rotatie- of een push button signaal zijn

bool iterate_UI(enum inputs input);

void init_settings(void);

#endif /* INC_USER_INTERFACE_H_ */
