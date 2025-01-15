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
	
#define ui_init_default_Mode() \
_ui_init_default_Mode_0(); \
_ui_init_default_Mode_1(); \
_ui_init_default_Mode_2()

#define ui_update_default_Mode() \
_ui_update_default_Mode_0(); \
_ui_update_default_Mode_1()

#define ui_remove_default_Mode() \
_ui_remove_default_Mode_0(); \
_ui_remove_default_Mode_1()
    


#ifdef __cplusplus
}
#endif

#endif //UI_H
