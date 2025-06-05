//
// Created by RM UI Designer
//

#include "ui_default_AIMBOT_0.h"
#include "communicate_task.h"
#define FRAME_ID 0
#define GROUP_ID 2
#define START_ID 0
#define OBJ_NUM 1
#define FRAME_OBJ_NUM 1

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_AIMBOT_0;
ui_interface_line_t *ui_default_AIMBOT_EnemyColor = (ui_interface_line_t *)&(ui_default_AIMBOT_0.data[0]);

void _ui_init_default_AIMBOT_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_AIMBOT_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_AIMBOT_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_AIMBOT_0.data[i].figure_name[2] = i + START_ID;
        ui_default_AIMBOT_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_AIMBOT_0.data[i].operate_tpyel = 0;
    }

    ui_default_AIMBOT_EnemyColor->figure_tpye = 0;
    ui_default_AIMBOT_EnemyColor->layer = 0;
    ui_default_AIMBOT_EnemyColor->start_x = 80;
    ui_default_AIMBOT_EnemyColor->start_y = 608;
    ui_default_AIMBOT_EnemyColor->end_x = 80;
    ui_default_AIMBOT_EnemyColor->end_y = 708;
		if(gimbal_data.EnemyColor == BLUE)
				ui_default_AIMBOT_EnemyColor->color = 6;
		else if(gimbal_data.EnemyColor == RED)
				ui_default_AIMBOT_EnemyColor->color = 5;
    ui_default_AIMBOT_EnemyColor->width = 50;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_AIMBOT_0);
    SEND_MESSAGE((uint8_t *) &ui_default_AIMBOT_0, sizeof(ui_default_AIMBOT_0));
}

void _ui_update_default_AIMBOT_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_AIMBOT_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_AIMBOT_0);
    SEND_MESSAGE((uint8_t *) &ui_default_AIMBOT_0, sizeof(ui_default_AIMBOT_0));
}

void _ui_remove_default_AIMBOT_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_AIMBOT_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_AIMBOT_0);
    SEND_MESSAGE((uint8_t *) &ui_default_AIMBOT_0, sizeof(ui_default_AIMBOT_0));
}
