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
/// \file	FormIMU.cs
/// \author	Jean-Sebastien Gonsette
///
/// Form providing IMU calibration interface.
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
	public partial class FormCalibIMU : Form
	{
		private Pendulum pendulum;

		/// Calibration step
		private int step;

		/// Current step status
		private enum Status { Wait, Ready, Busy, Done };

		/// Status for the current step
		private Status status;

		/// Referance timestamp for pendulum movement detection
		private uint timestampRef;

		/// Accumulator for gyro calibration
		float gyroAccumulator;

		/// Accumulator for acc X calibration
		float [] accAccumulatorX = { 0, 0, 0, 0 };

		/// Accumulator for acc Y calibration
		float [] accAccumulatorY = { 0, 0, 0, 0 };

		/// Accumulator for acc Z calibration
		float [] accAccumulatorZ = { 0, 0, 0, 0 };

		/// Number of accumulated values
		int integrationCounter;

		/// Delegate definition for asynchronous update
		private delegate void DlgtUpdateControl ();

		/// Delegate for asynchronous update
		private DlgtUpdateControl dlgtUpdateControl;

		/// Number of samples required for a calibration step
		private const int N_SAMPLES = 100;



		// ###########################################################################
		//
		// P U B L I C
		//
		// ###########################################################################
		#region PUBLIC
	
		// ===========================================================================
		/// \brief		Constructor
		///
		/// \param		pendulum	Pendulum to calibrate
		// ===========================================================================	
		public FormCalibIMU (Pendulum pendulum)
		{
			InitializeComponent ( );
			
			this.pendulum = pendulum;

			// Regsiter to pendulum events
			pendulum.UpdatedEvent += EvPendulum_Update;

			// Delegates in order to support asynchronously form update
			dlgtUpdateControl = new DlgtUpdateControl (UpdateControl);

			// Reset calibration values
			pendulum.pendulumInterface.CalibrateIMU (new float [2] { 1f, 1f }, new float [3] { 0f, 0f, 0f }, 0f);

			step = 1;
			barProgress.Minimum = 0;
			barProgress.Maximum = N_SAMPLES;
		}

		#endregion



		// ###########################################################################
		//
		// P R I V A T E
		//
		// ###########################################################################
		#region PRIVATE

		// ===========================================================================
		/// \brief	Pendulum state changed event
		///
		/// Process the new sensor values for the current calibration step
		// ===========================================================================
		private void EvPendulum_Update (Pendulum pendulum)
		{
			bool r = false;

			// Nothing to do until the next step
			if (status == Status.Done) return;

			// Validate the conditions are met to make this calibration step
			if (step <= 2) r = CheckStepFaceDown ( );
			else r = CheckStepRightSide ( );

			// Not in position ?
			if (r == false)
			{
				timestampRef = pendulum.Timestamp;
				status = Status.Wait;
				RestartStep ( );
			}

			// Processin ongoing ?
			else if (status == Status.Busy)
			{
				if (step <= 2) r = ProcessStepFaceDown ( );
				else r = ProcessStepRightSide ( );

				if (r == true)
				{
					status = Status.Done;

					// Peform the final calibration at the end of the last step
					if (step == 4)
					{
						Calibrate ( );
						step = 5;
					}
				}
			}

			// 1 second stability ? We are ready to process this step
			else if (pendulum.Timestamp - timestampRef >= 1000)	status = Status.Ready;

			// Update the control to reflect changes
			BeginInvoke (dlgtUpdateControl);
		}


		// ===========================================================================
		/// \brief	Check the pendulum motor is face down
		// ===========================================================================
		private bool CheckStepFaceDown ()
		{
			// Check X, Y accelerometers are below 60mG and Z accelerometer is below -0.8G
			if (pendulum.SensorsAccX >= 80 * 9.81f / 1000.0f) return false;
			if (pendulum.SensorsAccY >= 80 * 9.81f / 1000.0f) return false;
			if (pendulum.SensorsAccX <= -80 * 9.81f / 1000.0f) return false;
			if (pendulum.SensorsAccY <= -80 * 9.81f / 1000.0f) return false;
			if (pendulum.SensorsAccZ >= -800 * 9.81 / 1000.0f) return false;
			return true;
		}


		// ===========================================================================
		/// \brief	Check the pendulum is on its right side
		// ===========================================================================
		private bool CheckStepRightSide ()
		{
			// Check X, Y accelerometers are at 0.7G and Z accelerometer is near 0G
			if (pendulum.SensorsAccX <= -800 * 9.81f / 1000.0f) return false;
			if (pendulum.SensorsAccX >= -600 * 9.81f / 1000.0f) return false;
			if (pendulum.SensorsAccY >= 800 * 9.81f / 1000.0f) return false;
			if (pendulum.SensorsAccY <= 600 * 9.81f / 1000.0f) return false;
			if (pendulum.SensorsAccZ >= 150 * 9.81 / 1000.0f) return false;
			if (pendulum.SensorsAccZ <= -150 * 9.81 / 1000.0f) return false;
			return true;
		}


		// ===========================================================================
		/// \brief	Reset the accumulator for the current step of the calibration
		// ===========================================================================
		private void RestartStep ()
		{
			integrationCounter = 0;

			if (step >= 1 && step <= 2)
			{
				accAccumulatorX [step - 1] = 0;
				accAccumulatorY [step - 1] = 0;
			}
			else
			{
				accAccumulatorX [step - 1] = 0;
				accAccumulatorY [step - 1] = 0;
				accAccumulatorZ [step - 1] = 0;
				if (step == 3) gyroAccumulator = 0;
			}
		}


		// ===========================================================================
		/// \brief	Integrate accelerometers when the pendulum is motor face down
		// ===========================================================================
		private bool ProcessStepFaceDown ()
		{
			integrationCounter++;

			accAccumulatorX [step - 1] += pendulum.SensorsAccX;
			accAccumulatorY [step - 1] += pendulum.SensorsAccY;

			if (integrationCounter >= N_SAMPLES) return true;
			return false;
		}


		// ===========================================================================
		/// \brief	Integrate accelerometers when the pendulum is on its right side
		// ===========================================================================
		private bool ProcessStepRightSide ()
		{
			integrationCounter++;

			accAccumulatorX [step - 1] += pendulum.SensorsAccX;
			accAccumulatorY [step - 1] += pendulum.SensorsAccY;
			accAccumulatorZ [step - 1] += pendulum.SensorsAccZ;
			if (step == 3) gyroAccumulator += pendulum.SensorsGyroZ;

			if (integrationCounter >= N_SAMPLES) return true;
			return false;
		}


		// ===========================================================================
		/// \brief	Perform the final calibration on the basis the measurements
		// ===========================================================================
		private void Calibrate ()
		{
			float xBias, yBias, zBias, gBias;
			float x1, x2, y1, y2;
			float sx, dx, sy, dy;
			float k, theta, kx, ky;

			// Compute bias for the 3 acc axis
			xBias = (accAccumulatorX [0] + accAccumulatorX [1]) / (2*N_SAMPLES);
			yBias = (accAccumulatorY [0] + accAccumulatorY [1]) / (2*N_SAMPLES);
			zBias = (accAccumulatorZ [2] + accAccumulatorZ [3]) / (2*N_SAMPLES);

			// Compute gyro bias
			gBias = gyroAccumulator / N_SAMPLES;

			// Remove bias from step 2 and 3 mean values
			x1 = (accAccumulatorX [2] / N_SAMPLES) - xBias;
			x2 = (accAccumulatorX [3] / N_SAMPLES) - xBias;
			y1 = (accAccumulatorY [2] / N_SAMPLES) - yBias;
			y2 = (accAccumulatorY [3] / N_SAMPLES) - yBias;

			//
			sx = x1 + x2;
			sy = y1 + y2;
			dx = x1 - x2;
			dy = y2 - y1;

			// Recover kx / ky
			k = (float) Math.Sqrt ((float) (sx * dx / (sy * dy)));

			// Recover pendulum angle
			theta = -(float) Math.Atan (Math.Sqrt ((float) (sx * dy / (sy * dx))));

			// Recover kx and ky
			ky = (sx - sy) / (2.0f * 9.81f * (float)(k * Math.Sin (theta) - Math.Cos (theta)));
			kx = k * ky;

			kx = ky = 1.0f;
			pendulum.pendulumInterface.CalibrateIMU (new float [2] { kx, ky }, new float [3] { xBias, yBias, zBias }, gBias);
		}


		// ===========================================================================
		/// \brief	Refresh the control on the basis of the last calibration status
		// ===========================================================================
		private void UpdateControl ()
		{			
			// Update instruction according to current step
			if (step == 1)
			{
				txtInstruction.Text = "Direct the pendulum motor face down, like on the drawing below";
				picture.Image = Properties.Resources.Front_Down;
			}
			else if (step == 2)
			{
				txtInstruction.Text = "Let the motor face down and turn it half a turn";
				picture.Image = Properties.Resources.Back_Down;
			}
			else if (step == 3)
			{
				txtInstruction.Text = "Move the pendulum on its right side";
				picture.Image = Properties.Resources.Front_Right;
			}
			else if (step == 4)
			{
				txtInstruction.Text = "Let the pendulum on its right side and turn it half a turn";
				picture.Image = Properties.Resources.Back_Right;
			}
			else if (step == 5)
			{
				txtInstruction.Text = "Calibration complete !";
				bttNext.Text = "Close";
			}

			// Update instructions color according to current status
			if (status == Status.Wait) txtInstruction.BackColor = Color.Pink;
			else if (status == Status.Busy) txtInstruction.BackColor = Color.Moccasin;
			else if (status == Status.Ready) txtInstruction.BackColor = SystemColors.Window;
			else if (status == Status.Done) txtInstruction.BackColor = Color.LightGreen;

			// Step number
			lblLevel.Text = step.ToString ( );

			// Step progress
			barProgress.Value = integrationCounter;
		
			// Update button to the status
			bttValidate.Enabled = (status == Status.Ready);
			bttNext.Enabled = (status == Status.Done);
		}


		// ===========================================================================
		/// \brief	Handle click on button "Next" or "Validate"
		// ===========================================================================
		private void EvButton_Click (object sender, EventArgs e)
		{
			// Ask for next step
			if (sender == bttNext && status == Status.Done)
			{
				// Finish ?
				if (step >= 5) Close ( );

				// Move on nex step
				step++;
				timestampRef = pendulum.Timestamp;
				integrationCounter = 0;
				status = Status.Wait;
			}

			// Validate for this step
			else if (sender == bttValidate && status == Status.Ready)
			{
				integrationCounter = 0;
				status = Status.Busy;
			}

			UpdateControl ( );
		}


		// ===========================================================================
		/// \brief	Form about to close
		// ===========================================================================
		private void EvForm_Closing (object sender, FormClosingEventArgs e)
		{
			// Unregister the form
			pendulum.UpdatedEvent -= EvPendulum_Update;
		}


		// ===========================================================================
		/// \brief	Instructions text box get the focus
		///
		/// Let another item get the focus to avoid the text being selected
		// ===========================================================================
		private void EvInstructions_Enter (object sender, EventArgs e)
		{
			lblLevel.Focus ( );
		}

		#endregion
	}
}
