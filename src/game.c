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
	if (*field != VALUE_NONE) return;
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

void game_reset_fields(field_value fields[3][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			fields[i][j] = VALUE_NONE;
		}
	}
}

game_result game_test_winner(field_value fields[3][3]) {
	int cross_hits = 0, circle_hits = 0;
	for (int i = 0; i < 3; i++ ) {
		// Test rows
		cross_hits = 0;
		circle_hits = 0;
		for (int j = 0; j < 3; j++) {
			if (fields[j][i] == VALUE_CROSS) cross_hits++;
			if (fields[j][i] == VALUE_CIRCLE) circle_hits++;
		}
		if (cross_hits >= 3) return RESULT_CROSS;
		if (circle_hits >= 3) return RESULT_CIRCLE;

		// Test columns
		cross_hits = 0;
		circle_hits = 0;
		for (int j = 0; j < 3; j++) {
			if (fields[i][j] == VALUE_CROSS) cross_hits++;
			if (fields[i][j] == VALUE_CIRCLE) circle_hits++;
		}
		if (cross_hits >= 3) return RESULT_CROSS;
		if (circle_hits >= 3) return RESULT_CIRCLE;
	}

	// Test diagonally
	for (int i = 0; i < 3; i++ ) {
		cross_hits = 0;
		circle_hits = 0;
		if (fields[i][i] == VALUE_CROSS) cross_hits++;
		if (fields[i][i] == VALUE_CIRCLE) circle_hits++;
		if (cross_hits >= 3) return RESULT_CROSS;
		if (circle_hits >= 3) return RESULT_CIRCLE;
	}

	// Test reversed diagonally
	for (int i = 0; i < 3; i++ ) {
		cross_hits = 0;
		circle_hits = 0;
		if (fields[3-i][i] == VALUE_CROSS) cross_hits++;
		if (fields[3-i][i] == VALUE_CIRCLE) circle_hits++;
		if (cross_hits >= 3) return RESULT_CROSS;
		if (circle_hits >= 3) return RESULT_CIRCLE;
	}

	// Test if board is full, if not, keep playing
	int empty_hits = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (fields[i][j] == VALUE_NONE) empty_hits++;
		}
	}
	if (empty_hits > 0) return RESULT_KEEP_PLAYING;

	// No matches, i.e. tie if this point is reached
	return RESULT_TIE;
}
