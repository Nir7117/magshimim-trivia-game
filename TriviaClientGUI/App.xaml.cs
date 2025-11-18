using System;
using System.Collections.Generic;
using System.Windows;

namespace TriviaClientGUI
{
    public partial class App : Application
    {

        public static Communicator Comm = new Communicator();
        public static string Username = "";
        public static string CurrentUsername { get; set; }

    }
}
