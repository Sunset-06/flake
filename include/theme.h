// theme.h
#ifndef THEME_H
#define THEME_H

#include <SDL2/SDL.h>
typedef struct {
    struct nk_color background;
    struct nk_color primary;
    struct nk_color secondary;
    struct nk_color accent;
    struct nk_color text;
} PurpleTheme;

void setup_purple_theme(struct nk_context *ctx);
void apply_button_style(struct nk_context *ctx);
void apply_window_style(struct nk_context *ctx);

#endif