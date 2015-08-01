namespace dae_to_csv
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.btn_load = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.btn_save = new System.Windows.Forms.Button();
            this.label_found_instances = new System.Windows.Forms.Label();
            this.textbox_seperator = new System.Windows.Forms.TextBox();
            this.label_seperator = new System.Windows.Forms.Label();
            this.checkBox_handednessFix = new System.Windows.Forms.CheckBox();
            this.backgroundWorker_regex = new System.ComponentModel.BackgroundWorker();
            this.progressBar_regex = new System.Windows.Forms.ProgressBar();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.version = new System.Windows.Forms.ComboBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // btn_load
            // 
            this.btn_load.Location = new System.Drawing.Point(12, 159);
            this.btn_load.Name = "btn_load";
            this.btn_load.Size = new System.Drawing.Size(249, 23);
            this.btn_load.TabIndex = 0;
            this.btn_load.Text = "Open .dae";
            this.btn_load.UseVisualStyleBackColor = true;
            this.btn_load.Click += new System.EventHandler(this.btn_load_Click);
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // btn_save
            // 
            this.btn_save.Location = new System.Drawing.Point(12, 296);
            this.btn_save.Name = "btn_save";
            this.btn_save.Size = new System.Drawing.Size(249, 23);
            this.btn_save.TabIndex = 1;
            this.btn_save.Text = "Save .csv";
            this.btn_save.UseVisualStyleBackColor = true;
            this.btn_save.Click += new System.EventHandler(this.btn_save_Click);
            // 
            // label_found_instances
            // 
            this.label_found_instances.Anchor = System.Windows.Forms.AnchorStyles.Left;
            this.label_found_instances.Location = new System.Drawing.Point(12, 270);
            this.label_found_instances.Name = "label_found_instances";
            this.label_found_instances.Size = new System.Drawing.Size(249, 23);
            this.label_found_instances.TabIndex = 2;
            this.label_found_instances.Text = "Ready to import!";
            // 
            // textbox_seperator
            // 
            this.textbox_seperator.Location = new System.Drawing.Point(179, 43);
            this.textbox_seperator.Name = "textbox_seperator";
            this.textbox_seperator.Size = new System.Drawing.Size(64, 20);
            this.textbox_seperator.TabIndex = 3;
            this.textbox_seperator.Text = ",";
            // 
            // label_seperator
            // 
            this.label_seperator.Location = new System.Drawing.Point(3, 46);
            this.label_seperator.Name = "label_seperator";
            this.label_seperator.Size = new System.Drawing.Size(82, 20);
            this.label_seperator.TabIndex = 4;
            this.label_seperator.Text = "Seperator:";
            // 
            // checkBox_handednessFix
            // 
            this.checkBox_handednessFix.Location = new System.Drawing.Point(6, 19);
            this.checkBox_handednessFix.Name = "checkBox_handednessFix";
            this.checkBox_handednessFix.Size = new System.Drawing.Size(237, 24);
            this.checkBox_handednessFix.TabIndex = 5;
            this.checkBox_handednessFix.Text = "Unreal handedness fix";
            this.checkBox_handednessFix.UseVisualStyleBackColor = true;
            // 
            // backgroundWorker_regex
            // 
            this.backgroundWorker_regex.WorkerReportsProgress = true;
            this.backgroundWorker_regex.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker_regex_DoWork);
            this.backgroundWorker_regex.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.backgroundWorker_regex_ProgressChanged);
            this.backgroundWorker_regex.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.backgroundWorker_regex_RunWorkerCompleted);
            // 
            // progressBar_regex
            // 
            this.progressBar_regex.Location = new System.Drawing.Point(12, 244);
            this.progressBar_regex.Name = "progressBar_regex";
            this.progressBar_regex.Size = new System.Drawing.Size(249, 23);
            this.progressBar_regex.TabIndex = 6;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.version);
            this.groupBox1.Controls.Add(this.checkBox_handednessFix);
            this.groupBox1.Controls.Add(this.textbox_seperator);
            this.groupBox1.Controls.Add(this.label_seperator);
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(249, 141);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Settings";
            // 
            // version
            // 
            this.version.FormattingEnabled = true;
            this.version.Items.AddRange(new object[] {
            "3DS Max 2015",
            "3DS Max 2016"});
            this.version.Location = new System.Drawing.Point(6, 69);
            this.version.Name = "version";
            this.version.Size = new System.Drawing.Size(237, 21);
            this.version.TabIndex = 6;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(273, 331);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.progressBar_regex);
            this.Controls.Add(this.label_found_instances);
            this.Controls.Add(this.btn_save);
            this.Controls.Add(this.btn_load);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "UE4 .DAE to .CSV converter";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btn_load;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button btn_save;
        private System.Windows.Forms.Label label_found_instances;
        private System.Windows.Forms.TextBox textbox_seperator;
        private System.Windows.Forms.Label label_seperator;
        private System.Windows.Forms.CheckBox checkBox_handednessFix;
        private System.ComponentModel.BackgroundWorker backgroundWorker_regex;
        private System.Windows.Forms.ProgressBar progressBar_regex;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox version;

    }
}

