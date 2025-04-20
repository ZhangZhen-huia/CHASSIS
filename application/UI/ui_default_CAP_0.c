//
// Created by RM UI Designer
//

#include "ui_default_CAP_0.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 0
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_CAP_0;
ui_interface_line_t *ui_default_CAP_Buffer = (ui_interface_line_t *)&(ui_default_CAP_0.data[0]);
ui_interface_rect_t *ui_default_CAP_Cap_Rec = (ui_interface_rect_t *)&(ui_default_CAP_0.data[1]);

void _ui_init_default_CAP_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_CAP_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_CAP_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_CAP_0.data[i].figure_name[2] = i + START_ID;
        ui_default_CAP_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_CAP_0.data[i].operate_tpyel = 0;
    }

    ui_default_CAP_Buffer->figure_tpye = 0;
    ui_default_CAP_Buffer->layer = 0;
    ui_default_CAP_Buffer->start_x = 628;
    ui_default_CAP_Buffer->start_y = 145;
    ui_default_CAP_Buffer->end_x = 1200;
    ui_default_CAP_Buffer->end_y = 145;
    ui_default_CAP_Buffer->color = 8;
    ui_default_CAP_Buffer->width = 50;

    ui_default_CAP_Cap_Rec->figure_tpye = 1;
    ui_default_CAP_Cap_Rec->layer = 0;
    ui_default_CAP_Cap_Rec->start_x = 628;
    ui_default_CAP_Cap_Rec->start_y = 117;
    ui_default_CAP_Cap_Rec->color = 7;
    ui_default_CAP_Cap_Rec->width = 5;
    ui_default_CAP_Cap_Rec->end_x = 1340;
    ui_default_CAP_Cap_Rec->end_y = 172;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_CAP_0);
    SEND_MESSAGE((uint8_t *) &ui_default_CAP_0, sizeof(ui_default_CAP_0));
}

void _ui_update_default_CAP_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_CAP_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_CAP_0);
    SEND_MESSAGE((uint8_t *) &ui_default_CAP_0, sizeof(ui_default_CAP_0));
}

void _ui_remove_default_CAP_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_CAP_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_CAP_0);
    SEND_MESSAGE((uint8_t *) &ui_default_CAP_0, sizeof(ui_default_CAP_0));
}
