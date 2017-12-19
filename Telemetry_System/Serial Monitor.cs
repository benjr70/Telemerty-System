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

namespace Telemetry_System
{

    public partial class Serial_Monitor : Form
    {
        SerialPort myport;
        private DateTime datetime;
        private string in_data;
        public Serial_Monitor(SerialPort port)
        {
           InitializeComponent();
           myport = port;
           myport.DataReceived += myport_DataReceived;
          // myport.Open();

        }
         void myport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {

            
            try {
                in_data = myport.ReadLine();
                this.Invoke(new EventHandler(displaydata_event));
            }
            catch(Exception ex) {
                //MessageBox.Show(ex.Message, "Error");
               // myport.Close();
            }

        }

        private void displaydata_event(object sender, EventArgs e)
        {
            datetime = DateTime.Now;
            string time = datetime.Hour + ":" + datetime.Minute + ":" + datetime.Second + ":" + datetime.Millisecond;
            textBox1.AppendText(in_data + "\n");
        }
    }
}
