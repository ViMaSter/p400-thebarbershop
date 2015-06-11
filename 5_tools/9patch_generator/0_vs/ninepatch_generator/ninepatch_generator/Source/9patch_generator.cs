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
        StretchableImage CurrentImage;
        string Filename;
        Size DestinationSize
        {
            get
            {            
                return new Size((int)numericWidth.Value, (int)numericHeight.Value);
            }
            set
            {
                numericHeight.Value = value.Height;
                numericWidth.Value = value.Width;
            }
        }
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
                        LoadImage(Path.GetFileNameWithoutExtension(fileLoc), new System.Drawing.Bitmap(fileLoc));
                    }
                }
            }
        }

        private void LoadImage(string filename, Bitmap bitmap, bool supressWarning = false)
        {
            // "Overwrite current file?"-warning
            if (CurrentImage != null && !supressWarning)
            {
                if (MessageBox.Show("Do you want to discard the currently loaded image?", "Overwrite current file", MessageBoxButtons.OKCancel, MessageBoxIcon.Warning) == System.Windows.Forms.DialogResult.Cancel)
                {
                    return;
                }
            }

            // Validity check
            StretchableImage NewImage = new StretchableImage(bitmap);
            if (!NewImage.IsValid())
            {
                DialogResult res;
                res = MessageBox.Show("The image you provided is not a valid 9-patch image!", "Error loading file", MessageBoxButtons.RetryCancel, MessageBoxIcon.Error);

                if (res == DialogResult.Retry)
                {
                    BrowseForFile(true);
                }

                return;
            }

            numericWidth.Enabled = true;
            numericHeight.Enabled = true;

            // Applying and initial rendering
            CurrentImage = NewImage;
            Filename = filename;
            DestinationSize = CurrentImage.StaticAreaTotal + CurrentImage.DynamicAreaTotal;
            pictureBoxPreview.Image = CurrentImage.GenerateImage(DestinationSize);
        }

        private void BrowseForFile(bool supressWarning = false)
        {
            // Prepare file dialog
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Supported image files (*.BMP, *.GIF, *.JPG, *.PNG, *.TIFF, *.EXIF)|*.BMP;*.GIF;*.JPG;*.PNG;*.TIFF;*.EXIF";
            ofd.Title = "Load 9-patch image...";

            // Open image dialog
            if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
                LoadImage(Path.GetFileNameWithoutExtension(ofd.FileName), new System.Drawing.Bitmap(ofd.FileName), supressWarning);
            }
        }

        private void ninepatch_generator_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Copy;
            Console.WriteLine(e.Data);
        }

        private void buttonImport_Click(object sender, EventArgs e)
        {
            BrowseForFile();
        }

        private void buttonExport_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.DefaultExt = ".png";
            sfd.Filter = "PNG file|*.PNG|BMP file|*.BMP|EMF file|*.EMF|EXIF file|*.EXIF|GIF file|*.GIF|ICON file|*.ICO|JPEG file|*.JPG|TIFF file|*.TIFF|WMF file|*.WMF";
            sfd.FileName = string.Format("{0}_{1}x{2}", Filename, DestinationSize.Width, DestinationSize.Height);
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                System.Drawing.Imaging.ImageFormat desiredImageFormat;
                switch (sfd.FilterIndex) {
                    case 1:
                        desiredImageFormat = System.Drawing.Imaging.ImageFormat.Png;
                        break;
                    case 2:
                        desiredImageFormat = System.Drawing.Imaging.ImageFormat.Bmp;
                        break;
                    case 3:
                        desiredImageFormat = System.Drawing.Imaging.ImageFormat.Emf;
                        break;
                    case 4:
                        desiredImageFormat = System.Drawing.Imaging.ImageFormat.Exif;
                        break;
                    case 5:
                        desiredImageFormat = System.Drawing.Imaging.ImageFormat.Gif;
                        break;
                    case 6:
                        desiredImageFormat = System.Drawing.Imaging.ImageFormat.Icon;
                        break;
                    case 7:
                        desiredImageFormat = System.Drawing.Imaging.ImageFormat.Jpeg;
                        break;
                    case 8:
                        desiredImageFormat = System.Drawing.Imaging.ImageFormat.Tiff;
                        break;
                    case 9:
                        desiredImageFormat = System.Drawing.Imaging.ImageFormat.Wmf;
                        break;
                    default:
                        desiredImageFormat = System.Drawing.Imaging.ImageFormat.Png;
                        break;
                }
            
                CurrentImage.GenerateImage(DestinationSize).Save(sfd.FileName, desiredImageFormat);
            }
        }

        private void numericWidth_ValueChanged(object sender, EventArgs e)
        {
            try
            {
                pictureBoxPreview.Image = CurrentImage.GenerateImage(DestinationSize);
                numericWidthTooSmall_error.Clear();
                buttonExport.Enabled = true;
            }
            catch (Exceptions.StretchableImage_TooSmallException ex)
            {
                numericWidthTooSmall_error.SetError(numericWidth, "Value too small! Minimum required width: " + CurrentImage.StaticAreaTotal.Width);
                buttonExport.Enabled = false;
            }
            catch (Exceptions.StretchableImage_NullException ex)
            {
            }
        }

        private void numericHeight_ValueChanged(object sender, EventArgs e)
        {
            try
            {
                pictureBoxPreview.Image = CurrentImage.GenerateImage(DestinationSize);
                numericHeightTooSmall_error.Clear();
                buttonExport.Enabled = true;
            }
            catch (Exceptions.StretchableImage_TooSmallException ex)
            {
                numericHeightTooSmall_error.SetError(numericHeight, "Value too small! Minimum required height: " + CurrentImage.StaticAreaTotal.Height);
                buttonExport.Enabled = false;
            }
            catch (Exceptions.StretchableImage_NullException ex)
            {
            }
        }        

    }
}