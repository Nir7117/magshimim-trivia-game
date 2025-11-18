using System;
using System.Collections.Generic;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
using System.Linq;

namespace TriviaClientGUI
{
    public partial class GameWindow : Window
    {
        private int totalQuestions = 0;
        private int correctAnswers = 0;
        private int currentAnswerId = -1;
        private int questionIndex = 0;
        private Timer questionTimer;
        private int secondsLeft = 10;
        private int answerTimeout = 10;

        public GameWindow()
        {
            InitializeComponent();

            try
            {
                LoadNextQuestion();
            }
            catch (Exception ex)
            {
                MessageBox.Show(">>> GameWindow Exception: " + ex.Message);
            }
        }

        private void LoadNextQuestion()
        {
            try
            {
                byte[] msg = App.Comm.BuildMessage(214, new Dictionary<string, object>());
                App.Comm.Stream.Write(msg, 0, msg.Length);

                var response = App.Comm.ReadResponse(out byte code);
                if (code != 114 || !response.ContainsKey("question") || !response.ContainsKey("answers"))
                {
                    MessageBox.Show("Failed to get valid question from server.");
                    return;
                }

                answerTimeout = Convert.ToInt32(response["answerTimeout"]);
                totalQuestions = Convert.ToInt32(response["questionCount"]);

                QuestionText.Text = response["question"].ToString();

                var rawAnswers = response["answers"];
                List<string> answers = new List<string>();

                if (rawAnswers is Dictionary<string, object> dict)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        string key = i.ToString();
                        answers.Add(dict.ContainsKey(key) ? dict[key].ToString() : "N/A");
                    }
                }
                else return;

                Answer1.Content = answers[0];
                Answer2.Content = answers[1];
                Answer3.Content = answers[2];
                Answer4.Content = answers[3];

                currentAnswerId = -1;
                secondsLeft = answerTimeout;
                TimerText.Text = $"Time: {secondsLeft}";
                StartTimer();

                questionIndex++;
                CounterText.Text = $"Question: {questionIndex}";
                RemainingText.Text = $"Remaining: {totalQuestions - questionIndex}";
                CorrectSoFarText.Text = $"Correct: {correctAnswers}";
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error in LoadNextQuestion: " + ex.Message);
            }
        }

        private void Answer_Click(object sender, RoutedEventArgs e)
        {
            if (currentAnswerId != -1) return;

            Button clicked = sender as Button;
            if (clicked == null) return;

            if (clicked == Answer1) currentAnswerId = 0;
            else if (clicked == Answer2) currentAnswerId = 1;
            else if (clicked == Answer3) currentAnswerId = 2;
            else if (clicked == Answer4) currentAnswerId = 3;

            SendAnswer();
        }

        private void StartTimer()
        {
            questionTimer = new Timer(1000);
            questionTimer.Elapsed += TimerTick;
            questionTimer.Start();
        }

        private void TimerTick(object sender, ElapsedEventArgs e)
        {
            Dispatcher.Invoke(() =>
            {
                secondsLeft--;
                TimerText.Text = $"Time: {secondsLeft}";
                if (secondsLeft <= 0)
                {
                    questionTimer.Stop();
                    if (currentAnswerId == -1)
                    {
                        currentAnswerId = 999;
                        SendAnswer();
                    }
                }
            });
        }

        private void SendAnswer()
        {
            try
            {
                questionTimer?.Stop();

                var data = new Dictionary<string, object>
                {
                    { "answerId", currentAnswerId }
                };

                byte[] msg = App.Comm.BuildMessage(215, data);
                App.Comm.Stream.Write(msg, 0, msg.Length);
                var response = App.Comm.ReadResponse(out byte code);

                if (response.ContainsKey("isCorrect"))
                {

                    bool isCorrect = response["isCorrect"].ToString() == "True";
                    if (isCorrect) correctAnswers++;
                    CorrectSoFarText.Text = $"Correct: {correctAnswers}";
                }
                else
                {
                    MessageBox.Show(">>> response does not contain isCorrect!");
                }


                var timer = new System.Timers.Timer(1000);
                timer.Elapsed += (s, e) =>
                {
                    timer.Stop();
                    Dispatcher.Invoke(() =>
                    {
                        if (questionIndex >= totalQuestions)
                        {
                            new GameResultsWindow().Show();
                            this.Close();
                        }
                        else
                        {
                            LoadNextQuestion();
                        }
                    });
                };
                timer.Start();

            }
            catch (Exception ex)
            {
                MessageBox.Show("Error in SendAnswer: " + ex.Message);
            }
        }
    }
}
