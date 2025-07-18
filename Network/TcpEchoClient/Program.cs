using System;
using System.Net.Sockets;
using System.Text;

class TcpEchoClient
{
    static void Main()
    {
        try
        {
            TcpClient client = new TcpClient("127.0.0.1", 13000);

            NetworkStream stream = client.GetStream();
            Console.Write("Enter message: ");
            string msg = Console.ReadLine();

            byte[] data = Encoding.ASCII.GetBytes(msg);
            stream.Write(data, 0, data.Length);

            byte[] buffer = new byte[256];
            int bytesRead = stream.Read(buffer, 0, buffer.Length);
            string response = Encoding.ASCII.GetString(buffer, 0, bytesRead);

            Console.WriteLine("[CLIENT] Received: " + response);

            stream.Close();
            client.Close();
        }
        catch (Exception e)
        {
            Console.WriteLine("Client error: " + e.Message);
        }
    }
}
