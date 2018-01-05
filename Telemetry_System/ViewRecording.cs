using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Telemetry_System
{
    public partial class ViewRecording : Form
    {
       public LinkedList data;
        public string recoredstring;
        public ViewRecording(LinkedList RecordedData)
        {
            InitializeComponent();
            data = RecordedData;
            recoredstring = data.printNodes();
            textBox1.Text = recoredstring;
        }
    }
}
