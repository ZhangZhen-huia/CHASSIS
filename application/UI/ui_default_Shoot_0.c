//
// Created by RM UI Designer
//

#include "ui_default_Shoot_0.h"

#define FRAME_ID 0
#define GROUP_ID 1
#define START_ID 0
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Shoot_0;
ui_interface_line_t *ui_default_Shoot_Sight1 = (ui_interface_line_t *)&(ui_default_Shoot_0.data[0]);
ui_interface_line_t *ui_default_Shoot_Sight2 = (ui_interface_line_t *)&(ui_default_Shoot_0.data[1]);

void _ui_init_default_Shoot_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Shoot_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_Shoot_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_Shoot_0.data[i].figure_name[2] = i + START_ID;
        ui_default_Shoot_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Shoot_0.data[i].operate_tpyel = 0;
    }

    ui_default_Shoot_Sight1->figure_tpye = 0;
    ui_default_Shoot_Sight1->layer = 0;
    ui_default_Shoot_Sight1->start_x = 947;
    ui_default_Shoot_Sight1->start_y = 494;
    ui_default_Shoot_Sight1->end_x = 977;
    ui_default_Shoot_Sight1->end_y = 494;
    ui_default_Shoot_Sight1->color = 6;
    ui_default_Shoot_Sight1->width = 1;

    ui_default_Shoot_Sight2->figure_tpye = 0;
    ui_default_Shoot_Sight2->layer = 0;
    ui_default_Shoot_Sight2->start_x = 958;
    ui_default_Shoot_Sight2->start_y = 478;
    ui_default_Shoot_Sight2->end_x = 958;
    ui_default_Shoot_Sight2->end_y = 508;
    ui_default_Shoot_Sight2->color = 6;
    ui_default_Shoot_Sight2->width = 1;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Shoot_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Shoot_0, sizeof(ui_default_Shoot_0));
}

void _ui_update_default_Shoot_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Shoot_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Shoot_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Shoot_0, sizeof(ui_default_Shoot_0));
}

void _ui_remove_default_Shoot_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Shoot_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Shoot_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Shoot_0, sizeof(ui_default_Shoot_0));
}
