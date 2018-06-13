namespace iPendulumCS
{
	partial class FormMotorManual
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
			this.bttBrake = new System.Windows.Forms.Button();
			this.bttFwd = new System.Windows.Forms.Button();
			this.bttReverse = new System.Windows.Forms.Button();
			this.txtRPM = new System.Windows.Forms.TextBox();
			this.lblTorque = new System.Windows.Forms.Label();
			this.bttPlusRough = new System.Windows.Forms.Button();
			this.bttMinusRough = new System.Windows.Forms.Button();
			this.bttMinusFine = new System.Windows.Forms.Button();
			this.bttPlusFine = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// bttBrake
			// 
			this.bttBrake.Location = new System.Drawing.Point(59, 133);
			this.bttBrake.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.bttBrake.Name = "bttBrake";
			this.bttBrake.Size = new System.Drawing.Size(56, 32);
			this.bttBrake.TabIndex = 0;
			this.bttBrake.Text = "Brake";
			this.bttBrake.UseVisualStyleBackColor = true;
			this.bttBrake.Click += new System.EventHandler(this.EvBtt_Brake);
			// 
			// bttFwd
			// 
			this.bttFwd.Location = new System.Drawing.Point(120, 133);
			this.bttFwd.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.bttFwd.Name = "bttFwd";
			this.bttFwd.Size = new System.Drawing.Size(42, 32);
			this.bttFwd.TabIndex = 1;
			this.bttFwd.Text = "Fwd";
			this.bttFwd.UseVisualStyleBackColor = true;
			this.bttFwd.Click += new System.EventHandler(this.EvBtt_Fwd);
			// 
			// bttReverse
			// 
			this.bttReverse.Location = new System.Drawing.Point(13, 133);
			this.bttReverse.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.bttReverse.Name = "bttReverse";
			this.bttReverse.Size = new System.Drawing.Size(42, 32);
			this.bttReverse.TabIndex = 2;
			this.bttReverse.Text = "Rev.";
			this.bttReverse.UseVisualStyleBackColor = true;
			this.bttReverse.Click += new System.EventHandler(this.EvBtt_Reverse);
			// 
			// txtRPM
			// 
			this.txtRPM.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.txtRPM.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.txtRPM.Cursor = System.Windows.Forms.Cursors.Default;
			this.txtRPM.Enabled = false;
			this.txtRPM.Font = new System.Drawing.Font("Microsoft Sans Serif", 19.8F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.txtRPM.ForeColor = System.Drawing.Color.LimeGreen;
			this.txtRPM.Location = new System.Drawing.Point(13, 14);
			this.txtRPM.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.txtRPM.Name = "txtRPM";
			this.txtRPM.ReadOnly = true;
			this.txtRPM.Size = new System.Drawing.Size(150, 37);
			this.txtRPM.TabIndex = 3;
			this.txtRPM.Text = "5200 RPM";
			this.txtRPM.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			// 
			// lblTorque
			// 
			this.lblTorque.Font = new System.Drawing.Font("Microsoft Sans Serif", 18F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lblTorque.Location = new System.Drawing.Point(32, 67);
			this.lblTorque.Margin = new System.Windows.Forms.Padding(2, 0, 2, 0);
			this.lblTorque.Name = "lblTorque";
			this.lblTorque.Size = new System.Drawing.Size(118, 37);
			this.lblTorque.TabIndex = 4;
			this.lblTorque.Text = "100 %";
			this.lblTorque.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// bttPlusRough
			// 
			this.bttPlusRough.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.bttPlusRough.Location = new System.Drawing.Point(13, 67);
			this.bttPlusRough.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.bttPlusRough.Name = "bttPlusRough";
			this.bttPlusRough.Size = new System.Drawing.Size(20, 20);
			this.bttPlusRough.TabIndex = 5;
			this.bttPlusRough.Text = "+";
			this.bttPlusRough.UseVisualStyleBackColor = true;
			this.bttPlusRough.Click += new System.EventHandler(this.EvBtt_Torque);
			// 
			// bttMinusRough
			// 
			this.bttMinusRough.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.bttMinusRough.Location = new System.Drawing.Point(13, 89);
			this.bttMinusRough.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.bttMinusRough.Name = "bttMinusRough";
			this.bttMinusRough.Size = new System.Drawing.Size(20, 20);
			this.bttMinusRough.TabIndex = 6;
			this.bttMinusRough.Text = "-";
			this.bttMinusRough.UseVisualStyleBackColor = true;
			this.bttMinusRough.Click += new System.EventHandler(this.EvBtt_Torque);
			// 
			// bttMinusFine
			// 
			this.bttMinusFine.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.bttMinusFine.Location = new System.Drawing.Point(147, 89);
			this.bttMinusFine.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.bttMinusFine.Name = "bttMinusFine";
			this.bttMinusFine.Size = new System.Drawing.Size(20, 20);
			this.bttMinusFine.TabIndex = 8;
			this.bttMinusFine.Text = "-";
			this.bttMinusFine.UseVisualStyleBackColor = true;
			this.bttMinusFine.Click += new System.EventHandler(this.EvBtt_Torque);
			// 
			// bttPlusFine
			// 
			this.bttPlusFine.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.bttPlusFine.Location = new System.Drawing.Point(147, 67);
			this.bttPlusFine.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.bttPlusFine.Name = "bttPlusFine";
			this.bttPlusFine.Size = new System.Drawing.Size(20, 20);
			this.bttPlusFine.TabIndex = 7;
			this.bttPlusFine.Text = "+";
			this.bttPlusFine.UseVisualStyleBackColor = true;
			this.bttPlusFine.Click += new System.EventHandler(this.EvBtt_Torque);
			// 
			// FormMotorManual
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(187, 188);
			this.Controls.Add(this.bttMinusFine);
			this.Controls.Add(this.bttPlusFine);
			this.Controls.Add(this.bttMinusRough);
			this.Controls.Add(this.bttPlusRough);
			this.Controls.Add(this.lblTorque);
			this.Controls.Add(this.txtRPM);
			this.Controls.Add(this.bttReverse);
			this.Controls.Add(this.bttFwd);
			this.Controls.Add(this.bttBrake);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormMotorManual";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Manual Motor Control";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button bttBrake;
		private System.Windows.Forms.Button bttFwd;
		private System.Windows.Forms.Button bttReverse;
		private System.Windows.Forms.TextBox txtRPM;
		private System.Windows.Forms.Label lblTorque;
		private System.Windows.Forms.Button bttPlusRough;
		private System.Windows.Forms.Button bttMinusRough;
		private System.Windows.Forms.Button bttMinusFine;
		private System.Windows.Forms.Button bttPlusFine;
	}
}