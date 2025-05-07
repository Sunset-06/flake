#include "../include/theme.h"

static PurpleTheme colors;

void setup_purple_theme(struct nk_context *ctx) {

    colors.background = nk_rgba(30, 15, 50, 255);
    colors.primary = nk_rgba(120, 70, 200, 255);
    colors.secondary = nk_rgba(80, 40, 140, 255);
    colors.accent = nk_rgba(200, 180, 255, 255);
    colors.text = nk_rgba(240, 240, 255, 255);

    struct nk_style *s = &ctx->style;
    // Base style
    s->window.background = colors.background;
    s->window.fixed_background = nk_style_item_color(colors.background);
    s->window.border_color = colors.accent;
    s->text.color = colors.text;
    
    // Modern rounded corners
    s->window.rounding = 8.0f;
    s->button.rounding = 6.0f;
    s->slider.rounding = 4.0f;
    
    // Apply component styles
    apply_window_style(ctx);
    apply_button_style(ctx);
}

void apply_window_style(struct nk_context *ctx) {
    struct nk_style_window *win = &ctx->style.window;
    
    win->header.normal = nk_style_item_color(colors.secondary);
    win->header.active = nk_style_item_color(colors.primary);
    win->header.label_normal = colors.text;
    win->header.label_active = nk_rgb(255,255,255);
    win->header.padding = nk_vec2(15,15);
    win->header.spacing = nk_vec2(0,10);
}

void apply_button_style(struct nk_context *ctx) {
    struct nk_style_button *btn = &ctx->style.button;
    
    btn->normal = nk_style_item_color(colors.secondary);
    btn->hover = nk_style_item_color(nk_rgba(
        colors.primary.r + 20,
        colors.primary.g + 20,
        colors.primary.b + 20, 255));
    btn->active = nk_style_item_color(colors.primary);
    btn->border_color = colors.accent;
    btn->text_normal = colors.text;
    btn->text_hover = colors.text;
    btn->border = 1.5f;
}