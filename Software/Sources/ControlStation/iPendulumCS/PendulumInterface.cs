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
/// \file	PendulumInterface.cs
/// \author	Jean-Sebastien Gonsette
///
/// Wrapper around the UART communication link with the actual pendulum.
// ###########################################################################

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO.Ports;
using System.IO;


namespace iPendulumCS
{
	/// Interface with the pendulum through an UART connection
	public class PendulumInterface : IDisposable
	{
		#region DATA
		/// Rx thread
		private Thread rxThread;

		/// Flag to stop the rx thread
		private bool stopThread;

		/// Response received to a query
		private string queryResponse;

		/// Event to signal the reception of a response
		private AutoResetEvent responseEvent;

		/// Serial port for communication with the actual pendulum
		private SerialPort port = null;

		/// File to log the data going over the UART
		private FileStream logFile;

		/// Mutex
		private object locker;

		/// Pendulum control mode
		private CtrlMode mode;

		#endregion


		#region PUBLIC
		// ###########################################################################
		//
		// P U B L I C
		//
		// ###########################################################################
		
		/// Sensor values from the pendulum
		public struct Sensors
		{
			public float accX;		///< Acceleration along X axis [m/s^2]
			public float accY;		///< Acceleration along Y axis [m/s^2]
			public float accZ;		///< Acceleration along Z axis [m/s^2]
			public float gyro;		///< Angular speed [rad/s]
			public float hall;		///< Motor angular speed [rad/s]

		}

		/// Pendulum state
		public struct State
		{
			public float bodyAngle;		///< Body angle [rad]
			public float bodySpeed;		///< Body angular speed [rad/s]
			public float wheelSpeed;	///< Motor angular speed [rad/s]
		}

		/// Pendulum control command
		public struct Command
		{
			public float torque;		///< Torque command to the motor [N.m]
			public bool brake;			///< Brake activated?
		}

		/// All the telemetries sent by the pendulum during a cycle
		public struct Telemetries
		{
			public uint timestamp;		///< Timestamp [ms]
			public Sensors sensors;		///< Sensor values
			public State state;			///< Pendulum estimated state
			public State targetState;	///< Pendulum target state
			public Command command;		///< Control command
		}

		/// Pendulum control modes
		public enum CtrlMode
		{
			Stop = 0,				///< Controller is off
			Free = 1,				///< No control, just angle estimation
			Equilibrium = 2			///< Pendulum is controlled in equilibrium		
		}

		/// Firmware task information
		public struct Task
		{
			public string name;		///< Task name
			public uint priority;	///< Task priority
			public uint stack;		///< Stack size [B]
			public uint stackUsage; ///< Max usage of stack [B]
			public uint procUsage;	///< [°/°°]
		}
		
		/// Mode for manual motor control
		public enum MotorManualControl
		{
			Disabled = 0,
			Brake = 1,
			Forward = 2,
			Reverse = 3
		}

		/// Handler when connection with hardware is closed
		public delegate void ConnectionChangedHandler (object sender, bool status);

		/// Event when connection with hardware is closed
		public event ConnectionChangedHandler ConnectionChangedEvent;

		/// Handler to signal reception of telemetries
		public delegate void TelemetriesHandler (object sender, Telemetries telemetries);

		/// Event when connection with hardware is closed
		public event TelemetriesHandler TelemetriesEvent;


		// ===========================================================================
		/// \brief	Constructor
		// ===========================================================================
		public PendulumInterface ()
		{
			locker = new object ( );
			responseEvent = new AutoResetEvent (false);
		}


		// ===========================================================================
		/// \brief Finalizer
		// ===========================================================================
		~PendulumInterface ()
		{
			Dispose ( );
		}
		

		// ===========================================================================
		/// \brief		Test the serial connection with the pendulum
		///
		/// \param		port	Serial port for communication, 
		///						it must be opened and the physical parameters 
		///						already configured
		///						
		/// \return		True if remote device is responding
		// ===========================================================================
		public static bool TestConnection (SerialPort port)
		{
			int i;
			string str;

			// Send a simple query
			port.ReadTimeout = 100;
			str = ":SYS.Version\r\n";
			port.Write (str);

			// If pendulum sends telemetries, we can receive garbage
			// loop a bit until we get what we want.
			for (i = 0; i < 100; i++)
			{
				// Get a response
				try { str = port.ReadLine ( ); }
				catch (Exception) { return false; }

				// Check the query
				if (str.StartsWith ("=SYS.Version.{") == true) break;
			}
			if (i >= 100) return false;
			return true;
		}


		// ===========================================================================
		/// \brief	Connect to the pendulum with the given serial port
		///
		/// \param	port	Serial port properly configured to sustain the connection
		// ===========================================================================
		public void Connect (SerialPort port)
		{
			if (IsConnected ()) Disconnect ();
			this.port = port;
			this.port.NewLine = "\r\n";
			this.port.ReadTimeout = 200;

			stopThread = false;
			rxThread = new Thread (ThreadRx);
			rxThread.Start ( );

			// Signalisation
			if (ConnectionChangedEvent != null) ConnectionChangedEvent (this, true);
		}


		// ===========================================================================
		/// \brief	Disconnect from the pendulum
		// ===========================================================================
		public void Disconnect ()
		{
			// Stop rx thread
			stopThread = true;

			if (rxThread != null && Thread.CurrentThread != rxThread) rxThread.Join ( );
			rxThread = null;

			// Stop logging
			StopLogging ( );

			// Close the port and send the event
			if (port != null)
			{
				port.Close ( );
				port.Dispose ( );
				port = null;
			}

			// Signalisation
			if (ConnectionChangedEvent != null) ConnectionChangedEvent (this, false);
		}


		// ===========================================================================
		/// \brief		Determine if the connection with the hardware is set up
		///
		/// \return		True if we are connected
		// ===========================================================================
		public bool IsConnected ()
		{
			return (port != null);
		}


		// ===========================================================================
		/// \brief	Dispose the object
		// ===========================================================================
		public void Dispose ()
		{
			Disconnect ( );
		}


		// ===========================================================================
		/// \brief	Start to log UART communication into a log file
		///
		/// \param	filePath	File path 
		// ===========================================================================
		public bool StartLogging (string filePath)
		{
			bool success;

			try
			{
				logFile = new FileStream (filePath, FileMode.Append);
				success = true;
			}
			catch (Exception)
			{
				success = false;
				logFile = null;
			}

			return success;
		}


		// ===========================================================================
		/// \brief	Stop logging in the logfile
		// ===========================================================================
		public void StopLogging ()
		{
			lock (locker)
			{
				if (logFile != null)
				{
					logFile.Close ( );
					logFile.Dispose ( );
					logFile = null;
				}
			}
		}

		
		// ===========================================================================
		/// \brief		Send a query to the hardware to get System version
		///
		/// \param[out]	version			Pendulum software version
		// ===========================================================================
		public void GetSystemVersion (out string version)
		{
			string rsp;
			string [] parameters;

			version = null;

			rsp = SendQuery ("SYS.Version", true, out parameters);
			if (rsp == null) return;

			// Build and set version string
			version = "iPendulum v" + parameters [0].ToString ( ) + '.' +
					parameters [1].ToString ( ) + '.' + parameters [2].ToString ( );
		}


		// ===========================================================================
		/// \brief		Send a query to the hardware to get OS version
		///
		/// \param[out]	version			OS version
		// ===========================================================================
		public void GetOsVersion (out string version)
		{
			string rsp;
			string [] parameters;

			version = null;

			rsp = SendQuery ("OS.Version", true, out parameters);
			if (rsp == null) return;

			// Build and set version string
			version = "NanoScale v" +
					parameters [0].ToString ( ) + '.' + parameters [1].ToString ( ) + '.' +
					parameters [2].ToString ( ) + '.' + parameters [3].ToString ( );
		}


		// ===========================================================================
		/// \brief		Send a query to the hardware to get OS memory usage
		///
		/// \param[out]	stat			Static usage, Bytes
		/// \param[out]	dynamic			Dynamic usage, Bytes
		/// \param[out]	maxDynamic		Max. Dynamic usage, Bytes
		/// \param[out]	kernel			Kernel usage, Bytes
		/// \param[out]	free			Free memory, Bytes
		// ===========================================================================
		public void GetOsMemory (out uint stat, out uint dynamic, out uint maxDynamic, out uint kernel, out uint free)
		{
			string rsp;
			string [] parameters;

			stat = dynamic = maxDynamic = kernel = free = 0;

			rsp = SendQuery ("OS.Memory", true, out parameters);
			if (rsp == null) return;

			// Get memory usage
			stat = Convert.ToUInt16 (parameters [0]);
			dynamic = Convert.ToUInt16 (parameters [1]);
			maxDynamic = Convert.ToUInt16 (parameters [2]);
			kernel = Convert.ToUInt16 (parameters [3]);
			free = Convert.ToUInt16 (parameters [4]);
		}

		
		// ===========================================================================
		/// \brief		Send a query to the hardware to get information about tasks
		///
		/// \param[out]	tasks			Array of tasks to fill in
		///
		/// \return		Number of tasks (ideal size of tasks)
		// ===========================================================================
		public uint GetOsTasks (ref Task [] tasks)
		{
			string rsp;
			string [] parameters;
			uint i = 0;
			uint procUsage;

			// Go through all tasks
			while (true)
			{
				// Query task number 'i'
				rsp = SendQuery ("OS.Task." + i.ToString ( ), true, out parameters);
				if (rsp == null) break;
				if (parameters.Count ( ) != 5) break;

				if (i < tasks.Count ( ))
				{
					// Extract and convert processor usage in °/°°
					procUsage = Convert.ToUInt16 (parameters [4]);
					procUsage = (procUsage * 1000) >> 16;

					tasks [i].name = parameters [0];
					tasks [i].priority = Convert.ToUInt16 (parameters [1]);
					tasks [i].stack = Convert.ToUInt16 (parameters [2]);
					tasks [i].stackUsage = Convert.ToUInt16 (parameters [3]);
					tasks [i].procUsage = procUsage;
				}

				i++;
			}
			
			return i;
		}


		// ===========================================================================
		// <GetOsProcUsage>
		///
		/// \brief		Send a query to get kernel and idle processor usage
		///
		/// \param[out]		kernelProcUsage		Kernel processor usage [°/°°]
		/// \param[out]		idleProcUsage		Idle processor usage [°/°°]
		// ===========================================================================
		public void GetOsProcUsage (out uint kernelProcUsage, out uint idleProcUsage)
		{
			string rsp;
			string [] parameters;

			kernelProcUsage = 0;
			idleProcUsage = 0;

			rsp = SendQuery ("OS.Kernel", true, out parameters);
			if (rsp == null) return;
					
			// Get kernel and idle proc usage
			kernelProcUsage = Convert.ToUInt16 (parameters [0]);
			kernelProcUsage = (kernelProcUsage * 1000) >> 16;

			idleProcUsage = Convert.ToUInt16 (parameters [1]);
			idleProcUsage = (idleProcUsage * 1000) >> 16;
		}


		// ===========================================================================
		/// \brief		Send a query to the hardware to get battery voltage and SOC
		///
		/// \param[out]		voltage		Battery voltage [V]
		/// \param[out]		soc			Battery SOC [%]
		// ===========================================================================
		public void GetBatteryVoltage (out float voltage, out uint soc)
		{
			string rsp;
			string [] parameters;

			voltage = 0.0f;
			soc = 0;

			rsp = SendQuery ("BATTERY.Voltage", true, out parameters);
			if (rsp == null) return;
			
			// Get battery voltage and corresponding SOC
			voltage = (Convert.ToUInt16 (parameters [0])) / 100.0f;
			soc = Convert.ToUInt16 (parameters [1]);
		}


		// ===========================================================================
		/// \brief		Send a query to calibrate the battery voltage
		///
		/// \param	refVoltage		Measured battery voltage [V]
		// ===========================================================================
		public void CalibrateBattery (float refVoltage)
		{
			string [] parameters;
			uint v;
			v = Convert.ToUInt32 (refVoltage * 100);
			SendQuery ("BATTERY.VoltageCalib.{" + v + "}", false, out parameters);
		}


		// ===========================================================================
		/// \brief	Send a query to calibrate the IMU
		///
		/// \param	accScales	Measured acc scale factors (x, y)
		/// \param	accBias		Measured acc bias (x, y, z) [m/s^2]
		/// \param	gyroBias	Measured gyro bias [rad/s]
		// ===========================================================================
		public void CalibrateIMU (float [] accScales, float [] accBias, float gyroBias)
		{
			string [] parameters;
			int gz, ax, ay, az, kx, ky;

			// rad/s to 1/1000 °/s
			gz = Convert.ToInt32 (gyroBias * 1000 * 180 / (float) Math.PI);

			// m/s^2 to µG
			ax = (int) (accBias [0] * 1e6f / 9.81f);
			ay = (int) (accBias [1] * 1e6f / 9.81f);
			az = (int) (accBias [2] * 1e6f / 9.81f);

			kx = (int) (1000f * accScales [0]);
			ky = (int) (1000f * accScales [1]);

			SendQuery ("CALIB.IMU.{" + kx + "," + ky + "," + ax + "," + ay + "," + az + "," + gz + "}", false, out parameters);
		}


		// ===========================================================================
		/// \brief		Set telemetry upload rate
		///
		/// \param		divisor		0: pendulum don't send telemetry
		///							1: telemetry is uploaded full speed
		///							n: telemetry is uploaded 1 time out of n
		// ===========================================================================
		public void SetTelemetryRate (int divisor)
		{
			string [] parameters;
			SendQuery ("SYS.SetTM.{" + divisor + "}", false, out parameters);
		}


		// ===========================================================================
		/// \brief		Define the control mode of the pendulum
		///
		/// \param		mode		Controller mode
		// ===========================================================================
		public void SetControlMode (CtrlMode mode)
		{
			string [] parameters;
			SendQuery ("SYS.SetMode.{" + Convert.ToUInt16 (mode) + "}", false, out parameters);

			this.mode = mode;
		}

	
		// ===========================================================================
		/// \brief		Return pendulum control mode
		// ===========================================================================
		public CtrlMode GetControlMode ()
		{
			return mode;
		}


		// ===========================================================================
		/// \brief		Custom command to the motor
		///
		/// \param		ctrl		Enabled, brake, forwad or reverse
		/// \param		torque		[0..100%]. Valid only for 'forward' and 'reverse' mode
		// ===========================================================================
		public void ManualMotorControl (MotorManualControl ctrl, int torque)
		{
			string [] parameters;

			torque = Math.Max (0, torque);
			torque = Math.Min (100, torque);
			SendQuery ("SYS.SetMotor.{" + Convert.ToUInt16 (ctrl) + "," + torque.ToString ( ) + "}", false, out parameters);
		}

		#endregion



		// ###########################################################################
		//
		// P R I V A T E
		//
		// ###########################################################################
		#region PRIVATE

		// ===========================================================================
		/// \brief		Thread for UART reception
		///
		/// The thread stops in case of any reception error
		// ===========================================================================
		private void ThreadRx ()
		{
			Telemetries telemetries = new Telemetries ( );

			// Reception loop
			while (stopThread == false)
			{				
				string msg = null;

				// Get incoming message
				try
				{
					msg = port.ReadLine ( );
				}
				catch (TimeoutException) { continue; }
				catch (Exception) { break; }
				if (msg == null || msg == "") break;

				// Log the data
				Log (msg + "\n");

				// Process the incoming telemetry
				if (msg [0] == '#')
				{
					bool lastTm;

					try 
					{
						lastTm = ProcessTelemetry (msg, ref telemetries);
					}
					// Stop in case of transmission error
					catch (Exception e) { break; }

					// Signal the end of a telemetry cycle
					if (lastTm && TelemetriesEvent != null) TelemetriesEvent (this, telemetries);
				}

				// Process the response to a query
				else if (msg [0] == '=')
				{
					queryResponse = String.Copy (msg);
					responseEvent.Set ( );
				}

				// Error
				else break;
			}
		}


		// ===========================================================================
		/// \brief		Send a query to the hardware through the UART
		///
		/// Caller is blocked until the response is received through 'ThreadRx'
		/// or until the timeout expires
		///
		/// \param	query			Query string to send
		/// \param	waitResponse	Is a response expected ?
		/// \param	parameters		Parameters of the response
		///
		///	\return	Response received through the UART interface
		// ===========================================================================
		private string SendQuery (string query, bool waitResponse, out string [] parameters)
		{
			parameters = null;
			if (IsConnected ( ) == false) return null;

			// Send the query
			query = ':' + query + "\r\n";
			port.Write (query);

			// Wait and process the response by separating the body and the parameters
			if (waitResponse)
			{
				if (responseEvent.WaitOne (400) == false)
				{
					Log ("\nERROR\n\n");
					Disconnect ( );
					return null;
				}
				else
				{
					int idxParam = queryResponse.IndexOf ('{');
					string paramString = queryResponse.Substring (idxParam + 1, queryResponse.Length - idxParam - 2);
					parameters = paramString.Split (',');

					string response = queryResponse.Substring (1, idxParam - 2);
					return response;
				}
			}
			else return null;
		}


		// ===========================================================================
		/// \brief		Process an incoming telemetry
		///
		/// \param			tm			Telemetry string
		/// \param[in,out]	telemetries	All the telemetries, among which one is updated
		///								according to the incoming string.
		///
		/// \return		True if the last telemetry of a cycle is received
		// ===========================================================================		
		private bool ProcessTelemetry (string tm, ref Telemetries telemetries)
		{
			bool last = false;

			// Extract all the parameters into an array.
			int idxParam = tm.IndexOf ('{');
			string paramString = tm.Substring (idxParam + 1, tm.Length - idxParam -2);
			string [] parameters = paramString.Split (',');

			// Keep the telemetry name
			tm = tm.Substring (1, idxParam -2);

			// Processing
			switch (tm)
			{
				case "SENSORS":
					telemetries.sensors.hall = Convert.ToInt32 (parameters [0]) * (2.0f * (float) Math.PI / 60.0f);
					telemetries.sensors.gyro = Convert.ToInt32 (parameters [1]) * ((float) Math.PI / (1000.0f * 180.0f));
					telemetries.sensors.accX = Convert.ToInt32 (parameters [2]) * 9.81f / 1000.0f;
					telemetries.sensors.accY = Convert.ToInt32 (parameters [3]) * 9.81f / 1000.0f;
					telemetries.sensors.accZ = Convert.ToInt32 (parameters [4]) * 9.81f / 1000.0f;
					break;

				case "STATE":
					telemetries.state.bodyAngle = Convert.ToInt32 (parameters [0]) * ((float) Math.PI / (10.0f * 180.0f));
					telemetries.state.bodySpeed = Convert.ToInt32 (parameters [1]) * ((float) Math.PI / (10.0f * 180.0f));
					telemetries.state.wheelSpeed = Convert.ToInt32 (parameters [2]) * (2.0f * (float) Math.PI / 60.0f);
					telemetries.targetState.bodyAngle = Convert.ToInt32 (parameters [3]) * ((float) Math.PI / (10.0f * 180.0f));
					telemetries.targetState.bodySpeed = Convert.ToInt32 (parameters [4]) * ((float) Math.PI / (10.0f * 180.0f));
					telemetries.targetState.wheelSpeed = Convert.ToInt32 (parameters [5]) * (2.0f * (float) Math.PI / 60.0f);
					break;
					
				case "CTRL":
					telemetries.command.brake = (Convert.ToInt32 (parameters [0]) > 0);
					telemetries.command.torque = Convert.ToInt32 (parameters [1]) * 1e-6f;
					break;

				case "TM.Start":
					telemetries.timestamp = Convert.ToUInt32 (parameters [1]);
					break;

				case "TM.End":
					last = true;
					break;
			}

			return last;
		}


		// ===========================================================================
		/// \brief		Log a string in the log file
		///
		/// \param		str			String to log
		// ===========================================================================		
		private void Log (string str)
		{
			lock (locker)
			{
				if (logFile != null)
				{
					UTF8Encoding enc = new UTF8Encoding ( );
					byte [] data = enc.GetBytes (str);
					logFile.Write (data, 0, data.Count ( ));
				}
			}
		}
		#endregion
	}
}
