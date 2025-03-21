//
// Created by RM UI Designer
//

#ifndef UI_H
#define UI_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ui_interface.h"

#include "ui_default_Mode_0.h"
#include "ui_default_Mode_1.h"
#include "ui_default_Mode_2.h"
#include "ui_default_Shoot_0.h"
#include "ui_default_Body_0.h"
#include "ui_default_AIMBOT_0.h"
#include "ui_default_SuperPower_0.h"
#include "ui_default_SuperPower_1.h"	
#include "ui_default_Armour_0.h"
#include "ui_default_ShootMode_0.h"
	
	
#define ui_init_default_Mode() \
_ui_init_default_Mode_0(); \
_ui_init_default_Mode_1(); \
_ui_init_default_Mode_2(); \
_ui_init_default_Shoot_0(); \
_ui_init_default_Body_0(); \
_ui_init_default_AIMBOT_0(); \
_ui_init_default_SuperPower_0(); \
_ui_init_default_SuperPower_1(); \
_ui_init_default_Armour_0(); \
_ui_init_default_ShootMode_0();

#define ui_update_default_Mode() \
_ui_update_default_Mode_0(); \
_ui_update_default_Mode_1()

#define ui_remove_default_Mode() \
_ui_remove_default_Mode_0(); \
_ui_remove_default_Mode_1()
    
void Armour_Func(uint8_t i);


#ifdef __cplusplus
}
#endif

#endif //UI_H
