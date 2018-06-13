namespace iPendulumCS
{
	partial class FormCalibBattery
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
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.label1 = new System.Windows.Forms.Label();
			this.txtVoltageRaw = new System.Windows.Forms.TextBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.label2 = new System.Windows.Forms.Label();
			this.txtVoltage = new System.Windows.Forms.TextBox();
			this.bttCalibrate = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.txtVoltageRaw);
			this.groupBox1.Location = new System.Drawing.Point(12, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(213, 77);
			this.groupBox1.TabIndex = 2;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Uncalibrated Voltage";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(172, 37);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(39, 17);
			this.label1.TabIndex = 1;
			this.label1.Text = "Volts";
			// 
			// txtVoltageRaw
			// 
			this.txtVoltageRaw.Enabled = false;
			this.txtVoltageRaw.Location = new System.Drawing.Point(6, 37);
			this.txtVoltageRaw.Name = "txtVoltageRaw";
			this.txtVoltageRaw.ReadOnly = true;
			this.txtVoltageRaw.Size = new System.Drawing.Size(160, 22);
			this.txtVoltageRaw.TabIndex = 0;
			this.txtVoltageRaw.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.label2);
			this.groupBox2.Controls.Add(this.txtVoltage);
			this.groupBox2.Location = new System.Drawing.Point(231, 12);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(213, 77);
			this.groupBox2.TabIndex = 3;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Calibrated Voltage";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(172, 37);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(39, 17);
			this.label2.TabIndex = 1;
			this.label2.Text = "Volts";
			// 
			// txtVoltage
			// 
			this.txtVoltage.Location = new System.Drawing.Point(6, 37);
			this.txtVoltage.Name = "txtVoltage";
			this.txtVoltage.Size = new System.Drawing.Size(160, 22);
			this.txtVoltage.TabIndex = 0;
			this.txtVoltage.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this.txtVoltage.Enter += new System.EventHandler(this.EvVoltage_Enter);
			// 
			// bttCalibrate
			// 
			this.bttCalibrate.Location = new System.Drawing.Point(343, 96);
			this.bttCalibrate.Name = "bttCalibrate";
			this.bttCalibrate.Size = new System.Drawing.Size(100, 30);
			this.bttCalibrate.TabIndex = 4;
			this.bttCalibrate.Text = "Calibrate";
			this.bttCalibrate.UseVisualStyleBackColor = true;
			this.bttCalibrate.Click += new System.EventHandler(this.EvButton_Click);
			// 
			// FormCalibBattery
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(457, 154);
			this.Controls.Add(this.bttCalibrate);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormCalibBattery";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Pendulum Battery Calibration";
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.TextBox txtVoltageRaw;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox txtVoltage;
		private System.Windows.Forms.Button bttCalibrate;
	}
}