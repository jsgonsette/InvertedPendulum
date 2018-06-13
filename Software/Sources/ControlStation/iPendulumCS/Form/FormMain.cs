// ###########################################################################
//
// Copyright (C) 2015 GenId SPRL
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
///
/// \file	FormMain.cs
/// \author	Jean-Sebastien Gonsette
///
/// Main Application form
// ###########################################################################

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

using iPendulumCS.Control;

namespace iPendulumCS
{
	/// Main application form
	public partial class FormMain : Form
	{
		#region DATA
		
		/// Pendulum interface with the hardware
		private PendulumInterface pendulumInterface;

		/// Pendulum
		private Pendulum pendulum;

		/// Control drawing the pendulum state
		private CtrlPendulum ctrlPendulum = null;

		/// Control displaying one pendulum parameter at a time
		private CtrlParamDisplay ctrlParamDisplay = null;

		/// Get 1 TM out of 5
		private const int TM_RATE = 5;
		#endregion

		

		// ###########################################################################
		//
		// P U B L I C
		//
		// ###########################################################################

		// ===========================================================================
		/// \brief Constructor
		// ===========================================================================
		public FormMain ()
		{
			// Instantiate the application components
			pendulumInterface = new PendulumInterface ();
			pendulum = new Pendulum (pendulumInterface);

			// Instantiate HMI components
			ctrlPendulum = new CtrlPendulum (pendulum);
			ctrlParamDisplay = new CtrlParamDisplay (pendulum);

			// Designer
			InitializeComponent ( );

			// HMI disposal
			splitMain.Panel2.Controls.Add (ctrlPendulum);
			splitMain.Panel1.Controls.Add (ctrlParamDisplay);

			ctrlPendulum.Width = splitMain.Panel2.Width;
			ctrlPendulum.Height = splitMain.Panel2.Height;
			ctrlPendulum.Enabled = false;

			ctrlParamDisplay.Dock = DockStyle.Top;

			// Linking
			ctrlLog.StartLoggingEvent += EvLog_StartLogging;
			ctrlLog.StopLoggingEvent += EvLog_StopLogging;
			pendulumInterface.ConnectionChangedEvent += EvInterface_ConnectionChanged;
			ctrlScopes.AttachPendulum (pendulum);

			UpdateMenuState ( );
		}

	

	
		// ###########################################################################
		//
		// E V E N T S
		//
		// ###########################################################################
		#region Events

		// ===========================================================================
		/// \brief Intercept "FormClosing" event in order to dispose some objects
		// ===========================================================================
		protected override void OnFormClosing (FormClosingEventArgs e)
		{
			pendulumInterface.Dispose ( );
			base.OnFormClosing (e);
		}

		
		// ===========================================================================
		/// \brief Handle "start logging" event
		// ===========================================================================
		void EvLog_StartLogging (object sender, string filePath)
		{
			pendulumInterface.StartLogging (filePath);
		}


		// ===========================================================================
		/// \brief Handle "stop logging" event
		// ===========================================================================
		void EvLog_StopLogging (object sender)
		{
			pendulumInterface.StopLogging ();
		}


		// ===========================================================================
		/// \brief Handle "UART disconnected" event
		// ===========================================================================
		void EvInterface_ConnectionChanged (object sender, bool status)
		{
			UpdateMenuState ( );
		}


		// ===========================================================================
		/// \brief Handle "Connection to pendulum" menu event
		// ===========================================================================
		private void EvMenu_Connection (object sender, EventArgs e)
		{
			FormConnection fm = new FormConnection ( );
			SerialPort port;

			// If successfull we should get back a valid serial port
			fm.ShowDialog ( );
			port = fm.GetPort ( );
			fm.Dispose ( );

			// In this case, we inform the serial interface
			if (port != null)
			{
				pendulumInterface.Connect (port);
				pendulumInterface.SetTelemetryRate (TM_RATE);
				pendulumInterface.SetControlMode (PendulumInterface.CtrlMode.Free);
			}
		}


		// ===========================================================================
		/// \brief Handle "Menu->Disconnect" event
		// ===========================================================================
		private void EvMenu_Disconnect (object sender, EventArgs e)
		{
			pendulumInterface.Disconnect ( );
		}


		// ===========================================================================
		/// \brief Handle "Menu->About" event
		// ===========================================================================
		private void EvMenu_About (object sender, EventArgs e)
		{
			FormAbout fm = new FormAbout ( );

			fm.ShowDialog ( );
			fm.Dispose ( );
		}


		// ===========================================================================
		/// \brief Handle "Menu->Exit" event
		// ===========================================================================
		private void EvMenu_Exit (object sender, EventArgs e)
		{
			Close ( );
		}


		// ===========================================================================
		/// \brief Handle "Menu->Tools->Calibration" event
		// ===========================================================================
		private void EvMenuCalib (object sender, EventArgs e)
		{
			ToolStripMenuItem mi = sender as ToolStripMenuItem;

			if (mi == miCalibBattery)
			{
				FormCalibBattery fcb = new FormCalibBattery (pendulum);
				fcb.ShowDialog ( );
			}
			else if (mi == miCalibIMU)
			{
				FormCalibIMU fci = new FormCalibIMU (pendulum);
				fci.ShowDialog ( );
			}				
		}
		

		// ===========================================================================
		/// \brief Handle "Menu->Tools->Mode" event
		// ===========================================================================
		private void EvMenu_SetMode (object sender, EventArgs e)
		{
			ToolStripMenuItem mi = sender as ToolStripMenuItem;
			
			if (mi == miToolsModeOff) pendulumInterface.SetControlMode (PendulumInterface.CtrlMode.Stop);
			else if (mi == miToolsModeFree) pendulumInterface.SetControlMode (PendulumInterface.CtrlMode.Free);
			else if (mi == miToolsModeEquilibrium) pendulumInterface.SetControlMode (PendulumInterface.CtrlMode.Equilibrium);

			if (mi == miToolsModeOff) pendulumInterface.SetTelemetryRate (0);
			else pendulumInterface.SetTelemetryRate (TM_RATE);

			UpdateMenuState ( );
		}


		// ===========================================================================
		/// \brief Handle "Menu->Tools->Manual Motor" event
		// ===========================================================================
		private void EvMenuTools_Manual (object sender, EventArgs e)
		{
			FormMotorManual fmm = new FormMotorManual (pendulum);
			fmm.Show ( );
		}


		// ===========================================================================
		/// \brief Handle "Selection of a tree item" event
		// ===========================================================================
		private void EvTreeParameters_AfterSelect (object sender, TreeViewEventArgs e)
		{
			string tag;

			if (e.Node.Tag == null) tag = null;
			else tag = e.Node.Tag as string;

			// Select the parameter to show in 'CtrlParamDisplay'
			ctrlParamDisplay.SelectParam (tag);

			//  Update the tree to reflect the right number of tasks
			if (tag != null && tag.Equals ("OS.Tasks"))
			{
				TreeNode [] tn = treeParameters.Nodes.Find ("tiTasks", true);
				if (tn.Length != pendulum.OsTasks.Length)
				{
					PendulumInterface.Task [] tasks = pendulum.OsTasks;
					
					for (int i = 0; i < tasks.Length; i++)
					{
						// Create a new tree node if it doesn't exist
						if (i >= tn [0].Nodes.Count)
						{
							TreeNode n = new TreeNode ( );
							tn [0].Nodes.Add (n);
						}

						// Update task name
						if (tn [0].Nodes [i].Text.Equals (tasks [i].name) == false)
						{
							tn [0].Nodes [i].Text = tasks [i].name;
							tn [0].Nodes [i].Tag = "OS.Tasks." + i.ToString ( );
						}
					}
				}
			}

		}


		// ===========================================================================
		/// \brief Handle Scope button events
		// ===========================================================================
		private void EvScopeBtt_Click (object sender, EventArgs e)
		{
			if (ctrlScopes.Enabled)
			{
				if (sender == bttAll) ctrlScopes.ZoomGlobal();
				else if (sender == bttPlay) ctrlScopes.Follow();
				else if (sender == bttZoomIn) ctrlScopes.ZoomIn();
				else if (sender == bttZoomOut) ctrlScopes.ZoomOut();
			}
		}


		// ===========================================================================
		/// \brief Handle modification of the split bar psotion
		// ===========================================================================
		private void splitMain_Panel2_SizeChanged (object sender, EventArgs e)
		{
			// Adjust 'CtrlPendulum' size
			if (ctrlPendulum != null)
			{
				ctrlPendulum.Width = splitMain.Panel2.Width;
				ctrlPendulum.Height = splitMain.Panel2.Height;
			}
		}


		// ===========================================================================
		/// \brief Handle modification of the form size
		// ===========================================================================
		private void EvForm_SizeChanged (object sender, EventArgs e)
		{
			treeParameters.Height = ClientSize.Height - 250;
			ctrlParamDisplay.Height = 250;
		}


		#endregion



		// ###########################################################################
		//
		// P R I V A T E
		//
		// ###########################################################################
		#region Private
	
		// ===========================================================================
		/// \brief Update the form when pendulum connection status change
		// ===========================================================================
		private void UpdateMenuState ()
		{
			bool connected;

			// Depending on connection state:
			connected = pendulumInterface.IsConnected ( );
			miToolsConnect.Enabled = !connected;
			miToolsDisconnect.Enabled = connected;
			miToolsMode.Enabled = connected;
			miToolsManual.Enabled = connected;
			miToolsCalib.Enabled = connected;
			ctrlParamDisplay.Enabled = connected;
			ctrlPendulum.Enabled = connected;
			ctrlScopes.Enabled = connected;
			
			if (connected)
			{
				miToolsModeEquilibrium.Checked = false;
				miToolsModeFree.Checked = false;
				miToolsModeOff.Checked = false;

				switch (pendulum.pendulumInterface.GetControlMode ())
				{
					case PendulumInterface.CtrlMode.Equilibrium: miToolsModeEquilibrium.Checked = true; break;
					case PendulumInterface.CtrlMode.Free: miToolsModeFree.Checked = true; break;
					case PendulumInterface.CtrlMode.Stop: miToolsModeOff.Checked = true; break;
				}
			}
		}
		#endregion

	
		private void ScopeHorizontalSplitters_SplitterMoved (object sender, SplitterEventArgs e)
		{
			// Ensure both spliters have the same position
			//splitScopesH1.SplitterDistance = (sender as SplitContainer).SplitterDistance;
		//	splitScopesH2.SplitterDistance = (sender as SplitContainer).SplitterDistance;
		}	

	}
}
