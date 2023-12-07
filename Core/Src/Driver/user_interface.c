#include "stm32f4xx.h"
#include "interrupts.h"
#include <stdbool.h>
#include <string.h>


#define settings_count 3

extern uint8_t PDLC_intensity;
extern uint8_t Address;

//todo: add settings struct

struct setting settings[settings_count];

uint8_t settings_current = 0;

bool settings_menu = true;

enum inputs{CW, CCW, PB}; // input kan; een clockwise rotatie-, counter-clockwise rotatie- of een push button signaal zijn

bool exit_UI(void)
{
//todo clear and update screen
	return true;
}

struct setting new_setting(char* name, uint32_t* value_ptr, uint32_t value_max, uint32_t value_min, void* fun_ptr)
{
	struct settings new_setting;

	strcpy(new_setting.name, name);

	new_setting.value_ptr = value_ptr;


	new_setting.value_max = value_max;
	new_setting.value_min = value_min;

	new_setting.fun_ptr = fun_ptr;

	return new_setting;
}

void init_UI(void)
{
	settings[0] = new_setting("exit ui", 0, 0, 0, &exit_UI);

	settings[1] = new_setting("intensity", &PDLC_intensity, 100, 0, 0);
	settings[2] = new_setting("ID", &Address, 31, 0, 0);


}

bool iterate_UI(enum inputs input)
{
	if (settings_menu)
	{
		switch(input) {
		  case CW:
			  settings_current = (settings_current + (settings_count + 1)) % settings_count;
			  //todo update menu and screen
			  break;

		  case CCW:
			  settings_current = (settings_current + (settings_count - 1)) % settings_count;
			  //todo update menu and screen
		    break;

		  case PB:
			  settings_menu = false;
			  //todo show setting and value BIG and update screen
			  break;

		  //default:
		    // code block
		}
	}
	else if (!settings_menu)
	{
		if (settings[settings_current].fun_ptr = 0)
		{
			switch(input) {
			  case CW:
				  *(settings[settings_current].value) = (*(settings[settings_current].value) + (settings[settings_current].max_value + 1)) % settings[settings_current].max_value;
				  //todo update screen
				  break;

			  case CCW:
				  *(settings[settings_current].value) = (*(settings[settings_current].value) + (settings[settings_current].max_value - 1)) % settings[settings_current].max_value;
				  //todo update screen
				break;

			  case PB:
				  settings_menu = true;
				  //todo show menu and update screen
				  break;

			  //default:
				// code block
			}
		}
		else
		{
			if (input = PB)
			{
				return (*(settings[settings_current].fun_ptr))();
			}
		}
	}

}
