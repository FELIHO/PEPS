using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Wrapper;

namespace WindowsFormProject
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void goBtn_Click(object sender, EventArgs e)
        {
            // Retrieve the values of the parameters in the TextBoxes
            WrapperClass wc = new WrapperClass();
            wc.getPriceCallEuro(int.Parse(nbSamples.Text), double.Parse(maturity.Text), double.Parse(S0.Text), double.Parse(strike.Text), double.Parse(sigma.Text), double.Parse(r.Text));
            prixLabel.Text = wc.getPrice().ToString();
            icLabel.Text = wc.getIC().ToString();
        }
    }
}
