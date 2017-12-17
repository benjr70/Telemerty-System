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
        SerialPort myPort;
        inputdata data = new inputdata();
        public MainScreen(SerialPort port)
        {
            InitializeComponent();
            myPort = port;
            myPort.Open();
            lineParse();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //this.Hide();
            myPort.Close();
            Serial_Monitor SerialMonitor = new Serial_Monitor(myPort);
            SerialMonitor.Show();
        }

        private void lineParse()
        {
            int index = 0;
            string line = myPort.ReadLine();
            string temp = "";
            int x = 0;
            int[] data;
            int dataindex = 0;
            while(line[index] != '\0')
            {
                x = 0;
                while(line[index] != '\t')
                {
                    temp[x] = line[index];
                    x++;
                    index++;
                }
                data[dataindex] = Convert.ToInt32(temp);
                dataindex++;
                temp = "";
                index++;

            }

        }

    }
}
