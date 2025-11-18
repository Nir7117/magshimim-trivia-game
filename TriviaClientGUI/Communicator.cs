using System;
using System.Collections.Generic;
using System.IO;
using System.Net.Sockets;
using System.Text;
using System.Windows;
using MongoDB.Bson;
using MongoDB.Bson.IO;
using MongoDB.Bson.Serialization;

namespace TriviaClientGUI
{
    public class Communicator
    {
        private TcpClient _client;
        public NetworkStream _stream;
        public NetworkStream Stream => _stream;

        public Communicator(string ip = "127.0.0.1", int port = 8826)
        {
            _client = new TcpClient(ip, port);
            _stream = _client.GetStream();
        }

        public void Close()
        {
            _stream?.Close();
            _client?.Close();
        }

        public byte[] BuildMessage(byte code, Dictionary<string, object> payload)
        {
            var bsonDoc = new BsonDocument(payload);
            byte[] bsonData;

            using (var ms = new MemoryStream())
            using (var writer = new BsonBinaryWriter(ms))
            {
                BsonSerializer.Serialize(writer, bsonDoc);
                bsonData = ms.ToArray();
            }

            using (var ms = new MemoryStream())
            using (var bw = new BinaryWriter(ms))
            {
                bw.Write(code);
                bw.Write(System.Net.IPAddress.HostToNetworkOrder(bsonData.Length));
                bw.Write(bsonData);
                return ms.ToArray();
            }
        }

        public Dictionary<string, object> ReadResponse(out byte responseCode)
        {
            responseCode = 0;
            var codeBuffer = ReadExact(1);
            if (codeBuffer == null)
            {
                Console.WriteLine("codeBuffer is null");
                return null;
            }

            responseCode = codeBuffer[0];

            var lengthBuffer = ReadExact(4);
            if (lengthBuffer == null)
            {
                Console.WriteLine("lengthBuffer is null");
                return null;
            }

            int length = System.Net.IPAddress.NetworkToHostOrder(BitConverter.ToInt32(lengthBuffer, 0));
            var bsonData = ReadExact(length);

            if (bsonData == null)
            {
                Console.WriteLine("bsonData is null");
                return null;
            }

            var doc = BsonSerializer.Deserialize<BsonDocument>(bsonData);
            return doc.ToDictionary();
        }


        private byte[] ReadExact(int count)
        {
            byte[] buffer = new byte[count];
            int totalRead = 0;
            while (totalRead < count)
            {
                int bytesRead = _stream.Read(buffer, totalRead, count - totalRead);
                if (bytesRead == 0) return null;
                totalRead += bytesRead;
            }
            return buffer;
        }

        public bool CreateRoomWithFullResponse(string name, int maxUsers, int questionCount, int timeout, out int roomId, out string admin)
        {
            var payload = new Dictionary<string, object>
    {
        { "name", name },
        { "maxPlayers", maxUsers },
        { "questionCount", questionCount },
        { "answerTimeout", timeout }
    };

            byte[] message = BuildMessage(206, payload);
            Stream.Write(message, 0, message.Length);

            var response = ReadResponse(out byte code);
            if (response != null && response.ContainsKey("status") && Convert.ToInt32(response["status"]) == 1)
            {
                roomId = Convert.ToInt32(response["roomId"]);
                admin = response.ContainsKey("admin") ? response["admin"].ToString() : "unknown";
                return true;
            }

            roomId = 0;
            admin = "";
            return false;
        }

        public bool SendLogin(string username, string password)
        {
            //MessageBox.Show("SendLogin called");

            var payload = new Dictionary<string, object>
            {
                { "username", username },
                { "password", password }
            };
            byte[] message = BuildMessage(200, payload);
            _stream.Write(message, 0, message.Length);

            var response = ReadResponse(out byte code);
            return response != null && response.ContainsKey("status") && Convert.ToInt32(response["status"]) == 1;
        }

        public bool SendSignup(string username, string password, string email)
        {
            var payload = new Dictionary<string, object>
            {
                { "username", username },
                { "password", password },
                { "email", email }
            };
            byte[] message = BuildMessage(201, payload);
            _stream.Write(message, 0, message.Length);

            var response = ReadResponse(out byte code);
            return response != null && response.ContainsKey("status") && Convert.ToInt32(response["status"]) == 1;
        }

        public bool CreateRoom(string roomName, int maxUsers, int questionCount, int answerTimeout, out int roomId)
        {
            var payload = new Dictionary<string, object>
            {
                { "name", roomName },
                { "maxPlayers", maxUsers },
                { "questionCount", questionCount },
                { "answerTimeout", answerTimeout }
            };

            byte[] message = BuildMessage(206, payload);
            _stream.Write(message, 0, message.Length);

            var response = ReadResponse(out byte code);

            if (response != null && response.ContainsKey("status") && Convert.ToInt32(response["status"]) == 1)
            {
                roomId = Convert.ToInt32(response["roomId"]);
                return true;
            }

            roomId = -1;
            return false;
        }
        public Dictionary<string, object> GetStatistics()
        {
           //MessageBox.Show("SENDING STATS REQUEST.");

            byte[] message = BuildMessage((byte)208, new Dictionary<string, object>());
            _stream.Write(message, 0, message.Length);

            var response = ReadResponse(out byte code);

            //MessageBox.Show("GOT RESPONSE.");

            return response;
        }
        public Dictionary<string, object> GetHighScores()
        {
            byte[] message = BuildMessage((byte)207, new Dictionary<string, object>());
            _stream.Write(message, 0, message.Length);

            var response = ReadResponse(out byte code);
            return response;
        }


    }
}
