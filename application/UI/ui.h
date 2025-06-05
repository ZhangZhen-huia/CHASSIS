//
// Created by RM UI Designer
//

#ifndef UI_H
#define UI_H
#ifdef __cplusplus
extern "C" {
#endif

#include "ui_interface.h"

#include "ui_default_Shoot_0.h"
#include "ui_default_AIMBOT_0.h"
#include "ui_default_CAP_0.h"
#include "ui_default_ChassisStatus_0.h"
#include "ui_default_Control_0.h"


	
	
#define ui_init_default_Mode() \
_ui_init_default_CAP_0(); \
_ui_init_default_AIMBOT_0(); \
_ui_init_default_ChassisStatus_0(); \
_ui_init_default_Shoot_0(); \
_ui_init_default_Control_0();


    


#ifdef __cplusplus
}
#endif




#endif //UI_H
