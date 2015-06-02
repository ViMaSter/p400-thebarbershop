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
            this.SuspendLayout();
            // 
            // ninepatch_generator
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Name = "ninepatch_generator";
            this.Text = "9 patch generator";
            this.DragDrop += new System.Windows.Forms.DragEventHandler(this.ninepatch_generator_DragDrop);
            this.DragEnter += new System.Windows.Forms.DragEventHandler(this.ninepatch_generator_DragEnter);
            this.ResumeLayout(false);

        }

        #endregion

    }
}

