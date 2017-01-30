#include <gtk/gtk.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "game.h"
#include "application.h"
#include "window.h"

#define LABEL_NONE " "
#define LABEL_CROSS "X"
#define LABEL_CIRCLE "O"
struct _MainAppWindow { GtkApplicationWindow parent; };
struct _MainAppWindowClass { GtkApplicationWindowClass parent_class; };
typedef struct _MainAppWindowPrivate MainAppWindowPrivate;
struct _MainAppWindowPrivate {
	GtkWidget* a1_button;
	GtkWidget* b1_button;
	GtkWidget* c1_button;
	GtkWidget* a2_button;
	GtkWidget* b2_button;
	GtkWidget* c2_button;
	GtkWidget* a3_button;
	GtkWidget* b3_button;
	GtkWidget* c3_button;
	GtkWidget* player_label;
};

G_DEFINE_TYPE_WITH_PRIVATE(MainAppWindow, main_app_window, GTK_TYPE_APPLICATION_WINDOW);

void button_set_label_to_field_value(field_value* value, GtkButton* button) {
	switch (*value) {
		case VALUE_NONE:
			gtk_button_set_label(button, LABEL_NONE);
			break;
		case VALUE_CROSS:
			gtk_button_set_label(button, LABEL_CROSS);
			break;
		case VALUE_CIRCLE:
			gtk_button_set_label(button, LABEL_CIRCLE);
			break;
		default:
			fprintf(stderr, PACKAGE ": ERROR: Unrecognized field value\n");
	}
}

void button_clear_labels(MainAppWindowPrivate* private) {
	field_value field = VALUE_NONE;
	button_set_label_to_field_value(&field, GTK_BUTTON(private->a1_button));
	button_set_label_to_field_value(&field, GTK_BUTTON(private->b1_button));
	button_set_label_to_field_value(&field, GTK_BUTTON(private->c1_button));
	button_set_label_to_field_value(&field, GTK_BUTTON(private->a2_button));
	button_set_label_to_field_value(&field, GTK_BUTTON(private->b2_button));
	button_set_label_to_field_value(&field, GTK_BUTTON(private->c2_button));
	button_set_label_to_field_value(&field, GTK_BUTTON(private->a3_button));
	button_set_label_to_field_value(&field, GTK_BUTTON(private->b3_button));
	button_set_label_to_field_value(&field, GTK_BUTTON(private->c3_button));
}

void update_player_label(GtkLabel* label, active_player* player) {
	switch (*player) {
		case PLAYER_CROSS:
			gtk_label_set_text(label, "Aktiv Spiller: X");
			break;
		case PLAYER_CIRCLE:
			gtk_label_set_text(label, "Aktiv Spiller: O");
			break;
		default:
			gtk_label_set_text(label, "Aktiv Spiller: ukendt");
	}
}

// TODO: find a more sensible name for this function
void update_from_pressed_button(field_value* field, active_player* player, GtkButton* button, MainAppWindow* window) {
	game_state_toggle(field, player);
	button_set_label_to_field_value(field, button);
	MainAppWindowPrivate* private = main_app_window_get_instance_private(window);
	update_player_label(GTK_LABEL(private->player_label), player);
	switch (game_test_winner(game_state)) {
		case RESULT_CROSS: {
			GtkWidget* dialog = gtk_message_dialog_new(
				GTK_WINDOW(window),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_INFO,
				GTK_BUTTONS_CLOSE,
				"Kryds vandt! Luk dialogen for at starte nyt spil."
			);
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
			game_reset_fields(game_state);
			button_clear_labels(private);
			} break;
		case RESULT_CIRCLE:	{
			GtkWidget* dialog = gtk_message_dialog_new(
				GTK_WINDOW(window),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_INFO,
				GTK_BUTTONS_CLOSE,
				"Bolle vandt! Luk dialogen for at starte nyt spil."
			);
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
			game_reset_fields(game_state);
			button_clear_labels(private);
			} break;
		case RESULT_TIE:	{
			GtkWidget* dialog = gtk_message_dialog_new(
				GTK_WINDOW(window),
				GTK_DIALOG_DESTROY_WITH_PARENT,
				GTK_MESSAGE_INFO,
				GTK_BUTTONS_CLOSE,
				"Uafgjort. Luk dialogen for at spille igen."
			);
			gtk_dialog_run(GTK_DIALOG(dialog));
			gtk_widget_destroy(dialog);
			game_reset_fields(game_state);
			button_clear_labels(private);
			} break;
		case RESULT_KEEP_PLAYING:
			return;
			break;
		default:
			fprintf(stderr, PACKAGE ": ERROR: Unrecognized game result type. Please report bug.\n");
			break;
	}
}

static void activate_close(GSimpleAction* action, GVariant* parameter, gpointer win) {
	gtk_window_close(win);
}

static void activate_a1pressed(GSimpleAction* action, GVariant* parameter, gpointer win) {
	MainAppWindowPrivate* private = main_app_window_get_instance_private(MAIN_APP_WINDOW(win));
	update_from_pressed_button(&game_state[0][0], &game_player, GTK_BUTTON(private->a1_button), win);
}

static void activate_b1pressed(GSimpleAction* action, GVariant* parameter, gpointer win) {
	MainAppWindowPrivate* private = main_app_window_get_instance_private(MAIN_APP_WINDOW(win));
	update_from_pressed_button(&game_state[1][0], &game_player, GTK_BUTTON(private->b1_button), win);
}

static void activate_c1pressed(GSimpleAction* action, GVariant* parameter, gpointer win) {
	MainAppWindowPrivate* private = main_app_window_get_instance_private(MAIN_APP_WINDOW(win));
	update_from_pressed_button(&game_state[2][0], &game_player, GTK_BUTTON(private->c1_button), win);
}

static void activate_a2pressed(GSimpleAction* action, GVariant* parameter, gpointer win) {
	MainAppWindowPrivate* private = main_app_window_get_instance_private(MAIN_APP_WINDOW(win));
	update_from_pressed_button(&game_state[0][1], &game_player, GTK_BUTTON(private->a2_button), win);
}

static void activate_b2pressed(GSimpleAction* action, GVariant* parameter, gpointer win) {
	MainAppWindowPrivate* private = main_app_window_get_instance_private(MAIN_APP_WINDOW(win));
	update_from_pressed_button(&game_state[1][1], &game_player, GTK_BUTTON(private->b2_button), win);
}

static void activate_c2pressed(GSimpleAction* action, GVariant* parameter, gpointer win) {
	MainAppWindowPrivate* private = main_app_window_get_instance_private(MAIN_APP_WINDOW(win));
	update_from_pressed_button(&game_state[2][1], &game_player, GTK_BUTTON(private->c2_button), win);
}

static void activate_a3pressed(GSimpleAction* action, GVariant* parameter, gpointer win) {
	MainAppWindowPrivate* private = main_app_window_get_instance_private(MAIN_APP_WINDOW(win));
	update_from_pressed_button(&game_state[0][2], &game_player, GTK_BUTTON(private->a3_button), win);
}

static void activate_b3pressed(GSimpleAction* action, GVariant* parameter, gpointer win) {
	MainAppWindowPrivate* private = main_app_window_get_instance_private(MAIN_APP_WINDOW(win));
	update_from_pressed_button(&game_state[1][2], &game_player, GTK_BUTTON(private->b3_button), win);
}

static void activate_c3pressed(GSimpleAction* action, GVariant* parameter, gpointer win) {
	MainAppWindowPrivate* private = main_app_window_get_instance_private(MAIN_APP_WINDOW(win));
	update_from_pressed_button(&game_state[2][2], &game_player, GTK_BUTTON(private->c3_button), win);
}

static GActionEntry win_actions[] = {
	{"close", activate_close, NULL, NULL, NULL},

	{"a1pressed", activate_a1pressed, NULL, NULL, NULL},
	{"b1pressed", activate_b1pressed, NULL, NULL, NULL},
	{"c1pressed", activate_c1pressed, NULL, NULL, NULL},
	{"a2pressed", activate_a2pressed, NULL, NULL, NULL},
	{"b2pressed", activate_b2pressed, NULL, NULL, NULL},
	{"c2pressed", activate_c2pressed, NULL, NULL, NULL},
	{"a3pressed", activate_a3pressed, NULL, NULL, NULL},
	{"b3pressed", activate_b3pressed, NULL, NULL, NULL},
	{"c3pressed", activate_c3pressed, NULL, NULL, NULL}
};

static void main_app_window_init (MainAppWindow *win) {
	gtk_widget_init_template(GTK_WIDGET(win));

	g_action_map_add_action_entries(G_ACTION_MAP(win), win_actions, G_N_ELEMENTS(win_actions), win);
	MainAppWindowPrivate* private = main_app_window_get_instance_private(win);
	update_player_label(GTK_LABEL(private->player_label), &game_player);
}
static void main_app_window_class_init (MainAppWindowClass *class) {
	gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/net/agraven/test/main.ui");
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), MainAppWindow, a1_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), MainAppWindow, b1_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), MainAppWindow, c1_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), MainAppWindow, a2_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), MainAppWindow, b2_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), MainAppWindow, c2_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), MainAppWindow, a3_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), MainAppWindow, b3_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), MainAppWindow, c3_button);
	gtk_widget_class_bind_template_child_private(GTK_WIDGET_CLASS(class), MainAppWindow, player_label);
}
MainAppWindow* main_app_window_new (MainApp *app) {
	return g_object_new (MAIN_APP_WINDOW_TYPE, "application", app, NULL);
}
void main_app_window_open (MainAppWindow *win, GFile* file) {
}
