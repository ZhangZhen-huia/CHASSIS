//
// Created by RM UI Designer
//

#include "ui_default_SuperPower_0.h"
#include "string.h"

#define FRAME_ID 0
#define GROUP_ID 4
#define START_ID 0

ui_string_frame_t ui_default_SuperPower_0;

ui_interface_string_t* ui_default_SuperPower_Cap = &ui_default_SuperPower_0.option;

void _ui_init_default_SuperPower_0() {
    ui_default_SuperPower_0.option.figure_name[0] = FRAME_ID;
    ui_default_SuperPower_0.option.figure_name[1] = GROUP_ID;
    ui_default_SuperPower_0.option.figure_name[2] = START_ID;
    ui_default_SuperPower_0.option.operate_tpyel = 1;
    ui_default_SuperPower_0.option.figure_tpye = 7;
    ui_default_SuperPower_0.option.layer = 0;
    ui_default_SuperPower_0.option.font_size = 20;
    ui_default_SuperPower_0.option.start_x = 879;
    ui_default_SuperPower_0.option.start_y = 250;
    ui_default_SuperPower_0.option.color = 6;
    ui_default_SuperPower_0.option.str_length = 7;
    ui_default_SuperPower_0.option.width = 3;
    strcpy(ui_default_SuperPower_Cap->string, "Buffer:");

    ui_proc_string_frame(&ui_default_SuperPower_0);
    SEND_MESSAGE((uint8_t *) &ui_default_SuperPower_0, sizeof(ui_default_SuperPower_0));
}

void _ui_update_default_SuperPower_0() {
    ui_default_SuperPower_0.option.operate_tpyel = 2;

    ui_proc_string_frame(&ui_default_SuperPower_0);
    SEND_MESSAGE((uint8_t *) &ui_default_SuperPower_0, sizeof(ui_default_SuperPower_0));
}

void _ui_remove_default_SuperPower_0() {
    ui_default_SuperPower_0.option.operate_tpyel = 3;

    ui_proc_string_frame(&ui_default_SuperPower_0);
    SEND_MESSAGE((uint8_t *) &ui_default_SuperPower_0, sizeof(ui_default_SuperPower_0));
}