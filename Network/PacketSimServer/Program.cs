using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

class PacketSimServer
{
    static Random rng = new Random();

    static void Main()
    {
        UdpClient server = new UdpClient(15000);
        IPEndPoint remoteEP = new IPEndPoint(IPAddress.Any, 0);

        Console.WriteLine("[Packet Server] Listening on port 15000...");

        while (true)
        {
            byte[] bytes = server.Receive(ref remoteEP);

            // Simulate packet drop (10% chance)
            if (rng.Next(0, 10) == 0)
            {
                Console.WriteLine("[DROP] Packet from " + remoteEP);
                continue;
            }

            // Simulate network delay
            Thread.Sleep(rng.Next(100, 600));

            string received = Encoding.ASCII.GetString(bytes);
            Console.WriteLine("[RECEIVED] " + received);

            string echo = "Echo: " + received;
            server.Send(Encoding.ASCII.GetBytes(echo), echo.Length, remoteEP);
        }
    }
}
