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
/// \file	Pendulum.cs
/// \author	Jean-Sebastien Gonsette
///
/// Data object reflecting the actual pendulum state, along with its history.
// ###########################################################################

using System.Windows.Forms;
using System;

namespace iPendulumCS
{
	public class Pendulum
	{
		#region DATA

		/// Pendulum interface
		public PendulumInterface pendulumInterface;

		/// Max. history length
		private const int HISTORY_LENGTH = 1000;

		/// Pendulum telemetries history
		private History<PendulumInterface.Telemetries> historyTelemetries = 
			new History<PendulumInterface.Telemetries> (HISTORY_LENGTH);

		/// Timer to acquire data from the pendulum
		private Timer timer;

		/// Information about tasks
		private PendulumInterface.Task [] tasks = new PendulumInterface.Task [3];

		/// Next tm update time
		private DateTime nextTmUpdate;

		#endregion



		// ###########################################################################
		//
		// P R O P E R T I E S
		//
		// ###########################################################################
		#region Properties	

		// ===========================================================================
		/// \brief	Property: Telemetries timestamp [ms]
		// ===========================================================================
		public uint Timestamp
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0;
				return historyTelemetries.Last ( ).timestamp;
			}
		}


		// ===========================================================================
		/// \brief	Property: Frist telemetries timestamp in history [ms]
		// ===========================================================================
		public uint TimestampMin
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0;
				return historyTelemetries.First ( ).timestamp;
			}
		}


		// ===========================================================================
		/// \brief	Property: Motor speed hall sensor [rad/s]
		// ===========================================================================
		public float SensorsMotorSpeed
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0.0f;
				return historyTelemetries.Last ( ).sensors.hall;
			}
		}
	
		
		// ===========================================================================
		/// \brief	Property: Gyro sensor value around Z axis [rad/s]
		// ===========================================================================
		public float SensorsGyroZ
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0.0f;
				return historyTelemetries.Last ( ).sensors.gyro;
			}
		}

		
		// ===========================================================================
		/// \brief	Property: Acc sensor value along X axis [m/s^2]
		// ===========================================================================
		public float SensorsAccX
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0.0f;
				return historyTelemetries.Last ( ).sensors.accX;
			}
		}


		// ===========================================================================
		/// \brief	Property: Acc sensor value along Y axis [m/s^2]
		// ===========================================================================
		public float SensorsAccY
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0.0f;
				return historyTelemetries.Last ( ).sensors.accY;
			}
		}


		// ===========================================================================
		/// \brief	Property: Acc sensor value along Z axis [m/s^2]
		// ===========================================================================
		public float SensorsAccZ
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0.0f;
				return historyTelemetries.Last ( ).sensors.accZ;
			}
		}


		// ===========================================================================
		/// \brief	Property: pendulum body angle [rad]
		// ===========================================================================
		public float StateBodyAngle
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0.0f;
				return historyTelemetries.Last ( ).state.bodyAngle;
			}
		}
		
		
		// ===========================================================================
		/// \brief	Property: pendulum body angular speed [rad/s]
		// ===========================================================================
		public float StateBodySpeed
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0.0f;
				return historyTelemetries.Last ( ).state.bodySpeed;
			}
		}		
		
		
		// ===========================================================================
		/// \brief	Property: pendulum wheel angular speed [rad/s]
		// ===========================================================================
		public float StateWheelSpeed
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0.0f;
				return historyTelemetries.Last ( ).state.wheelSpeed;
			}
		}


		// ===========================================================================
		/// \brief	Property: pendulum target body angle [rad]
		// ===========================================================================
		public float TargetBodyAngle
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0.0f;
				return historyTelemetries.Last ( ).targetState.bodyAngle;
			}
		}
		
		
		// ===========================================================================
		/// \brief	Property: pendulum target body angular speed [rad/s]
		// ===========================================================================
		public float TargetBodySpeed
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0.0f;
				return historyTelemetries.Last ( ).targetState.bodySpeed;
			}
		}		
		
		
		// ===========================================================================
		/// \brief	Property: pendulum target wheel angular speed [rad/s]
		// ===========================================================================
		public float TargetWheelSpeed
		{
			get
			{
				if (historyTelemetries.Count ( ) == 0) return 0.0f;
				return historyTelemetries.Last ( ).targetState.wheelSpeed;
			}
		}


		// ===========================================================================
		/// \brief	Property: pendulum motor current [A]
		// ===========================================================================
		public float MotorCurrent
		{
			get;
			private set;
		}

		// ===========================================================================
		/// \brief	Property: pendulum battery voltage [V]
		// ===========================================================================
		public float BatteryVoltage
		{
			get;
			private set;
		}


		// ===========================================================================
		/// \brief	Property: pendulum battery state of Charge [%]
		// ===========================================================================
		public uint BatterySOC
		{
			get;
			private set;
		}


		// ===========================================================================
		/// \brief	Property: System version
		// ===========================================================================
		public string SysVersion
		{
			get;
			private set;
		}


		// ===========================================================================
		/// \brief	Property: OS version
		// ===========================================================================
		public string OsVersion
		{
			get;
			private set;
		}


		// ===========================================================================
		/// \brief	Property: OS Memory - Static usage
		// ===========================================================================
		public uint OsMemoryStatic
		{
			get;
			private set;
		}


		// ===========================================================================
		/// \brief	Property: OS Memory - Dynamic usage
		// ===========================================================================
		public uint OsMemoryDynamic
		{
			get;
			private set;
		}


		// ===========================================================================
		/// \brief	Property: OS Memory - Max Dynamic usage
		// ===========================================================================
		public uint OsMemoryMaxDynamic
		{
			get;
			private set;
		}


		// ===========================================================================
		/// \brief	Property: OS Memory - Kernel usage
		// ===========================================================================
		public uint OsMemoryKernel
		{
			get;
			private set;
		}


		// ===========================================================================
		/// \brief	Property: OS Memory - Free
		// ===========================================================================
		public uint OsMemoryFree
		{
			get;
			private set;
		}


		// ===========================================================================
		/// \brief	Property: Kernel processor usage
		// ===========================================================================
		public uint OsKernelProcUsage
		{
			get;
			private set;
		}


		// ===========================================================================
		/// \brief	Property: processor Idle usage
		// ===========================================================================
		public uint OsIdleProcUsage
		{
			get;
			private set;
		}


		// ===========================================================================
		/// \brief	Property: Os tasks
		// ===========================================================================
		public PendulumInterface.Task [] OsTasks
		{
			get { return tasks; }
		}
		#endregion



		// ###########################################################################
		//
		// P U B L I C
		//
		// ###########################################################################
		#region Public

		/// Handler when pendulum status changed in any way
		public delegate void UpdatedHandler (Pendulum pendulum);

		/// Event when the pendulum status changed in any way
		public event UpdatedHandler UpdatedEvent;


		// ===========================================================================
		/// \brief	Constructor
		// ===========================================================================
		public Pendulum (PendulumInterface pendulumInterface)
		{
			// Connect to pendulum interface
			this.pendulumInterface = pendulumInterface;
			pendulumInterface.TelemetriesEvent += EvInterface_TelemetriesEvent;
			pendulumInterface.ConnectionChangedEvent += EvInterface_ConnectionChanged;

			// Timer to refresh data from the pendulum
			timer = new Timer ();
			timer.Interval = 1000;
			timer.Tick += EvTimer_Tick;
			timer.Start ( );			
		}
	
		
		// ===========================================================================
		///\brief	Return history of recorded telemetries
		// ===========================================================================
		public ISafeEnumerable<PendulumInterface.Telemetries> GetTelemetriesHistory ()
		{
			return historyTelemetries;
		}
	
		#endregion



		// ###########################################################################
		//
		// P R I V A T E
		//
		// ###########################################################################
		#region Private

		// ===========================================================================
		/// \brief	Private constructor
		// ===========================================================================
		private Pendulum () { }


		// ===========================================================================
		/// \brief	Handle arrival of new telemetries values
		// ===========================================================================
		void EvInterface_TelemetriesEvent (object sender, PendulumInterface.Telemetries telemetries)
		{
			// Store them in the history
			historyTelemetries.Add (telemetries);

			// Limit signalisation rate @ 10 events by seconds
			DateTime date = DateTime.Now;
			if (date > nextTmUpdate)
			{
				nextTmUpdate = date + new TimeSpan (100 * 10000);
				if (UpdatedEvent != null) UpdatedEvent (this);
			}			
		}

		
		// ===========================================================================
		/// \brief	Handle change of connection state
		// ===========================================================================
		void EvInterface_ConnectionChanged (object sender, bool status)
		{
			if (status == true) historyTelemetries.Clear ( );
		}


		// ===========================================================================
		/// \brief	Refresh timer elapsed. Acquisition of pendulm data
		// ===========================================================================
		void EvTimer_Tick (object sender, EventArgs e)
		{
			if (pendulumInterface.IsConnected ( ) == false) return;

			// Get battery voltage
			{
				float voltage;
				uint soc;
				pendulumInterface.GetBatteryVoltage (out voltage, out soc);
				this.BatteryVoltage = voltage;
				this.BatterySOC = soc;
			}

			// Get memory usage
			{
				uint stat, dynamic, maxDynamic, free, kernel;
				pendulumInterface.GetOsMemory (out stat, out dynamic, out maxDynamic, out kernel, out free);
				this.OsMemoryStatic = stat;
				this.OsMemoryMaxDynamic = maxDynamic;
				this.OsMemoryFree = free;
				this.OsMemoryDynamic = dynamic;
				this.OsMemoryKernel = kernel;
			}

			// Get processor usage
			{
				uint kernel, idle;
				pendulumInterface.GetOsProcUsage (out kernel, out idle);
				this.OsKernelProcUsage = kernel;
				this.OsIdleProcUsage = idle;
			}

			// Versions
			{
				string osVersion, sysVersion;
				pendulumInterface.GetOsVersion (out osVersion);
				pendulumInterface.GetSystemVersion (out sysVersion);
				this.OsVersion = osVersion;
				this.SysVersion = sysVersion;
			}

			// Firmware tasks
			{
				uint numTasks;
				numTasks = pendulumInterface.GetOsTasks (ref tasks);

				if (numTasks != tasks.Length)
				{
					tasks = new PendulumInterface.Task [numTasks];
					pendulumInterface.GetOsTasks (ref tasks);
				}
			}
		}


		#endregion
	}
}
