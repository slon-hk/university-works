#ifndef OOP_PROJECT_DISPLAY_MODE_SELECTOR_H
#define OOP_PROJECT_DISPLAY_MODE_SELECTOR_H

#include <string>

#include "Display_mode.h"

// Хранит текущий режим отображения и циклически переключает режимы.
class Display_mode_selector {
private:
	Display_mode mode_;

public:
	Display_mode_selector();

	Display_mode get_mode() const;
	std::string get_name() const;
	// переход к следующему режиму по кругу
	void next();
};

#endif
