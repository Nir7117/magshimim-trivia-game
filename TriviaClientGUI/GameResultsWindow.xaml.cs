using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace TriviaClientGUI
{
    public partial class GameResultsWindow : Window
    {
        public GameResultsWindow()
        {
            InitializeComponent();
            LoadResults();
        }

        private async void LoadResults()
        {
            while (true)
            {
                try
                {
                    byte[] message = App.Comm.BuildMessage(216, new Dictionary<string, object>());
                    App.Comm.Stream.Write(message, 0, message.Length);

                    var response = App.Comm.ReadResponse(out byte code);

                    if (code == 117 && response.ContainsKey("results") && response["results"] is object[] rawResults && rawResults.Length > 0)
                    {
                        var players = new List<ResultEntry>();

                        foreach (var item in rawResults)
                        {
                            if (item is Dictionary<string, object> dict)
                            {
                                players.Add(new ResultEntry
                                {
                                    Username = dict["username"].ToString(),
                                    CorrectCount = Convert.ToInt32(dict["correctAnswerCount"]),
                                    AvgTime = $"{Math.Round(Convert.ToDouble(dict["averageAnswerTime"]), 2)} sec"
                                });
                            }
                        }


                        int maxCorrect = players.Max(p => p.CorrectCount);
                        var winners = players.Where(p => p.CorrectCount == maxCorrect).ToList();
                        var winner = winners.OrderBy(p => double.Parse(p.AvgTime.Replace(" sec", ""))).First();

                        ResultsListView.ItemsSource = players;


                        await Dispatcher.InvokeAsync(() =>
                        {
                            ResultsListView.UpdateLayout();
                            foreach (var item in ResultsListView.Items)
                            {
                                if (item is ResultEntry result && result.Username == winner.Username)
                                {
                                    if (ResultsListView.ItemContainerGenerator.ContainerFromItem(item) is ListViewItem container)
                                    {
                                        container.Foreground = Brushes.Gold;
                                        container.FontWeight = FontWeights.Bold;
                                    }
                                }
                            }
                        });

                        break;
                    }

                    await Task.Delay(1000);
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error loading results: " + ex.Message);
                    break;
                }
            }
        }



        private void Back_Click(object sender, RoutedEventArgs e)
        {
            new MainMenuWindow().Show();
            this.Close();
        }

        public class ResultEntry
        {
            public string Username { get; set; }
            public int CorrectCount { get; set; }
            public string AvgTime { get; set; }


            public int RawCorrect { get; set; }
            public double RawAvg { get; set; }
        }
    }
}
