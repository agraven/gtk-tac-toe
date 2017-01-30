#include "game.h"
#include "config.h"
#include <stdio.h>

field_value game_state[3][3] = {
	{VALUE_NONE, VALUE_NONE, VALUE_NONE},
	{VALUE_NONE, VALUE_NONE, VALUE_NONE},
	{VALUE_NONE, VALUE_NONE, VALUE_NONE}
};
active_player game_player = PLAYER_CROSS;

void game_state_toggle(field_value* field, active_player* player) {
	switch (*player) {
		case PLAYER_CROSS:
			*field = VALUE_CROSS;
			*player = PLAYER_CIRCLE;
			break;
		case PLAYER_CIRCLE:
			*field = VALUE_CIRCLE;
			*player = PLAYER_CROSS;
			break;
		default:
			fprintf(stderr, PACKAGE ": ERROR: Unrecognized player value\n");
	}
}
