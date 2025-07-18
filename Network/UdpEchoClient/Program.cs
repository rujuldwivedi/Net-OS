using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class UdpEchoClient
{
    static void Main()
    {
        UdpClient client = new UdpClient();
        client.Connect("127.0.0.1", 14000);

        Console.Write("Enter message: ");
        string message = Console.ReadLine();
        byte[] data = Encoding.ASCII.GetBytes(message);
        client.Send(data, data.Length);

        IPEndPoint serverEP = new IPEndPoint(IPAddress.Any, 0);
        byte[] received = client.Receive(ref serverEP);

        string reply = Encoding.ASCII.GetString(received);
        Console.WriteLine("[UDP CLIENT] Received: " + reply);

        client.Close();
    }
}
