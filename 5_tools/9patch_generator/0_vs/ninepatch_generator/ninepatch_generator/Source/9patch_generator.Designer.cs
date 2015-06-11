namespace ninepatch_generator
{
    partial class ninepatch_generator
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
            this.components = new System.ComponentModel.Container();
            this.pictureBoxPreview = new System.Windows.Forms.PictureBox();
            this.labelWidth = new System.Windows.Forms.Label();
            this.labelHeight = new System.Windows.Forms.Label();
            this.numericHeight = new System.Windows.Forms.NumericUpDown();
            this.buttonImport = new System.Windows.Forms.Button();
            this.buttonExport = new System.Windows.Forms.Button();
            this.numericWidth = new System.Windows.Forms.NumericUpDown();
            this.numericWidthTooSmall_error = new System.Windows.Forms.ErrorProvider(this.components);
            this.numericHeightTooSmall_error = new System.Windows.Forms.ErrorProvider(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPreview)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericWidth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericWidthTooSmall_error)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericHeightTooSmall_error)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBoxPreview
            // 
            this.pictureBoxPreview.BackgroundImage = global::ninepatch_generator.Properties.Resources.background;
            this.pictureBoxPreview.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.pictureBoxPreview.Location = new System.Drawing.Point(13, 13);
            this.pictureBoxPreview.Name = "pictureBoxPreview";
            this.pictureBoxPreview.Size = new System.Drawing.Size(1000, 657);
            this.pictureBoxPreview.TabIndex = 0;
            this.pictureBoxPreview.TabStop = false;
            // 
            // labelWidth
            // 
            this.labelWidth.AutoSize = true;
            this.labelWidth.Location = new System.Drawing.Point(1019, 53);
            this.labelWidth.Name = "labelWidth";
            this.labelWidth.Size = new System.Drawing.Size(38, 13);
            this.labelWidth.TabIndex = 2;
            this.labelWidth.Text = "Width:";
            // 
            // labelHeight
            // 
            this.labelHeight.AutoSize = true;
            this.labelHeight.Location = new System.Drawing.Point(1019, 79);
            this.labelHeight.Name = "labelHeight";
            this.labelHeight.Size = new System.Drawing.Size(41, 13);
            this.labelHeight.TabIndex = 4;
            this.labelHeight.Text = "Height:";
            // 
            // numericHeight
            // 
            this.numericHeight.Enabled = false;
            this.numericHeight.Location = new System.Drawing.Point(1132, 77);
            this.numericHeight.Maximum = new decimal(new int[] {
            -1,
            -1,
            -1,
            0});
            this.numericHeight.Name = "numericHeight";
            this.numericHeight.Size = new System.Drawing.Size(120, 20);
            this.numericHeight.TabIndex = 3;
            this.numericHeight.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericHeight.ValueChanged += new System.EventHandler(this.numericHeight_ValueChanged);
            // 
            // buttonImport
            // 
            this.buttonImport.Location = new System.Drawing.Point(1022, 13);
            this.buttonImport.Name = "buttonImport";
            this.buttonImport.Size = new System.Drawing.Size(230, 23);
            this.buttonImport.TabIndex = 5;
            this.buttonImport.Text = "Import image";
            this.buttonImport.UseVisualStyleBackColor = true;
            this.buttonImport.Click += new System.EventHandler(this.buttonImport_Click);
            // 
            // buttonExport
            // 
            this.buttonExport.Location = new System.Drawing.Point(1022, 119);
            this.buttonExport.Name = "buttonExport";
            this.buttonExport.Size = new System.Drawing.Size(230, 23);
            this.buttonExport.TabIndex = 6;
            this.buttonExport.Text = "Export image";
            this.buttonExport.UseVisualStyleBackColor = true;
            this.buttonExport.Click += new System.EventHandler(this.buttonExport_Click);
            // 
            // numericWidth
            // 
            this.numericWidth.Enabled = false;
            this.numericWidth.Location = new System.Drawing.Point(1132, 51);
            this.numericWidth.Maximum = new decimal(new int[] {
            -1,
            -1,
            -1,
            0});
            this.numericWidth.Name = "numericWidth";
            this.numericWidth.Size = new System.Drawing.Size(120, 20);
            this.numericWidth.TabIndex = 1;
            this.numericWidth.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericWidth.ValueChanged += new System.EventHandler(this.numericWidth_ValueChanged);
            // 
            // numericWidthTooSmall_error
            // 
            this.numericWidthTooSmall_error.BlinkRate = 1000;
            this.numericWidthTooSmall_error.ContainerControl = this;
            this.numericWidthTooSmall_error.RightToLeft = true;
            // 
            // numericHeightTooSmall_error
            // 
            this.numericHeightTooSmall_error.BlinkRate = 1000;
            this.numericHeightTooSmall_error.ContainerControl = this;
            this.numericHeightTooSmall_error.RightToLeft = true;
            // 
            // ninepatch_generator
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1264, 682);
            this.Controls.Add(this.buttonExport);
            this.Controls.Add(this.buttonImport);
            this.Controls.Add(this.labelHeight);
            this.Controls.Add(this.numericHeight);
            this.Controls.Add(this.labelWidth);
            this.Controls.Add(this.numericWidth);
            this.Controls.Add(this.pictureBoxPreview);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "ninepatch_generator";
            this.Text = "9 patch generator";
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.ninepatch_generator_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.ninepatch_generator_DragEnter);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPreview)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericWidth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericWidthTooSmall_error)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericHeightTooSmall_error)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBoxPreview;
        private System.Windows.Forms.Label labelWidth;
        private System.Windows.Forms.Label labelHeight;
        private System.Windows.Forms.NumericUpDown numericHeight;
        private System.Windows.Forms.Button buttonImport;
        private System.Windows.Forms.Button buttonExport;
        private System.Windows.Forms.NumericUpDown numericWidth;
        private System.Windows.Forms.ErrorProvider numericWidthTooSmall_error;
        private System.Windows.Forms.ErrorProvider numericHeightTooSmall_error;

    }
}

