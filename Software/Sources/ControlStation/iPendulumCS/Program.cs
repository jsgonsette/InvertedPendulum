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
/// \file	Program.cs
/// \author	Jean-Sebastien Gonsette
///
/// Application entry point.
// ###########################################################################

using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace iPendulumCS
{
	static class Program
	{
		// Program entry point
		[STAThread]
		static void Main ()
		{
			Application.EnableVisualStyles ( );
			Application.SetCompatibleTextRenderingDefault (false);

			try
			{
				Application.Run (new FormMain ( ));
			}
			catch (Exception e)
			{
				MessageBox.Show (e.Message);
			}
		}
	}
}
