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
extern char turn_speed_duty;
extern char pid_on;
extern char striaght_duty;
extern char turn_duty;

// ATK-ESP8266 WIFI STA����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//     ����,�������
u8 netpro = 0; //����ģʽ
u8 atk_8266_wifista_test(void)
{
    // u8 netpro=0;	//����ģʽ
    u8 key;
    u8 timex = 0;
    u8 ipbuf[16]; // IP����
    u8 *p;
    u16 t = 999; //���ٵ�һ�λ�ȡ����״̬
    u8 res = 0;
    u16 rlen = 0;
    u8 constate = 0;                            //����״̬
    p = mymalloc(32);                           //����32�ֽ��ڴ�
    atk_8266_send_cmd("AT+CWMODE=1", "OK", 50); //����WIFI STAģʽ
    atk_8266_send_cmd("AT+RST", "OK", 20);      // DHCP�������ر�(��APģʽ��Ч)
    delay_ms(1000);                             //��ʱ3S�ȴ������ɹ�
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    //�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!!
    sprintf((char *)p, "AT+CWJAP=\"%s\",\"%s\"", wifista_ssid, wifista_password); //�������߲���:ssid,����
    while (atk_8266_send_cmd(p, "WIFI GOT IP", 300))
        ; //����Ŀ��·����,���һ��IP
          //? PRESTA:
    /// netpro|=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[0]);	//ѡ������ģʽ
    if (netpro & 0X02) // UDP
    {
        /// LCD_Clear(WHITE);
        /// POINT_COLOR=RED;
        /// Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240);
        /// Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
        /// if(atk_8266_ip_set("WIFI-STA Զ��UDP IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
        sprintf((char *)p, "AT+CIPSTART=\"UDP\",\"%s\",%s", ipbuf, (u8 *)portnum); //����Ŀ��UDP������
        delay_ms(200);
        atk_8266_send_cmd("AT+CIPMUX=0", "OK", 20); //������ģʽ
        delay_ms(200);
        /// LCD_Clear(WHITE);
        while (atk_8266_send_cmd(p, "OK", 500))
            ;
    }
    else // TCP
    {
        if (netpro & 0X01) // TCP Client    ͸��ģʽ����
        {
            /// LCD_Clear(WHITE);
            /// POINT_COLOR=RED;
            /// Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240);
            /// Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
            /// if(atk_8266_ip_set("WIFI-STA Զ��IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
            atk_8266_send_cmd("AT+CIPMUX=0", "OK", 20);                                // 0�������ӣ�1��������
            sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"%s\",%s", ipbuf, (u8 *)portnum); //����Ŀ��TCP������
            /// while(atk_8266_send_cmd(p,"OK",200))
            ///{
            /// LCD_Clear(WHITE);
            /// POINT_COLOR=RED;
            /// Show_Str_Mid(0,40,"WK_UP:������ѡ",16,240);
            /// Show_Str(30,80,200,12,"ATK-ESP ����TCPʧ��",12,0); //����ʧ��
            /// key=KEY_Scan(0);
            /// if(key==WKUP_PRES)goto PRESTA;
            ///}
            atk_8266_send_cmd("AT+CIPMODE=1", "OK", 200); //����ģʽΪ��͸��
        }
        else // TCP Server
        {
            /// LCD_Clear(WHITE);
            /// POINT_COLOR=RED;
            /// Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240);
            /// Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
            atk_8266_send_cmd("AT+CIPMUX=1", "OK", 20);             // 0�������ӣ�1��������
            sprintf((char *)p, "AT+CIPSERVER=1,%s", (u8 *)portnum); //����Serverģʽ(0���رգ�1����)���˿ں�Ϊportnum
            atk_8266_send_cmd(p, "OK", 50);
        }
    }
    /// LCD_Clear(WHITE);
    /// POINT_COLOR=RED;
    /// Show_Str_Mid(0,30,"ATK-ESP WIFI-STA ����",16,240);
    /// Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
    /// LCD_Fill(30,50,239,50+12,WHITE);			//���֮ǰ����ʾ
    /// Show_Str(30,50,200,16,"WK_UP:�˳�����  KEY0:��������",12,0);
    /// LCD_Fill(30,80,239,80+12,WHITE);
    atk_8266_get_wanip(ipbuf); //������ģʽ,��ȡWAN IP
    sprintf((char *)p, "IP��ַ:%s �˿�:%s", ipbuf, (u8 *)portnum);
    /// Show_Str(30,65,200,12,p,12,0);				//��ʾIP��ַ�Ͷ˿�
    /// Show_Str(30,80,200,12,"״̬:",12,0); 		//����״̬
    /// Show_Str(120,80,200,12,"ģʽ:",12,0); 		//����״̬
    /// Show_Str(30,100,200,12,"��������:",12,0); 	//��������
    /// Show_Str(30,115,200,12,"��������:",12,0);	//��������
    /// atk_8266_wificonf_show(30,180,"������·�������߲���Ϊ:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
    /// POINT_COLOR=BLUE;
    /// Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//����״̬
    USART3_RX_STA = 0;
    while (1)
    {
        key = KEY_Scan(0);
        if (key == KEY1_PRES) // WK_UP �˳�����
        {
            res = 0;
            atk_8266_quit_trans();                       //�˳�͸��
            atk_8266_send_cmd("AT+CIPMODE=0", "OK", 20); //�ر�͸��ģʽ
            break;
        }
        else if (key == KEY2_PRES) // KEY0 ��������
        {

            if ((netpro == 3) || (netpro == 2)) // UDP
            {
                sprintf((char *)p, "ATK-8266%s����%02d\r\n", ATK_ESP8266_WORKMODE_TBL[netpro], t / 10); //��������
                /// Show_Str(30+54,100,200,12,p,12,0);
                atk_8266_send_cmd("AT+CIPSEND=25", "OK", 200); //����ָ�����ȵ�����
                delay_ms(200);
                atk_8266_send_data(p, "OK", 100); //����ָ�����ȵ�����
                timex = 100;
            }
            else if ((netpro == 1)) // TCP Client
            {
                atk_8266_quit_trans();
                atk_8266_send_cmd("AT+CIPSEND", "OK", 20);                                            //��ʼ͸��
                sprintf((char *)p, "ATK-8266%s����%d\r\n", ATK_ESP8266_WORKMODE_TBL[netpro], t / 10); //��������
                /// Show_Str(30+54,100,200,12,p,12,0);
                u3_printf("%s", p);
                timex = 100;
            }
            else // TCP Server
            {
                sprintf((char *)p, "ATK-8266%s����%02d\r\n", ATK_ESP8266_WORKMODE_TBL[netpro], t / 10); //��������
                /// Show_Str(30+54,100,200,12,p,12,0);
                atk_8266_send_cmd("AT+CIPSEND=0,25", "OK", 200); //����ָ�����ȵ�����
                delay_ms(200);
                atk_8266_send_data(p, "OK", 100); //����ָ�����ȵ�����
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
        if (USART3_RX_STA & 0X8000) //���յ�һ��������
        {
            rlen = USART3_RX_STA & 0X7FFF;                   //�õ����ν��յ������ݳ���
            USART3_RX_BUF[rlen] = 0;                         //��ӽ�����
            printf("%s", USART3_RX_BUF);                     //���͵�����
            sprintf((char *)p, "�յ�%d�ֽ�,��������", rlen); //���յ����ֽ���
            /// LCD_Fill(30+54,115,239,130,WHITE);
            /// POINT_COLOR=BRED;
            /// Show_Str(30+54,115,156,12,p,12,0); 			//��ʾ���յ������ݳ���
            /// POINT_COLOR=BLUE;
            /// LCD_Fill(30,130,239,319,WHITE);
            /// Show_Str(30,130,180,190,USART2_RX_BUF,12,0);//��ʾ���յ�������
            USART3_RX_STA = 0;
            if (constate != '+')
                t = 1000; //״̬Ϊ��δ����,������������״̬
            else
                t = 0; //״̬Ϊ�Ѿ�������,10����ټ��
        }
        if (t == 1000) //����10����û���յ��κ�����,��������ǲ��ǻ�����.
        {
            //			//		LCD_Fill(30+54,125,239,130,WHITE);
            //					LCD_Fill(60,80,120,92,WHITE);
            constate = atk_8266_consta_check(); //�õ�����״̬
            /// if(constate=='+')Show_Str(30+30,80,200,12,"���ӳɹ�",12,0);  //����״̬
            /// else Show_Str(30+30,80,200,12,"����ʧ��",12,0);
            t = 0;
        }
        /// if((t%20)==0)LED0=!LED0;
        atk_8266_at_response(1);
    }
    myfree(p); //�ͷ��ڴ�
    return res;
}

void atk_8266_wifista_Init(void)
{
    // u8 netpro=0;	//����ģʽ
    // u8 key;
    // u8 timex=0;
    // u8 ipbuf[16]; 	//IP����
    u8 *p;
    // u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
    // u8 res=0;
    // u16 rlen=0;
    // u8 constate=0;	//����״̬
    p = mymalloc(64); //����64�ֽ��ڴ�

    while (atk_8266_send_cmd("AT", "OK", 20)) //���WIFIģ���Ƿ�����
    {
        atk_8266_quit_trans();                        //�˳�͸��
        atk_8266_send_cmd("AT+CIPMODE=0", "OK", 200); //�ر�͸��ģʽ
        sprintf((char *)p, "AT+SAVETRANSLINK=0");     //�ر��ϵ��Զ�����͸��ģʽ
        // while(atk_8266_send_cmd(p,"OK",200));
        atk_8266_send_cmd(p, "OK", 200);
        delay_ms(800);
    }

    atk_8266_send_cmd("AT+CWMODE=1", "OK", 50); //����WIFI STAģʽ
    atk_8266_send_cmd("AT+RST", "OK", 20);      // DHCP�������ر�(��APģʽ��Ч)
    delay_ms(1000);                             //��ʱ3S�ȴ������ɹ�
    delay_ms(1000);
    delay_ms(1000);
    delay_ms(1000);
    //�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!!
    sprintf((char *)p, "AT+CWJAP=\"%s\",\"%s\"", wifista_ssid, wifista_password); //�������߲���:ssid,����
    while (atk_8266_send_cmd(p, "WIFI GOT IP", 300))
        ;
    USART3_RX_STA = 0;
    while (atk_8266_send_cmd("AT+CIPMUX=0", "OK", 20))
        ;                                                                           // 0�������ӣ�1��������
    sprintf((char *)p, "AT+CIPSTART=\"TCP\",\"%s\",%s", wifista_ip, (u8 *)portnum); //����Ŀ��TCP������
    // while(atk_8266_send_cmd(p,"OK",200));
    atk_8266_send_cmd(p, "OK", 200); //�˴���ȴ������������ӳɹ�����ʹ��while���ҷ�����δ��������ѭ��
    //=====================================================================
    //�ϵ��Զ�����͸��ģʽ
    // sprintf((char*)p,"AT+SAVETRANSLINK=1,\"%s\",%s,\"TCP\"",wifista_ip,(u8*)portnum);
    // while(atk_8266_send_cmd(p,"OK",200));
    //=====================================================================
    while (atk_8266_send_cmd("AT+CIPMODE=1", "OK", 200))
        ; //����ģʽΪ��͸��
    // atk_8266_quit_trans();

    // while(atk_8266_send_cmd("AT+CIPSEND","OK",20));         //��ʼ͸��
    atk_8266_send_cmd("AT+CIPSEND", "OK", 20); //��ʼ͸��
    myfree(p);                                 //�ͷ��ڴ�
}

void atk_8266_wifista_Tran(void)
{
    // u8 *p;
    // p = malloc(512); //����150�ֽ��ڴ�
    // sprintf((char *)p, "AA,55,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%f,%d,%d,%d,55,AA",
    //         distance_tran, avg_speed, left_speed, right_speed, tempIMU, aacx, aacy, aacz, gyrox, gyroy, gyroz, Bright_check, void_left, void_right, Search_L, Search_M, Search_R, Red_light, Green_light, LF_Light, RF_Light, LB_Light, RB_Light, left_speed_duty, right_speed_duty, Kp, Ki, Kd, factor, striaght_duty, turn_duty, pid_on);
    u3_printf("AA,55,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%f,%f,%d,%d,%d,%d,55,AA",
              distance_tran, avg_speed, left_speed, right_speed, tempIMU, aacx, aacy, aacz, gyrox, gyroy, gyroz, Bright_check, void_left, void_right, Search_L, Search_M, Search_R, Red_light, Green_light, LF_Light, RF_Light, LB_Light, RB_Light, left_speed_duty, right_speed_duty, Kp, Ki, Kd, turn_speed_duty, striaght_duty, turn_duty, pid_on); //ͨ������3������WIFIģ�飬��͸������λ��
    // free(p);
}

void accept(void)
{
    u3_printf("BB,55,%s,55,BB", (char *)USART3_RX_BUF);
}

void switch_pid(void)
{
    int on = 0;
    sscanf((char *)USART3_RX_BUF, "%*s %d", &on);
    pid_on = on;
}

void IntegratedInstruction(char instruction)
{
    switch (USART3_RX_BUF[0])
    {
    case 0x31:
        WF_comm = COMM_UP;
        break;
    case 0x32:
        WF_comm = COMM_DOWN;
        break;
    case 0x33:
        WF_comm = COMM_LEFT;
        break;
    case 0x34:
        WF_comm = COMM_RIGHT;
        break;
    case 0x30:
        WF_comm = COMM_STOP;
        break;
    case 0x36:
        WF_comm = CCD_UP;
        break;
    case 0x37:
        WF_comm = CCD_DOWN;
        break;
    case 0x38:
        WF_comm = CCD_LEFT;
        break;
    case 0x39:
        WF_comm = CCD_RIGHT;
        break;
    case 0x35:
        WF_comm = CCD_CENTER;
        break;
    default:
        break;
    }
    wf_rec_flag = 1;
}

void atk_8266_wifista_Rece(void) //���յ��ֽڿ���ָ��
{
    u16 rlen = 0;
    int tmp = 0;
    char cmd[32];
    if (USART3_RX_STA & 0X8000) //���յ�һ��������
    {
        rlen = USART3_RX_STA & 0X7FFF; //�õ����ν��յ������ݳ���
        USART3_RX_BUF[rlen] = 0;       //��ӽ�����
        USART3_RX_STA = 0;
        if (isdigit(USART3_RX_BUF[0]))
            IntegratedInstruction(USART3_RX_BUF[0]), accept();
        else
        {
            sscanf((char *)USART3_RX_BUF, "%s", cmd);
            if (strcmp(cmd, "cfactor") == 0)
                sscanf((char *)USART3_RX_BUF, "%*s %d", &tmp), turn_speed_duty = tmp, accept();
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
            else if (strcmp(cmd, "cstra") == 0)
                sscanf((char *)USART3_RX_BUF, "%*s %d", &tmp), striaght_duty = tmp, accept();
            else if (strcmp(cmd, "cturn") == 0)
                sscanf((char *)USART3_RX_BUF, "%*s %d", &tmp), turn_duty = tmp, accept();
        }
    }	
}
