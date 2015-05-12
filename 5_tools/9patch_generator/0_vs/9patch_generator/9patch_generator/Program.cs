using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace csv_concatenator
{
    class Program
    {
        [STAThreadAttribute]
        static void Main(string[] args)
        {
            Console.WriteLine("Loading .PNG-files...");

            if (args.Length < 1)
            {
                OpenFileDialog openFileDialog1 = new OpenFileDialog();
                openFileDialog1.Multiselect = true;
                openFileDialog1.Filter = ".PNG-files (*.png)|*.png";
                DialogResult dialogResult = openFileDialog1.ShowDialog();
                if (dialogResult == DialogResult.OK) // Test result.
                {
                    args = openFileDialog1.FileNames;
                }
            }

            foreach (string pathToFile in args)
            {
                Bitmap src = new Bitmap(pathToFile);
                Bitmap target = new Bitmap(src.Width - 2, src.Height - 2);

                for (int y = 0; y < target.Height; y++)
                {
                    for (int x = 0; x < target.Width; x++)
                    {
                        target.SetPixel(x, y, src.GetPixel(x+1, y+1));
                    }
                }

                target.Save(pathToFile.Replace(".png", ".9patch.png"));
            }

            Console.WriteLine("");
            Console.WriteLine("FERTIG!");
            Console.WriteLine("Eingabetaste zum Schliessen druecken!");
            Console.Read();
        }
    }
}
