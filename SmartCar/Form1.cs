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
                }, () =>
                {
                    button1.Text = "等待连接...";
                    textBox4.Text = "";
                    textBox3.Text = "";
                });
            });
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