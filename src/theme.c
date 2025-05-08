#include"../include/nuklear_config.h"
#include "../include/theme.h"

static PurpleTheme colors;

void setup_purple_theme(struct nk_context *ctx) {
    colors.background = nk_rgba(18, 18, 28, 255);       // deeper dark background
    colors.primary    = nk_rgba(138, 43, 226, 255);     // vibrant purple
    colors.secondary  = nk_rgba(75, 0, 130, 255);       // indigo accent
    colors.accent     = nk_rgba(186, 85, 211, 255);     // orchid (bright accent)
    colors.text       = nk_rgba(230, 230, 250, 255);    // light lavender text

    struct nk_style *s = &ctx->style;

    s->window.background = colors.background;
    s->window.fixed_background = nk_style_item_color(colors.background);
    s->window.border_color = colors.accent;
    s->text.color = colors.text;

    s->window.rounding = 8.0f;
    s->button.rounding = 6.0f;
    s->slider.rounding = 4.0f;

    apply_window_style(ctx);
    apply_button_style(ctx);
}

void apply_window_style(struct nk_context *ctx) {
    struct nk_style_window *win = &ctx->style.window;

    win->header.normal = nk_style_item_color(colors.secondary);
    win->header.active = nk_style_item_color(colors.primary);
    win->header.label_normal = colors.text;
    win->header.label_active = nk_rgb(255, 255, 255);
    win->header.padding = nk_vec2(15, 15);
    win->header.spacing = nk_vec2(0, 10);
}

void apply_button_style(struct nk_context *ctx) {
    struct nk_style_button *btn = &ctx->style.button;

    btn->normal = nk_style_item_color(colors.secondary);
    btn->hover = nk_style_item_color(nk_rgba(
        NK_MIN(colors.primary.r + 15, 255),
        NK_MIN(colors.primary.g + 15, 255),
        NK_MIN(colors.primary.b + 15, 255), 255));
    btn->active = nk_style_item_color(colors.primary);
    btn->border_color = colors.accent;
    btn->text_normal = colors.text;
    btn->text_hover = colors.text;
    btn->border = 1.5f;
}
