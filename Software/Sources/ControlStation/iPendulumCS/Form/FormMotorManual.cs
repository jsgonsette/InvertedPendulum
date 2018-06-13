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
/// \file	FormMotorManual.cs
/// \author	Jean-Sebastien Gonsette
///
/// Form providing an interface to manually drive the pendulum motor.
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
	public partial class FormMotorManual : Form
	{
		/// Pendulum
		private Pendulum pendulum = null;

		/// Torque level [%]
		private int torque;

		/// Motor speed [RPM]
		private float rpm;

		private bool brake;
		private bool forward;
		private bool enabled;

		/// Delegate definition for asynchronous update
		private delegate void DlgtUpdateForm ();

		/// Delegate for asynchronous update
		private DlgtUpdateForm dlgtUpdateForm;



		// ###########################################################################
		//
		// P U B L I C
		//
		// ###########################################################################

		#region PUBLIC

		// ===========================================================================
		/// \brief		Constructor
		// ===========================================================================
		public FormMotorManual (Pendulum pendulum)
		{
			InitializeComponent ( );

			// Delegates in order to support asynchronously form update
			dlgtUpdateForm = new DlgtUpdateForm (UpdateForm);

			// Initial state
			torque = 0;
			enabled = false;
			brake = false;
			forward = true;
			lblTorque.Text = "0 %";
			txtRPM.Enabled = true;

			// Get events from the pendulum
			pendulum.UpdatedEvent += EvPendulum_Update;
			this.pendulum = pendulum;
		}

	
		#endregion



		// ###########################################################################
		//
		// P R I V A TE
		//
		// ###########################################################################
		#region PRIVATE

		// ===========================================================================
		/// \brief		Handle pendulum update event
		///
		/// Refresh motor speed
		// ===========================================================================
		private void EvPendulum_Update (Pendulum pendulum)
		{
			if (pendulum == null) return;
			
			// Get the speed activate synchronous update
			rpm = pendulum.StateWheelSpeed * 60.0f / (2.0f * (float) Math.PI);
			BeginInvoke (dlgtUpdateForm);
		}


		// ===========================================================================
		/// \brief		Handle click on "brake" button
		// ===========================================================================
		private void EvBtt_Brake (object sender, EventArgs e)
		{
			brake = !brake;
			enabled = false;
			
			PendulumInterface.MotorManualControl ctrl;
			if (brake) ctrl = PendulumInterface.MotorManualControl.Brake;
			else ctrl = PendulumInterface.MotorManualControl.Disabled;

			Invalidate ( );
			pendulum.pendulumInterface.ManualMotorControl (ctrl, 0);
		}

	
		// ===========================================================================
		/// \brief		Handle click on "Forward" button
		// ===========================================================================
		private void EvBtt_Fwd (object sender, EventArgs e)
		{
			if (enabled && forward) enabled = false;
			else
			{
				enabled = true;
				forward = true;
			}
			brake = false;
			Invalidate ( );

			PendulumInterface.MotorManualControl ctrl;
			if (enabled) ctrl = PendulumInterface.MotorManualControl.Forward;
			else ctrl = PendulumInterface.MotorManualControl.Disabled;
			pendulum.pendulumInterface.ManualMotorControl (ctrl, this.torque);
		}


		// ===========================================================================
		/// \brief		Handle click on "Reverse" button
		// ===========================================================================
		private void EvBtt_Reverse (object sender, EventArgs e)
		{
			if (enabled && !forward) enabled = false;
			else
			{
				enabled = true;
				forward = false;
			}
			brake = false;
			Invalidate ( );

			PendulumInterface.MotorManualControl ctrl;
			if (enabled) ctrl = PendulumInterface.MotorManualControl.Reverse;
			else ctrl = PendulumInterface.MotorManualControl.Disabled;
			pendulum.pendulumInterface.ManualMotorControl (ctrl, this.torque);
		}


		// ===========================================================================
		/// \brief		Handle click on buttons to change torque level
		// ===========================================================================
		private void EvBtt_Torque (object sender, EventArgs e)
		{
			if (sender == bttMinusFine) torque--;
			else if (sender == bttPlusFine) torque++;
			else if (sender == bttPlusRough) torque += 10;
			else if (sender == bttMinusRough) torque -= 10;

			torque = Math.Min (torque, 100);
			torque = Math.Max (torque, 0);

			lblTorque.Text = torque.ToString ( ) + " %";

			if (enabled)
			{
				PendulumInterface.MotorManualControl ctrl;
				
				if (enabled == false) ctrl = PendulumInterface.MotorManualControl.Disabled;
				else if (forward) ctrl = PendulumInterface.MotorManualControl.Forward;
				else ctrl = PendulumInterface.MotorManualControl.Reverse;
				
				pendulum.pendulumInterface.ManualMotorControl (ctrl, this.torque);
			}
		}


		// ===========================================================================
		/// \brief		Update form controls
		// ===========================================================================
		private void UpdateForm ()
		{
			txtRPM.Text = rpm.ToString ("F0") + " RPM ";
		}

		#endregion PRIVATE




		// ###########################################################################
		//
		// P R O T E C T E D
		//
		// ###########################################################################
		#region PROTECTED

		// ===========================================================================
		/// \brief		Form is about to close
		// ===========================================================================
		protected override void OnClosing (CancelEventArgs e)
		{
			base.OnClosing (e);

			// Unregister for the events
			pendulum.UpdatedEvent -= EvPendulum_Update;
		}


		// ===========================================================================
		/// \brief		Repaint form
		// ===========================================================================
		protected override void OnPaint (PaintEventArgs e)
		{
			base.OnPaint (e);

			float x, y;
			y = bttReverse.Location.Y - 30;

			if (enabled && !forward)
			{
				x = bttReverse.Location.X + bttReverse.Width / 2 - 10;
				e.Graphics.FillEllipse (Brushes.LightGreen, x, y, 20, 20);
			}
			if (brake)
			{
				x = bttBrake.Location.X + bttBrake.Width / 2 - 10;
				e.Graphics.FillEllipse (Brushes.Red, x, y, 20, 20);
			}
			if (enabled && forward)
			{
				x = bttFwd.Location.X + bttFwd.Width / 2 - 10;
				e.Graphics.FillEllipse (Brushes.LightGreen, x, y, 20, 20);
			}
		}
		#endregion


	}
}
