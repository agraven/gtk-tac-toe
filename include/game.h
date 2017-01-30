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

extern field_value game_state[3][3];
extern active_player game_player;

void game_state_toggle(field_value* field, active_player* player);

#endif
