namespace iPendulumCS
{
	partial class FormCalibIMU
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormCalibIMU));
			this.bttNext = new System.Windows.Forms.Button();
			this.lblLevel = new System.Windows.Forms.Label();
			this.txtInstruction = new System.Windows.Forms.TextBox();
			this.bttValidate = new System.Windows.Forms.Button();
			this.barProgress = new System.Windows.Forms.ProgressBar();
			this.picture = new System.Windows.Forms.PictureBox();
			((System.ComponentModel.ISupportInitialize)(this.picture)).BeginInit();
			this.SuspendLayout();
			// 
			// bttNext
			// 
			this.bttNext.Location = new System.Drawing.Point(511, 397);
			this.bttNext.Margin = new System.Windows.Forms.Padding(2);
			this.bttNext.Name = "bttNext";
			this.bttNext.Size = new System.Drawing.Size(75, 24);
			this.bttNext.TabIndex = 4;
			this.bttNext.Text = "Next";
			this.bttNext.UseVisualStyleBackColor = true;
			this.bttNext.Click += new System.EventHandler(this.EvButton_Click);
			// 
			// lblLevel
			// 
			this.lblLevel.AutoSize = true;
			this.lblLevel.BackColor = System.Drawing.Color.Transparent;
			this.lblLevel.Font = new System.Drawing.Font("Microsoft Sans Serif", 72F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lblLevel.Location = new System.Drawing.Point(9, 324);
			this.lblLevel.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.lblLevel.Name = "lblLevel";
			this.lblLevel.Size = new System.Drawing.Size(98, 108);
			this.lblLevel.TabIndex = 9;
			this.lblLevel.Text = "1";
			// 
			// txtInstruction
			// 
			this.txtInstruction.BackColor = System.Drawing.SystemColors.ControlLightLight;
			this.txtInstruction.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.txtInstruction.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtInstruction.Location = new System.Drawing.Point(9, 10);
			this.txtInstruction.Margin = new System.Windows.Forms.Padding(2);
			this.txtInstruction.Multiline = true;
			this.txtInstruction.Name = "txtInstruction";
			this.txtInstruction.ReadOnly = true;
			this.txtInstruction.Size = new System.Drawing.Size(577, 57);
			this.txtInstruction.TabIndex = 8;
			this.txtInstruction.Text = "Orient the iPendulum face down on a flat horizontal surface";
			this.txtInstruction.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			this.txtInstruction.Enter += new System.EventHandler(this.EvInstructions_Enter);
			// 
			// bttValidate
			// 
			this.bttValidate.Location = new System.Drawing.Point(431, 397);
			this.bttValidate.Margin = new System.Windows.Forms.Padding(2);
			this.bttValidate.Name = "bttValidate";
			this.bttValidate.Size = new System.Drawing.Size(75, 24);
			this.bttValidate.TabIndex = 10;
			this.bttValidate.Text = "Validate";
			this.bttValidate.UseVisualStyleBackColor = true;
			this.bttValidate.Click += new System.EventHandler(this.EvButton_Click);
			// 
			// barProgress
			// 
			this.barProgress.Location = new System.Drawing.Point(26, 428);
			this.barProgress.Margin = new System.Windows.Forms.Padding(2);
			this.barProgress.MarqueeAnimationSpeed = 10;
			this.barProgress.Name = "barProgress";
			this.barProgress.Size = new System.Drawing.Size(560, 19);
			this.barProgress.TabIndex = 11;
			// 
			// picture
			// 
			this.picture.Image = ((System.Drawing.Image)(resources.GetObject("picture.Image")));
			this.picture.Location = new System.Drawing.Point(75, 72);
			this.picture.Name = "picture";
			this.picture.Size = new System.Drawing.Size(453, 320);
			this.picture.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
			this.picture.TabIndex = 12;
			this.picture.TabStop = false;
			// 
			// FormCalibIMU
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(595, 457);
			this.Controls.Add(this.picture);
			this.Controls.Add(this.barProgress);
			this.Controls.Add(this.lblLevel);
			this.Controls.Add(this.bttValidate);
			this.Controls.Add(this.txtInstruction);
			this.Controls.Add(this.bttNext);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(2);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormCalibIMU";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Pendulum IMU Calibration";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.EvForm_Closing);
			((System.ComponentModel.ISupportInitialize)(this.picture)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button bttNext;
		private System.Windows.Forms.Label lblLevel;
		private System.Windows.Forms.TextBox txtInstruction;
		private System.Windows.Forms.Button bttValidate;
		private System.Windows.Forms.ProgressBar barProgress;
		private System.Windows.Forms.PictureBox picture;
	}
}