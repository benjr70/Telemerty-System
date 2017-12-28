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
    public partial class VeiwDatabase : Form
    {
        public VeiwDatabase()
        {
            InitializeComponent();
        }

        private void dataTableBindingNavigatorSaveItem_Click(object sender, EventArgs e)
        {
            this.Validate();
            this.dataTableBindingSource.EndEdit();
            this.tableAdapterManager.UpdateAll(this.databasDataSet);

        }

        private void VeiwDatabase_Load(object sender, EventArgs e)
        {
            // TODO: This line of code loads data into the 'databasDataSet.DataTable' table. You can move, or remove it, as needed.
            this.dataTableTableAdapter.Fill(this.databasDataSet.DataTable);

        }
    }
}
