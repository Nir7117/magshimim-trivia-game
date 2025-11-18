using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Windows;

namespace TriviaClientGUI
{
    public partial class JoinRoomWindow : Window
    {
        private Dictionary<string, int> _roomNameToId = new Dictionary<string, int>();
        private Thread _refreshRoomsThread;
        private bool _keepRefreshing = true;

        public JoinRoomWindow()
        {
            InitializeComponent();
            LoadRooms();

            _refreshRoomsThread = new Thread(AutoRefreshRooms);
            _refreshRoomsThread.IsBackground = true;
            _refreshRoomsThread.Start();
        }

        private void AutoRefreshRooms()
        {
            try
            {
                while (_keepRefreshing)
                {
                    Dispatcher.Invoke(() =>
                    {
                        LoadRooms();
                    });

                    Thread.Sleep(3000); 
                }
            }
            catch (Exception ex)
            {
                Dispatcher.Invoke(() =>
                {
                    MessageBox.Show("Auto-refresh error: " + ex.Message);
                });
            }
        }

        private void LoadRooms()
        {
            byte[] message = App.Comm.BuildMessage(203, new Dictionary<string, object>());
            App.Comm.Stream.Write(message, 0, message.Length);

            var response = App.Comm.ReadResponse(out byte code);

            if (response != null && response.ContainsKey("Rooms"))
            {
                var rooms = response["Rooms"] as IEnumerable<object>;
                var newList = new List<string>();
                _roomNameToId.Clear();

                foreach (var roomObj in rooms)
                {
                    var dict = roomObj as Dictionary<string, object>;
                    if (dict != null)
                    {
                        string name = dict["name"].ToString();
                        int id = Convert.ToInt32(dict["id"]);
                        _roomNameToId[name] = id;
                        newList.Add(name);
                    }
                }

                RoomsList.ItemsSource = newList;
            }
            else
            {
                RoomsList.ItemsSource = new List<string>();
            }
        }

        private void JoinButton_Click(object sender, RoutedEventArgs e)
        {
            if (RoomsList.SelectedItem == null)
            {
                MessageBox.Show("Please select a room.");
                return;
            }

            string selectedRoom = RoomsList.SelectedItem.ToString();
            int roomId = _roomNameToId[selectedRoom];

            var payload = new Dictionary<string, object>
            {
                { "roomId", roomId }
            };

            byte[] message = App.Comm.BuildMessage(205, payload);
            App.Comm.Stream.Write(message, 0, message.Length);

            var response = App.Comm.ReadResponse(out byte code);

            if (response != null && response.ContainsKey("status") && Convert.ToInt32(response["status"]) == 1)
            {
                string adminName = response.ContainsKey("admin") ? response["admin"].ToString() : "unknown";
                _keepRefreshing = false;
                var lobby = new RoomLobbyWindow(selectedRoom, adminName, roomId, 55);
                lobby.Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("Failed to join room.");
            }
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            _keepRefreshing = false;
            new MainMenuWindow().Show();
            this.Close();
        }
    }
}
