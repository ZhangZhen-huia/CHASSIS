//
// Created by RM UI Designer
//

#include "ui_default_TrigMode_0.h"

#define FRAME_ID 0
#define GROUP_ID 5
#define START_ID 0
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_TrigMode_0;
ui_interface_arc_t *ui_default_TrigMode_TrigStatusL = (ui_interface_arc_t *)&(ui_default_TrigMode_0.data[0]);
ui_interface_arc_t *ui_default_TrigMode_TrigStatusR = (ui_interface_arc_t *)&(ui_default_TrigMode_0.data[1]);

void _ui_init_default_TrigMode_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_TrigMode_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_TrigMode_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_TrigMode_0.data[i].figure_name[2] = i + START_ID;
        ui_default_TrigMode_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_TrigMode_0.data[i].operate_tpyel = 0;
    }

    ui_default_TrigMode_TrigStatusL->figure_tpye = 4;
    ui_default_TrigMode_TrigStatusL->layer = 0;
    ui_default_TrigMode_TrigStatusL->rx = 300;
    ui_default_TrigMode_TrigStatusL->ry = 300;
    ui_default_TrigMode_TrigStatusL->start_x = 1033;
    ui_default_TrigMode_TrigStatusL->start_y = 518;
    ui_default_TrigMode_TrigStatusL->color = 7;
    ui_default_TrigMode_TrigStatusL->width = 20;
    ui_default_TrigMode_TrigStatusL->start_angle = 90;
    ui_default_TrigMode_TrigStatusL->end_angle = 135;

    ui_default_TrigMode_TrigStatusR->figure_tpye = 4;
    ui_default_TrigMode_TrigStatusR->layer = 0;
    ui_default_TrigMode_TrigStatusR->rx = 300;
    ui_default_TrigMode_TrigStatusR->ry = 300;
    ui_default_TrigMode_TrigStatusR->start_x = 886;
    ui_default_TrigMode_TrigStatusR->start_y = 518;
    ui_default_TrigMode_TrigStatusR->color = 7;
    ui_default_TrigMode_TrigStatusR->width = 20;
    ui_default_TrigMode_TrigStatusR->start_angle = 225;
    ui_default_TrigMode_TrigStatusR->end_angle = 270;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_TrigMode_0);
    SEND_MESSAGE((uint8_t *) &ui_default_TrigMode_0, sizeof(ui_default_TrigMode_0));
}

void _ui_update_default_TrigMode_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_TrigMode_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_TrigMode_0);
    SEND_MESSAGE((uint8_t *) &ui_default_TrigMode_0, sizeof(ui_default_TrigMode_0));
}

void _ui_remove_default_TrigMode_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_TrigMode_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_TrigMode_0);
    SEND_MESSAGE((uint8_t *) &ui_default_TrigMode_0, sizeof(ui_default_TrigMode_0));
}
