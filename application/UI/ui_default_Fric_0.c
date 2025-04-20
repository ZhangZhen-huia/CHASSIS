//
// Created by RM UI Designer
//

#include "ui_default_Fric_0.h"

#define FRAME_ID 0
#define GROUP_ID 4
#define START_ID 0
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Fric_0;
ui_interface_arc_t *ui_default_Fric_FricStatusL = (ui_interface_arc_t *)&(ui_default_Fric_0.data[0]);
ui_interface_arc_t *ui_default_Fric_FricStatusR = (ui_interface_arc_t *)&(ui_default_Fric_0.data[1]);

void _ui_init_default_Fric_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Fric_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_Fric_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_Fric_0.data[i].figure_name[2] = i + START_ID;
        ui_default_Fric_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Fric_0.data[i].operate_tpyel = 0;
    }

    ui_default_Fric_FricStatusL->figure_tpye = 4;
    ui_default_Fric_FricStatusL->layer = 0;
    ui_default_Fric_FricStatusL->rx = 0;
    ui_default_Fric_FricStatusL->ry = 0;
    ui_default_Fric_FricStatusL->start_x = 868;
    ui_default_Fric_FricStatusL->start_y = 536;
    ui_default_Fric_FricStatusL->color = 8;
    ui_default_Fric_FricStatusL->width = 10;
    ui_default_Fric_FricStatusL->start_angle = 225;
    ui_default_Fric_FricStatusL->end_angle = 315;

    ui_default_Fric_FricStatusR->figure_tpye = 4;
    ui_default_Fric_FricStatusR->layer = 0;
    ui_default_Fric_FricStatusR->rx = 0;
    ui_default_Fric_FricStatusR->ry = 0;
    ui_default_Fric_FricStatusR->start_x = 1051;
    ui_default_Fric_FricStatusR->start_y = 541;
    ui_default_Fric_FricStatusR->color = 8;
    ui_default_Fric_FricStatusR->width = 10;
    ui_default_Fric_FricStatusR->start_angle = 45;
    ui_default_Fric_FricStatusR->end_angle = 135;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Fric_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Fric_0, sizeof(ui_default_Fric_0));
}

void _ui_update_default_Fric_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Fric_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Fric_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Fric_0, sizeof(ui_default_Fric_0));
}

void _ui_remove_default_Fric_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Fric_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Fric_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Fric_0, sizeof(ui_default_Fric_0));
}
