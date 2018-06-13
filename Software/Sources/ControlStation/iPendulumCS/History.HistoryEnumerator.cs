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
/// \file	History.Enumerator.cs
/// \author	Jean-Sebastien Gonsette
///
/// Enumerator for the History class.
// ###########################################################################

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;

namespace iPendulumCS
{		
	public partial class History<T> : ISafeEnumerable<T>
	{		
		
		/// Enumerator for the SafeEnumerable interface
		public  class HistoryEnumerator<T> : IEnumerator<T>
		{
			#region DATA
			/// History to enumerate
			private readonly History<T> history;

			/// Standard enumerator on the wrapped collection
			private readonly IEnumerator<T> enumerator;

			/// Object used as a mutex
			private readonly object locker;

			/// Scaler function in order to scale the enumerated data
			private Scaler<T> scaler;

			/// First element of the history to start with
			private int idxStart;

			/// Last element of the history to end with
			private int idxEnd;

			/// Current enumerated element
			private int idx = -1;

			private int l = 0;
			#endregion



			// ###########################################################################
			//
			// P U B L I C
			//
			// ###########################################################################

			// ===========================================================================
			/// \brief	Enumerator construction
			// ===========================================================================
			public HistoryEnumerator (History<T> history, Scaler<T> s, int idxStart, int idxEnd, object locker)
			{
				this.history = history;
				this.locker = locker;
				this.scaler = s;
				this.idxStart = idxStart;
				this.idxEnd = idxEnd;

				// Get 'not thread safe' collection enumerator
				enumerator = history.items.GetEnumerator ( );

				// Entering lock in constructor
				l++;
				Monitor.Enter (locker);
			}


			// ===========================================================================
			/// \brief	Finalize the enumerator by releasing the lock
			// ===========================================================================
			public void Dispose ()
			{
				// Exiting lock on Dispose()
				l--;
				Monitor.Exit (locker);
			}
				

			// ===========================================================================
			/// \brief	Set the view to use for the enumeration
			// ===========================================================================
			public void SetView (Scaler<T> s, int idxStart, int idxEnd)
			{
				this.scaler = s;
				this.idxStart = idxStart;
				this.idxEnd = idxEnd;
			}


			// ===========================================================================
			/// \brief	Move on the next element of the enumerated collection
			// ===========================================================================
			public bool MoveNext ()
			{
				// Skip first items if needed
				while (idx < idxStart - 1)
				{
					enumerator.MoveNext ( );
					idx++;
				}

				// Skip last items if needed
				idx++;
				if (idx <= idxEnd || idxEnd == -1) return enumerator.MoveNext ( );
				else return false;
			}


			// ===========================================================================
			/// \brief	Reset the enumerator
			// ===========================================================================
			public void Reset ()
			{
				idx = -1;
				enumerator.Reset ( );
			}


			// ===========================================================================
			/// \brief	Return the current element of the enumeration
			// ===========================================================================
			public T Current
			{
				get 
				{
					// Aply scaler on current item if needed
					if (scaler != null) return scaler (enumerator.Current);
					else return enumerator.Current;
				}
			}


			// ===========================================================================
			/// \brief	Return the current element of the enumeration
			// ===========================================================================
			object System.Collections.IEnumerator.Current
			{
				get { return Current; }
			}
		}
		
	}
}
