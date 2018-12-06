using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using Wrapper;

namespace WindowsFormProject
{
    static class Program
    {
        /// <summary>
        /// Point d'entrée principal de l'application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            string newPath = string.Concat(Environment.GetEnvironmentVariable("PATH"), ";", Properties.Settings.Default.AdditionalPaths);
            Environment.SetEnvironmentVariable("PATH", newPath, EnvironmentVariableTarget.Process);
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new Form1());
            WrapperClass wc = new WrapperClass();
            wc.getPriceCallEuro(5000, 3.0, 100, 110, 0.4, 0.01);
        }
    }
}
