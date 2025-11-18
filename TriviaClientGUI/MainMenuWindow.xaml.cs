using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;


namespace TriviaClientGUI
{
    public partial class MainMenuWindow : Window
    {
        public MainMenuWindow()
        {
            InitializeComponent();
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            var win = new CreateRoomWindow(App.Comm, App.Username);
            win.Show();
            this.Close();
        }

        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            var win = new JoinRoomWindow();
            win.Show();
            this.Close();
        }

        private void Statistics_Click(object sender, RoutedEventArgs e)
        {
            var win = new StatisticsWindow();
            win.Show();
            this.Close();
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }
        private void back_Click(object sender, RoutedEventArgs e)
        {
            var win = new WelcomeWindow();
            win.Show();
            this.Close();
        }

    }
}
