using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Reflection;
using System.Windows.Media.Media3D;
using System.Text;
using System.Text.RegularExpressions;
using System.Windows.Forms;


namespace dae_to_csv
{
    public partial class Form1 : Form
    {
        // Container
        // Temporary container of loaded rows
        List<CSVColumns> CSVRows;

        // Regex strings
        // Used to seperate nodes
        string NodeRegex = @"<node.*?</node";
        // Used to grab the values from a node
        string ValueRegex = @"name=\""(.*?)\"".*?translate\"">(.+?) (.+?) (.+?)<\/translate.*?rotateZ\"">\d \d \d (.*?)<\/rotate.*?rotateY\"">\d \d \d (.*?)<\/rotate.*?rotateX\"">\d \d \d (.*?)<\/rotate";

        public Form1()
        {
            InitializeComponent();

            CSVRows = new List<CSVColumns>();
        }

        OpenFileDialog ofd;
        SaveFileDialog sfd;

        // Events
        private void btn_load_Click(object sender, EventArgs e)
        {
            ofd = new OpenFileDialog();
            ofd.Filter = "COLLADA Files (.dae)|*.dae";
            ofd.FilterIndex = 1;
            
            DialogResult userDialogResponse = ofd.ShowDialog();
            if (userDialogResponse == DialogResult.OK)
            {
                // Appending dialog
                if (CSVRows.Count > 0)
                {
                    DialogResult appendDialog = MessageBox.Show("You've previously imported another file - should the old data be removed?\n(Click \"No\", if you want to append instances.)", "Appending of values", MessageBoxButtons.YesNoCancel);
                    if (appendDialog == DialogResult.Yes)
                    {
                        CSVRows = new List<CSVColumns>();
                    }
                    if (appendDialog == DialogResult.Cancel)
                    {
                        return;
                    }
                }

                if (backgroundWorker_regex.IsBusy != true)
                {
                    backgroundWorker_regex.RunWorkerAsync();
                    btn_load.Enabled = false;
                    btn_save.Enabled = false;
                }
            }

            // Update UI
            label_found_instances.Text = "Working...";
        }
        
        private void btn_save_Click(object sender, EventArgs e)
        {
            sfd = new SaveFileDialog();
            sfd.Filter = "Comma-Seperated-Values Files (.csv)|*.csv";
            sfd.FilterIndex = 1;

            List<UE4CSVColumns> rows = CSVRows.Cast<UE4CSVColumns>().ToList();

            string CSVText = CSVHelper.ToCsv(textbox_seperator.Text, rows);

            DialogResult userClickedOK = sfd.ShowDialog();
            if (userClickedOK == DialogResult.OK)
            {
                using (StreamWriter sw = new StreamWriter(sfd.FileName))
                    sw.Write(CSVText);

                label_found_instances.Text = "Done!";
            }
        }

        int progress = 0;

        private void backgroundWorker_regex_DoWork(object sender, System.ComponentModel.DoWorkEventArgs e)
        {
            System.IO.Stream readStream = ofd.OpenFile();
            long length = readStream.Length;
            byte[] byteBuffer = new byte[length];

            readStream.Read(byteBuffer, 0, (int)length);

            string contentAsString = System.Text.Encoding.Default.GetString(byteBuffer);

            // Match nodes
            MatchCollection collection = Regex.Matches(contentAsString, NodeRegex, RegexOptions.Singleline);
            int i = 0;
            foreach (Match match in collection)
            {
                // Match value-regex
                Match m = Regex.Match(match.Groups[0].Value, ValueRegex);
                if (m.Groups.Count >= 8)
                {
                    CSVColumns column = new CSVColumns(m.Groups[1].Value, m.Groups[2].Value, m.Groups[3].Value, m.Groups[4].Value, m.Groups[7].Value, m.Groups[6].Value, m.Groups[5].Value);
                    if (checkBox_handednessFix.Checked)
                    {
                        column.HandednessFix();
                    }
                    CSVRows.Add(column);
                }
                i++;
                progress = (int)((float)(collection.Count - (collection.Count - i)) / (float)collection.Count * 100);
                backgroundWorker_regex.ReportProgress(progress);
            }
        }

        private void backgroundWorker_regex_ProgressChanged(object sender, System.ComponentModel.ProgressChangedEventArgs e)
        {
            progressBar_regex.Value = e.ProgressPercentage;
        }

        private void backgroundWorker_regex_RunWorkerCompleted(object sender, System.ComponentModel.RunWorkerCompletedEventArgs e)
        {
            btn_load.Enabled = true;
            btn_save.Enabled = true;
            label_found_instances.Text = string.Format("Found instances: {0}", CSVRows.Count);
        }
    }

    // CSV generator
    public class CSVHelper
    {
        public static string ToCsv<T>(string separator, IEnumerable<T> objectlist)
        {
            Type t = typeof(T);
            FieldInfo[] fields = t.GetFields();

            string header = String.Join(separator, fields.Select(f => f.Name).ToArray());

            StringBuilder csvdata = new StringBuilder();
            csvdata.AppendLine(header);

            foreach (var o in objectlist)
                csvdata.AppendLine(ToCsvFields(separator, fields, o));

            return csvdata.ToString();
        }

        public static string ToCsvFields(string separator, FieldInfo[] fields, object o)
        {
            StringBuilder linie = new StringBuilder();

            foreach (var f in fields)
            {
                if (linie.Length > 0)
                    linie.Append(separator);

                var x = f.GetValue(o);

                if (x != null)
                    linie.Append(x.ToString());
            }

            return linie.ToString();
        }
    }

    // Custom CSV container format
    public class UE4CSVColumns
    {
        public string ID;
        public float TranslateX;
        public float TranslateY;
        public float TranslateZ;

        public float Roll;
        public float Pitch;
        public float Yaw;
    }

    public class CSVColumns : UE4CSVColumns
    {
        public CSVColumns(string id, string translateX, string translateY, string translateZ, string roll, string pitch, string yaw)
        {
            ID = id;
            TranslateX = float.Parse(translateX);
            TranslateY = float.Parse(translateY);
            TranslateZ = float.Parse(translateZ);

            Roll = float.Parse(roll);
            Pitch = float.Parse(pitch);
            Yaw = float.Parse(yaw);
        }

        public void HandednessFix() {
            TranslateY = -TranslateY;

            Roll = Roll;
            Pitch = -Pitch;
            Yaw = -Yaw;
        }
    }
}
