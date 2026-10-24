#include "includes/Board_serializer.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

const long long kMaxSpeedLevel = 100;
const long long kMaxCellStatuses = 10;

void Board_serializer::write(Save_writer& out, const Game_board& board) const {
	pair<size_t, size_t> size = board.get_board_size();
	out.write_key("board");
	out.write_int(size.first);
	out.write_int(size.second);
	out.end_line();
	for (int y = 0; y < (int) size.second; y++) {
		for (int x = 0; x < (int) size.first; x++) {
			const vector<shared_ptr<Robot_status>>& statuses = board.get_square_statuses(x, y);
			out.write_key("cell");
			out.write_int(board.get_square_speed_level(x, y));
			out.write_bool(board.get_square_border(x, y));
			out.write_bool(board.get_square_discovered(x, y));
			out.write_int(statuses.size());
			out.end_line();
			for (size_t i = 0; i < statuses.size(); i++) {
				statuses_.write(out, *statuses[i]);
			}
		}
	}
}

Game_board Board_serializer::read(Save_reader& in) const {
	in.expect("board");
	int width = (int) in.read_int(Game_board::kMinSize, Game_board::kMaxSize, "board width");
	int height = (int) in.read_int(Game_board::kMinSize, Game_board::kMaxSize, "board height");
	Game_board board(width, height);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			in.expect("cell");
			board.set_square_speed_level(x, y, (int) in.read_int(1, kMaxSpeedLevel, "cell speed level"));
			bool border = in.read_bool("cell border");
			board.set_square_border(x, y, border);
			board.set_square_discovered(x, y, in.read_bool("cell discovered"));
			long long count = in.read_int(0, kMaxCellStatuses, "cell status count");
			if (border && count > 0) {
				throw runtime_error("obstacle cell (" + to_string(x) + ", " + to_string(y) + ") has statuses");
			}
			for (long long i = 0; i < count; i++) {
				board.add_square_status(x, y, statuses_.read(in, false));
			}
		}
	}
	return board;
}
