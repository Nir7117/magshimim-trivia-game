using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Windows;

namespace TriviaClientGUI
{
    public partial class RoomLobbyWindow : Window
    {
        private string _roomName;
        private string _adminName;
        private int _roomId;
        private int time;

        private bool _keepListening = true;
        private Thread _listenerThread;
        private Thread _refreshThread;

        public RoomLobbyWindow(string roomName, string adminName, int roomId, int time)
        {
            InitializeComponent();
            _roomName = roomName;
            _adminName = adminName;
            _roomId = roomId;
            this.time = time;

            RoomNameLabel.Text = $"Room: {_roomName}";
            AdminLabel.Text = $"Admin: {_adminName}";

            if (App.CurrentUsername == _adminName)
            {
                StartGameButton.Visibility = Visibility.Visible;
                CloseRoomButton.Visibility = Visibility.Visible;
            }
            else
            {
                StartGameButton.Visibility = Visibility.Collapsed;
                CloseRoomButton.Visibility = Visibility.Collapsed;

                _listenerThread = new Thread(ListenForRoomClosure);
                _listenerThread.IsBackground = true;
                _listenerThread.Start();
            }

            _refreshThread = new Thread(StartAutoRefreshPlayers);
            _refreshThread.IsBackground = true;
            _refreshThread.Start();
        }

        private void ListenForRoomClosure()
        {
            try
            {
                while (_keepListening)
                {
                    if (!App.Comm.Stream.DataAvailable)
                    {
                        Thread.Sleep(100);
                        continue;
                    }

                    var response = App.Comm.ReadResponse(out byte code);

                    if ((code == 110 || code == 115) &&
                        response.ContainsKey("status") &&
                        Convert.ToInt32(response["status"]) == 1)
                    {
                        Dispatcher.Invoke(() =>
                        {
                            if (code == 110)
                            {
                                new MainMenuWindow().Show();
                                this.Close();

                            }
                               
                            else if (code == 115)
                            {
                                var msg = App.Comm.BuildMessage(216, new Dictionary<string, object>());
                                App.Comm.Stream.Write(msg, 0, msg.Length);
                                App.Comm.ReadResponse(out _);
                                new GameWindow().Show();
                            }

                            _keepListening = false;
                            this.Close();
                        });
                        break;
                    }

                    Thread.Sleep(50);
                }
            }
            catch (Exception ex)
            {
                Dispatcher.Invoke(() =>
                {
                    MessageBox.Show("Error in listener: " + ex.Message);
                    _keepListening = false;
                    new MainMenuWindow().Show();
                    this.Close();
                });
            }
        }

        private void StartAutoRefreshPlayers()
        {
            try
            {
                while (_keepListening)
                {
                    Dispatcher.Invoke(() => LoadPlayers());
                    Thread.Sleep(1000);
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

        private void LoadPlayers()
        {
            var payload = new Dictionary<string, object> { { "roomId", _roomId } };
            byte[] message = App.Comm.BuildMessage(204, payload);
            App.Comm.Stream.Write(message, 0, message.Length);

            var response = App.Comm.ReadResponse(out byte code);

            if (response != null && response.ContainsKey("PlayersInRoom"))
            {
                var rawPlayers = response["PlayersInRoom"];
                var playersList = new List<string>();

                if (rawPlayers is IEnumerable<object> enumerable)
                {
                    playersList = enumerable.Select(p => p.ToString()).ToList();
                }

                PlayersList.ItemsSource = playersList;
            }
            else
            {
                PlayersList.ItemsSource = new List<string>();
            }
        }

        private void BackToMenu_Click(object sender, RoutedEventArgs e)
        {
            LeaveRoomAndReturn();
        }

        private void RefreshButton_Click(object sender, RoutedEventArgs e)
        {
            LoadPlayers();
        }

        private void CloseRoom_Click(object sender, RoutedEventArgs e)
        {
            var payload = new Dictionary<string, object> { { "roomId", _roomId } };
            byte[] message = App.Comm.BuildMessage(110, payload);
            App.Comm.Stream.Write(message, 0, message.Length);

            var response = App.Comm.ReadResponse(out byte code);
            if (response != null && response.ContainsKey("status") &&
                Convert.ToInt32(response["status"]) == 1)
            {
                MessageBox.Show("Room closed.");
                _keepListening = false;
                new MainMenuWindow().Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("Failed to close room.");
            }
        }

        private void StartGame_Click(object sender, RoutedEventArgs e)
        {
            var payload = new Dictionary<string, object> { { "roomId", _roomId } };
            byte[] message = App.Comm.BuildMessage(111, payload);
            App.Comm.Stream.Write(message, 0, message.Length);

            var response = App.Comm.ReadResponse(out byte code);
            if (response != null && response.ContainsKey("status") &&
                Convert.ToInt32(response["status"]) == 1)
            {
                new GameWindow().Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("Failed to start game.");
            }
        }

        private void LeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            LeaveRoomAndReturn();
        }

        private void LeaveRoomAndReturn()
        {
            var payload = new Dictionary<string, object> { { "roomId", _roomId } };
            byte[] message = App.Comm.BuildMessage(212, payload);
            App.Comm.Stream.Write(message, 0, message.Length);

            var response = App.Comm.ReadResponse(out byte code);
            if (response != null && response.ContainsKey("status") &&
                Convert.ToInt32(response["status"]) == 1)
            {
                _keepListening = false;
                new MainMenuWindow().Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("Failed to leave room.");
            }
        }

        protected override void OnClosed(EventArgs e)
        {
            _keepListening = false;
            base.OnClosed(e);
        }
    }
}
