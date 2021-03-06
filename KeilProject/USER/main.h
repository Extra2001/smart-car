#ifndef __MAIN_H
#define __MAIN_H

extern float pitch, roll, yaw;    //欧拉角
extern short aacx, aacy, aacz;    //加速度传感器原始数据
extern short gyrox, gyroy, gyroz; //陀螺仪原始数据
extern short tempIMU;             //温度
extern char Bright_check;         //亮度检测结果
extern char Red_light;            //红灯识别结果
extern char Green_light;          //绿灯识别结果
extern char Identification;       //图形识别结果
extern char void_right;           //右侧红外检测结果
extern char void_left;            //左侧红外检测结果
extern char Search_L;             //循迹左侧结果
extern char Search_M;             //循迹中间结果
extern char Search_R;             //循迹右侧结果
extern char LF_Light;             //左前灯状态
extern char RF_Light;             //右前灯状态
extern char LB_Light;             //左后灯状态
extern char RB_Light;             //右后灯状态
extern char Is_Display;           //是否通过串口发送图像
extern char ctrl_comm;            //上一次的指令

extern unsigned char left_speed;
extern unsigned char right_speed;
extern unsigned char avg_speed;

#endif
