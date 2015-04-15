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
            this.btn_load = new System.Windows.Forms.Button();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.btn_save = new System.Windows.Forms.Button();
            this.label_found_instances = new System.Windows.Forms.Label();
            this.textbox_seperator = new System.Windows.Forms.TextBox();
            this.label_seperator = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btn_load
            // 
            this.btn_load.Location = new System.Drawing.Point(12, 12);
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
            this.label_found_instances.Location = new System.Drawing.Point(12, 76);
            this.label_found_instances.Name = "label_found_instances";
            this.label_found_instances.Size = new System.Drawing.Size(249, 23);
            this.label_found_instances.TabIndex = 2;
            this.label_found_instances.Text = "Ready to import!";
            // 
            // textbox_seperator
            // 
            this.textbox_seperator.Location = new System.Drawing.Point(197, 41);
            this.textbox_seperator.Name = "textbox_seperator";
            this.textbox_seperator.Size = new System.Drawing.Size(64, 20);
            this.textbox_seperator.TabIndex = 3;
            this.textbox_seperator.Text = ",";
            // 
            // label_seperator
            // 
            this.label_seperator.Location = new System.Drawing.Point(12, 44);
            this.label_seperator.Name = "label_seperator";
            this.label_seperator.Size = new System.Drawing.Size(82, 20);
            this.label_seperator.TabIndex = 4;
            this.label_seperator.Text = "Seperator:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(273, 331);
            this.Controls.Add(this.label_seperator);
            this.Controls.Add(this.textbox_seperator);
            this.Controls.Add(this.label_found_instances);
            this.Controls.Add(this.btn_save);
            this.Controls.Add(this.btn_load);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btn_load;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.Button btn_save;
        private System.Windows.Forms.Label label_found_instances;
        private System.Windows.Forms.TextBox textbox_seperator;
        private System.Windows.Forms.Label label_seperator;

    }
}

