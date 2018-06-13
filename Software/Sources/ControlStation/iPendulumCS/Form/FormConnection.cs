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
/// \file	FormConnection.cs
/// \author	Jean-Sebastien Gonsette
///
/// Form providing an interface to connect to the pendulum through a user 
/// selected UART port.
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

namespace iPendulumCS
{
	/// Form that enables to choose the serial port to connect to the pendulum
	public partial class FormConnection : Form
	{
		/// Serial port used to open the connection
		SerialPort port;

		/// Timer to refresh the dialog
		Timer t = new Timer ( );



		// ###########################################################################
		//
		// P U B L I C
		//
		// ###########################################################################
		#region PUBLIC

		// ===========================================================================
		// <FormConnection>
		///
		/// \brief		Constructor
		// ===========================================================================
		public FormConnection ()
		{
			string [] names;

			InitializeComponent ( );
			lblError.Text = "";

			// Configure the timer for error display
			t.Interval = 1500;
			t.Tick += EvTimer_Tick;

			// Get all known serial ports and put them in the combo
			names = SerialPort.GetPortNames ( );
			
			if (names == null || names.Count () == 0) names = new string [] {"No serial port found"};
			cbPorts.Items.AddRange (names);
			cbPorts.SelectedIndex = 0;
		}


		// ===========================================================================
		// <GetPort>
		///
		/// \brief		Return the serial port opened after user choice
		///
		/// \return		Serial Port. Null if a valid port could not be found.
		// ===========================================================================
		public SerialPort GetPort ()
		{
			if (port != null && port.IsOpen == false) return null;
			return port;
		}
		#endregion



		// ###########################################################################
		//
		// P R I V A TE
		//
		// ###########################################################################
		#region PRIVATE

		// ===========================================================================
		// <EvButton_Connect>
		///
		/// \brief		Event: Button "Connect" has been pressed
		// ===========================================================================
		private void EvButton_Connect (object sender, EventArgs e)
		{
			if (cbPorts.Text == null || cbPorts.Text == "") return;
			
			cbPorts.Enabled = false;
			bttConnect.Enabled = false;
			lblError.Text = "";

			// Open port with the required configuration
			port = new SerialPort (cbPorts.Text, 115200, Parity.Even, 8, StopBits.One);
			try { port.Open ( ); }
			catch (UnauthorizedAccessException) { lblError.Text = "Unauthorize acess to this port"; }
			catch (ArgumentException) { lblError.Text = "Invalid port name"; }
			catch (InvalidOperationException) { lblError.Text = "This port is already opened"; }
			catch (System.IO.IOException) { lblError.Text = "This port doesn't exist"; }

			// Check there is someone responding
			if (port.IsOpen && PendulumInterface.TestConnection (port) == false)
			{
				lblError.Text = "Remote device isn't responding";
				port.Close ( );
			}
			
			// Connection failed
			if (port.IsOpen == false)
			{
				cbPorts.Enabled = true;
				bttConnect.Enabled = true;
			}
			// Success
			else
			{
				lblError.ForeColor = Color.Green;
				lblError.Text = "Connection successful";
			}
						
			t.Start ( );
		}


		// ===========================================================================
		// <EvTimer_Tick>
		///
		/// \brief		Event: Timer ellapsed, close the dialog if needed
		// ===========================================================================
		void EvTimer_Tick (object sender, EventArgs e)
		{
			t.Stop ( );

			// Close the dialog if port connected
			if (port.IsOpen == true) { this.Close ( ); }
		}
		#endregion
	}
}
