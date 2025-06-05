//
// Created by RM UI Designer
//

#include "ui_default_Speed_0.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 0
#define OBJ_NUM 1
#define FRAME_OBJ_NUM 1

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Speed_0;
ui_interface_line_t *ui_default_Speed_FricSpeed = (ui_interface_line_t *)&(ui_default_Speed_0.data[0]);

void _ui_init_default_Speed_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Speed_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_Speed_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_Speed_0.data[i].figure_name[2] = i + START_ID;
        ui_default_Speed_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Speed_0.data[i].operate_tpyel = 0;
    }

    ui_default_Speed_FricSpeed->figure_tpye = 0;
    ui_default_Speed_FricSpeed->layer = 0;
    ui_default_Speed_FricSpeed->start_x = 1600;
    ui_default_Speed_FricSpeed->start_y = 298;
    ui_default_Speed_FricSpeed->end_x = 1600;
    ui_default_Speed_FricSpeed->end_y = 398;
    ui_default_Speed_FricSpeed->color = 6;
    ui_default_Speed_FricSpeed->width = 50;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Speed_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Speed_0, sizeof(ui_default_Speed_0));
}

void _ui_update_default_Speed_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Speed_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Speed_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Speed_0, sizeof(ui_default_Speed_0));
}

void _ui_remove_default_Speed_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Speed_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Speed_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Speed_0, sizeof(ui_default_Speed_0));
}
