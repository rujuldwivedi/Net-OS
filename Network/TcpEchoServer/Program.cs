using System;
using System.Net;
using System.Net.Sockets;
using System.Text;

class TcpEchoServer
{
    static void Main()
    {
        TcpListener server = null;
        try
        {
            int port = 13000;
            IPAddress localAddr = IPAddress.Parse("127.0.0.1");

            server = new TcpListener(localAddr, port);
            server.Start();

            Console.WriteLine("[SERVER] Listening on port " + port);

            while (true)
            {
                Console.WriteLine("[SERVER] Waiting for a connection...");
                TcpClient client = server.AcceptTcpClient();
                Console.WriteLine("[SERVER] Connected!");

                NetworkStream stream = client.GetStream();

                byte[] buffer = new byte[256];
                int bytesRead = stream.Read(buffer, 0, buffer.Length);
                string msg = Encoding.ASCII.GetString(buffer, 0, bytesRead);

                Console.WriteLine("[SERVER] Received: " + msg);

                // Echo back
                byte[] reply = Encoding.ASCII.GetBytes("Echo: " + msg);
                stream.Write(reply, 0, reply.Length);

                client.Close();
                Console.WriteLine("[SERVER] Connection closed.\n");
            }
        }
        catch (SocketException e)
        {
            Console.WriteLine("Socket error: " + e.Message);
        }
        finally
        {
            server?.Stop();
        }
    }
}
