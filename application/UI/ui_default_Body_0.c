//
// Created by RM UI Designer
//

#include "ui_default_Body_0.h"

#define FRAME_ID 0
#define GROUP_ID 2
#define START_ID 0
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Body_0;
ui_interface_line_t *ui_default_Body_contour1 = (ui_interface_line_t *)&(ui_default_Body_0.data[0]);
ui_interface_line_t *ui_default_Body_contour2 = (ui_interface_line_t *)&(ui_default_Body_0.data[1]);

void _ui_init_default_Body_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Body_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_Body_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_Body_0.data[i].figure_name[2] = i + START_ID;
        ui_default_Body_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Body_0.data[i].operate_tpyel = 0;
    }

    ui_default_Body_contour1->figure_tpye = 0;
    ui_default_Body_contour1->layer = 0;
    ui_default_Body_contour1->start_x = 440;
    ui_default_Body_contour1->start_y = 295;
    ui_default_Body_contour1->end_x = 440;
    ui_default_Body_contour1->end_y = 713;
    ui_default_Body_contour1->color = 7;
    ui_default_Body_contour1->width = 5;

    ui_default_Body_contour2->figure_tpye = 0;
    ui_default_Body_contour2->layer = 0;
    ui_default_Body_contour2->start_x = 1449;
    ui_default_Body_contour2->start_y = 302;
    ui_default_Body_contour2->end_x = 1449;
    ui_default_Body_contour2->end_y = 720;
    ui_default_Body_contour2->color = 7;
    ui_default_Body_contour2->width = 5;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Body_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Body_0, sizeof(ui_default_Body_0));
}

void _ui_update_default_Body_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Body_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Body_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Body_0, sizeof(ui_default_Body_0));
}

void _ui_remove_default_Body_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Body_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Body_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Body_0, sizeof(ui_default_Body_0));
}
