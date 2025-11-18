using System;
using System.Collections.Generic;
using System.Windows;

namespace TriviaClientGUI
{
    public partial class CreateRoomWindow : Window
    {
        private Communicator _comm;
        private string _username;

        public CreateRoomWindow(Communicator comm, string username)
        {
            InitializeComponent();
            _comm = comm;
            _username = username;
        }

        private void CreateButton_Click(object sender, RoutedEventArgs e)
        {
            string roomName = RoomNameBox.Text;
            int maxUsers = int.TryParse(MaxUsersBox.Text, out var max) ? max : 0;
            int questions = int.TryParse(QuestionsBox.Text, out var q) ? q : 0;
            int timeout = int.TryParse(TimeoutBox.Text, out var t) ? t : 0;

            if (maxUsers <= 0 || questions <= 0 || timeout <= 0 || string.IsNullOrWhiteSpace(roomName))
            {
                MessageBox.Show("Please fill all fields correctly.");
                return;
            }

            var success = _comm.CreateRoomWithFullResponse(roomName, maxUsers, questions, timeout, out int roomId, out string adminName);

            if (success)
            {
                MessageBox.Show("Room created successfully!");
                var lobby = new RoomLobbyWindow(roomName, adminName, roomId,timeout); 
                lobby.Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("Failed to create room.");
            }
        }

    }
}
