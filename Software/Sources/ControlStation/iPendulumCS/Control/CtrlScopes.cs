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
/// \file	CtrlScopes.cs
/// \author	Jean-Sebastien Gonsette
///
/// Control displaying 4 scopes with the history of the pendulum main
/// data: angle, speed, etc.
// ###########################################################################

using System;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace iPendulumCS.Control
{
	public partial class CtrlScopes : UserControl
	{
		/// All the charts
		Chart [] charts;

		/// Scrollbar common for all the graphs
		private HScrollBar scrollBar = new HScrollBar ( );

		/// Mouse click coordinate 
		private int mouseClickX = -1;

		/// Pendulum attached to the scopes
		private Pendulum pendulum;

		/// Flag if the scopes slide automatically to follow fresh data
		private bool follow = true;

		/// Flag if it is necessary to rebind with the data
		private bool rebind = false;

		/// The most 
		private float bindLimit;

		/// X axis view start position
		private float viewPosition;

		/// X axis minimum start position
		private float viewPositionMin;

		/// X axis view size
		private float viewSize;

		/// X axis maximum view size
		private float viewSizeMax;

		/// Scopes data
		private List<float> dataTimestamp = new List<float> ( );
		private List<float> dataBodyAngle = new List<float> ( );
		private List<float> dataBodySpeed = new List<float> ( );
		private List<float> dataWheelSpeed = new List<float> ( );
		private List<float> dataWheelTorque = new List<float> ( );
		private List<float> dataMotorSpeed = new List<float> ( );
		private List<float> dataAccX = new List<float> ( );
		private List<float> dataAccY = new List<float> ( );
		private List<float> dataGyroZ = new List<float> ( );

		/// 
		private delegate void DlgtUpdateCharts ();
		private DlgtUpdateCharts dlgtUpdateCharts;



		// ###########################################################################
		//
		// P U B L I C
		//
		// ###########################################################################
		#region PUBLIC

		// ===========================================================================
		/// \brief Constructor
		// ===========================================================================
		public CtrlScopes ()
		{
			InitializeComponent ( );
			
			charts = new Chart [] { chartSensorsMotor, chartBody, chartWheel, chartSensorsIMU };

			// Add a scroll bar in the bottom of the control
			scrollBar.Maximum = 1000;
			scrollBar.Minimum = 0;
			scrollBar.Dock = DockStyle.Bottom;
			scrollBar.Scroll += EvScrollBar_Scroll;
			Controls.Add (scrollBar);

			// Ensure graphs are empty
			chartBody.Series ["dataAngle"].Points.Clear ( );
			chartBody.Series ["dataSpeed"].Points.Clear ( );
			chartSensorsMotor.Series ["dataHall"].Points.Clear ( );
			chartSensorsIMU.Series ["dataX"].Points.Clear ( );
			chartSensorsIMU.Series ["dataY"].Points.Clear ( );
			chartSensorsIMU.Series ["dataZ"].Points.Clear ( );
			chartWheel.Series ["dataTorque"].Points.Clear ( );
			chartWheel.Series ["dataWheelSpeed"].Points.Clear ( );

			// Charts configuration
			foreach (Chart c in charts)
			{
				c.MouseWheel += ChartEv_MouseWheel;
				c.ChartAreas [0].AxisX.LabelStyle.Format = "N1";
				c.ChartAreas [0].AxisX.ScaleView.Position = 0;
				c.ChartAreas [0].AxisX.ScaleView.Size = 10;
			}
			
			// Delegates in order to update the control asynchronously
			dlgtUpdateCharts = new DlgtUpdateCharts (UpdateCharts);

			// Timer to refresh the graphs
			Timer timer = new Timer ( );
			timer.Interval = 500;
			timer.Tick += timer_Tick;
			timer.Start ( );

			// Default view
			viewPositionMin = 0.0f;
			viewPosition = 0.0f;
			bindLimit = 1.0f;
			viewSize = 10.0f;
			follow = true;

			Enabled = false;
		}

	
		
		// ===========================================================================
		/// \brief Zoom in the X axis of all graphs
		// ===========================================================================
		public void ZoomIn ()
		{
			// Zoom on the center of the graph
			Zoom (0.6f, viewPosition + viewSize / 2f);
		}


		// ===========================================================================
		/// \brief Zoom out the X axis of all graphs
		// ===========================================================================
		public void ZoomOut ()
		{
			// Zoom from the center of the graph
			Zoom (1.4f, viewPosition + viewSize / 2f);
		}


		// ===========================================================================
		/// \brief Zoom out the X axis of all graphs to display all the data
		// ===========================================================================
		public void ZoomGlobal ()
		{
			follow = true;
			viewSize = viewSizeMax;
		}


		// ===========================================================================
		/// \brief	Activate following of new incoming data by the automatic slinding
		///			of the graph window along the X axis
		// ===========================================================================
		public void Follow ()
		{
			follow = true;
		}


		// ===========================================================================
		/// \brief	Attach this control to a a pendulum
		// ===========================================================================
		public void AttachPendulum (Pendulum pendulum)
		{
			this.pendulum = pendulum;
			pendulum.UpdatedEvent += EvPendulum_Updated;			
		}
		#endregion



		// ###########################################################################
		//
		// E V E N T S
		//
		// ###########################################################################
		#region EVENTS

		// ===========================================================================
		/// \brief		Event when control is enabled or disabled
		///
		/// Update the scopes
		// ===========================================================================
		protected override void OnEnabledChanged (EventArgs e)
		{
			charts = new Chart [] { chartSensorsMotor, chartBody, chartWheel, chartSensorsIMU };

			base.OnEnabledChanged (e);

			// Charts configuration
			foreach (Chart c in charts)
			{
				if (Enabled)
				{
					c.Enabled = true;
					c.BackColor = Color.White;
					c.ChartAreas [0].BackColor = Color.White;	
				}
				else
				{
					c.Enabled = false;
					c.BackColor = Color.LightGray;
					c.ChartAreas [0].BackColor = Color.Gray;
				}
			}

			if (Enabled)
			{
				// Ensure graphs are empty
				chartBody.Series ["dataAngle"].Points.Clear ( );
				chartBody.Series ["dataSpeed"].Points.Clear ( );
				chartSensorsMotor.Series ["dataHall"].Points.Clear ( );
				chartSensorsIMU.Series ["dataX"].Points.Clear ( );
				chartSensorsIMU.Series ["dataY"].Points.Clear ( );
				chartSensorsIMU.Series ["dataZ"].Points.Clear ( );
				chartWheel.Series ["dataTorque"].Points.Clear ( );
				chartWheel.Series ["dataWheelSpeed"].Points.Clear ( );
				bindLimit = 0;
			}
		}


		// ===========================================================================
		/// \brief		Event when something changed in the pendulum
		///
		/// Update the scopes to reflect the new data
		// ===========================================================================
		private void EvPendulum_Updated (Pendulum pendulum)
		{
			float tsMin, tsMax;

			tsMin = pendulum.TimestampMin / 1000.0f;
			tsMax = pendulum.Timestamp / 1000.0f;

			viewPositionMin = tsMin;
			viewSizeMax = tsMax - tsMin;

			// Ensure the view still agree with the pendulum data
			if (viewSizeMax < 1.0f) viewSizeMax = 1.0f;

			if (viewSize > viewSizeMax)
			{
				if (viewSizeMax > 10.0f) viewSize = viewSizeMax;
				else viewSize = 10.0f;
			}
			if (viewPosition < tsMin) viewPosition = tsMin;
			if (viewPosition + viewSize > tsMax) viewPosition = tsMax - viewSize;

			// Follow the data by sliding the view if needed
			if (follow) viewPosition = tsMax - viewSize;
			if (viewPosition < 0) viewPosition = 0.0f;

			// Chart must be refreshed if the view is beyond what as already been see
			if (viewPosition + viewSize >= bindLimit)
			{
				rebind = true;
				bindLimit = tsMax;
			}

			// Update charts to reflect the new view
			BeginInvoke (dlgtUpdateCharts);
		}


		// ===========================================================================
		/// \brief		Event when the scrollbar is scrolled
		///
		/// The X axis view in the 4 graphs must be updated
		// ===========================================================================
		private void EvScrollBar_Scroll (object sender, ScrollEventArgs e)
		{
			float position;
			follow = false;

			// Update chart position according to the scroll bar increment
			position = viewPosition;
			position += (e.NewValue - e.OldValue) * viewSizeMax / 1000.0f;
			if (position + viewSize > viewPositionMin + viewSizeMax) position = viewPositionMin + viewSizeMax - viewSize;
			if (position < viewPositionMin) position = viewPositionMin;

			viewPosition = position;
			chartSensorsMotor.ChartAreas [0].AxisX.ScaleView.Position = position;
			chartBody.ChartAreas [0].AxisX.ScaleView.Position = position;
			chartWheel.ChartAreas [0].AxisX.ScaleView.Position = position;
			chartSensorsIMU.ChartAreas [0].AxisX.ScaleView.Position = position;
		}


		// ===========================================================================
		/// \brief		Event when the mouse wheel is rolled on a graph
		///
		/// Graph X axis scale is zoomed in or out
		// ===========================================================================
		private void ChartEv_MouseWheel (object sender, MouseEventArgs e)
		{
			float scaler;
			float val;

			// Scale view
			if (e.Delta > 0) scaler = 1.2f;
			else
			{
				scaler = 0.8f;
				follow = false;
			}

			// Zoom center point
			val = (float) (chartSensorsMotor.ChartAreas [0].AxisX.PixelPositionToValue (e.X));

			// Zoom
			Zoom (scaler, val);
		}


		// ===========================================================================
		// <ChartEv_MouseEnter>
		///
		/// \brief		Event when the mouse enter a graph
		///
		/// Focus is set on this graph
		// ===========================================================================
		private void ChartEv_MouseEnter (object sender, EventArgs e)
		{
			Chart chart = sender as Chart;
			chart.Focus ( );
		}


		// ===========================================================================
		/// \brief		Event when the mouse leaves a graph
		///
		/// This action stops graph sliding
		// ===========================================================================
		private void ChartEv_MouseLeave (object sender, EventArgs e)
		{
			mouseClickX = -1;
		}
		

		// ===========================================================================
		/// \brief		Event when a click down is done on a graph
		///
		/// This action allows to slide the graph with the mouse
		// ===========================================================================
		private void ChartEv_MouseDown (object sender, MouseEventArgs e)
		{
			mouseClickX = e.X;
		}


		// ===========================================================================
		/// \brief		Event when a click up is done on a graph
		///
		/// This action stops graph sliding
		// ===========================================================================
		private void ChartEv_MouseUp (object sender, MouseEventArgs e)
		{
			mouseClickX = -1;
		}


		// ===========================================================================
		/// \brief		Event when mouse is moved over a graph
		///
		/// If mouse button is down, graphs are scrolled
		// ===========================================================================
		private void ChartEv_MouseMove (object sender, MouseEventArgs e)
		{
			Chart chart = sender as Chart;
			float position;
			float v1, v2;

			if (mouseClickX < 0) return;
		
			// Get translation value
			try
			{
				v1 = (float) (chart.ChartAreas [0].AxisX.PixelPositionToValue (e.X));
				v2 = (float) (chart.ChartAreas [0].AxisX.PixelPositionToValue (mouseClickX));
			}
			catch (Exception) { return; }

			// Apply translation
			position = viewPosition - v1 + v2;
			mouseClickX = e.X;

			if (position + viewSize > viewPositionMin + viewSizeMax) position = viewPositionMin + viewSizeMax - viewSize;
			if (position < viewPositionMin) position = viewPositionMin;
			viewPosition = position;

			// Update all the scopes
			follow = false;
			UpdateCharts ( );
		}
		#endregion



		// ###########################################################################
		//
		// P R I V A T E
		//
		// ###########################################################################
		#region PRIVATE

		// ===========================================================================
		/// \brief	Callback activated periodically to redraw scope content
		// ===========================================================================
		private void timer_Tick (object sender, EventArgs e)
		{
			if (rebind)
			{
				rebind = false;

				dataTimestamp.Clear ( );
				dataBodyAngle.Clear ( );
				dataBodySpeed.Clear ( );
				dataMotorSpeed.Clear ( );
				dataAccX.Clear ( );
				dataAccY.Clear ( );
				dataGyroZ.Clear ( );
				dataWheelSpeed.Clear ( );
				dataWheelTorque.Clear ( );

				// Build data vectors
				ISafeEnumerable<PendulumInterface.Telemetries> history = pendulum.GetTelemetriesHistory ( );
				foreach (PendulumInterface.Telemetries tm in history)
				{
					dataTimestamp.Add (tm.timestamp / 1000.0f);
					dataBodyAngle.Add (tm.state.bodyAngle * 180.0f / (float) Math.PI);
					dataBodySpeed.Add (tm.state.bodySpeed * 180.0f / (float) Math.PI);
					dataMotorSpeed.Add (tm.sensors.hall * 60f / (2f * (float) Math.PI));
					dataAccX.Add (tm.sensors.accX / 9.81f);
					dataAccY.Add (tm.sensors.accY / 9.81f);
					dataGyroZ.Add (tm.sensors.gyro * 180.0f / (float) Math.PI);
					dataWheelSpeed.Add (tm.state.wheelSpeed * 60.0f / (2f * (float) Math.PI));
					dataWheelTorque.Add (tm.command.torque*1000f);
				}

				// Attach to graphs
				chartBody.Series ["dataAngle"].Points.DataBindXY (dataTimestamp, dataBodyAngle);
				chartBody.Series ["dataSpeed"].Points.DataBindXY (dataTimestamp, dataBodySpeed);

				chartSensorsMotor.Series ["dataHall"].Points.DataBindXY (dataTimestamp, dataMotorSpeed);

				chartSensorsIMU.Series ["dataX"].Points.DataBindXY (dataTimestamp, dataAccX);
				chartSensorsIMU.Series ["dataY"].Points.DataBindXY (dataTimestamp, dataAccY);
				chartSensorsIMU.Series ["dataZ"].Points.DataBindXY (dataTimestamp, dataGyroZ);

				chartWheel.Series ["dataWheelSpeed"].Points.DataBindXY (dataTimestamp, dataWheelSpeed);
				chartWheel.Series ["dataTorque"].Points.DataBindXY (dataTimestamp, dataWheelTorque);
			}
		}


		// ===========================================================================
		/// \brief		Change zoom level
		///
		/// \param	scaler			Zoom scaler factor
		/// \param	zoomPosition	Reference position to zoom in / out
		// ===========================================================================	
		private void Zoom (float scaler, float zoomPosition)
		{
			viewSize *= scaler;

			// Limit the scale
			if (viewSize > viewSizeMax) viewSize = viewSizeMax;
			if (viewSize < 0.1f) viewSize = 0.1f;

			// Update position so that mouse pointer always point to the same graph location
			viewPosition += zoomPosition - viewPosition - (zoomPosition - viewPosition) * scaler;

			// Limit the position
			if (viewPosition + viewSize > viewPositionMin + viewSizeMax) viewPosition = viewPositionMin + viewSizeMax - viewSize;
			if (viewPosition < viewPositionMin) viewPosition = viewPositionMin;

			UpdateCharts ( );
		}


		// ===========================================================================
		/// \brief		Configure the charts to reflect view state of the X axis
		// ===========================================================================
		private void UpdateCharts ()
		{
			foreach (Chart c in charts) c.SuspendLayout ( );

			// Set chart position
			foreach (Chart c in charts) c.ChartAreas [0].AxisX.ScaleView.Position = viewPosition;

			// Set chart view size
			if (charts [0].ChartAreas [0].AxisX.ScaleView.Size != viewSize)
			{
				// Compute grid interval
				int logInterval = (int) (Math.Round (2.0 * Math.Log10 (viewSize / 10.0f)));
				double interval = Math.Pow (10, logInterval / 2.0);

				foreach (Chart c in charts)
				{
					c.ChartAreas [0].AxisX.ScaleView.Size = viewSize;
					c.ChartAreas [0].AxisX.Interval = interval;
					c.ChartAreas [0].AxisX.MajorGrid.Interval = interval;
					c.ChartAreas [0].AxisX.MajorTickMark.Interval = interval;
				}
			}

			// Update the scroll bar accordingly. Computation is based on a scroll bar that is 1000 wide
			scrollBar.LargeChange = (int) (Math.Round (1000.0f * (viewSize / viewSizeMax)));
			scrollBar.SmallChange = scrollBar.LargeChange / 5;

			int value = Convert.ToInt32 (Math.Round (1000.0f * (viewPosition-viewPositionMin) / viewSizeMax, 0));
			value = Math.Max (value, 0);
			value = Math.Min (value, 1000);
			scrollBar.Value = value;

			foreach (Chart c in charts) c.ResumeLayout ( );
		}
		#endregion		
	}
}
