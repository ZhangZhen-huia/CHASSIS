//
// Created by RM UI Designer
//

#include "ui_default_Shoot_0.h"
#include "ui_default_AIMBOT_0.h"


#define FRAME_ID 0
#define GROUP_ID 3
#define START_ID 0
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Shoot_0;
ui_interface_line_t *ui_default_Shoot_Sight1 = (ui_interface_line_t *)&(ui_default_Shoot_0.data[0]);
ui_interface_line_t *ui_default_Shoot_sight2 = (ui_interface_line_t *)&(ui_default_Shoot_0.data[1]);
ui_interface_arc_t *ui_default_Shoot_FricStatusL = (ui_interface_arc_t *)&(ui_default_Shoot_0.data[2]);
ui_interface_arc_t *ui_default_Shoot_FricStatusR = (ui_interface_arc_t *)&(ui_default_Shoot_0.data[3]);
ui_interface_arc_t *ui_default_Shoot_TrigStatusL = (ui_interface_arc_t *)&(ui_default_Shoot_0.data[4]);
ui_interface_arc_t *ui_default_Shoot_TrigStatusR = (ui_interface_arc_t *)&(ui_default_Shoot_0.data[5]);
ui_interface_line_t *ui_default_Shoot_InitSpeed = (ui_interface_line_t *)&(ui_default_Shoot_0.data[6]);

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
    ui_default_Shoot_Sight1->start_x = 935;
    ui_default_Shoot_Sight1->start_y = 485;
    ui_default_Shoot_Sight1->end_x = 975;
    ui_default_Shoot_Sight1->end_y = 485;
    ui_default_Shoot_Sight1->color = 8;
    ui_default_Shoot_Sight1->width = 3;

    ui_default_Shoot_sight2->figure_tpye = 0;
    ui_default_Shoot_sight2->layer = 0;
    ui_default_Shoot_sight2->start_x = 955;
    ui_default_Shoot_sight2->start_y = 470;
    ui_default_Shoot_sight2->end_x = 955;
    ui_default_Shoot_sight2->end_y = 500;
    ui_default_Shoot_sight2->color = 8;
    ui_default_Shoot_sight2->width = 3;

    ui_default_Shoot_FricStatusL->figure_tpye = 4;
    ui_default_Shoot_FricStatusL->layer = 0;
    ui_default_Shoot_FricStatusL->rx = 0;
    ui_default_Shoot_FricStatusL->ry = 0;
    ui_default_Shoot_FricStatusL->start_x = 868;
    ui_default_Shoot_FricStatusL->start_y = 536;
    ui_default_Shoot_FricStatusL->color = 8;
    ui_default_Shoot_FricStatusL->width = 10;
    ui_default_Shoot_FricStatusL->start_angle = 225;
    ui_default_Shoot_FricStatusL->end_angle = 315;

    ui_default_Shoot_FricStatusR->figure_tpye = 4;
    ui_default_Shoot_FricStatusR->layer = 0;
    ui_default_Shoot_FricStatusR->rx = 0;
    ui_default_Shoot_FricStatusR->ry = 0;
    ui_default_Shoot_FricStatusR->start_x = 1051;
    ui_default_Shoot_FricStatusR->start_y = 541;
    ui_default_Shoot_FricStatusR->color = 8;
    ui_default_Shoot_FricStatusR->width = 10;
    ui_default_Shoot_FricStatusR->start_angle = 45;
    ui_default_Shoot_FricStatusR->end_angle = 135;


    ui_default_Shoot_InitSpeed->figure_tpye = 0;
    ui_default_Shoot_InitSpeed->layer = 0;
    ui_default_Shoot_InitSpeed->start_x = 0;
    ui_default_Shoot_InitSpeed->start_y = ui_default_AIMBOT_EnemyColor->start_y + 18.5f*5.0f;

    ui_default_Shoot_InitSpeed->end_x = 200;
    ui_default_Shoot_InitSpeed->end_y = ui_default_AIMBOT_EnemyColor->start_y + 18.5f*5.0f;
    ui_default_Shoot_InitSpeed->color = 0;
    ui_default_Shoot_InitSpeed->width = 2;


    ui_default_Shoot_TrigStatusL->figure_tpye = 4;
    ui_default_Shoot_TrigStatusL->layer = 0;
    ui_default_Shoot_TrigStatusL->rx = 300;
    ui_default_Shoot_TrigStatusL->ry = 300;
    ui_default_Shoot_TrigStatusL->start_x = 1033;
    ui_default_Shoot_TrigStatusL->start_y = 518;
    ui_default_Shoot_TrigStatusL->color = 7;
    ui_default_Shoot_TrigStatusL->width = 20;
    ui_default_Shoot_TrigStatusL->start_angle = 90;
    ui_default_Shoot_TrigStatusL->end_angle = 135;
                                  
    ui_default_Shoot_TrigStatusR->figure_tpye = 4;
    ui_default_Shoot_TrigStatusR->layer = 0;
    ui_default_Shoot_TrigStatusR->rx = 300;
    ui_default_Shoot_TrigStatusR->ry = 300;
    ui_default_Shoot_TrigStatusR->start_x = 886;
    ui_default_Shoot_TrigStatusR->start_y = 518;
    ui_default_Shoot_TrigStatusR->color = 7;
    ui_default_Shoot_TrigStatusR->width = 20;
    ui_default_Shoot_TrigStatusR->start_angle = 225;
    ui_default_Shoot_TrigStatusR->end_angle = 270;


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
