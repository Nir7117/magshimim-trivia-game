using System;
using System.Windows;

namespace TriviaClientGUI
{
    public partial class LoginWindow : Window
    {
        public LoginWindow()
        {
            InitializeComponent();
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            var start = new WelcomeWindow();
            start.Show();
            this.Close();
        }

        private void LoginButton_Click(object sender, RoutedEventArgs e)
        {
            string username = UsernameBox.Text;
            string password = PasswordBox.Password;

            try
            {
                bool success = App.Comm.SendLogin(username, password);

                if (success)
                {
                    MessageBox.Show("Login succeeded!");
                    App.CurrentUsername = username;

                    var start = new MainMenuWindow();
                    start.Show();
                    this.Close();
                }
                else
                {
                    MessageBox.Show("Login failed.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Connection error: " + ex.Message);
            }
        }
    }
}
