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
/// \file	FormCalibBattery.cs
/// \author	Jean-Sebastien Gonsette
///
/// Form providing battery calibration interface.
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

namespace iPendulumCS
{
	public partial class FormCalibBattery : Form
	{
		private Pendulum pendulum;
		private Timer timer;

		// ===========================================================================
		/// \brief		Constructor
		///
		/// \param		pendulum	Pendulum to calibrate
		// ===========================================================================	
		public FormCalibBattery (Pendulum pendulum)
		{
			InitializeComponent ( );
			
			this.pendulum = pendulum;

			// Timer for update
			timer = new Timer ( );
			timer.Interval = 1000;
			timer.Tick += EvTimer_Tick;
			timer.Start ( );
		}



		// ###########################################################################
		//
		// P R I V A T E
		//
		// ###########################################################################
		#region PRIVATE

		// ===========================================================================
		/// \brief		Timer event to refresh the form
		// ===========================================================================	
		void EvTimer_Tick (object sender, EventArgs e)
		{
			txtVoltageRaw.Text = pendulum.BatteryVoltage.ToString ("F2");
		}


		// ===========================================================================
		/// \brief		Calibrate button click event
		// ===========================================================================	
		private void EvButton_Click (object sender, EventArgs e)
		{
			float voltage;

			try { voltage = Convert.ToSingle (txtVoltage.Text); }
			catch (Exception)
			{
				txtVoltage.BackColor = Color.Pink;
				return;
			}

			txtVoltage.BackColor = Color.LightGreen;
			pendulum.pendulumInterface.CalibrateBattery (voltage);
		}


		// ===========================================================================
		/// \brief		Event when cursor is activated in text box
		// ===========================================================================	
		private void EvVoltage_Enter (object sender, EventArgs e)
		{
			txtVoltage.BackColor = SystemColors.Window;
		}

		#endregion
	}
}
