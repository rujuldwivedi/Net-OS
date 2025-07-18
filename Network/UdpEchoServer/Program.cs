using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class UdpEchoServer
{
    static void Main()
    {
        UdpClient server = new UdpClient(14000);
        IPEndPoint remoteEP = new IPEndPoint(IPAddress.Any, 0);

        Console.WriteLine("[UDP SERVER] Listening on port 14000...");

        while (true)
        {
            byte[] bytes = server.Receive(ref remoteEP);
            string message = Encoding.ASCII.GetString(bytes);

            Console.WriteLine($"[UDP SERVER] Received from {remoteEP}: {message}");

            // Echo back
            byte[] echoBytes = Encoding.ASCII.GetBytes("Echo: " + message);
            server.Send(echoBytes, echoBytes.Length, remoteEP);
        }
    }
}
