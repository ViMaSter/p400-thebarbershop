namespace beardhair_headergenerator
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.textbox_variablename = new System.Windows.Forms.TextBox();
            this.label_count = new System.Windows.Forms.Label();
            this.button_generate = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.numericUpDown_amount = new System.Windows.Forms.NumericUpDown();
            this.label_feedback = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown_amount)).BeginInit();
            this.SuspendLayout();
            // 
            // textbox_variablename
            // 
            this.textbox_variablename.Location = new System.Drawing.Point(124, 30);
            this.textbox_variablename.Name = "textbox_variablename";
            this.textbox_variablename.Size = new System.Drawing.Size(149, 20);
            this.textbox_variablename.TabIndex = 0;
            this.textbox_variablename.Text = "BeardInfo{0}";
            this.textbox_variablename.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textbox_variablename_KeyDown);
            // 
            // label_count
            // 
            this.label_count.Location = new System.Drawing.Point(12, 33);
            this.label_count.Name = "label_count";
            this.label_count.Size = new System.Drawing.Size(106, 24);
            this.label_count.TabIndex = 1;
            this.label_count.Text = "Variable name";
            // 
            // button_generate
            // 
            this.button_generate.Location = new System.Drawing.Point(12, 75);
            this.button_generate.Name = "button_generate";
            this.button_generate.Size = new System.Drawing.Size(259, 23);
            this.button_generate.TabIndex = 2;
            this.button_generate.Text = "Generate header-file!";
            this.button_generate.UseVisualStyleBackColor = true;
            this.button_generate.Click += new System.EventHandler(this.button_generate_Click);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(106, 24);
            this.label1.TabIndex = 4;
            this.label1.Text = "Amount of variables";
            // 
            // numericUpDown_amount
            // 
            this.numericUpDown_amount.Location = new System.Drawing.Point(124, 7);
            this.numericUpDown_amount.Maximum = new decimal(new int[] {
            221599065,
            -1190558714,
            0,
            0});
            this.numericUpDown_amount.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDown_amount.Name = "numericUpDown_amount";
            this.numericUpDown_amount.Size = new System.Drawing.Size(147, 20);
            this.numericUpDown_amount.TabIndex = 5;
            this.numericUpDown_amount.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDown_amount.KeyDown += new System.Windows.Forms.KeyEventHandler(this.numericUpDown_amount_KeyDown);
            // 
            // label_feedback
            // 
            this.label_feedback.Location = new System.Drawing.Point(12, 56);
            this.label_feedback.Name = "label_feedback";
            this.label_feedback.Size = new System.Drawing.Size(257, 16);
            this.label_feedback.TabIndex = 6;
            this.label_feedback.Text = "Ready to write!";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 110);
            this.Controls.Add(this.label_feedback);
            this.Controls.Add(this.numericUpDown_amount);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.button_generate);
            this.Controls.Add(this.label_count);
            this.Controls.Add(this.textbox_variablename);
            this.Name = "Form1";
            this.Text = "Beardhair Headergenerator";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown_amount)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textbox_variablename;
        private System.Windows.Forms.Label label_count;
        private System.Windows.Forms.Button button_generate;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDown_amount;
        private System.Windows.Forms.Label label_feedback;
    }
}

