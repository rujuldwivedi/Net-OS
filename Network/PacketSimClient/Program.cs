using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class PacketSimClient
{
    static void Main()
    {
        UdpClient client = new UdpClient();
        client.Connect("127.0.0.1", 15000);

        Console.Write("Enter message: ");
        string payload = Console.ReadLine();

        // Simulate OSI Layers via custom header
        string header = "[OSI-L3:IP][OSI-L4:UDP]";
        string fullPacket = header + payload;

        byte[] data = Encoding.ASCII.GetBytes(fullPacket);
        client.Send(data, data.Length);

        IPEndPoint serverEP = new IPEndPoint(IPAddress.Any, 0);
        byte[] received = client.Receive(ref serverEP);
        string response = Encoding.ASCII.GetString(received);

        Console.WriteLine("[Client] Received: " + response);
        client.Close();
    }
}
