#include "includes/Layout.h"

// размеры элементов интерфейса в пикселях
const float kCellSize = 24.0f;
const float kPanelWidth = 400.0f;
const float kBarHeight = 120.0f;
const float kPanelPadding = 15.0f;
const float kSlotWidth = 150.0f;
const float kSlotHeight = 100.0f;
const float kSlotGap = 10.0f;
const int kAbilitySlots = 4;
const float kInventorySlotSize = 44.0f;
const float kInventorySlotGap = 8.0f;
const float kInventoryTop = 250.0f;

Layout::Layout(int board_columns, int board_rows)
	: board_columns_(board_columns > 0 ? board_columns : 1), board_rows_(board_rows > 0 ? board_rows : 1) {
}

float Layout::get_cell_size() const {
	return kCellSize;
}

float Layout::get_board_width() const {
	return board_columns_ * kCellSize;
}

float Layout::get_board_height() const {
	return board_rows_ * kCellSize;
}

int Layout::get_window_width() const {
	return (int) (get_board_width() + kPanelWidth);
}

int Layout::get_window_height() const {
	return (int) (get_board_height() + kBarHeight);
}

SDL_FRect Layout::get_cell_rect(int x, int y) const {
	return { x * kCellSize, y * kCellSize, kCellSize, kCellSize };
}

bool Layout::find_cell(float pixel_x, float pixel_y, int& x, int& y) const {
	if (pixel_x < 0 || pixel_y < 0 || pixel_x >= get_board_width() || pixel_y >= get_board_height()) {
		return false;
	}
	x = (int) (pixel_x / kCellSize);
	y = (int) (pixel_y / kCellSize);
	return true;
}

SDL_FRect Layout::get_side_panel() const {
	return { get_board_width(), 0, kPanelWidth, get_board_height() };
}

SDL_FRect Layout::get_ability_bar() const {
	return { 0, get_board_height(), (float) get_window_width(), kBarHeight };
}

SDL_FRect Layout::get_ability_slot(int slot) const {
	float y = get_board_height() + (kBarHeight - kSlotHeight) / 2;
	return { kSlotGap + slot * (kSlotWidth + kSlotGap), y, kSlotWidth, kSlotHeight };
}

SDL_FRect Layout::get_inventory_slot(int slot) const {
	return { get_text_x() + slot * (kInventorySlotSize + kInventorySlotGap), kInventoryTop,
			 kInventorySlotSize, kInventorySlotSize };
}

float Layout::get_text_x() const {
	return get_board_width() + kPanelPadding;
}

float Layout::get_hint_x() const {
	return kSlotGap + kAbilitySlots * (kSlotWidth + kSlotGap) + 10;
}
