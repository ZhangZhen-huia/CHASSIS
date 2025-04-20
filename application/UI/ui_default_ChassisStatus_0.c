//
// Created by RM UI Designer
//

#include "ui_default_ChassisStatus_0.h"

#define FRAME_ID 0
#define GROUP_ID 1
#define START_ID 0
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_ChassisStatus_0;
ui_interface_round_t *ui_default_ChassisStatus_ChassisRound = (ui_interface_round_t *)&(ui_default_ChassisStatus_0.data[0]);
ui_interface_arc_t *ui_default_ChassisStatus_RelativeAngle = (ui_interface_arc_t *)&(ui_default_ChassisStatus_0.data[1]);

void _ui_init_default_ChassisStatus_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_ChassisStatus_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_ChassisStatus_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_ChassisStatus_0.data[i].figure_name[2] = i + START_ID;
        ui_default_ChassisStatus_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_ChassisStatus_0.data[i].operate_tpyel = 0;
    }

    ui_default_ChassisStatus_ChassisRound->figure_tpye = 2;
    ui_default_ChassisStatus_ChassisRound->layer = 0;
    ui_default_ChassisStatus_ChassisRound->r = 300;
    ui_default_ChassisStatus_ChassisRound->start_x = 961;
    ui_default_ChassisStatus_ChassisRound->start_y = 541;
    ui_default_ChassisStatus_ChassisRound->color = 7;
    ui_default_ChassisStatus_ChassisRound->width = 3;

    ui_default_ChassisStatus_RelativeAngle->figure_tpye = 4;
    ui_default_ChassisStatus_RelativeAngle->layer = 0;
    ui_default_ChassisStatus_RelativeAngle->rx = 300;
    ui_default_ChassisStatus_RelativeAngle->ry = 300;
    ui_default_ChassisStatus_RelativeAngle->start_x = 963;
    ui_default_ChassisStatus_RelativeAngle->start_y = 539;
    ui_default_ChassisStatus_RelativeAngle->color = 8;
    ui_default_ChassisStatus_RelativeAngle->width = 20;
    ui_default_ChassisStatus_RelativeAngle->start_angle = 350;
    ui_default_ChassisStatus_RelativeAngle->end_angle = 10;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_ChassisStatus_0);
    SEND_MESSAGE((uint8_t *) &ui_default_ChassisStatus_0, sizeof(ui_default_ChassisStatus_0));
}

void _ui_update_default_ChassisStatus_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_ChassisStatus_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_ChassisStatus_0);
    SEND_MESSAGE((uint8_t *) &ui_default_ChassisStatus_0, sizeof(ui_default_ChassisStatus_0));
}

void _ui_remove_default_ChassisStatus_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_ChassisStatus_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_ChassisStatus_0);
    SEND_MESSAGE((uint8_t *) &ui_default_ChassisStatus_0, sizeof(ui_default_ChassisStatus_0));
}
