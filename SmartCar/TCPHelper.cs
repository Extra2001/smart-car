using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.NetworkInformation;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace SmartCar
{
    internal class TCPHelper
    {
        public static TcpListener server = default;
        public static Socket client = default;
        public static IPEndPoint clientipe = default;
        private static System.Timers.Timer timer = new System.Timers.Timer(200);
        private static Action<string> callback = null;

        public static string GetIP()
        {
            string ipv4 = "";
            NetworkInterface[] nics = NetworkInterface.GetAllNetworkInterfaces();
            foreach (NetworkInterface adapter in nics)
            {
                if (adapter.NetworkInterfaceType == NetworkInterfaceType.Wireless80211)
                {
                    //网卡Mac地址
                    PhysicalAddress mac = adapter.GetPhysicalAddress();
                    //TODO:根据mac 地址 匹配目标网卡

                    //获取以太网卡网络接口信息
                    IPInterfaceProperties ip = adapter.GetIPProperties();
                    
                    //获取单播地址集
                    UnicastIPAddressInformationCollection ipCollection = ip.UnicastAddresses;
                    foreach (UnicastIPAddressInformation ipadd in ipCollection)
                    {
                        if (ipadd.Address.AddressFamily == AddressFamily.InterNetwork)
                        {
                            ipv4 = ipadd.Address.ToString();
                        }
                    }
                }
            }
            return ipv4;
        }

        public static void StartServer(int port, Action connected)
        {
            TcpListener server = new TcpListener(System.Net.IPAddress.Any, port);
            server.Start();

            server.AcceptSocketAsync().ContinueWith(x =>
            {
                client = x.Result;
                clientipe = (IPEndPoint)client.RemoteEndPoint;
                connected();
            });
        }

        public static void StartRecieve(Action<string> callback, Action<byte[]> callbackBytes, Action end)
        {
            if (timer != null) timer.Close();
            timer = new System.Timers.Timer(50);
            timer.Elapsed += (sender, args) =>
            {
                if (client.Connected)
                {
                    if (client.Available > 0)
                    {
                        client.ReceiveTimeout = 50;
                        byte[] readBytes = new byte[client.Available];
                        client.Receive(readBytes);
                        string str = Encoding.ASCII.GetString(readBytes);
                        callback?.Invoke(str);
                        callbackBytes?.Invoke(readBytes);
                    }
                }
                else
                {
                    StopRecieve();
                    end?.Invoke();
                }
            };
            timer.Start();
        }

        public static void StopRecieve()
        {
            timer.Stop();
        }

        public static bool SendString(string x)
        {
            try
            {
                client.Send(Encoding.ASCII.GetBytes(x));
                return true;
            }
            catch
            {
                return false;
            }
        }
    }
}
