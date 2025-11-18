using System;
using System.Collections.Generic;
using System.Windows;

namespace TriviaClientGUI
{
    public partial class HighScoresWindow : Window
    {
        public HighScoresWindow()
        {
            InitializeComponent();
            LoadHighScores();
        }

        private void LoadHighScores()
        {
            var response = App.Comm.GetHighScores();
            if (response == null || !response.ContainsKey("highscores"))
            {
                MessageBox.Show("Failed to retrieve high scores.");
                return;
            }

            if (response["highscores"] is object[] scoresArray)
            {
                var displayList = new List<HighScoreItem>();

                foreach (var item in scoresArray)
                {
                    string line = item.ToString(); 

                    string username = "";
                    int score = 0;

                    try
                    {
                        
                        var parts = line.Split(',');
                        foreach (var part in parts)
                        {
                            if (part.Contains("Username:"))
                                username = part.Split(':')[1].Trim();
                            else if (part.Contains("Score:"))
                                score = int.Parse(part.Split(':')[1].Trim());
                        }

                        displayList.Add(new HighScoreItem
                        {
                            Username = username,
                            Score = score
                        });
                    }
                    catch
                    {
                        
                        continue;
                    }
                }

                HighScoresList.ItemsSource = displayList;
            }
            else
            {
                MessageBox.Show("Invalid highscores format.");
            }
        }


        private void Back_Click(object sender, RoutedEventArgs e)
        {
            var win = new MainMenuWindow();
            win.Show();
            this.Close();
        }

        public class HighScoreItem
        {
            public string Username { get; set; }
            public int Score { get; set; }
        }
    }
}
