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
/// \file	CtrlPendulum.cs
/// \author	Jean-Sebastien Gonsette
///
/// Control displaying an animated picture of the pendulum current state
// ###########################################################################

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace iPendulumCS.Control
{
	/// Control displaying a 2D drawing of the pendulum state
	public partial class CtrlPendulum : UserControl
	{
		private float pendulumAngle = 0.0f;
		private uint soc = 0;



		// ###########################################################################
		//
		// P U B L I C
		//
		// ###########################################################################
		#region PUBLIC

		// ===========================================================================
		/// \brief	Constructor
		// ===========================================================================
		public CtrlPendulum (Pendulum pendulum)
		{
			InitializeComponent ( );
			DoubleBuffered = true;

			// Register to pendulum updates
			pendulum.UpdatedEvent += EvPendulum_Updated;
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
		/// \param	pendulum	Pendulum data model.
		// ===========================================================================
		private void EvPendulum_Updated (Pendulum pendulum)
		{
			bool redraw = false;
			float bAngle;

			bAngle = pendulum.StateBodyAngle;
			if ( Math.Abs (pendulumAngle - bAngle) > 0.001)
			{
				pendulumAngle = bAngle;
				redraw = true;
			}
			if (soc != pendulum.BatterySOC)
			{
				soc = pendulum.BatterySOC;
				redraw = true;
			}
			
			if (redraw) this.Invalidate ( );
		}


		// ===========================================================================
		/// \brief	Draw the battery indicator
		///
		/// \param	e			Paint arguments
		/// \param hGround		Ground drawing height
		/// \param hPendulum	Pendulum drawing height
		/// \param p			Outline pen
		// ===========================================================================
		private void DrawBattery (PaintEventArgs e, int hGround, int hPendulum, Pen p)
		{
			Graphics g;
			Color [] colors = {Color.Red, Color.Orange, Color.GreenYellow, Color.LimeGreen};

			int hBat, wBat, off;
			int x, y;
			uint idx;
			float alpha;

			g = e.Graphics;
			
			// Adjust dimension
			hBat = (hPendulum * 4) / 10;
			wBat = (hBat) / 2;
			x = wBat / 3;
			y = Height - hGround - x;
			off = wBat / 10;

			// Draw battery outline
			g.DrawLine (p, x, y, x + wBat, y);
			g.DrawLine (p, x, y, x, y - hBat);
			g.DrawLine (p, x + wBat, y, x + wBat, y - hBat);
			
			g.DrawLine (p, x, y - hBat, x + wBat/4, y - hBat);
			g.DrawLine (p, x+3*wBat/4, y - hBat, x + wBat, y - hBat);

			g.DrawRectangle (p, x, y - hBat, wBat, hBat);
			g.FillRectangle (Brushes.White, x + wBat / 4, y - hBat-wBat/4, wBat / 2, wBat / 4);
		
			// Fill in battery
			if (Enabled)
			{
				if (soc > 20)
				{
					// Choose the color bucket
					idx = (soc-20) / 27;
					alpha = ((soc - 20) % 27) / 27.0f;
				
					// Blend colors
					int R = Convert.ToByte (colors [idx+1].R * alpha + colors [idx].R * (1.0f - alpha));
					int G = Convert.ToByte (colors [idx+1].G * alpha + colors [idx].G * (1.0f - alpha));
					int B = Convert.ToByte (colors [idx+1].B * alpha + colors [idx].B * (1.0f - alpha));
					Color c = Color.FromArgb (R, G, B);
					Brush b = new SolidBrush (c);
					
					// And draw
					hBat = (int) ((hBat-2*off) * ((soc - 20.0f) / 80.0f));
					g.FillRectangle (b, x+off, y-hBat-off, wBat-2*off, hBat);

					b.Dispose ( );
				}
				// Less than 20% -> cross
				else
				{
					p.Color = Color.Red;
					g.DrawLine (p, x, y - wBat / 2, x + wBat, y - wBat - wBat / 2);
					g.DrawLine (p, x + wBat, y - wBat / 2, x, y - wBat - wBat / 2);
				}
			}
		}


		// ===========================================================================
		/// \brief	Draw the sky and the ground
		///
		/// \param	e			Paint arguments
		/// \param hGround		Ground drawing height
		// ===========================================================================
		private void DrawBackground (PaintEventArgs e, int hGround)
		{
			Brush brushGround, brushSky;
			Brush brushSky2;
			Graphics g = e.Graphics;

			if (Enabled)
			{
				brushGround = new LinearGradientBrush (new Point (0, Height), new Point (0, Height - hGround), Color.Black, Color.SaddleBrown);
				brushSky = new LinearGradientBrush (new Point (0, 0), new Point (0, hGround / 2), Color.White, Color.CornflowerBlue);
				brushSky2 = Brushes.CornflowerBlue;
			}
			else
			{
				brushGround = new LinearGradientBrush (new Point (0, Height), new Point (0, Height - hGround), Color.Black, Color.Gray);
				brushSky = new LinearGradientBrush (new Point (0, 0), new Point (0, hGround / 2), Color.White, Color.LightGray);
				brushSky2 = Brushes.LightGray;				
			}

			g.FillRectangle (brushGround, 0, Height - hGround, Width, Height);
			g.FillRectangle (brushSky, 0, 0, Width, hGround / 2);
			g.FillRectangle (brushSky2, 0, hGround / 2, Width, Height - 3 * hGround / 2);

			brushGround.Dispose ( );
			brushSky.Dispose ( );
		}
		#endregion



		// ###########################################################################
		//
		// O V E R R I D E
		//
		// ###########################################################################
		#region OVERRIDE

		// ===========================================================================
		/// \brief	Called when the background has to be painted
		// ===========================================================================
		protected override void OnPaintBackground (PaintEventArgs e)
		{
			Brush brushPendulum;
			Graphics g;
			Point [] vertices = new Point [3];
			Pen p;			

			int hGround;
			int wPendulum = (int) (Width * 0.70);
			int hPendulum = (int) (Height / 2.2);

			g = e.Graphics;

			// Don't draw if control is too small
			if (Width < 10 || Height < 10)
			{
				base.OnPaintBackground (e);
				return;
			}

			// Adapt drawing to window size
			if (hPendulum * 2 > wPendulum) hPendulum = wPendulum / 2;
			else wPendulum = hPendulum * 2;
			hGround = (Height - hPendulum) / 3;

			p = new Pen (Brushes.White, wPendulum / 150);
			
			// Draw sky and ground
			DrawBackground (e, hGround);

			// Draw pendulum
			float r = hPendulum * 1.414f;

			if (Enabled) brushPendulum = Brushes.Beige;
			else brushPendulum = Brushes.Gray;

			vertices [0] = new Point (Width / 2, Height - hGround);
			vertices [1] = new Point (Width / 2 + (int) (Math.Cos (pendulumAngle + Math.PI * 0.75f) * r), 
										Height - hGround - (int) (Math.Sin (pendulumAngle + Math.PI * 0.75f) * r));
			vertices [2] = new Point (Width / 2 + (int) (Math.Cos (pendulumAngle + Math.PI * 0.25f) * r), 
										Height - hGround - (int) (Math.Sin (pendulumAngle + Math.PI * 0.25f) * r));
			g.FillPolygon (brushPendulum, vertices);

			//
			int x, y;
			r = hPendulum * 1.6f;
			x = Width / 2 + (int) (Math.Cos (pendulumAngle + Math.PI / 2) * r);
			y = Height - hGround - (int) (Math.Sin (pendulumAngle + Math.PI / 2) * r);

			r = hPendulum * 0.1f;
			vertices [0] = new Point (x, y);
			vertices [1] = new Point (x + (int) (r * Math.Cos (pendulumAngle + Math.PI * 0.75f)),
										y - (int) (r * Math.Sin (pendulumAngle + Math.PI * 0.70f)));
			vertices [2] = new Point (x + (int) (r * Math.Cos (pendulumAngle + Math.PI * 0.30f)),
										y - (int) (r * Math.Sin (pendulumAngle + Math.PI * 0.30f)));
			g.FillPolygon (Brushes.Green, vertices);

			//
			r = hPendulum * 1.6f;
			

			for (int i = 45; i <= 135; i += 5)
			{
				float x1, y1, x2, y2;
				float d;

				if (i % 15 == 0) d = 0.05f;
				else d = 0.02f;

				x1 = (float) Math.Cos (i*3.14f/180.0f) * r;
				y1 = (float) Math.Sin (i*3.14f/180.0f) * r;
				x2 = Width/2 +  x1*(1f+d);
				y2 = Height - hGround - y1 * (1f + d);
				x1 = Width / 2 + x1 * (1f);
				y1 = Height - hGround - y1 * (1f);
				g.DrawLine (p, x1, y1, x2, y2);
			}

			// Draw
			r = hPendulum * 1.6f;
			g.DrawArc (p, Width / 2 - r, Height - hGround - r, 2 * r, 2 * r, -45, -90);

			// Draw battery indicator
			DrawBattery (e, hGround, hPendulum, p);
			
			p.Dispose ( );
			
		}
		#endregion
	}
}
