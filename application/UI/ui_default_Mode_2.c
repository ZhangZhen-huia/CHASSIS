//
// Created by RM UI Designer
//

#include "ui_default_Mode_2.h"
#include "string.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 2

ui_string_frame_t ui_default_Mode_2;

ui_interface_string_t* ui_default_Mode_SuperPower = &ui_default_Mode_2.option;

void _ui_init_default_Mode_2() {
    ui_default_Mode_2.option.figure_name[0] = FRAME_ID;
    ui_default_Mode_2.option.figure_name[1] = GROUP_ID;
    ui_default_Mode_2.option.figure_name[2] = START_ID;
    ui_default_Mode_2.option.operate_tpyel = 1;
    ui_default_Mode_2.option.figure_tpye = 7;
    ui_default_Mode_2.option.layer = 0;
    ui_default_Mode_2.option.font_size = 20;
    ui_default_Mode_2.option.start_x = 25;
    ui_default_Mode_2.option.start_y = 750;
    ui_default_Mode_2.option.color = 8;
    ui_default_Mode_2.option.str_length = 9;
    ui_default_Mode_2.option.width = 2;
    strcpy(ui_default_Mode_SuperPower->string, "Cap:Close");

    ui_proc_string_frame(&ui_default_Mode_2);
    SEND_MESSAGE((uint8_t *) &ui_default_Mode_2, sizeof(ui_default_Mode_2));
}

void _ui_update_default_Mode_2() {
    ui_default_Mode_2.option.operate_tpyel = 2;

    ui_proc_string_frame(&ui_default_Mode_2);
    SEND_MESSAGE((uint8_t *) &ui_default_Mode_2, sizeof(ui_default_Mode_2));
}

void _ui_remove_default_Mode_2() {
    ui_default_Mode_2.option.operate_tpyel = 3;

    ui_proc_string_frame(&ui_default_Mode_2);
    SEND_MESSAGE((uint8_t *) &ui_default_Mode_2, sizeof(ui_default_Mode_2));
}