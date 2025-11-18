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
    /// <summary>
    /// Interaction logic for StatisticsWindow.xaml
    /// </summary>
    public partial class StatisticsWindow : Window
    {
        public StatisticsWindow()
        {
            InitializeComponent();
        }
        private void PersonalStats_Click(object sender, RoutedEventArgs e)
        {
            var win = new PersonalStatsWindow();
            win.Show();
            this.Close();
        }
        private void HighScores_Click(object sender, RoutedEventArgs e)
        {
            var win = new HighScoresWindow();
            win.Show();
            this.Close();
        }
        private void Back_Click(object sender, RoutedEventArgs e)
        {
            var win = new MainMenuWindow();
            win.Show();
            this.Close();
        }

    }
}
