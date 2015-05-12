using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace beardhair_headergenerator
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        string uproperty = "\tUPROPERTY(EditAnywhere, BlueprintReadWrite, Category = \"Beard\")	int32 {0};\n";
        string initializerList = "\t\t{0}(0),\n";
        string layout =
@"struct FBeardComparisonData : public FTableRowBase
{{
	GENERATED_USTRUCT_BODY()

public:

	FBeardComparisonData()
		:
{0}
	{{}}

{1}
}};";

        void TryGenerateFile()
        {
            if (textbox_variablename.Text == "")
            {
                label_feedback.Text = "Variable doesn't have to be empty!";
                textbox_variablename.Focus();
                return;
            }
            if (numericUpDown_amount.Value <= 0)
            {
                label_feedback.Text = "Amount has to be bigger than 0!";
                numericUpDown_amount.Focus();
                return;
            }

            GenerateFile();
        }

        void GenerateFile()
        {
            string finalContent = "";
            string finalUpropertyContent = "";
            string finalInitializerListContent = "";

            string variableName = textbox_variablename.Text;
            int amount = (int)numericUpDown_amount.Value;

            for (int i = 0; i < amount; i++)
            {
                finalUpropertyContent += string.Format(uproperty, string.Format(variableName, i));
                finalInitializerListContent += string.Format(initializerList, string.Format(variableName, i));
            }
            finalInitializerListContent = finalInitializerListContent.Substring(0, finalInitializerListContent.Length - 2);
            finalContent = string.Format(layout, finalInitializerListContent, finalUpropertyContent);

            SaveFileDialog sfd = new SaveFileDialog();
            sfd.Filter = "C++ Header Files|*.h";
            sfd.FilterIndex = 1;
            DialogResult userDialogResult = sfd.ShowDialog();
            if (userDialogResult == DialogResult.OK)
            {
                using (StreamWriter sw = new StreamWriter(sfd.FileName))
                    sw.Write(finalContent);

                label_feedback.Text = "Done! Wrote " + amount + " propert" + (amount!=1?"ies":"y") + "!";
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button_generate_Click(object sender, EventArgs e)
        {
            TryGenerateFile();
        }

        private void numericUpDown_amount_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                TryGenerateFile();
        }

        private void textbox_variablename_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
                TryGenerateFile();
        }
    }
}
