#include "common.h"
#include "stdlib.h"
#include "UltrasonicCtrol.h"
#include "SpeedCtrol.h"
#include "main.h"
#include "malloc.h"
#include "delay.h"
#include "interface.h"
#include "motor.h"
#include <ctype.h>

extern float Kp;
extern float Ki;
extern float Kd;
extern char front_duty;
extern char behind_duty;
extern char search_line_mode;
extern short avg_distance;

extern u8 *OV7670_RAM;
// ATK-ESP8266 WIFI STA测试
//用于测试TCP/UDP连接
//返回值:0,正常
//     其他,错误代码
u8 netpro = 0; //网络模式
u8 atk_8266_wifista_test(void)
{
    // u8 netpro=0;	//网络模式
    u8 key;
    u8 timex = 0;
    u8 ipbuf[16]; // IP缓存
    u8 *p;
    u16 t = 999; //加速第一次获取链接状态
    u8 res = 0;
    u16 rlen = 0;
    u8 constate = 0;                            //连接状态
    p = mymalloc(32);                           //申请32字节内存
    atk_8266_send_cmd("AT+CWMODE=1", "OK", 50); //设置WIFI STA模式
    atk_8266_send_cmd("AT+RST", "OK", 20);      // DHCP服务器关闭(仅AP模式有效)
    delay_ms(1000);                             //延时3S等待重启成功
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    //设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!!
    sprintf((char *)p, "AT+CWJAP=\"%s\",\"%s\"", wifista_ssid, wifista_password); //设置无线参数:ssid,密码
    while (atk_8266_send_cmd(p, "WIFI GOT IP", 300))
        ; //连接目标路由器,并且获得IP
          //? PRESTA:
    /// netpro|=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[0]);	//选择网络模式
    if (netpro & 0X02) // UDP
    {
        /// LCD_Clear(WHITE);
        /// POINT_COLOR=RED;
        /// Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240);
        /// Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
        /// if(atk_8266_ip_set("WIFI-STA 远端UDP IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
        sprintf((char *)p, "AT+CIPSTART=\"UDP\",\"%s\",%s", ipbuf, (u8 *)portnum); //配置目标UDP服务器
        delay_ms(200);
        atk_8266_send_cmd("AT+CIPMUX=0", "OK", 20); //单链接模式
        delay_ms(200);
        /// LCD_Clear(WHITE);
        while (atk_8266_send_cmd(p, "OK", 500))
            ;
    }
    else // TCP
    {
        if (netpro & 0X01) // TCP Client    透传模式测试
        {
            /// LCD_Clear(WHITE);
            /// POINT_COLOR=RED;
            /// Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240);
            /// Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
            /// if(atk_8266_ip_set("WIFI-STA 远端IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
            atk_8266_send_cmd("AT+CIPMUX=0", "OK", 20);                                // 0：单连接，1：多连接
            sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"%s\",%s", ipbuf, (u8 *)portnum); //配置目标TCP服务器
            /// while(atk_8266_send_cmd(p,"OK",200))
            ///{
            /// LCD_Clear(WHITE);
            /// POINT_COLOR=RED;
            /// Show_Str_Mid(0,40,"WK_UP:返回重选",16,240);
            /// Show_Str(30,80,200,12,"ATK-ESP 连接TCP失败",12,0); //连接失败
            /// key=KEY_Scan(0);
            /// if(key==WKUP_PRES)goto PRESTA;
            ///}
            atk_8266_send_cmd("AT+CIPMODE=1", "OK", 200); //传输模式为：透传
        }
        else // TCP Server
        {
            /// LCD_Clear(WHITE);
            /// POINT_COLOR=RED;
            /// Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240);
            /// Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
            atk_8266_send_cmd("AT+CIPMUX=1", "OK", 20);             // 0：单连接，1：多连接
            sprintf((char *)p, "AT+CIPSERVER=1,%s", (u8 *)portnum); //开启Server模式(0，关闭；1，打开)，端口号为portnum
            atk_8266_send_cmd(p, "OK", 50);
        }
    }
    /// LCD_Clear(WHITE);
    /// POINT_COLOR=RED;
    /// Show_Str_Mid(0,30,"ATK-ESP WIFI-STA 测试",16,240);
    /// Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
    /// LCD_Fill(30,50,239,50+12,WHITE);			//清除之前的显示
    /// Show_Str(30,50,200,16,"WK_UP:退出测试  KEY0:发送数据",12,0);
    /// LCD_Fill(30,80,239,80+12,WHITE);
    atk_8266_get_wanip(ipbuf); //服务器模式,获取WAN IP
    sprintf((char *)p, "IP地址:%s 端口:%s", ipbuf, (u8 *)portnum);
    /// Show_Str(30,65,200,12,p,12,0);				//显示IP地址和端口
    /// Show_Str(30,80,200,12,"状态:",12,0); 		//连接状态
    /// Show_Str(120,80,200,12,"模式:",12,0); 		//连接状态
    /// Show_Str(30,100,200,12,"发送数据:",12,0); 	//发送数据
    /// Show_Str(30,115,200,12,"接收数据:",12,0);	//接收数据
    /// atk_8266_wificonf_show(30,180,"请设置路由器无线参数为:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
    /// POINT_COLOR=BLUE;
    /// Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//连接状态
    USART3_RX_STA = 0;
    while (1)
    {
        key = KEY_Scan(0);
        if (key == KEY1_PRES) // WK_UP 退出测试
        {
            res = 0;
            atk_8266_quit_trans();                       //退出透传
            atk_8266_send_cmd("AT+CIPMODE=0", "OK", 20); //关闭透传模式
            break;
        }
        else if (key == KEY2_PRES) // KEY0 发送数据
        {

            if ((netpro == 3) || (netpro == 2)) // UDP
            {
                sprintf((char *)p, "ATK-8266%s测试%02d\r\n", ATK_ESP8266_WORKMODE_TBL[netpro], t / 10); //测试数据
                /// Show_Str(30+54,100,200,12,p,12,0);
                atk_8266_send_cmd("AT+CIPSEND=25", "OK", 200); //发送指定长度的数据
                delay_ms(200);
                atk_8266_send_data(p, "OK", 100); //发送指定长度的数据
                timex = 100;
            }
            else if ((netpro == 1)) // TCP Client
            {
                atk_8266_quit_trans();
                atk_8266_send_cmd("AT+CIPSEND", "OK", 20);                                            //开始透传
                sprintf((char *)p, "ATK-8266%s测试%d\r\n", ATK_ESP8266_WORKMODE_TBL[netpro], t / 10); //测试数据
                /// Show_Str(30+54,100,200,12,p,12,0);
                u3_printf("%s", p);
                timex = 100;
            }
            else // TCP Server
            {
                sprintf((char *)p, "ATK-8266%s测试%02d\r\n", ATK_ESP8266_WORKMODE_TBL[netpro], t / 10); //测试数据
                /// Show_Str(30+54,100,200,12,p,12,0);
                atk_8266_send_cmd("AT+CIPSEND=0,25", "OK", 200); //发送指定长度的数据
                delay_ms(200);
                atk_8266_send_data(p, "OK", 100); //发送指定长度的数据
                timex = 100;
            }
        }
        else
            ;

        if (timex)
            timex--;
        /// if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
        t++;
        delay_ms(10);
        if (USART3_RX_STA & 0X8000) //接收到一次数据了
        {
            rlen = USART3_RX_STA & 0X7FFF;                   //得到本次接收到的数据长度
            USART3_RX_BUF[rlen] = 0;                         //添加结束符
            printf("%s", USART3_RX_BUF);                     //发送到串口
            sprintf((char *)p, "收到%d字节,内容如下", rlen); //接收到的字节数
            /// LCD_Fill(30+54,115,239,130,WHITE);
            /// POINT_COLOR=BRED;
            /// Show_Str(30+54,115,156,12,p,12,0); 			//显示接收到的数据长度
            /// POINT_COLOR=BLUE;
            /// LCD_Fill(30,130,239,319,WHITE);
            /// Show_Str(30,130,180,190,USART2_RX_BUF,12,0);//显示接收到的数据
            USART3_RX_STA = 0;
            if (constate != '+')
                t = 1000; //状态为还未连接,立即更新连接状态
            else
                t = 0; //状态为已经连接了,10秒后再检查
        }
        if (t == 1000) //连续10秒钟没有收到任何数据,检查连接是不是还存在.
        {
            //			//		LCD_Fill(30+54,125,239,130,WHITE);
            //					LCD_Fill(60,80,120,92,WHITE);
            constate = atk_8266_consta_check(); //得到连接状态
            /// if(constate=='+')Show_Str(30+30,80,200,12,"连接成功",12,0);  //连接状态
            /// else Show_Str(30+30,80,200,12,"连接失败",12,0);
            t = 0;
        }
        /// if((t%20)==0)LED0=!LED0;
        atk_8266_at_response(1);
    }
    myfree(p); //释放内存
    return res;
}

void atk_8266_wifista_Init(void)
{
    u8 *p;
    p = mymalloc(64); //申请64字节内存

    while (atk_8266_send_cmd("AT", "OK", 20)) //检查WIFI模块是否在线
    {
        atk_8266_quit_trans();                        //退出透传
        atk_8266_send_cmd("AT+CIPMODE=0", "OK", 200); //关闭透传模式
        sprintf((char *)p, "AT+SAVETRANSLINK=0");     //关闭上电自动进入透传模式
        atk_8266_send_cmd(p, "OK", 200);
    }

    atk_8266_send_cmd("AT+CWMODE=1", "OK", 50); //设置WIFI STA模式
    atk_8266_send_cmd("AT+RST", "OK", 20);      // DHCP服务器关闭(仅AP模式有效)
    delay_ms(1000);                             //延时1S等待重启成功
    //设置连接到的WIFI网络名称/加密方式/密码,这几个参数需要根据您自己的路由器设置进行修改!!
    sprintf((char *)p, "AT+CWJAP=\"%s\",\"%s\"", wifista_ssid, wifista_password); //设置无线参数:ssid,密码
    while (atk_8266_send_cmd(p, "WIFI GOT IP", 300))
        ;
    USART3_RX_STA = 0;
    while (atk_8266_send_cmd("AT+CIPMUX=0", "OK", 20))
        ;                                                                           // 0：单连接，1：多连接
    sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"%s\",%s", wifista_ip, (u8 *)portnum); //配置目标TCP服务器
    atk_8266_send_cmd(p, "OK", 200);
    while (atk_8266_send_cmd("AT+CIPMODE=1", "OK", 200))
        ;                                      //传输模式为：透传
    atk_8266_send_cmd("AT+CIPSEND", "OK", 20); //开始透传
    myfree(p);                                 //释放内存
}

void atk_8266_wifista_Tran(void)
{
    // u3_printf("AA,55,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%d,%d,%d,55,AA", avg_distance, avg_speed, left_speed, right_speed, tempIMU, aacx, aacy, aacz, gyrox, gyroy, gyroz, Bright_check, void_left, void_right, Search_L, Search_M, Search_R, Red_light, Green_light, LF_Light, RF_Light, LB_Light, RB_Light, left_front_speed_duty, left_behind_speed_duty, right_front_speed_duty, right_behind_speed_duty, Kp, Ki, Kd, front_duty, behind_duty, search_line_mode);
    //使用USART3发送图像数据
    // UART_DMA_Config(DMA1_Channel2, (u32)&USART3->DR, (u32)OV7670_RAM);
    // while (DMA_GetCurrDataCounter(DMA1_Channel2) != 0)
    //     ;
    // UART_DMA_Enable(DMA1_Channel2, strlen((const char *)OV7670_RAM)); //通过dma发送出去
}

void accept(void)
{
    u3_printf("BB,55,%s,55,BB", (char *)USART3_RX_BUF);
}

void switch_pid(void)
{
    int on = 0;
    sscanf((char *)USART3_RX_BUF, "%*s %d", &on);
    search_line_mode = on;
}

void IntegratedInstruction()
{
    switch (USART3_RX_BUF[0])
    {
    case 0x31:
        ctrl_comm = COMM_UP;
        break;
    case 0x32:
        ctrl_comm = COMM_DOWN;
        break;
    case 0x33:
        ctrl_comm = COMM_LEFT;
        break;
    case 0x34:
        ctrl_comm = COMM_RIGHT;
        break;
    case 0x30:
        ctrl_comm = COMM_STOP;
        break;
    default:
        break;
    }
}

void atk_8266_wifista_Rece(void) //接收单字节控制指令
{
    u16 rlen = 0;
    int tmp = 0;
    char cmd[32];
    if (USART3_RX_STA & 0X8000) //接收到一次数据了
    {
        rlen = USART3_RX_STA & 0X7FFF; //得到本次接收到的数据长度
        USART3_RX_BUF[rlen] = 0;       //添加结束符
        USART3_RX_STA = 0;
        if (isdigit(USART3_RX_BUF[0]))
            IntegratedInstruction(), accept();
        else
        {
            sscanf((char *)USART3_RX_BUF, "%s", cmd);
            if (strcmp(cmd, "cfront") == 0)
                sscanf((char *)USART3_RX_BUF, "%*s %d", &tmp), front_duty = tmp, accept();
            else if (strcmp(cmd, "cbehind") == 0)
                sscanf((char *)USART3_RX_BUF, "%*s %d", &tmp), behind_duty = tmp, accept();
            else if (strcmp(cmd, "cmotion") == 0)
                switch_pid(), accept();
            else if (strcmp(cmd, "cpid") == 0)
                sscanf((char *)USART3_RX_BUF, "%*s %f %f %f", &Kp, &Ki, &Kd), accept();
            else if (strcmp(cmd, "ckp") == 0)
                sscanf((char *)USART3_RX_BUF, "%*s %f", &Kp), accept();
            else if (strcmp(cmd, "cki") == 0)
                sscanf((char *)USART3_RX_BUF, "%*s %f", &Ki), accept();
            else if (strcmp(cmd, "ckd") == 0)
                sscanf((char *)USART3_RX_BUF, "%*s %f", &Kd), accept();
        }
    }
}
