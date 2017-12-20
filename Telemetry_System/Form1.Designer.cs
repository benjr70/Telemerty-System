namespace Telemetry_System
{
    partial class Main_Menu
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.button1 = new System.Windows.Forms.Button();
            this.port_name_box = new System.Windows.Forms.TextBox();
            this.Port_name = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.baudrate = new System.Windows.Forms.TextBox();
            this.Disconnect = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(12, 164);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(239, 39);
            this.button1.TabIndex = 0;
            this.button1.Text = "Connect";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // port_name_box
            // 
            this.port_name_box.Location = new System.Drawing.Point(93, 33);
            this.port_name_box.Name = "port_name_box";
            this.port_name_box.Size = new System.Drawing.Size(138, 20);
            this.port_name_box.TabIndex = 2;
            this.port_name_box.Text = "COM3";
            // 
            // Port_name
            // 
            this.Port_name.AutoSize = true;
            this.Port_name.Location = new System.Drawing.Point(30, 39);
            this.Port_name.Name = "Port_name";
            this.Port_name.Size = new System.Drawing.Size(57, 13);
            this.Port_name.TabIndex = 3;
            this.Port_name.Text = "Port Name";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(30, 70);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(55, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "BaudRate";
            // 
            // baudrate
            // 
            this.baudrate.Location = new System.Drawing.Point(93, 67);
            this.baudrate.Name = "baudrate";
            this.baudrate.Size = new System.Drawing.Size(138, 20);
            this.baudrate.TabIndex = 5;
            this.baudrate.Text = "115200";
            // 
            // Disconnect
            // 
            this.Disconnect.Location = new System.Drawing.Point(12, 220);
            this.Disconnect.Name = "Disconnect";
            this.Disconnect.Size = new System.Drawing.Size(239, 39);
            this.Disconnect.TabIndex = 6;
            this.Disconnect.Text = "Disconnect";
            this.Disconnect.UseVisualStyleBackColor = true;
            this.Disconnect.Click += new System.EventHandler(this.Disconnect_Click);
            // 
            // Main_Menu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(278, 382);
            this.Controls.Add(this.Disconnect);
            this.Controls.Add(this.baudrate);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.Port_name);
            this.Controls.Add(this.port_name_box);
            this.Controls.Add(this.button1);
            this.Name = "Main_Menu";
            this.Text = "Connect";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox port_name_box;
        private System.Windows.Forms.Label Port_name;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox baudrate;
        private System.Windows.Forms.Button Disconnect;
    }
}

