namespace iPendulumCS.Control
{
	partial class CtrlParamDisplay
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
			this.lblParamValue = new System.Windows.Forms.Label();
			this.lblParamName = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// lblParamValue
			// 
			this.lblParamValue.AutoSize = true;
			this.lblParamValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lblParamValue.Location = new System.Drawing.Point(4, 49);
			this.lblParamValue.Name = "lblParamValue";
			this.lblParamValue.Size = new System.Drawing.Size(66, 24);
			this.lblParamValue.TabIndex = 4;
			this.lblParamValue.Text = "label2";
			// 
			// lblParamName
			// 
			this.lblParamName.AutoSize = true;
			this.lblParamName.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.lblParamName.ForeColor = System.Drawing.SystemColors.HotTrack;
			this.lblParamName.Location = new System.Drawing.Point(3, 0);
			this.lblParamName.Name = "lblParamName";
			this.lblParamName.Size = new System.Drawing.Size(66, 29);
			this.lblParamName.TabIndex = 3;
			this.lblParamName.Text = "Title";
			// 
			// CtrlParamDisplay
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.lblParamValue);
			this.Controls.Add(this.lblParamName);
			this.Name = "CtrlParamDisplay";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label lblParamValue;
		private System.Windows.Forms.Label lblParamName;
	}
}
