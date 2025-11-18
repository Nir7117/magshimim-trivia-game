using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;

namespace TriviaClientGUI
{
    public partial class PersonalStatsWindow : Window
    {
        public PersonalStatsWindow()
        {
            InitializeComponent();
            LoadStats();
        }

        private void LoadStats()
        {
            var response = App.Comm.GetStatistics();
            if (response == null || !response.ContainsKey("statistics"))
            {
                MessageBox.Show("Failed to retrieve statistics.");
                return;
            }

            if (response["statistics"] is object[] statsArray && statsArray.Length >= 4)
            {
                AvgTimeText.Text = statsArray[0] + " sec";
                CorrectAnswersText.Text ="" + statsArray[1];
                TotalAnswersText.Text = "" + statsArray[2];
                GamesPlayedText.Text = "" + statsArray[3];
            }
            else
            {
                MessageBox.Show("Statistics data not in expected format.");
            }
        }






        private void Back_Click(object sender, RoutedEventArgs e)
        {
            var win = new MainMenuWindow();
            win.Show();
            this.Close();        
        }
    }
}
