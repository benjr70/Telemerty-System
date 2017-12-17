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


    public partial class Main_Menu : Form
    {
        private SerialPort myport;
        private DateTime datetime;
        private string in_data;
        public Main_Menu()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            myport = new SerialPort();
            myport.BaudRate = Convert.ToInt32(baudrate.Text);
            myport.PortName = port_name_box.Text;
            myport.Parity = Parity.None;
            myport.DataBits = 8;
            myport.StopBits = StopBits.One;
            try{
                myport.Open();
                MessageBox.Show("Connection successful");//mpu9255 
                myport.Close();
               // this.Hide();
                MainScreen mainScreen = new MainScreen(myport);
                mainScreen.Show();
                
                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message,"errors opening port COM3 ");
            }
            
        }


    }
}
