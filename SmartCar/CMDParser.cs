using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SmartCar
{
    public class CarData
    {
        public string distance { get; set; }
        public string avg_speed { get; set; }
        public string left_speed { get; set; }
        public string right_speed { get; set; }
        public string temp_IMU { get; set; }
        public string aacx { get; set; }
        public string aacy { get; set; }
        public string aacz { get; set; }
        public string gyrox { get; set; }
        public string gyroy { get; set; }
        public string gyroz { get; set; }
        public bool bright { get; set; }
        public bool void_left { get; set; }
        public bool void_right { get; set; }
        public bool search_l { get; set; }
        public bool search_m { get; set; }
        public bool search_r { get; set; }
        public bool red_light { get; set; }
        public bool green_light { get; set; }
        public bool lf_light { get; set; }
        public bool rf_light { get; set; }
        public bool lb_light { get; set; }
        public bool rb_light { get; set; }
        public string duty_left { get; set; }
        public string duty_right { get; set; }
        public string kp { get; set; }
        public string ki { get; set; }
        public string kd { get; set; }
        public string factor { get; set; }
        public string striaght_duty { get; set; }
        public string turn_duty { get; set; }
        public string pid_on { get; set; }
    }

    public enum DataType
    {
        Null,
        CarData,
        Success
    }
    public class CMDParser
    {
        private static bool String2Bool(string x)
        {
            if(int.TryParse(x, out int a))
            {
                return a > 0;
            }
            else
            {
                return false;
            }
        }
        public static (DataType, CarData) Parse(string data)
        {
            CarData carData = new CarData();
            DataType dataType = DataType.Null;

            if (data.StartsWith("AA,55,") && data.EndsWith(",55,AA"))
            {
                dataType = DataType.CarData;
                var arr = data.Trim().Split(',');
                if (arr.Length != carData.GetType().GetProperties().Length + 4)
                    return (DataType.Null, null);
                carData = new CarData()
                {
                    distance = arr[2],
                    avg_speed = arr[3],
                    left_speed = arr[4],
                    right_speed = arr[5],
                    temp_IMU = arr[6],
                    aacx = arr[7],
                    aacy = arr[8],
                    aacz = arr[9],
                    gyrox = arr[10],
                    gyroy = arr[11],
                    gyroz = arr[12],
                    bright = String2Bool(arr[13]),
                    void_left = String2Bool(arr[14]),
                    void_right = String2Bool(arr[15]),
                    search_l = String2Bool(arr[16]),
                    search_m = String2Bool(arr[17]),
                    search_r = String2Bool(arr[18]),
                    red_light = String2Bool(arr[19]),
                    green_light = String2Bool(arr[20]),
                    lf_light = String2Bool(arr[21]),
                    rf_light = String2Bool(arr[22]),
                    lb_light = String2Bool(arr[23]),
                    rb_light = String2Bool(arr[24]),
                    duty_left = arr[25],
                    duty_right = arr[26],
                    kp = arr[27],
                    ki = arr[28],
                    kd = arr[29],
                    factor = arr[30],
                    striaght_duty = arr[31],
                    turn_duty = arr[32],
                    pid_on = arr[33]
                };
                return (dataType, carData);
            }
            else if (data.StartsWith("BB,55,") && data.EndsWith(",55,BB"))
            {
                dataType = DataType.Success;
            }
            return (dataType, null);
        }
    }
}
