#ifndef __MOTION_H_
#define __MOTION_H_

extern char Red_light;            //红灯识别结果
extern char Green_light;          //绿灯识别结果
extern char Identification;       //图形识别结果
extern char void_right;           //右侧红外检测结果
extern char void_left;            //左侧红外检测结果
extern char Search_L;             //循迹左侧结果
extern char Search_M;             //循迹中间结果
extern char Search_R;             //循迹右侧结果
extern char ctrl_comm;            //小车控制指令

extern float Kp;
extern float Ki;
extern float Kd;
extern char search_line_mode;
extern char front_duty;
extern char behind_duty;

void ToggleSearchLine(void);
void Move(void);

#endif
