//
// Created by RM UI Designer
//

#include "ui_default_OutLine_0.h"

#define FRAME_ID 0
#define GROUP_ID 7
#define START_ID 0
#define OBJ_NUM 1
#define FRAME_OBJ_NUM 1

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_OutLine_0;
ui_interface_line_t *ui_default_OutLine_HeadLine = (ui_interface_line_t *)&(ui_default_OutLine_0.data[0]);

void _ui_init_default_OutLine_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_OutLine_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_OutLine_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_OutLine_0.data[i].figure_name[2] = i + START_ID;
        ui_default_OutLine_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_OutLine_0.data[i].operate_tpyel = 0;
    }

    ui_default_OutLine_HeadLine->figure_tpye = 0;
    ui_default_OutLine_HeadLine->layer = 0;
    ui_default_OutLine_HeadLine->start_x = 766;
    ui_default_OutLine_HeadLine->start_y = 670;
    ui_default_OutLine_HeadLine->end_x = 1156;
    ui_default_OutLine_HeadLine->end_y = 670;
    ui_default_OutLine_HeadLine->color = 1;
    ui_default_OutLine_HeadLine->width = 5;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_OutLine_0);
    SEND_MESSAGE((uint8_t *) &ui_default_OutLine_0, sizeof(ui_default_OutLine_0));
}

void _ui_update_default_OutLine_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_OutLine_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_OutLine_0);
    SEND_MESSAGE((uint8_t *) &ui_default_OutLine_0, sizeof(ui_default_OutLine_0));
}

void _ui_remove_default_OutLine_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_OutLine_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_OutLine_0);
    SEND_MESSAGE((uint8_t *) &ui_default_OutLine_0, sizeof(ui_default_OutLine_0));
}
