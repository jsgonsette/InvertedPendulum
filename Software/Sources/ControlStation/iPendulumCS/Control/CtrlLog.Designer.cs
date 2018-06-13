namespace iPendulumCS.Control
{
	partial class CtrlLog
	{
		/// <summary> 
		/// Variable nécessaire au concepteur.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary> 
		/// Nettoyage des ressources utilisées.
		/// </summary>
		/// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
		protected override void Dispose (bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose ( );
			}
			base.Dispose (disposing);
		}

		#region Code généré par le Concepteur de composants

		/// <summary> 
		/// Méthode requise pour la prise en charge du concepteur - ne modifiez pas 
		/// le contenu de cette méthode avec l'éditeur de code.
		/// </summary>
		private void InitializeComponent ()
		{
			this.gbLog = new System.Windows.Forms.GroupBox();
			this.bttLog = new System.Windows.Forms.Button();
			this.bttBrowse = new System.Windows.Forms.Button();
			this.tbPath = new System.Windows.Forms.TextBox();
			this.lblPath = new System.Windows.Forms.Label();
			this.gbLog.SuspendLayout();
			this.SuspendLayout();
			// 
			// gbLog
			// 
			this.gbLog.Controls.Add(this.bttLog);
			this.gbLog.Controls.Add(this.bttBrowse);
			this.gbLog.Controls.Add(this.tbPath);
			this.gbLog.Controls.Add(this.lblPath);
			this.gbLog.Location = new System.Drawing.Point(3, 3);
			this.gbLog.Name = "gbLog";
			this.gbLog.Size = new System.Drawing.Size(1034, 82);
			this.gbLog.TabIndex = 0;
			this.gbLog.TabStop = false;
			this.gbLog.Text = "Log data to file";
			// 
			// bttLog
			// 
			this.bttLog.Location = new System.Drawing.Point(909, 32);
			this.bttLog.Name = "bttLog";
			this.bttLog.Size = new System.Drawing.Size(106, 33);
			this.bttLog.TabIndex = 3;
			this.bttLog.Text = "Start logging";
			this.bttLog.UseVisualStyleBackColor = true;
			this.bttLog.Click += new System.EventHandler(this.EvButton_Log);
			// 
			// bttBrowse
			// 
			this.bttBrowse.Location = new System.Drawing.Point(818, 32);
			this.bttBrowse.Name = "bttBrowse";
			this.bttBrowse.Size = new System.Drawing.Size(85, 33);
			this.bttBrowse.TabIndex = 2;
			this.bttBrowse.Text = "Browse";
			this.bttBrowse.UseVisualStyleBackColor = true;
			this.bttBrowse.Click += new System.EventHandler(this.EvButton_Browse);
			// 
			// tbPath
			// 
			this.tbPath.Location = new System.Drawing.Point(78, 37);
			this.tbPath.Name = "tbPath";
			this.tbPath.Size = new System.Drawing.Size(734, 22);
			this.tbPath.TabIndex = 1;
			this.tbPath.TextChanged += new System.EventHandler(this.EvText_TextChanged);
			// 
			// lblPath
			// 
			this.lblPath.AutoSize = true;
			this.lblPath.Location = new System.Drawing.Point(6, 37);
			this.lblPath.Name = "lblPath";
			this.lblPath.Size = new System.Drawing.Size(66, 17);
			this.lblPath.TabIndex = 0;
			this.lblPath.Text = "File path:";
			// 
			// CtrlLog
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.gbLog);
			this.Name = "CtrlLog";
			this.Size = new System.Drawing.Size(1062, 122);
			this.gbLog.ResumeLayout(false);
			this.gbLog.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox gbLog;
		private System.Windows.Forms.Button bttLog;
		private System.Windows.Forms.Button bttBrowse;
		private System.Windows.Forms.TextBox tbPath;
		private System.Windows.Forms.Label lblPath;
	}
}
