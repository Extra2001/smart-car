namespace SmartCar
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            textBox1.Text = TCPHelper.GetIP();
        }

        private string img = "";
        private List<byte> data = new List<byte>();
        private int count = 0;
        private bool receive = false;
        private void button1_Click(object sender, EventArgs e)
        {
            button1.Text = "等待连接...";

            TCPHelper.StartServer(Convert.ToInt32(textBox2.Text), () =>
            {
                button1.Text = "关闭连接";
                textBox4.Text = TCPHelper.clientipe.Address.ToString();
                textBox3.Text = TCPHelper.clientipe.Port.ToString();
                TCPHelper.StartRecieve(res =>
                {
                    if (res.StartsWith("Hh"))
                        receive = true;
                    else if (res.StartsWith("hH"))
                        receive = false;
                    res = res.Trim();
                    textBox5.Text = res;
                    var (a, b) = CMDParser.Parse(res);
                    if (a == DataType.CarData)
                    {
                        Load_data(b);
                    }
                    else if (a == DataType.Success)
                    {
                        label32.Text = "指令已接收";
                    }
                }, res =>
                {
                    if (receive && count < 740)
                    {
                        Paint_bmp(res);
                        count++;
                    }
                    else
                    {
                        hang = 0;
                        lie = 0;
                        count = 0;
                    }
                }, () =>
               {
                   button1.Text = "等待连接...";
                   textBox4.Text = "";
                   textBox3.Text = "";
               });
            });
        }

        void Paint_bmp(byte[] Data)
        {
            bool isheight = false;
            byte heightdate = 0;
            for (int i = 0; i < Data.Length; i++)
            {
                if (isheight)                           //判断是否为高位
                {
                    isheight = false;
                    heightdate = Data[i];
                }
                else
                {
                    isheight = true;                    //若为低8位,则转化颜色,并写入bmp
                    Color c = RGB565ToRGB24(Data[i], heightdate);
                    Write_Color(c);
                }
            }
        }
        Bitmap bmp = new Bitmap(320, 240);
        int lie = 0;
        int hang = 0;
        void Write_Color(Color c)
        {
            bmp.SetPixel(lie, hang, c);
            lie++;
            if (lie == 320)
            {
                lie = 0;
                try
                {
                    pictureBox1.Image = bmp.Clone() as Bitmap;
                }
                catch
                {
                }
                hang++;
            }
            if (hang == 240)
            {
                hang = 0;
            }
        }

        private Color RGB565ToRGB24(int RGB565_H, int RGB565_L)
        {
            int RGB565_MASK_RED = 0xF800;
            int RGB565_MASK_GREEN = 0x07E0;
            int RGB565_MASK_BLUE = 0x001F;
            //int RGB565_MASK_RED = 0x7C00;
            //int RGB565_MASK_GREEN = 0x03E0;
            //int RGB565_MASK_BLUE = 0x001F;
            int RGB565;
            int R, G, B;
            RGB565_H <<= 8;
            RGB565 = RGB565_H | RGB565_L;
            R = (RGB565 & RGB565_MASK_RED) >> 11;
            G = (RGB565 & RGB565_MASK_GREEN) >> 5;
            B = (RGB565 & RGB565_MASK_BLUE);
            R <<= 3;
            G <<= 2;
            B <<= 3;
            return Color.FromArgb(R, G, B);
        }

        private void Load_data(CarData carData)
        {
            textBox8.Text = carData.distance;
            textBox7.Text = carData.avg_speed;
            textBox10.Text = carData.left_speed;
            textBox9.Text = carData.right_speed;
            textBox19.Text = carData.duty_left_front;
            textBox18.Text = carData.duty_left_behind;
            textBox27.Text = carData.duty_right_front;
            textBox33.Text = carData.duty_right_behind;
            textBox12.Text = carData.temp_IMU;
            textBox11.Text = carData.aacx;
            textBox14.Text = carData.aacy;
            textBox13.Text = carData.aacz;
            textBox17.Text = carData.gyrox;
            textBox16.Text = carData.gyroy;
            textBox15.Text = carData.gyroz;
            textBox23.Text = carData.kp;
            textBox22.Text = carData.ki;
            textBox21.Text = carData.kd;
            textBox31.Text = carData.front_duty;
            textBox29.Text = carData.behind_duty;
            checkBox10.Checked = carData.lf_light;
            checkBox9.Checked = carData.rf_light;
            checkBox12.Checked = carData.lb_light;
            checkBox11.Checked = carData.rb_light;
            checkBox1.Checked = carData.bright;
            checkBox5.Checked = carData.void_left;
            checkBox6.Checked = carData.void_right;
            checkBox2.Checked = carData.search_l;
            checkBox3.Checked = carData.search_m;
            checkBox4.Checked = carData.search_r;
            checkBox7.Checked = carData.red_light;
            checkBox8.Checked = carData.green_light;
            textBox32.Text = carData.search_line_mode;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString(textBox6.Text))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button7_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString("1"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString("3"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString("2"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString("4"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString("0"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button8_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString("cmotion 1"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button15_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString("cmotion 0"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button11_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString($"ckp {textBox26.Text}"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button12_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString($"cki {textBox25.Text}"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button13_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString($"ckd {textBox24.Text}"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button10_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString($"cpid {textBox26.Text} {textBox25.Text} {textBox24.Text}"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button16_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString($"cfront {textBox28.Text}"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }

        private void button18_Click(object sender, EventArgs e)
        {
            if (TCPHelper.SendString($"cbehind {textBox30.Text}"))
                label32.Text = "发送成功";
            else label32.Text = "发送失败";
        }
    }
}