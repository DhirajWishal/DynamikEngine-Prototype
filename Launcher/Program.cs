/*
 * Main Launcher File.
 * 
 * Author:  Helitha Senaratne
 * Project: Dynamik Engine
 * Date:    --/--/----
 * IDE:     MS Visual Studio Community 2019
 * 
 * Program.cs file.
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Launcher
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
