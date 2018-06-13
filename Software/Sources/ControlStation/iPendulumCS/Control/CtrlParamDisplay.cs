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
/// \file	CtrlParamDisplay.cs
/// \author	Jean-Sebastien Gonsette
///
/// Control displaying the value of a parameter selected in the parameters tree
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

namespace iPendulumCS.Control
{
	public partial class CtrlParamDisplay : UserControl
	{
		// ###########################################################################
		//
		// D A T A
		//
		// ###########################################################################
		#region DATA

		/// List of tags giving the selected parameter
		private string [] selectedTag;

		/// Pendulum we display status from
		private Pendulum pendulum;
		private Font fontDescription;
		private Font fontValue;
		private Font fontParamName;
		private Font fontParamValue;
		
		#endregion



		// ###########################################################################
		//
		// P U B L I C
		//
		// ###########################################################################
		#region PUBLIC

		// ===========================================================================
		/// \brief	Control constructor
		///
		/// \param[in]	pendulum		Pendulum we display information from
		// ===========================================================================
		public CtrlParamDisplay (Pendulum pendulum)
		{
			InitializeComponent ( );

			// Connect to pendulum model
			this.pendulum = pendulum;
			pendulum.UpdatedEvent += EvPendulum_Updated;

			fontValue = lblParamValue.Font;
			fontDescription = new Font (lblParamValue.Font.FontFamily, 10.0f);
			fontParamName = new Font (lblParamValue.Font.FontFamily, 10.0f, FontStyle.Bold);
			fontParamValue = new Font (lblParamValue.Font.FontFamily, 11.0f, FontStyle.Italic);

			lblParamName.Text = "iPendulum CS";
			lblParamValue.Text = "";

			this.DoubleBuffered = true;
		}


		// ===========================================================================
		/// \brief	Define the parameter that must be displayed in the control
		///
		/// \param[in]	tag		Parameter tag ("token1.token2.token_n")
		// ===========================================================================
		public void SelectParam (string tag)
		{
			// Save new parameter tag (as a vector of tokens)
			if (tag == null) selectedTag = null;
			else selectedTag = tag.Split (new char [] { '.' });

			// Redraw the control
			Invalidate ( );
		}
		#endregion


		// ###########################################################################
		//
		// O V E R R I D E
		//
		// ###########################################################################
		#region OVERRIDE

		// ===========================================================================
		/// \brief	Job to perform when Paint event is raised
		///
		/// \param[in]	e		Paint event arguments
		// ===========================================================================
		protected override void OnPaint (PaintEventArgs e)
		{
			base.OnPaint (e);

			// Look at the first token of the Tag to know what category to display
			if (selectedTag == null) return;
			switch (selectedTag [0])
			{
				case "OS": DisplayOS (e, selectedTag); break;
				case "System": DisplaySystem (e, selectedTag); break;
				case "Battery": DisplayBattery (e, selectedTag); break;
				case "Sensors": DisplaySensors (e, selectedTag); break;
				case "State": DisplayState (e, selectedTag); break;
				case "Motor": DisplayMotor (e, selectedTag); break;

				default: break;
			}
		}
		#endregion



		// ###########################################################################
		//
		// P R I V A T E
		//
		// ###########################################################################
		#region PRIVATE
	
		// ===========================================================================
		/// \brief	Pendulum status changed. Control is repainted.
		///
		/// \param	pendulum	Pendulum.
		// ===========================================================================
		private void EvPendulum_Updated (Pendulum pendulum)
		{
			// Redraw the control if there is an update of the parameters
			Invalidate ( );
		}


		// ===========================================================================
		/// \brief	Draw a single line of the form "Param: value Unit"
		///
		/// \param	e
		/// \param	y			y coordinate to draw the line in the control
		/// \param	paramName	Name of the parameter
		/// \param	value		Value of the parameter
		/// \param	unit		Physical unit of the parameter
		// ===========================================================================
		private float DrawParamLine (PaintEventArgs e, float y, string paramName, string value, string unit)
		{
			float x = 10;
			SizeF size;

			if (Enabled == false) value = "-";

			e.Graphics.DrawString (paramName, fontParamName, Brushes.Black, x, y);
			x += e.Graphics.MeasureString (paramName, fontParamName).Width;

			x = Width-10;
			x -= e.Graphics.MeasureString (unit, fontParamValue).Width;
			e.Graphics.DrawString (unit, fontParamValue, Brushes.Black, x, y);

			size = e.Graphics.MeasureString (value, fontParamValue);
			x -= size.Width;
			e.Graphics.DrawString (value, fontParamValue, Brushes.Black, x, y);

			return size.Height * 1.1f;
		}


		// ===========================================================================
		/// \brief	Display information related to the pendulum hall sensors
		// ===========================================================================
		private void DisplaySensorsHall (PaintEventArgs e)
		{
			lblParamName.Text = "Hall sensors";
			lblParamValue.Text = "";

			DrawParamLine (e, 50, "Motor speed: ", (pendulum.SensorsMotorSpeed * (60.0f / (2.0f * Math.PI))).ToString ("F0"), "RPM");
		}


		// ===========================================================================
		/// \brief	Display information related to the accelerometer
		// ===========================================================================
		private void DisplaySensorsAcc (PaintEventArgs e)
		{
			float y = 50;
			lblParamName.Text = "Accelerometer sensors";
			lblParamValue.Text = "";

			y += DrawParamLine (e, y, "X axis: ", (1000.0f * pendulum.SensorsAccX / 9.81f).ToString ("F0"), "mg");
			y += DrawParamLine (e, y, "Y axis: ", (1000.0f * pendulum.SensorsAccY / 9.81f).ToString ("F0"), "mg");
			y += DrawParamLine (e, y, "Z axis: ", (1000.0f * pendulum.SensorsAccZ / 9.81f).ToString ("F0"), "mg");
		}


		// ===========================================================================
		/// \brief	Display information related to the gyro
		// ===========================================================================
		private void DisplaySensorsGyro (PaintEventArgs e)
		{
			lblParamName.Text = "Gyro. sensors";
			lblParamValue.Text = "";

			DrawParamLine (e, 50, "Z axis: ", (pendulum.SensorsGyroZ * (180f / (float) Math.PI)).ToString ("F0"), "°/s");
		}


		// ===========================================================================
		/// \brief	Display information about the OS memory
		///
		/// \param	tokens	Element to display, as a vector of tokens
		// ===========================================================================
		private void DisplayOsMemory (PaintEventArgs e, string [] tokens)
		{
			float y = 50;
			lblParamName.Text = "OS Memory";
			lblParamValue.Text = "";

			y += DrawParamLine (e, y, "Static: ", pendulum.OsMemoryStatic.ToString ( ), "Bytes");
			y += DrawParamLine (e, y, "Dynamic: ", pendulum.OsMemoryDynamic.ToString ( ) + " / " + pendulum.OsMemoryMaxDynamic.ToString ( ), "Bytes");
			y += DrawParamLine (e, y, "Kernel: ", pendulum.OsMemoryKernel.ToString ( ), "Bytes");
			y += DrawParamLine (e, y, "Free: ", pendulum.OsMemoryFree.ToString ( ), "Bytes");		
		}


		// ===========================================================================
		/// \brief	Display information about the OS kernel (memory, proc usage, etc.)
		///
		/// \param	tokens	Element to display, as a vector of tokens
		// ===========================================================================
		private void DisplayOsKernel (PaintEventArgs e, string [] tokens)
		{
			float y = 50;
			lblParamName.Text = "OS Kernel";
			lblParamValue.Text = "";

			y += DrawParamLine (e, y, "Processor: ", (pendulum.OsKernelProcUsage / 10.0f).ToString ("F1"), "%");
			y += DrawParamLine (e, y, "Idle time: ", (pendulum.OsIdleProcUsage / 10.0f).ToString ("F1"), "%");
		}


		// ===========================================================================
		/// \brief	Display information about the OS Tasks
		///
		/// \param	tokens	Element to display, as a vector of tokens
		// ===========================================================================
		private void DisplayOsTasks (PaintEventArgs e, string [] tokens)
		{
			float y = 50;
			float procUsage;

			if (tokens.Length == 3)
			{
				// Third token contains the task index
				uint idx = Convert.ToUInt16 (tokens [2]);

				lblParamName.Text = pendulum.OsTasks [idx].name;
				lblParamValue.Text = "";

				y += DrawParamLine (e, y, "Priority: ", pendulum.OsTasks [idx].priority.ToString ( ), "");
				y += DrawParamLine (e, y, "Stack size: ", pendulum.OsTasks [idx].stackUsage.ToString ( ) + " / " + pendulum.OsTasks [idx].stack.ToString ( ), "Bytes");

				procUsage = pendulum.OsTasks [idx].procUsage / 10.0f;
				y += DrawParamLine (e, y, "Processor: ", procUsage.ToString ("F1"), "%");
			}
		}


		// ===========================================================================
		/// \brief	Display information about the operating system
		///
		/// \param	tokens	Element to display, as a vector of tokens
		// ===========================================================================
		private void DisplayOS (PaintEventArgs e, string [] tokens)
		{
			if (tokens.Length == 1)
			{
				lblParamName.Text = "Operating System";
				DrawParamLine (e, 50, "Version: ", pendulum.OsVersion, "");
			}

			else
			{
				switch (tokens [1])
				{
					case "Memory": DisplayOsMemory (e, tokens); break;
					case "Tasks": DisplayOsTasks (e, tokens); break;
					case "Kernel": DisplayOsKernel (e, tokens); break;
					default: break;
				}
			}
		}


		// ===========================================================================
		/// \brief	Display information about the firmware
		///
		/// \param	tokens	Element to display, as a vector of tokens
		// ===========================================================================
		private void DisplaySystem (PaintEventArgs e, string [] tokens)
		{
			lblParamName.Text = "Firmware";
			DrawParamLine (e, 50, "Version: ", pendulum.SysVersion, "");			
		}


		// ===========================================================================
		/// \brief	Display information about the battery
		///
		/// \param	tokens	Element to display, as a vector of tokens
		// ===========================================================================
		private void DisplayBattery (PaintEventArgs e, string [] tokens)
		{
			float y = 50.0f;
			lblParamName.Text = "Battery";
			lblParamValue.Text = "";

			y += DrawParamLine (e, y, "Voltage: ", (pendulum.BatteryVoltage).ToString ("F1"), "V");
			DrawParamLine (e, y, "SOC: ", (pendulum.BatterySOC).ToString (), "%");
		}


		// ===========================================================================
		/// \brief	Display information about the sensors
		///
		/// \param	tokens	Element to display, as a vector of tokens
		// ===========================================================================
		private void DisplaySensors (PaintEventArgs e, string [] tokens)
		{
			if (tokens.Length == 1) return;

			else if (tokens [1] == "Hall") DisplaySensorsHall (e);
			else if (tokens [1] == "Acc") DisplaySensorsAcc (e);
			else if (tokens [1] == "Gyro") DisplaySensorsGyro (e);
		}


		// ===========================================================================
		/// \brief	Display information about the pendulum motor
		///
		/// \param	tokens	Element to display, as a vector of tokens
		// ===========================================================================
		private void DisplayMotor (PaintEventArgs e, string [] tokens)
		{
			lblParamName.Text = "Motor";
			lblParamValue.Text = "";

			DrawParamLine (e, 50, "Target current: ", (pendulum.MotorCurrent).ToString ("F1"), "A");
		}


		// ===========================================================================
		/// \brief	Display information about the state of the pendulum
		///
		/// \param	tokens	Element to display, as a vector of tokens
		// ===========================================================================
		private void DisplayState (PaintEventArgs e, string [] tokens)
		{
			if (tokens.Length == 1) return;

			else if (tokens [1] == "Free")
			{
				float y = 50;
				lblParamName.Text = "Free Observer";
				lblParamValue.Text = "";

				y += DrawParamLine (e, y, "Body angle: ", (pendulum.StateBodyAngle * (180f / (float) Math.PI)).ToString ("F0"), "°");
				y += DrawParamLine (e, y, "Body speed: ", (pendulum.StateBodySpeed * (180f / (float) Math.PI)).ToString ("F0"), "°/s");
				y += DrawParamLine (e, y, "Wheel speed: ", (pendulum.StateWheelSpeed * (60f / (2f * (float) Math.PI))).ToString ("F0"), "RPM");
			}		
		}
		#endregion


	}
}
