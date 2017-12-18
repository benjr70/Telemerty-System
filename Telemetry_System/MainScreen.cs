using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Text.RegularExpressions;

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
        public MainScreen(SerialPort port)
        {
            InitializeComponent();
            myport = port;
            myport.DataReceived += myport_DataReceived;
            myport.Open();
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //this.Hide();
            //myport.Close();
            Serial_Monitor SerialMonitor = new Serial_Monitor(myport);
            SerialMonitor.Show();
        }


        void myport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            in_data = myport.ReadLine();
            try
            {
                this.Invoke(new EventHandler(displaydata_event));
            }
            catch (Exception ex)
            {
                // MessageBox.Show(ex.Message, "Error");
                myport.Close();
            }

        }

        private void displaydata_event(object sender, EventArgs e)
        {
        
        
        
        }
       

    }
}
