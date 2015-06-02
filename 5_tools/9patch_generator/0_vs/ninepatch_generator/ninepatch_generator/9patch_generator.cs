using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ninepatch_generator
{
    public partial class ninepatch_generator : Form
    {
        StretchableImage img;
        public ninepatch_generator()
        {
            InitializeComponent();
        }

        private void ninepatch_generator_DragDrop(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent(DataFormats.FileDrop))
            {
                string[] filePaths = (string[])(e.Data.GetData(DataFormats.FileDrop));
                foreach (string fileLoc in filePaths)
                {
                    if (File.Exists(fileLoc))
                    {
                        img = new StretchableImage(new System.Drawing.Bitmap(fileLoc));
                    }
                }
            }
        }

        private void ninepatch_generator_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Copy;
            Console.WriteLine(e.Data);
        }        

    }
}
