//
// Created by RM UI Designer
//

#include "ui_default_Armour_0.h"

#define FRAME_ID 0
#define GROUP_ID 5
#define START_ID 0
#define OBJ_NUM 5
#define FRAME_OBJ_NUM 5

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Armour_0;
ui_interface_rect_t *ui_default_Armour_Armour1 = (ui_interface_rect_t *)&(ui_default_Armour_0.data[0]);
ui_interface_rect_t *ui_default_Armour_Armour2 = (ui_interface_rect_t *)&(ui_default_Armour_0.data[1]);
ui_interface_rect_t *ui_default_Armour_Armour3 = (ui_interface_rect_t *)&(ui_default_Armour_0.data[2]);
ui_interface_rect_t *ui_default_Armour_Armour4 = (ui_interface_rect_t *)&(ui_default_Armour_0.data[3]);
ui_interface_round_t *ui_default_Armour_Body = (ui_interface_round_t *)&(ui_default_Armour_0.data[4]);

void _ui_init_default_Armour_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Armour_0.data[i].figure_name[0] = FRAME_ID;
        ui_default_Armour_0.data[i].figure_name[1] = GROUP_ID;
        ui_default_Armour_0.data[i].figure_name[2] = i + START_ID;
        ui_default_Armour_0.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Armour_0.data[i].operate_tpyel = 0;
    }

    ui_default_Armour_Armour1->figure_tpye = 1;
    ui_default_Armour_Armour1->layer = 0;
    ui_default_Armour_Armour1->start_x = 1495;
    ui_default_Armour_Armour1->start_y = 767;
    ui_default_Armour_Armour1->color = 6;
    ui_default_Armour_Armour1->width = 1;
    ui_default_Armour_Armour1->end_x = 1545;
    ui_default_Armour_Armour1->end_y = 817;

    ui_default_Armour_Armour2->figure_tpye = 1;
    ui_default_Armour_Armour2->layer = 0;
    ui_default_Armour_Armour2->start_x = 1601;
    ui_default_Armour_Armour2->start_y = 770;
    ui_default_Armour_Armour2->color = 0;
    ui_default_Armour_Armour2->width = 1;
    ui_default_Armour_Armour2->end_x = 1651;
    ui_default_Armour_Armour2->end_y = 820;

    ui_default_Armour_Armour3->figure_tpye = 1;
    ui_default_Armour_Armour3->layer = 0;
    ui_default_Armour_Armour3->start_x = 1607;
    ui_default_Armour_Armour3->start_y = 675;
    ui_default_Armour_Armour3->color = 3;
    ui_default_Armour_Armour3->width = 1;
    ui_default_Armour_Armour3->end_x = 1657;
    ui_default_Armour_Armour3->end_y = 725;

    ui_default_Armour_Armour4->figure_tpye = 1;
    ui_default_Armour_Armour4->layer = 0;
    ui_default_Armour_Armour4->start_x = 1497;
    ui_default_Armour_Armour4->start_y = 673;
    ui_default_Armour_Armour4->color = 7;
    ui_default_Armour_Armour4->width = 1;
    ui_default_Armour_Armour4->end_x = 1547;
    ui_default_Armour_Armour4->end_y = 723;

    ui_default_Armour_Body->figure_tpye = 2;
    ui_default_Armour_Body->layer = 0;
    ui_default_Armour_Body->r = 31;
    ui_default_Armour_Body->start_x = 1575;
    ui_default_Armour_Body->start_y = 746;
    ui_default_Armour_Body->color = 0;
    ui_default_Armour_Body->width = 1;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Armour_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Armour_0, sizeof(ui_default_Armour_0));
}

void _ui_update_default_Armour_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Armour_0.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Armour_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Armour_0, sizeof(ui_default_Armour_0));
}

void _ui_remove_default_Armour_0() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Armour_0.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Armour_0);
    SEND_MESSAGE((uint8_t *) &ui_default_Armour_0, sizeof(ui_default_Armour_0));
}



void Armour_Func(uint8_t i)
{
	switch(i)
	{
		case 0:    
		ui_default_Armour_Armour1->figure_tpye = 1;
    ui_default_Armour_Armour1->layer = 0;
    ui_default_Armour_Armour1->start_x = 1495;
    ui_default_Armour_Armour1->start_y = 767;
    ui_default_Armour_Armour1->color = 6;
    ui_default_Armour_Armour1->width = 1;
    ui_default_Armour_Armour1->end_x = 1545;
    ui_default_Armour_Armour1->end_y = 817;

    ui_default_Armour_Armour2->figure_tpye = 1;
    ui_default_Armour_Armour2->layer = 0;
    ui_default_Armour_Armour2->start_x = 1601;
    ui_default_Armour_Armour2->start_y = 770;
    ui_default_Armour_Armour2->color = 0;
    ui_default_Armour_Armour2->width = 1;
    ui_default_Armour_Armour2->end_x = 1651;
    ui_default_Armour_Armour2->end_y = 820;

    ui_default_Armour_Armour3->figure_tpye = 1;
    ui_default_Armour_Armour3->layer = 0;
    ui_default_Armour_Armour3->start_x = 1607;
    ui_default_Armour_Armour3->start_y = 675;
    ui_default_Armour_Armour3->color = 3;
    ui_default_Armour_Armour3->width = 1;
    ui_default_Armour_Armour3->end_x = 1657;
    ui_default_Armour_Armour3->end_y = 725;

    ui_default_Armour_Armour4->figure_tpye = 1;
    ui_default_Armour_Armour4->layer = 0;
    ui_default_Armour_Armour4->start_x = 1497;
    ui_default_Armour_Armour4->start_y = 673;
    ui_default_Armour_Armour4->color = 7;
    ui_default_Armour_Armour4->width = 1;
    ui_default_Armour_Armour4->end_x = 1547;
    ui_default_Armour_Armour4->end_y = 723;

    ui_default_Armour_Body->figure_tpye = 2;
    ui_default_Armour_Body->layer = 0;
    ui_default_Armour_Body->r = 31;
    ui_default_Armour_Body->start_x = 1575;
    ui_default_Armour_Body->start_y = 746;
    ui_default_Armour_Body->color = 0;
    ui_default_Armour_Body->width = 1;
		break;
		
		case 1: ui_default_Armour_Armour1->figure_tpye = 1;
    ui_default_Armour_Armour1->layer = 0;
    ui_default_Armour_Armour1->start_x = 1494;
    ui_default_Armour_Armour1->start_y = 724;
    ui_default_Armour_Armour1->color = 6;
    ui_default_Armour_Armour1->width = 1;
    ui_default_Armour_Armour1->end_x = 1544;
    ui_default_Armour_Armour1->end_y = 774;

    ui_default_Armour_Armour2->figure_tpye = 1;
    ui_default_Armour_Armour2->layer = 0;
    ui_default_Armour_Armour2->start_x = 1552;
    ui_default_Armour_Armour2->start_y = 778;
    ui_default_Armour_Armour2->color = 0;
    ui_default_Armour_Armour2->width = 1;
    ui_default_Armour_Armour2->end_x = 1602;
    ui_default_Armour_Armour2->end_y = 828;

    ui_default_Armour_Armour3->figure_tpye = 1;
    ui_default_Armour_Armour3->layer = 0;
    ui_default_Armour_Armour3->start_x = 1608;
    ui_default_Armour_Armour3->start_y = 721;
    ui_default_Armour_Armour3->color = 3;
    ui_default_Armour_Armour3->width = 1;
    ui_default_Armour_Armour3->end_x = 1658;
    ui_default_Armour_Armour3->end_y = 771;

    ui_default_Armour_Armour4->figure_tpye = 1;
    ui_default_Armour_Armour4->layer = 0;
    ui_default_Armour_Armour4->start_x = 1552;
    ui_default_Armour_Armour4->start_y = 664;
    ui_default_Armour_Armour4->color = 7;
    ui_default_Armour_Armour4->width = 1;
    ui_default_Armour_Armour4->end_x = 1602;
    ui_default_Armour_Armour4->end_y = 714;

    ui_default_Armour_Body->figure_tpye = 2;
    ui_default_Armour_Body->layer = 0;
    ui_default_Armour_Body->r = 31;
    ui_default_Armour_Body->start_x = 1575;
    ui_default_Armour_Body->start_y = 746;
    ui_default_Armour_Body->color = 0;
    ui_default_Armour_Body->width = 1;
		break;
		
		case 2:
    ui_default_Armour_Armour1->figure_tpye = 1;
    ui_default_Armour_Armour1->layer = 0;
    ui_default_Armour_Armour1->start_x = 1497;
    ui_default_Armour_Armour1->start_y = 676;
    ui_default_Armour_Armour1->color = 6;
    ui_default_Armour_Armour1->width = 1;
    ui_default_Armour_Armour1->end_x = 1547;
    ui_default_Armour_Armour1->end_y = 726;

    ui_default_Armour_Armour2->figure_tpye = 1;
    ui_default_Armour_Armour2->layer = 0;
    ui_default_Armour_Armour2->start_x = 1491;
    ui_default_Armour_Armour2->start_y = 766;
    ui_default_Armour_Armour2->color = 0;
    ui_default_Armour_Armour2->width = 1;
    ui_default_Armour_Armour2->end_x = 1541;
    ui_default_Armour_Armour2->end_y = 816;

    ui_default_Armour_Armour3->figure_tpye = 1;
    ui_default_Armour_Armour3->layer = 0;
    ui_default_Armour_Armour3->start_x = 1602;
    ui_default_Armour_Armour3->start_y = 770;
    ui_default_Armour_Armour3->color = 3;
    ui_default_Armour_Armour3->width = 1;
    ui_default_Armour_Armour3->end_x = 1652;
    ui_default_Armour_Armour3->end_y = 820;

    ui_default_Armour_Armour4->figure_tpye = 1;
    ui_default_Armour_Armour4->layer = 0;
    ui_default_Armour_Armour4->start_x = 1603;
    ui_default_Armour_Armour4->start_y = 677;
    ui_default_Armour_Armour4->color = 7;
    ui_default_Armour_Armour4->width = 1;
    ui_default_Armour_Armour4->end_x = 1653;
    ui_default_Armour_Armour4->end_y = 727;

    ui_default_Armour_Body->figure_tpye = 2;
    ui_default_Armour_Body->layer = 0;
    ui_default_Armour_Body->r = 31;
    ui_default_Armour_Body->start_x = 1575;
    ui_default_Armour_Body->start_y = 746;
    ui_default_Armour_Body->color = 0;
    ui_default_Armour_Body->width = 1;
break;

		case 3:
			    ui_default_Armour_Armour1->figure_tpye = 1;
    ui_default_Armour_Armour1->layer = 0;
    ui_default_Armour_Armour1->start_x = 1551;
    ui_default_Armour_Armour1->start_y = 663;
    ui_default_Armour_Armour1->color = 6;
    ui_default_Armour_Armour1->width = 1;
    ui_default_Armour_Armour1->end_x = 1601;
    ui_default_Armour_Armour1->end_y = 713;

    ui_default_Armour_Armour2->figure_tpye = 1;
    ui_default_Armour_Armour2->layer = 0;
    ui_default_Armour_Armour2->start_x = 1489;
    ui_default_Armour_Armour2->start_y = 721;
    ui_default_Armour_Armour2->color = 0;
    ui_default_Armour_Armour2->width = 1;
    ui_default_Armour_Armour2->end_x = 1539;
    ui_default_Armour_Armour2->end_y = 771;

    ui_default_Armour_Armour3->figure_tpye = 1;
    ui_default_Armour_Armour3->layer = 0;
    ui_default_Armour_Armour3->start_x = 1549;
    ui_default_Armour_Armour3->start_y = 781;
    ui_default_Armour_Armour3->color = 3;
    ui_default_Armour_Armour3->width = 1;
    ui_default_Armour_Armour3->end_x = 1599;
    ui_default_Armour_Armour3->end_y = 831;

    ui_default_Armour_Armour4->figure_tpye = 1;
    ui_default_Armour_Armour4->layer = 0;
    ui_default_Armour_Armour4->start_x = 1611;
    ui_default_Armour_Armour4->start_y = 721;
    ui_default_Armour_Armour4->color = 7;
    ui_default_Armour_Armour4->width = 1;
    ui_default_Armour_Armour4->end_x = 1661;
    ui_default_Armour_Armour4->end_y = 771;

    ui_default_Armour_Body->figure_tpye = 2;
    ui_default_Armour_Body->layer = 0;
    ui_default_Armour_Body->r = 31;
    ui_default_Armour_Body->start_x = 1575;
    ui_default_Armour_Body->start_y = 746;
    ui_default_Armour_Body->color = 0;
    ui_default_Armour_Body->width = 1;
break;
		case 4:
			    ui_default_Armour_Armour1->figure_tpye = 1;
    ui_default_Armour_Armour1->layer = 0;
    ui_default_Armour_Armour1->start_x = 1605;
    ui_default_Armour_Armour1->start_y = 676;
    ui_default_Armour_Armour1->color = 6;
    ui_default_Armour_Armour1->width = 1;
    ui_default_Armour_Armour1->end_x = 1655;
    ui_default_Armour_Armour1->end_y = 726;

    ui_default_Armour_Armour2->figure_tpye = 1;
    ui_default_Armour_Armour2->layer = 0;
    ui_default_Armour_Armour2->start_x = 1498;
    ui_default_Armour_Armour2->start_y = 674;
    ui_default_Armour_Armour2->color = 0;
    ui_default_Armour_Armour2->width = 1;
    ui_default_Armour_Armour2->end_x = 1548;
    ui_default_Armour_Armour2->end_y = 724;

    ui_default_Armour_Armour3->figure_tpye = 1;
    ui_default_Armour_Armour3->layer = 0;
    ui_default_Armour_Armour3->start_x = 1494;
    ui_default_Armour_Armour3->start_y = 769;
    ui_default_Armour_Armour3->color = 3;
    ui_default_Armour_Armour3->width = 1;
    ui_default_Armour_Armour3->end_x = 1544;
    ui_default_Armour_Armour3->end_y = 819;

    ui_default_Armour_Armour4->figure_tpye = 1;
    ui_default_Armour_Armour4->layer = 0;
    ui_default_Armour_Armour4->start_x = 1601;
    ui_default_Armour_Armour4->start_y = 772;
    ui_default_Armour_Armour4->color = 7;
    ui_default_Armour_Armour4->width = 1;
    ui_default_Armour_Armour4->end_x = 1651;
    ui_default_Armour_Armour4->end_y = 822;

    ui_default_Armour_Body->figure_tpye = 2;
    ui_default_Armour_Body->layer = 0;
    ui_default_Armour_Body->r = 31;
    ui_default_Armour_Body->start_x = 1575;
    ui_default_Armour_Body->start_y = 746;
    ui_default_Armour_Body->color = 0;
    ui_default_Armour_Body->width = 1;
break;
		case 5:    ui_default_Armour_Armour1->figure_tpye = 1;
    ui_default_Armour_Armour1->layer = 0;
    ui_default_Armour_Armour1->start_x = 1610;
    ui_default_Armour_Armour1->start_y = 722;
    ui_default_Armour_Armour1->color = 6;
    ui_default_Armour_Armour1->width = 1;
    ui_default_Armour_Armour1->end_x = 1660;
    ui_default_Armour_Armour1->end_y = 772;

    ui_default_Armour_Armour2->figure_tpye = 1;
    ui_default_Armour_Armour2->layer = 0;
    ui_default_Armour_Armour2->start_x = 1552;
    ui_default_Armour_Armour2->start_y = 662;
    ui_default_Armour_Armour2->color = 0;
    ui_default_Armour_Armour2->width = 1;
    ui_default_Armour_Armour2->end_x = 1602;
    ui_default_Armour_Armour2->end_y = 712;

    ui_default_Armour_Armour3->figure_tpye = 1;
    ui_default_Armour_Armour3->layer = 0;
    ui_default_Armour_Armour3->start_x = 1490;
    ui_default_Armour_Armour3->start_y = 723;
    ui_default_Armour_Armour3->color = 3;
    ui_default_Armour_Armour3->width = 1;
    ui_default_Armour_Armour3->end_x = 1540;
    ui_default_Armour_Armour3->end_y = 773;

    ui_default_Armour_Armour4->figure_tpye = 1;
    ui_default_Armour_Armour4->layer = 0;
    ui_default_Armour_Armour4->start_x = 1551;
    ui_default_Armour_Armour4->start_y = 780;
    ui_default_Armour_Armour4->color = 7;
    ui_default_Armour_Armour4->width = 1;
    ui_default_Armour_Armour4->end_x = 1601;
    ui_default_Armour_Armour4->end_y = 830;

    ui_default_Armour_Body->figure_tpye = 2;
    ui_default_Armour_Body->layer = 0;
    ui_default_Armour_Body->r = 31;
    ui_default_Armour_Body->start_x = 1575;
    ui_default_Armour_Body->start_y = 746;
    ui_default_Armour_Body->color = 0;
    ui_default_Armour_Body->width = 1;
break;
		case 6:    ui_default_Armour_Armour1->figure_tpye = 1;
    ui_default_Armour_Armour1->layer = 0;
    ui_default_Armour_Armour1->start_x = 1597;
    ui_default_Armour_Armour1->start_y = 769;
    ui_default_Armour_Armour1->color = 6;
    ui_default_Armour_Armour1->width = 1;
    ui_default_Armour_Armour1->end_x = 1647;
    ui_default_Armour_Armour1->end_y = 819;

    ui_default_Armour_Armour2->figure_tpye = 1;
    ui_default_Armour_Armour2->layer = 0;
    ui_default_Armour_Armour2->start_x = 1597;
    ui_default_Armour_Armour2->start_y = 666;
    ui_default_Armour_Armour2->color = 0;
    ui_default_Armour_Armour2->width = 1;
    ui_default_Armour_Armour2->end_x = 1647;
    ui_default_Armour_Armour2->end_y = 716;

    ui_default_Armour_Armour3->figure_tpye = 1;
    ui_default_Armour_Armour3->layer = 0;
    ui_default_Armour_Armour3->start_x = 1496;
    ui_default_Armour_Armour3->start_y = 668;
    ui_default_Armour_Armour3->color = 3;
    ui_default_Armour_Armour3->width = 1;
    ui_default_Armour_Armour3->end_x = 1546;
    ui_default_Armour_Armour3->end_y = 718;

    ui_default_Armour_Armour4->figure_tpye = 1;
    ui_default_Armour_Armour4->layer = 0;
    ui_default_Armour_Armour4->start_x = 1494;
    ui_default_Armour_Armour4->start_y = 768;
    ui_default_Armour_Armour4->color = 7;
    ui_default_Armour_Armour4->width = 1;
    ui_default_Armour_Armour4->end_x = 1544;
    ui_default_Armour_Armour4->end_y = 818;

    ui_default_Armour_Body->figure_tpye = 2;
    ui_default_Armour_Body->layer = 0;
    ui_default_Armour_Body->r = 31;
    ui_default_Armour_Body->start_x = 1572;
    ui_default_Armour_Body->start_y = 742;
    ui_default_Armour_Body->color = 0;
    ui_default_Armour_Body->width = 1;

break;
		case 7:
   ui_default_Armour_Armour1->figure_tpye = 1;
    ui_default_Armour_Armour1->layer = 0;
    ui_default_Armour_Armour1->start_x = 1548;
    ui_default_Armour_Armour1->start_y = 783;
    ui_default_Armour_Armour1->color = 6;
    ui_default_Armour_Armour1->width = 1;
    ui_default_Armour_Armour1->end_x = 1598;
    ui_default_Armour_Armour1->end_y = 833;

    ui_default_Armour_Armour2->figure_tpye = 1;
    ui_default_Armour_Armour2->layer = 0;
    ui_default_Armour_Armour2->start_x = 1610;
    ui_default_Armour_Armour2->start_y = 725;
    ui_default_Armour_Armour2->color = 0;
    ui_default_Armour_Armour2->width = 1;
    ui_default_Armour_Armour2->end_x = 1660;
    ui_default_Armour_Armour2->end_y = 775;

    ui_default_Armour_Armour3->figure_tpye = 1;
    ui_default_Armour_Armour3->layer = 0;
    ui_default_Armour_Armour3->start_x = 1550;
    ui_default_Armour_Armour3->start_y = 659;
    ui_default_Armour_Armour3->color = 3;
    ui_default_Armour_Armour3->width = 1;
    ui_default_Armour_Armour3->end_x = 1600;
    ui_default_Armour_Armour3->end_y = 709;

    ui_default_Armour_Armour4->figure_tpye = 1;
    ui_default_Armour_Armour4->layer = 0;
    ui_default_Armour_Armour4->start_x = 1486;
    ui_default_Armour_Armour4->start_y = 724;
    ui_default_Armour_Armour4->color = 7;
    ui_default_Armour_Armour4->width = 1;
    ui_default_Armour_Armour4->end_x = 1536;
    ui_default_Armour_Armour4->end_y = 774;

    ui_default_Armour_Body->figure_tpye = 2;
    ui_default_Armour_Body->layer = 0;
    ui_default_Armour_Body->r = 31;
    ui_default_Armour_Body->start_x = 1575;
    ui_default_Armour_Body->start_y = 746;
    ui_default_Armour_Body->color = 0;
    ui_default_Armour_Body->width = 1;
	}
}