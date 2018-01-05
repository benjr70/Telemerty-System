namespace Telemetry_System
{
    partial class MainScreen
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainScreen));
            this.button1 = new System.Windows.Forms.Button();
            this.StartRecording = new System.Windows.Forms.Button();
            this.StopRecording = new System.Windows.Forms.Button();
            this.ExcelExport = new System.Windows.Forms.Button();
            this.View_Recording = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(1778, 4);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(114, 36);
            this.button1.TabIndex = 0;
            this.button1.Text = "Serial Monitor";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // StartRecording
            // 
            this.StartRecording.BackColor = System.Drawing.Color.Lime;
            this.StartRecording.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.StartRecording.Location = new System.Drawing.Point(3, 4);
            this.StartRecording.Name = "StartRecording";
            this.StartRecording.Size = new System.Drawing.Size(111, 36);
            this.StartRecording.TabIndex = 1;
            this.StartRecording.Text = "Start Recording";
            this.StartRecording.UseVisualStyleBackColor = false;
            this.StartRecording.Click += new System.EventHandler(this.StartRecording_Click);
            // 
            // StopRecording
            // 
            this.StopRecording.BackColor = System.Drawing.Color.Red;
            this.StopRecording.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.StopRecording.Location = new System.Drawing.Point(120, 4);
            this.StopRecording.Name = "StopRecording";
            this.StopRecording.Size = new System.Drawing.Size(111, 36);
            this.StopRecording.TabIndex = 2;
            this.StopRecording.Text = "Stop Recording";
            this.StopRecording.UseVisualStyleBackColor = false;
            this.StopRecording.Click += new System.EventHandler(this.StopRecording_Click);
            // 
            // ExcelExport
            // 
            this.ExcelExport.Location = new System.Drawing.Point(350, 4);
            this.ExcelExport.Name = "ExcelExport";
            this.ExcelExport.Size = new System.Drawing.Size(111, 36);
            this.ExcelExport.TabIndex = 3;
            this.ExcelExport.Text = "Export to Excel";
            this.ExcelExport.UseVisualStyleBackColor = true;
            this.ExcelExport.Click += new System.EventHandler(this.ExcelExport_Click);
            // 
            // View_Recording
            // 
            this.View_Recording.Location = new System.Drawing.Point(237, 4);
            this.View_Recording.Name = "View_Recording";
            this.View_Recording.Size = new System.Drawing.Size(107, 36);
            this.View_Recording.TabIndex = 4;
            this.View_Recording.Text = "View Recording";
            this.View_Recording.UseVisualStyleBackColor = true;
            this.View_Recording.Click += new System.EventHandler(this.View_Recording_Click);
            // 
            // MainScreen
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(1904, 1011);
            this.Controls.Add(this.View_Recording);
            this.Controls.Add(this.ExcelExport);
            this.Controls.Add(this.StopRecording);
            this.Controls.Add(this.StartRecording);
            this.Controls.Add(this.button1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainScreen";
            this.Text = "MainScreen";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button StartRecording;
        private System.Windows.Forms.Button StopRecording;
        private System.Windows.Forms.Button ExcelExport;
        private System.Windows.Forms.Button View_Recording;
    }
}