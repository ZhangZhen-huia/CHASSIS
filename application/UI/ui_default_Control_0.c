//
// Created by RM UI Designer
//

#include "ui_default_Control_0.h"

#define FRAME_ID 0
#define GROUP_ID 6
#define START_ID 0
#define OBJ_NUM 1
#define FRAME_OBJ_NUM 1

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Control_0;
ui_interface_line_t *ui_default_Control_ControlMode = (ui_interface_line_t *)&(ui_default_Control_0.data[0]);

void _ui_init_default_Control_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Control_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_Control_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_Control_0.data[i].figure_name[2] = i + START_ID;
        ui_default_Control_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Control_0.data[i].operate_tpyel = 0;
    }

    ui_default_Control_ControlMode->figure_tpye = 0;
    ui_default_Control_ControlMode->layer = 0;
    ui_default_Control_ControlMode->start_x = 299;
    ui_default_Control_ControlMode->start_y = 682;
    ui_default_Control_ControlMode->end_x = 241;
    ui_default_Control_ControlMode->end_y = 682;
    ui_default_Control_ControlMode->color = 7;
    ui_default_Control_ControlMode->width = 0;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Control_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Control_0, sizeof(ui_default_Control_0));
}

void _ui_update_default_Control_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Control_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Control_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Control_0, sizeof(ui_default_Control_0));
}

void _ui_remove_default_Control_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Control_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Control_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Control_0, sizeof(ui_default_Control_0));
}
