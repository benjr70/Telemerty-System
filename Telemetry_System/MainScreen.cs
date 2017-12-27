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


namespace Telemetry_System
{
    public struct inputdata
    {
       public int RotaryEcoder;
       public int ax, ay, az, gx, gy, gz, mx, my, mz; 
        
    }
    
  
    public partial class MainScreen : Form
    {
        SerialPort myport;
        inputdata data = new inputdata();
        string in_data;
        string currentDir = Directory.GetCurrentDirectory();
        int delaycounter = 0;
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

                    this.Invoke(new EventHandler(displaydata_event));
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
        private void displaydata_event(object sender, EventArgs e)
        {
            int count = 0;
            string patteren = "--";
            string[] substrings = Regex.Split(in_data, patteren);
            foreach (string match in substrings)
            {
                switch (count)
                {
                    case 0: data.RotaryEcoder = Convert.ToInt32(match);
                        break;
                    case 1: data.ax = Convert.ToInt32(match);
                        break;
                    case 2: data.ay = Convert.ToInt32(match);
                        break;
                    case 3: data.az = Convert.ToInt32(match);
                        break;
                    case 4: data.gx = Convert.ToInt32(match);
                        break;
                    case 5: data.gy = Convert.ToInt32(match);
                        break;
                    case 6: data.gz = Convert.ToInt32(match);
                        break;
                    case 7: data.mx = Convert.ToInt32(match);
                        break;
                    case 8: data.my = Convert.ToInt32(match);
                        break;
                    case 9: data.mz = Convert.ToInt32(match);
                        break;
                }
                //insert data to database here probably 
               
                delaycounter++;
                if (delaycounter >= 5)
                {
                    Addtodatabase();
                    delaycounter = 0;
                }
                count++;
            }
        }
        public void Addtodatabase()
        {
            SqlConnection DatabaseCon = new SqlConnection("Data Source = (LocalDB)\\MSSQLLocalDB; AttachDbFilename = |DataDirectory|\\Databas.mdf; Integrated Security = True");

            try
            {
                DatabaseCon.Open();
                SqlCommand cmd1 = new SqlCommand();
                cmd1.CommandText = "INSERT INTO DataTable(time, RotaryEcoder, ax, ay, az, gx, gy, gz, mx, my, mz) VALUES ( CURRENT_TIMESTAMP ,@rotaryencoder, @ax, @ay, @az, @gx, @gy, @gz, @mx, @my, @mz);";
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
        }

    }
}
