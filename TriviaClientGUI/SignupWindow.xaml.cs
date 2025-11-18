using System.Windows;

namespace TriviaClientGUI
{
    public partial class SignupWindow : Window
    {
        public SignupWindow()
        {
            InitializeComponent();
        }

        private void SignupButton_Click(object sender, RoutedEventArgs e)
        {
            string username = UsernameBox.Text;
            string password = PasswordBox.Password;
            string email = EmailBox.Text;

            bool success = App.Comm.SendSignup(username, password, email);

            if (success)
            {
                MessageBox.Show("Signup successful! Please login.");

                var start = new WelcomeWindow();
                start.Show();
                this.Close();
            }
            else
            {
                MessageBox.Show("Signup failed. Please try again.");
            }
        }

        private void BackButton_Click(object sender, RoutedEventArgs e)
        {
            var start = new WelcomeWindow();
            start.Show();
            this.Close();
        }
    }
}
