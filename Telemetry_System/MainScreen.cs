using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using System.Text.RegularExpressions;
using System.Data.SqlClient;
using System.Threading;
using Microsoft.Office.Interop.Excel;
using Excel = Microsoft.Office.Interop.Excel;
using SqlDataTable = System.Data;

namespace Telemetry_System
{
    public struct inputdata
    {
       public int RotaryEcoder;
       public int ax, ay, az, gx, gy, gz, mx, my, mz;
       public float lat, lon, mph;
       public int hour, minute, second;
    }
    
  
    public partial class MainScreen : Form
    {
        SerialPort myport;
        inputdata data = new inputdata();
        string in_data;
        string currentDir = Directory.GetCurrentDirectory();
        bool recording;
        Thread database;

        public MainScreen(SerialPort port)
        {
           
            InitializeComponent();
            myport = port;
            myport.DataReceived += myport_DataReceived;
            myport.Open();
           

        }
        //***************************************************************
        // Serial monitor button
        // it opens the serial monitor
        //***************************************************************
        private void button1_Click(object sender, EventArgs e)
        {
            //this.Hide();
            //myport.Close();
            Serial_Monitor SerialMonitor = new Serial_Monitor(myport);
            SerialMonitor.Show();
           // myport.Close();
            if (!myport.IsOpen)
            {
                myport.Open();
            }
        }


        void myport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            in_data = myport.ReadLine();
            
            try
            {
                if (!IsHandleCreated)
                {
                    this.CreateHandle();
                }

                    this.Invoke(new EventHandler(lineParse_even));
                  //  steeringAnimation();
                
            }
            catch (Exception ex)
            {
                
                // MessageBox.Show(ex.Message, "Error");
                // myport.Close();
            }

          
        }
        //***************************************************************
        //this parses the in_data line and assigns it to the variable in the structure
        //
        //***************************************************************
        private void lineParse_even(object sender, EventArgs e)
        {
            int count = 0;
            string patteren = "--";
            string[] substrings = Regex.Split(in_data, patteren);

            data.RotaryEcoder = Convert.ToInt32(substrings[0]);
            data.ax = Convert.ToInt32(substrings[1]);
            data.ay = Convert.ToInt32(substrings[2]);
            data.az = Convert.ToInt32(substrings[3]);
            data.gx = Convert.ToInt32(substrings[4]);
            data.gy = Convert.ToInt32(substrings[5]);
            data.gz = Convert.ToInt32(substrings[6]);
            data.mx = Convert.ToInt32(substrings[7]);
            data.my = Convert.ToInt32(substrings[8]);
            data.mz = Convert.ToInt32(substrings[9]);
            data.lat = Convert.ToInt32(substrings[10]);
            data.lon = Convert.ToInt32(substrings[11]);
            data.hour = Convert.ToInt32(substrings[12]);
            data.minute = Convert.ToInt32(substrings[13]);
            data.second = Convert.ToInt32(substrings[14]);
            data.mph = Convert.ToInt32(substrings[15]);

            //insert data to database here probably 
           // MessageBox.Show(data.RotaryEcoder.ToString());
                if (recording == true)
                {
                    // database = new Thread(Addtodatabase);
                    //  database.Start();
                    Addtodatabase();
                }

           
        }
        public void Addtodatabase()
        {
            SqlConnection DatabaseCon = new SqlConnection("Data Source = (LocalDB)\\MSSQLLocalDB; AttachDbFilename = |DataDirectory|\\Databas.mdf; Integrated Security = True");

            try
            {
                DatabaseCon.Open();
                SqlCommand cmd1 = new SqlCommand();
                cmd1.CommandText = "INSERT INTO DataTable(RotaryEcoder, ax, ay, az, gx, gy, gz, mx, my, mz) VALUES ( @rotaryencoder, @ax, @ay, @az, @gx, @gy, @gz, @mx, @my, @mz);";
              //  cmd1.Parameters.Add("@time", data.time);
                cmd1.Parameters.Add("@rotaryencoder", data.RotaryEcoder);
                cmd1.Parameters.Add("@ax", data.ax);
                cmd1.Parameters.Add("@ay", data.ay);
                cmd1.Parameters.Add("@az", data.az);
                cmd1.Parameters.Add("@gx", data.gx);
                cmd1.Parameters.Add("@gy", data.gy);
                cmd1.Parameters.Add("@gz", data.gz);
                cmd1.Parameters.Add("@mx", data.mx);
                cmd1.Parameters.Add("@my", data.my);
                cmd1.Parameters.Add("@mz", data.mz);
                cmd1.CommandType = CommandType.Text;
                cmd1.Connection = DatabaseCon;
                cmd1.ExecuteNonQuery();


                DatabaseCon.Close();
            }
            catch (Exception ex)
            {

            }

        }

        public void steeringAnimation()
        {
            /*
            Bitmap image;
            Graphics g;
            image = new Bitmap("SteeringWheel.jpg");
            g = this.CreateGraphics();
            float hw = 50;
            float hh = 50;
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
            g.TranslateTransform(hw,hh);
            g.RotateTransform(00.0f);
            g.TranslateTransform(-hw, -hh);
            g.DrawImage(image, new Point(500, 500));
            */
        }

        private void StartRecording_Click(object sender, EventArgs e)
        {
            recording = true;
        }

        private void StopRecording_Click(object sender, EventArgs e)
        {
            recording = false;
        }

        private void ExcelExport_Click(object sender, EventArgs e)
        {

            SqlConnection DatabaseCon = new SqlConnection("Data Source = (LocalDB)\\MSSQLLocalDB; AttachDbFilename = |DataDirectory|\\Databas.mdf; Integrated Security = True");
            DatabaseCon.Open();
            SqlCommand cmd2 = new SqlCommand("SELECT * FROM DataTable",DatabaseCon);
            SqlDataAdapter DA = new SqlDataAdapter(cmd2);
            SqlDataTable.DataTable DT = new SqlDataTable.DataTable();
            DA.Fill(DT);
            System.Windows.Forms.SaveFileDialog saveDlg = new System.Windows.Forms.SaveFileDialog();
            saveDlg.InitialDirectory = currentDir;
            saveDlg.Filter = "Excel Files(2003)|*.xls| Excel Files(2007)|*.xlsx";
            //saveDlg.RestoreDirectory = true;
            saveDlg.FileName = "Race Data";
            saveDlg.Title = "Export Excel File To";
            if (saveDlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                Microsoft.Office.Interop.Excel.Application ExcelApp = new Microsoft.Office.Interop.Excel.Application();
                ExcelApp.Application.Workbooks.Add(Type.Missing);

                ExcelApp.Columns.ColumnWidth = 20;

                ExcelApp.Cells[1, 1] = "Rotary Encoder";
                ExcelApp.Cells[1, 2] = "AX";
                ExcelApp.Cells[1, 3] = "AY";
                ExcelApp.Cells[1, 4] = "AZ";
                ExcelApp.Cells[1, 5] = "GX";
                ExcelApp.Cells[1, 6] = "GY";
                ExcelApp.Cells[1, 7] = "GZ";
                ExcelApp.Cells[1, 8] = "MX";
                ExcelApp.Cells[1, 9] = "MY";
                ExcelApp.Cells[1, 10] = "MZ";
                int i = 0;
                foreach (DataRow dr in DT.Rows)
                {

                    ExcelApp.Cells[i + 2, 1] = dr["RotaryEcoder"].ToString();
                    ExcelApp.Cells[i + 2, 2] = dr["ax"].ToString();
                    ExcelApp.Cells[i+2, 3] = dr["ay"].ToString();
                    ExcelApp.Cells[i+2, 4] = dr["az"].ToString();
                    ExcelApp.Cells[i+2, 5] = dr["gx"].ToString();
                    ExcelApp.Cells[i+2, 6] = dr["gy"].ToString();
                    ExcelApp.Cells[i+2, 7] = dr["gz"].ToString();
                    ExcelApp.Cells[i+2, 8] = dr["mx"].ToString();
                    ExcelApp.Cells[i+2, 9] = dr["my"].ToString();
                    ExcelApp.Cells[i+2, 10] = dr["mz"].ToString();
                    i++;
                }
                

                ExcelApp.ActiveWorkbook.SaveCopyAs(saveDlg.FileName.ToString());
                ExcelApp.ActiveWorkbook.Saved = true;
                // xlWorkBook.Close(true, misValue, misValue);
                ExcelApp.Quit();
            }


        }

        private void veiwRecording_Click(object sender, EventArgs e)
        {
            VeiwDatabase VD = new VeiwDatabase();
            VD.ShowDialog();
        }
    }
}
