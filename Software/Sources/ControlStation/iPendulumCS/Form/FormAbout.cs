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
/// \file	FormAbout.cs
/// \author	Jean-Sebastien Gonsette
///
/// About form
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
using System.Reflection;

namespace iPendulumCS
{
	/// About form
	public partial class FormAbout : Form
	{
		public FormAbout ()
		{
			InitializeComponent ( );

			// Get version from assembly
			Version version = Assembly.GetExecutingAssembly ( ).GetName ( ).Version;
			lblVersion.Text = version.Major.ToString ( ) + "." + version.Minor.ToString ( ) + "." + version.Revision.ToString ( );
		}

		private void Ev_Click (object sender, EventArgs e)
		{
			System.Diagnostics.Process.Start ("http://www.genid.eu");
		}		
	}
}
