#ifndef GAME_H
#define GAME_H

typedef enum {
	PLAYER_CROSS,
	PLAYER_CIRCLE
} active_player ;

typedef enum {
	VALUE_NONE,
	VALUE_CROSS,
	VALUE_CIRCLE
} field_value;

typedef enum {
	RESULT_KEEP_PLAYING,
	RESULT_CROSS,
	RESULT_CIRCLE,
	RESULT_TIE,
} game_result;

extern field_value game_state[3][3];
extern active_player game_player;

void game_state_toggle(field_value* field, active_player* player);
void game_reset_fields(field_value fields[3][3]);
game_result game_test_winner(field_value fields[3][3]);

#endif
