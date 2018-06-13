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
/// \file	CtrlLog.cs
/// \author	Jean-Sebastien Gonsette
///
/// Control providing an interface to select a log file.
// ###########################################################################

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace iPendulumCS.Control
{
	/// HMI Control used to manage log file
	public partial class CtrlLog : UserControl
	{
		/// Control behaves as if logging is activated
		private bool logging = false;

		/// Original corlo of the text box before we change it
		private Color colorOriginal;



		// ###########################################################################
		//
		// P U B L I C
		//
		// ###########################################################################

		/// Handler of the StartLoggingEvent event
		public delegate void StartLoggingHandler (object sender, string filePath);

		/// Notification that the user asked to start to log 
		public event StartLoggingHandler StartLoggingEvent;

		/// Handler of the StopLoggingEvent event
		public delegate void StopLoggingHandler (object sender);

		/// Notification that the user asked to stop to log
		public event StopLoggingHandler StopLoggingEvent;


		// ===========================================================================
		/// \brief	Initialize the control
		// ===========================================================================
		public CtrlLog ()
		{
			InitializeComponent ( );
		}


		// ===========================================================================
		/// \brief	Update the control to reflect that logging is stopped
		// ===========================================================================
		public void StopLogging ()
		{
			Stop ( );
			if (StopLoggingEvent != null) StopLoggingEvent (this);
		}


		// ===========================================================================
		/// \brief	Selected Log file path property
		// ===========================================================================
		public string LogFile
		{
			get;
			private set;
		}

		

		// ###########################################################################
		//
		// P R I V A T E
		//
		// ###########################################################################
		#region PRIVATE

		// ===========================================================================
		/// \brief	Adapt control content to the new dimensions of the container
		///
		/// \param	e	Parameters of the event
		// ===========================================================================
		protected override void OnSizeChanged (EventArgs e)
		{
			gbLog.Width = Width - 10;
			bttLog.Location = new Point (gbLog.Width - bttLog.Width - 10, bttLog.Location.Y);
			bttBrowse.Location = new Point (bttLog.Location.X - bttBrowse.Width - 10, bttBrowse.Location.Y);
			tbPath.Width = bttBrowse.Location.X - tbPath.Location.X - 10;
			base.OnSizeChanged (e);
		}


		// ===========================================================================
		/// \brief	Handle click on button "Browse"
		///
		/// \param	sender	Button that sent the event
		/// \param	e		Parameters of the event
		// ===========================================================================
		private void EvButton_Browse (object sender, EventArgs e)
		{
			SaveFileDialog sfd = new SaveFileDialog ();

			sfd.FileName = tbPath.Text;
			sfd.AddExtension = true;
			sfd.OverwritePrompt = true;
			sfd.CheckPathExists = true;
			sfd.DefaultExt = "log";
			sfd.Filter = "log files (*.log)|*.log";
			sfd.ShowDialog ( );

			tbPath.Text = sfd.FileName;
		}


		// ===========================================================================
		/// \brief	Handle click on button "Log"
		///
		/// \param	sender	Button that sent the event
		/// \param	e		Parameters of the event
		// ===========================================================================
		private void EvButton_Log (object sender, EventArgs e)
		{
			if (logging == false)
			{
				// Open the file to check it is valid. If yes, update the control
				try
				{
					using (FileStream fs = new FileStream (tbPath.Text, FileMode.Append))
					{
						tbPath.Enabled = false;
						colorOriginal = tbPath.BackColor;
						tbPath.BackColor = Color.PaleGreen;
						bttBrowse.Enabled = false;
						logging = true;
						bttLog.Text = "Stop logging";
					}
					if (StartLoggingEvent != null) StartLoggingEvent (this, tbPath.Text);
				}
				catch (Exception)
				{
					tbPath.ForeColor = Color.Red;
					MessageBox.Show ("Cannot open selected log file", "Invalid file name", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				}
			}

			else
			{
				Stop ( );
				if (StopLoggingEvent != null) StopLoggingEvent (this);
			}
		}


		// ===========================================================================
		/// \brief	File path changed
		///
		/// \param	sender	Item that sent the event
		/// \param	e		Parameters of the event
		// ===========================================================================
		private void EvText_TextChanged (object sender, EventArgs e)
		{
			tbPath.ForeColor = Color.Black;
		}


		// ===========================================================================
		/// \brief	Update the control so that logging appear to be stopped
		// ===========================================================================
		private void Stop ()
		{
			tbPath.Enabled = true;
			bttBrowse.Enabled = true;
			logging = false;
			bttLog.Text = "Start logging";
			tbPath.BackColor = colorOriginal;
		}
		#endregion
	}
}
