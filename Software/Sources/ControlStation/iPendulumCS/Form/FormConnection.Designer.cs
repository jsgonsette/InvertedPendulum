namespace iPendulumCS
{
	partial class FormConnection
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose (bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose ( );
			}
			base.Dispose (disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent ()
		{
			this.cbPorts = new System.Windows.Forms.ComboBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.bttConnect = new System.Windows.Forms.Button();
			this.lblError = new System.Windows.Forms.Label();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// cbPorts
			// 
			this.cbPorts.FormattingEnabled = true;
			this.cbPorts.Location = new System.Drawing.Point(6, 36);
			this.cbPorts.Name = "cbPorts";
			this.cbPorts.Size = new System.Drawing.Size(212, 24);
			this.cbPorts.TabIndex = 0;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.cbPorts);
			this.groupBox1.Location = new System.Drawing.Point(12, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(235, 87);
			this.groupBox1.TabIndex = 1;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Serial Port";
			// 
			// bttConnect
			// 
			this.bttConnect.Location = new System.Drawing.Point(336, 67);
			this.bttConnect.Name = "bttConnect";
			this.bttConnect.Size = new System.Drawing.Size(75, 32);
			this.bttConnect.TabIndex = 2;
			this.bttConnect.Text = "Connect";
			this.bttConnect.UseVisualStyleBackColor = true;
			this.bttConnect.Click += new System.EventHandler(this.EvButton_Connect);
			// 
			// lblError
			// 
			this.lblError.AutoSize = true;
			this.lblError.ForeColor = System.Drawing.Color.Crimson;
			this.lblError.Location = new System.Drawing.Point(15, 125);
			this.lblError.Name = "lblError";
			this.lblError.Size = new System.Drawing.Size(40, 17);
			this.lblError.TabIndex = 3;
			this.lblError.Text = "Error";
			// 
			// FormConnection
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(433, 151);
			this.Controls.Add(this.lblError);
			this.Controls.Add(this.bttConnect);
			this.Controls.Add(this.groupBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormConnection";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Connection to iPendulum";
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ComboBox cbPorts;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button bttConnect;
		private System.Windows.Forms.Label lblError;
	}
}