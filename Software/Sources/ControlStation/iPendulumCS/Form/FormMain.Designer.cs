namespace iPendulumCS
{
	partial class FormMain
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

		#region Code généré par le Concepteur Windows Form

		/// <summary>
		/// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
		/// le contenu de cette méthode avec l'éditeur de code.
		/// </summary>
		private void InitializeComponent ()
		{
			System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("Tasks");
			System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("Memory");
			System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("Kernel");
			System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("Operating System", new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2,
            treeNode3});
			System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("Version");
			System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("System", new System.Windows.Forms.TreeNode[] {
            treeNode5});
			System.Windows.Forms.TreeNode treeNode7 = new System.Windows.Forms.TreeNode("Battery");
			System.Windows.Forms.TreeNode treeNode8 = new System.Windows.Forms.TreeNode("Hall sensors");
			System.Windows.Forms.TreeNode treeNode9 = new System.Windows.Forms.TreeNode("Accelerometers");
			System.Windows.Forms.TreeNode treeNode10 = new System.Windows.Forms.TreeNode("Gyroscope");
			System.Windows.Forms.TreeNode treeNode11 = new System.Windows.Forms.TreeNode("Sensors", new System.Windows.Forms.TreeNode[] {
            treeNode8,
            treeNode9,
            treeNode10});
			System.Windows.Forms.TreeNode treeNode12 = new System.Windows.Forms.TreeNode("Motor");
			System.Windows.Forms.TreeNode treeNode13 = new System.Windows.Forms.TreeNode("Free Observer");
			System.Windows.Forms.TreeNode treeNode14 = new System.Windows.Forms.TreeNode("State estimation", new System.Windows.Forms.TreeNode[] {
            treeNode13});
			System.Windows.Forms.TreeNode treeNode15 = new System.Windows.Forms.TreeNode("iPendulum", new System.Windows.Forms.TreeNode[] {
            treeNode4,
            treeNode6,
            treeNode7,
            treeNode11,
            treeNode12,
            treeNode14});
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
			this.tabPages = new System.Windows.Forms.TabControl();
			this.tabData = new System.Windows.Forms.TabPage();
			this.splitMain = new System.Windows.Forms.SplitContainer();
			this.treeParameters = new System.Windows.Forms.TreeView();
			this.tabScopes = new System.Windows.Forms.TabPage();
			this.ctrlScopes = new iPendulumCS.Control.CtrlScopes();
			this.panel2 = new System.Windows.Forms.Panel();
			this.bttZoomOut = new System.Windows.Forms.Button();
			this.bttZoomIn = new System.Windows.Forms.Button();
			this.bttPlay = new System.Windows.Forms.Button();
			this.bttAll = new System.Windows.Forms.Button();
			this.tabLog = new System.Windows.Forms.TabPage();
			this.ctrlLog = new iPendulumCS.Control.CtrlLog();
			this.menuMain = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.miToolsConnect = new System.Windows.Forms.ToolStripMenuItem();
			this.miToolsDisconnect = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.miToolsMode = new System.Windows.Forms.ToolStripMenuItem();
			this.miToolsModeOff = new System.Windows.Forms.ToolStripMenuItem();
			this.miToolsModeFree = new System.Windows.Forms.ToolStripMenuItem();
			this.miToolsModeEquilibrium = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.miToolsCalib = new System.Windows.Forms.ToolStripMenuItem();
			this.miCalibBattery = new System.Windows.Forms.ToolStripMenuItem();
			this.miCalibIMU = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
			this.miToolsManual = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
			this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.tabPages.SuspendLayout();
			this.tabData.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitMain)).BeginInit();
			this.splitMain.Panel1.SuspendLayout();
			this.splitMain.SuspendLayout();
			this.tabScopes.SuspendLayout();
			this.panel2.SuspendLayout();
			this.tabLog.SuspendLayout();
			this.menuMain.SuspendLayout();
			this.SuspendLayout();
			// 
			// tabPages
			// 
			this.tabPages.Alignment = System.Windows.Forms.TabAlignment.Bottom;
			this.tabPages.Controls.Add(this.tabData);
			this.tabPages.Controls.Add(this.tabScopes);
			this.tabPages.Controls.Add(this.tabLog);
			this.tabPages.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabPages.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.tabPages.Location = new System.Drawing.Point(0, 24);
			this.tabPages.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.tabPages.Name = "tabPages";
			this.tabPages.SelectedIndex = 0;
			this.tabPages.Size = new System.Drawing.Size(1061, 600);
			this.tabPages.TabIndex = 1;
			// 
			// tabData
			// 
			this.tabData.Controls.Add(this.splitMain);
			this.tabData.Location = new System.Drawing.Point(4, 4);
			this.tabData.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.tabData.Name = "tabData";
			this.tabData.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.tabData.Size = new System.Drawing.Size(716, 577);
			this.tabData.TabIndex = 0;
			this.tabData.Text = "iPendulum Data";
			this.tabData.UseVisualStyleBackColor = true;
			// 
			// splitMain
			// 
			this.splitMain.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.splitMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitMain.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
			this.splitMain.Location = new System.Drawing.Point(2, 2);
			this.splitMain.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.splitMain.Name = "splitMain";
			// 
			// splitMain.Panel1
			// 
			this.splitMain.Panel1.Controls.Add(this.treeParameters);
			// 
			// splitMain.Panel2
			// 
			this.splitMain.Panel2.BackColor = System.Drawing.SystemColors.ControlDark;
			this.splitMain.Panel2.SizeChanged += new System.EventHandler(this.splitMain_Panel2_SizeChanged);
			this.splitMain.Size = new System.Drawing.Size(712, 573);
			this.splitMain.SplitterDistance = 278;
			this.splitMain.SplitterWidth = 3;
			this.splitMain.TabIndex = 1;
			// 
			// treeParameters
			// 
			this.treeParameters.BackColor = System.Drawing.SystemColors.Control;
			this.treeParameters.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.treeParameters.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.treeParameters.Location = new System.Drawing.Point(0, 85);
			this.treeParameters.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.treeParameters.Name = "treeParameters";
			treeNode1.Name = "tiTasks";
			treeNode1.Tag = "OS.Tasks";
			treeNode1.Text = "Tasks";
			treeNode2.Name = "Nœud15";
			treeNode2.Tag = "OS.Memory";
			treeNode2.Text = "Memory";
			treeNode3.Name = "Nœud0";
			treeNode3.Tag = "OS.Kernel";
			treeNode3.Text = "Kernel";
			treeNode4.Name = "nOS";
			treeNode4.Tag = "OS";
			treeNode4.Text = "Operating System";
			treeNode5.Name = "Nœud17";
			treeNode5.Tag = "System.Version";
			treeNode5.Text = "Version";
			treeNode6.Name = "nSystem";
			treeNode6.Tag = "System";
			treeNode6.Text = "System";
			treeNode7.Name = "Nœud0";
			treeNode7.Tag = "Battery";
			treeNode7.Text = "Battery";
			treeNode8.Name = "Nœud6";
			treeNode8.Tag = "Sensors.Hall";
			treeNode8.Text = "Hall sensors";
			treeNode9.Name = "Nœud7";
			treeNode9.Tag = "Sensors.Acc";
			treeNode9.Text = "Accelerometers";
			treeNode10.Name = "Nœud8";
			treeNode10.Tag = "Sensors.Gyro";
			treeNode10.Text = "Gyroscope";
			treeNode11.Name = "nSensors";
			treeNode11.Tag = "Sensors";
			treeNode11.Text = "Sensors";
			treeNode12.Name = "Nœud9";
			treeNode12.Tag = "Motor";
			treeNode12.Text = "Motor";
			treeNode13.Name = "Nœud4";
			treeNode13.Tag = "State.Free";
			treeNode13.Text = "Free Observer";
			treeNode14.Name = "nState";
			treeNode14.Tag = "State";
			treeNode14.Text = "State estimation";
			treeNode15.Name = "nPendulum";
			treeNode15.Text = "iPendulum";
			this.treeParameters.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode15});
			this.treeParameters.Size = new System.Drawing.Size(276, 486);
			this.treeParameters.TabIndex = 0;
			this.treeParameters.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.EvTreeParameters_AfterSelect);
			// 
			// tabScopes
			// 
			this.tabScopes.Controls.Add(this.ctrlScopes);
			this.tabScopes.Controls.Add(this.panel2);
			this.tabScopes.Location = new System.Drawing.Point(4, 4);
			this.tabScopes.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.tabScopes.Name = "tabScopes";
			this.tabScopes.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.tabScopes.Size = new System.Drawing.Size(1053, 570);
			this.tabScopes.TabIndex = 1;
			this.tabScopes.Text = "Scopes";
			this.tabScopes.UseVisualStyleBackColor = true;
			// 
			// ctrlScopes
			// 
			this.ctrlScopes.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ctrlScopes.Enabled = false;
			this.ctrlScopes.Location = new System.Drawing.Point(78, 2);
			this.ctrlScopes.Name = "ctrlScopes";
			this.ctrlScopes.Size = new System.Drawing.Size(973, 566);
			this.ctrlScopes.TabIndex = 1;
			// 
			// panel2
			// 
			this.panel2.BackColor = System.Drawing.SystemColors.ControlDark;
			this.panel2.Controls.Add(this.bttZoomOut);
			this.panel2.Controls.Add(this.bttZoomIn);
			this.panel2.Controls.Add(this.bttPlay);
			this.panel2.Controls.Add(this.bttAll);
			this.panel2.Dock = System.Windows.Forms.DockStyle.Left;
			this.panel2.Location = new System.Drawing.Point(2, 2);
			this.panel2.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.panel2.Name = "panel2";
			this.panel2.Size = new System.Drawing.Size(76, 566);
			this.panel2.TabIndex = 0;
			// 
			// bttZoomOut
			// 
			this.bttZoomOut.Image = ((System.Drawing.Image)(resources.GetObject("bttZoomOut.Image")));
			this.bttZoomOut.Location = new System.Drawing.Point(8, 203);
			this.bttZoomOut.Margin = new System.Windows.Forms.Padding(2);
			this.bttZoomOut.Name = "bttZoomOut";
			this.bttZoomOut.Size = new System.Drawing.Size(60, 60);
			this.bttZoomOut.TabIndex = 3;
			this.bttZoomOut.UseVisualStyleBackColor = true;
			this.bttZoomOut.Click += new System.EventHandler(this.EvScopeBtt_Click);
			// 
			// bttZoomIn
			// 
			this.bttZoomIn.Image = ((System.Drawing.Image)(resources.GetObject("bttZoomIn.Image")));
			this.bttZoomIn.Location = new System.Drawing.Point(8, 136);
			this.bttZoomIn.Margin = new System.Windows.Forms.Padding(2);
			this.bttZoomIn.Name = "bttZoomIn";
			this.bttZoomIn.Size = new System.Drawing.Size(60, 60);
			this.bttZoomIn.TabIndex = 2;
			this.bttZoomIn.UseVisualStyleBackColor = true;
			this.bttZoomIn.Click += new System.EventHandler(this.EvScopeBtt_Click);
			// 
			// bttPlay
			// 
			this.bttPlay.Image = ((System.Drawing.Image)(resources.GetObject("bttPlay.Image")));
			this.bttPlay.Location = new System.Drawing.Point(8, 72);
			this.bttPlay.Margin = new System.Windows.Forms.Padding(2);
			this.bttPlay.Name = "bttPlay";
			this.bttPlay.Size = new System.Drawing.Size(60, 60);
			this.bttPlay.TabIndex = 1;
			this.bttPlay.UseVisualStyleBackColor = true;
			this.bttPlay.Click += new System.EventHandler(this.EvScopeBtt_Click);
			// 
			// bttAll
			// 
			this.bttAll.Image = ((System.Drawing.Image)(resources.GetObject("bttAll.Image")));
			this.bttAll.Location = new System.Drawing.Point(8, 8);
			this.bttAll.Margin = new System.Windows.Forms.Padding(2);
			this.bttAll.Name = "bttAll";
			this.bttAll.Size = new System.Drawing.Size(60, 60);
			this.bttAll.TabIndex = 0;
			this.bttAll.UseVisualStyleBackColor = true;
			this.bttAll.Click += new System.EventHandler(this.EvScopeBtt_Click);
			// 
			// tabLog
			// 
			this.tabLog.Controls.Add(this.ctrlLog);
			this.tabLog.Location = new System.Drawing.Point(4, 4);
			this.tabLog.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.tabLog.Name = "tabLog";
			this.tabLog.Size = new System.Drawing.Size(716, 579);
			this.tabLog.TabIndex = 3;
			this.tabLog.Text = "Log";
			this.tabLog.UseVisualStyleBackColor = true;
			// 
			// ctrlLog
			// 
			this.ctrlLog.Dock = System.Windows.Forms.DockStyle.Top;
			this.ctrlLog.Location = new System.Drawing.Point(0, 0);
			this.ctrlLog.Name = "ctrlLog";
			this.ctrlLog.Size = new System.Drawing.Size(716, 93);
			this.ctrlLog.TabIndex = 0;
			// 
			// menuMain
			// 
			this.menuMain.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem,
            this.aboutToolStripMenuItem});
			this.menuMain.Location = new System.Drawing.Point(0, 0);
			this.menuMain.Name = "menuMain";
			this.menuMain.Padding = new System.Windows.Forms.Padding(4, 2, 0, 2);
			this.menuMain.Size = new System.Drawing.Size(1061, 24);
			this.menuMain.TabIndex = 2;
			this.menuMain.Text = "menuMain";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
			this.fileToolStripMenuItem.Text = "File";
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.Size = new System.Drawing.Size(92, 22);
			this.exitToolStripMenuItem.Text = "Exit";
			this.exitToolStripMenuItem.Click += new System.EventHandler(this.EvMenu_Exit);
			// 
			// toolsToolStripMenuItem
			// 
			this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miToolsConnect,
            this.miToolsDisconnect,
            this.toolStripSeparator1,
            this.miToolsMode,
            this.toolStripSeparator2,
            this.miToolsCalib,
            this.toolStripSeparator4,
            this.miToolsManual,
            this.toolStripSeparator3});
			this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
			this.toolsToolStripMenuItem.Size = new System.Drawing.Size(47, 20);
			this.toolsToolStripMenuItem.Text = "Tools";
			// 
			// miToolsConnect
			// 
			this.miToolsConnect.Name = "miToolsConnect";
			this.miToolsConnect.Size = new System.Drawing.Size(194, 22);
			this.miToolsConnect.Text = "Connect to iPendulum";
			this.miToolsConnect.Click += new System.EventHandler(this.EvMenu_Connection);
			// 
			// miToolsDisconnect
			// 
			this.miToolsDisconnect.Enabled = false;
			this.miToolsDisconnect.Name = "miToolsDisconnect";
			this.miToolsDisconnect.Size = new System.Drawing.Size(194, 22);
			this.miToolsDisconnect.Text = "Disconnect";
			this.miToolsDisconnect.Click += new System.EventHandler(this.EvMenu_Disconnect);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(191, 6);
			// 
			// miToolsMode
			// 
			this.miToolsMode.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miToolsModeOff,
            this.miToolsModeFree,
            this.miToolsModeEquilibrium});
			this.miToolsMode.Enabled = false;
			this.miToolsMode.Name = "miToolsMode";
			this.miToolsMode.Size = new System.Drawing.Size(194, 22);
			this.miToolsMode.Text = "Mode";
			// 
			// miToolsModeOff
			// 
			this.miToolsModeOff.Checked = true;
			this.miToolsModeOff.CheckState = System.Windows.Forms.CheckState.Checked;
			this.miToolsModeOff.Name = "miToolsModeOff";
			this.miToolsModeOff.Size = new System.Drawing.Size(135, 22);
			this.miToolsModeOff.Text = "Stopped";
			this.miToolsModeOff.Click += new System.EventHandler(this.EvMenu_SetMode);
			// 
			// miToolsModeFree
			// 
			this.miToolsModeFree.Name = "miToolsModeFree";
			this.miToolsModeFree.Size = new System.Drawing.Size(135, 22);
			this.miToolsModeFree.Text = "Free";
			this.miToolsModeFree.Click += new System.EventHandler(this.EvMenu_SetMode);
			// 
			// miToolsModeEquilibrium
			// 
			this.miToolsModeEquilibrium.Name = "miToolsModeEquilibrium";
			this.miToolsModeEquilibrium.Size = new System.Drawing.Size(135, 22);
			this.miToolsModeEquilibrium.Text = "Equilibrium";
			this.miToolsModeEquilibrium.Click += new System.EventHandler(this.EvMenu_SetMode);
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(191, 6);
			// 
			// miToolsCalib
			// 
			this.miToolsCalib.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.miCalibBattery,
            this.miCalibIMU});
			this.miToolsCalib.Enabled = false;
			this.miToolsCalib.Name = "miToolsCalib";
			this.miToolsCalib.Size = new System.Drawing.Size(194, 22);
			this.miToolsCalib.Text = "Calibration";
			// 
			// miCalibBattery
			// 
			this.miCalibBattery.Name = "miCalibBattery";
			this.miCalibBattery.Size = new System.Drawing.Size(111, 22);
			this.miCalibBattery.Text = "Battery";
			this.miCalibBattery.Click += new System.EventHandler(this.EvMenuCalib);
			// 
			// miCalibIMU
			// 
			this.miCalibIMU.Name = "miCalibIMU";
			this.miCalibIMU.Size = new System.Drawing.Size(111, 22);
			this.miCalibIMU.Text = "IMU";
			this.miCalibIMU.Click += new System.EventHandler(this.EvMenuCalib);
			// 
			// toolStripSeparator4
			// 
			this.toolStripSeparator4.Name = "toolStripSeparator4";
			this.toolStripSeparator4.Size = new System.Drawing.Size(191, 6);
			// 
			// miToolsManual
			// 
			this.miToolsManual.Enabled = false;
			this.miToolsManual.Name = "miToolsManual";
			this.miToolsManual.Size = new System.Drawing.Size(194, 22);
			this.miToolsManual.Text = "Manual Motor Control";
			this.miToolsManual.Click += new System.EventHandler(this.EvMenuTools_Manual);
			// 
			// toolStripSeparator3
			// 
			this.toolStripSeparator3.Name = "toolStripSeparator3";
			this.toolStripSeparator3.Size = new System.Drawing.Size(191, 6);
			// 
			// aboutToolStripMenuItem
			// 
			this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
			this.aboutToolStripMenuItem.Size = new System.Drawing.Size(52, 20);
			this.aboutToolStripMenuItem.Text = "About";
			this.aboutToolStripMenuItem.Click += new System.EventHandler(this.EvMenu_About);
			// 
			// FormMain
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1061, 624);
			this.Controls.Add(this.tabPages);
			this.Controls.Add(this.menuMain);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.menuMain;
			this.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
			this.MinimumSize = new System.Drawing.Size(304, 251);
			this.Name = "FormMain";
			this.Text = "iPendulum Control Station";
			this.SizeChanged += new System.EventHandler(this.EvForm_SizeChanged);
			this.tabPages.ResumeLayout(false);
			this.tabData.ResumeLayout(false);
			this.splitMain.Panel1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitMain)).EndInit();
			this.splitMain.ResumeLayout(false);
			this.tabScopes.ResumeLayout(false);
			this.panel2.ResumeLayout(false);
			this.tabLog.ResumeLayout(false);
			this.menuMain.ResumeLayout(false);
			this.menuMain.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TabControl tabPages;
		private System.Windows.Forms.TabPage tabData;
		private System.Windows.Forms.SplitContainer splitMain;
		private System.Windows.Forms.TreeView treeParameters;
		private System.Windows.Forms.TabPage tabScopes;
		private System.Windows.Forms.MenuStrip menuMain;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem miToolsConnect;
		private System.Windows.Forms.ToolStripMenuItem miToolsDisconnect;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.Button bttZoomIn;
		private System.Windows.Forms.Button bttPlay;
		private System.Windows.Forms.Button bttAll;
		private Control.CtrlScopes ctrlScopes;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem miToolsMode;
		private System.Windows.Forms.ToolStripMenuItem miToolsModeOff;
		private System.Windows.Forms.ToolStripMenuItem miToolsModeFree;
		private System.Windows.Forms.ToolStripMenuItem miToolsModeEquilibrium;
		private System.Windows.Forms.Button bttZoomOut;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripMenuItem miToolsManual;
		private System.Windows.Forms.TabPage tabLog;
		private Control.CtrlLog ctrlLog;
		private System.Windows.Forms.ToolStripMenuItem miToolsCalib;
		private System.Windows.Forms.ToolStripMenuItem miCalibBattery;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
		private System.Windows.Forms.ToolStripMenuItem miCalibIMU;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;

	}
}

