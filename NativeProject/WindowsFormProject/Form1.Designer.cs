namespace WindowsFormProject
{
    partial class Form1
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.goBtn = new System.Windows.Forms.Button();
            this.prixLabel = new System.Windows.Forms.Label();
            this.maturity = new System.Windows.Forms.Label();
            this.r = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.S0 = new System.Windows.Forms.Label();
            this.sigma = new System.Windows.Forms.Label();
            this.nbSamples = new System.Windows.Forms.Label();
            this.strike = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.textBox4 = new System.Windows.Forms.TextBox();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.textBox6 = new System.Windows.Forms.TextBox();
            this.icLabel = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // goBtn
            // 
            this.goBtn.Location = new System.Drawing.Point(598, 248);
            this.goBtn.Name = "goBtn";
            this.goBtn.Size = new System.Drawing.Size(75, 23);
            this.goBtn.TabIndex = 0;
            this.goBtn.Text = "button1";
            this.goBtn.UseVisualStyleBackColor = true;
            this.goBtn.Click += new System.EventHandler(this.goBtn_Click);
            // 
            // prixLabel
            // 
            this.prixLabel.AutoSize = true;
            this.prixLabel.Location = new System.Drawing.Point(598, 229);
            this.prixLabel.Name = "prixLabel";
            this.prixLabel.Size = new System.Drawing.Size(31, 13);
            this.prixLabel.TabIndex = 1;
            this.prixLabel.Text = "Price";
            // 
            // maturity
            // 
            this.maturity.AutoSize = true;
            this.maturity.Location = new System.Drawing.Point(83, 147);
            this.maturity.Name = "maturity";
            this.maturity.Size = new System.Drawing.Size(44, 13);
            this.maturity.TabIndex = 2;
            this.maturity.Text = "Maturity";
            // 
            // r
            // 
            this.r.AutoSize = true;
            this.r.Location = new System.Drawing.Point(77, 206);
            this.r.Name = "r";
            this.r.Size = new System.Drawing.Size(10, 13);
            this.r.TabIndex = 3;
            this.r.Text = "r";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(83, 248);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(0, 13);
            this.label4.TabIndex = 4;
            // 
            // S0
            // 
            this.S0.AutoSize = true;
            this.S0.Location = new System.Drawing.Point(83, 87);
            this.S0.Name = "S0";
            this.S0.Size = new System.Drawing.Size(29, 13);
            this.S0.TabIndex = 5;
            this.S0.Text = "Spot";
            // 
            // sigma
            // 
            this.sigma.AutoSize = true;
            this.sigma.Location = new System.Drawing.Point(83, 248);
            this.sigma.Name = "sigma";
            this.sigma.Size = new System.Drawing.Size(34, 13);
            this.sigma.TabIndex = 6;
            this.sigma.Text = "sigma";
            // 
            // nbSamples
            // 
            this.nbSamples.AutoSize = true;
            this.nbSamples.Location = new System.Drawing.Point(80, 324);
            this.nbSamples.Name = "nbSamples";
            this.nbSamples.Size = new System.Drawing.Size(59, 13);
            this.nbSamples.TabIndex = 7;
            this.nbSamples.Text = "nbSamples";
            // 
            // strike
            // 
            this.strike.AutoSize = true;
            this.strike.Location = new System.Drawing.Point(77, 382);
            this.strike.Name = "strike";
            this.strike.Size = new System.Drawing.Size(14, 13);
            this.strike.TabIndex = 8;
            this.strike.Text = "K";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(80, 103);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 9;
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(80, 163);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(100, 20);
            this.textBox2.TabIndex = 10;
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(80, 225);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(100, 20);
            this.textBox3.TabIndex = 11;
            // 
            // textBox4
            // 
            this.textBox4.Location = new System.Drawing.Point(80, 264);
            this.textBox4.Name = "textBox4";
            this.textBox4.Size = new System.Drawing.Size(100, 20);
            this.textBox4.TabIndex = 12;
            // 
            // textBox5
            // 
            this.textBox5.Location = new System.Drawing.Point(80, 340);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(100, 20);
            this.textBox5.TabIndex = 13;
            // 
            // textBox6
            // 
            this.textBox6.Location = new System.Drawing.Point(83, 398);
            this.textBox6.Name = "textBox6";
            this.textBox6.Size = new System.Drawing.Size(100, 20);
            this.textBox6.TabIndex = 14;
            // 
            // icLabel
            // 
            this.icLabel.AutoSize = true;
            this.icLabel.Location = new System.Drawing.Point(652, 228);
            this.icLabel.Name = "icLabel";
            this.icLabel.Size = new System.Drawing.Size(17, 13);
            this.icLabel.TabIndex = 15;
            this.icLabel.Text = "IC";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.icLabel);
            this.Controls.Add(this.textBox6);
            this.Controls.Add(this.textBox5);
            this.Controls.Add(this.textBox4);
            this.Controls.Add(this.textBox3);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.strike);
            this.Controls.Add(this.nbSamples);
            this.Controls.Add(this.sigma);
            this.Controls.Add(this.S0);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.r);
            this.Controls.Add(this.maturity);
            this.Controls.Add(this.prixLabel);
            this.Controls.Add(this.goBtn);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button goBtn;
        private System.Windows.Forms.Label prixLabel;
        private System.Windows.Forms.Label maturity;
        private System.Windows.Forms.Label r;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label S0;
        private System.Windows.Forms.Label sigma;
        private System.Windows.Forms.Label nbSamples;
        private System.Windows.Forms.Label strike;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox4;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.TextBox textBox6;
        private System.Windows.Forms.Label icLabel;
    }
}

