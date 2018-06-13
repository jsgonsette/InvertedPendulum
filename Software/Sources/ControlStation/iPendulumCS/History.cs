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
/// \file	History.cs
/// \author	Jean-Sebastien Gonsette
///
/// Implementation of a thread-safe history through the specified
/// ISafeEnumerable interface. An history is a wrapper around the generic
/// LinkedList<T> class.
// ###########################################################################

using System.Collections.Generic;
using System.Linq;

namespace iPendulumCS
{

	/// Scaling function to transform history data during enumeration
	public delegate T Scaler<T> (T t);
		
	/// Safe enumeration interface
	public interface ISafeEnumerable<T> : IEnumerable<T>
	{
		void BeginLockedEnumeration ();
		void EndLockedEnumeration ();
		void SetView (int idxStart, int idxEnd, Scaler<T> scaler);
	}
	
	/// Container that offers thread-safe operation on arbitrary data
	public partial class History<T> : ISafeEnumerable<T>
	{
		#region DATA

		/// Scaling fonction to transform the data
		private Scaler<T> scaler;

		/// Lock object to protect access to the history
		private readonly object locker = new object ();

		/// Not thread safe collection
		private LinkedList<T> items;

		/// First item to enumerate in the history
		private int idxStart;

		/// Last item to enumerate in the history
		private int idxEnd;

		/// Maximum number of items in the history
		private int length;

		/// Dedicated history enumerator
		private HistoryEnumerator<T> safeEnumerator;

		#endregion



		// ###########################################################################
		//
		// P U B L I C
		//
		// ###########################################################################

		// ===========================================================================
		/// \brief	Initialize the history for a given max length
		// ===========================================================================
		public History (int length) 
		{
			this.length = length;
			this.items = new LinkedList<T> ( );
			this.idxStart = 0;
			this.idxEnd = -1;
			this.safeEnumerator = null;
		}


		// ===========================================================================
		/// \brief	Add an element at the end of the history
		// ===========================================================================
		public void Add (T item)
		{
			// Take the lock and add the item
			lock (locker)
			{
				items.AddLast (item);

				// First items are also remove to not go beyond capacity
				while (items.Count ( ) > length) items.RemoveFirst ( );
			}
		}


		// ===========================================================================
		/// \brief	Return the last element of the history
		// ===========================================================================
		public T Last ()
		{
			T val;

			// Take the lock to get the last element
			lock (locker) { val = items.Last ( ); }
			return val;
		}


		// ===========================================================================
		/// \brief	Return the first element of the history
		// ===========================================================================
		public T First ()
		{
			T val;

			// Take the lock to get the last element
			lock (locker) { val = items.First ( ); }
			return val;
		}


		// ===========================================================================
		/// \brief	Clear the history
		// ===========================================================================
		public void Clear ()
		{
			// Take the lock and clear the collection
			lock (locker) { items.Clear ( ); }
		}


		// ===========================================================================
		/// \brief	Define the history window to use for next enumeration
		// ===========================================================================
		public void SetView (int idxStart, int idxEnd, Scaler<T> scaler)
		{
			this.scaler = scaler;
			this.idxStart = idxStart;
			this.idxEnd = idxEnd;

			if (safeEnumerator != null) safeEnumerator.SetView (scaler, idxStart, idxEnd);
		}


		// ===========================================================================
		/// \brief	Return a new enumerator on this history
		// ===========================================================================
		public IEnumerator<T> GetEnumerator ()
		{
			IEnumerator<T> e;

			lock (locker)
			{
				// Create a new enumerator only if 'BeginLockedEnumeration' is not in use
				if (safeEnumerator == null)
					e = new HistoryEnumerator<T> (this, scaler, idxStart, idxEnd, locker);
				else
				{
					safeEnumerator.Reset ( );
					e = safeEnumerator;
				}
			}

			return e;
		}


		// ===========================================================================
		/// \brief	Return a new enumerator on this history
		// ===========================================================================
		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator ()
		{
			return GetEnumerator ( );
		}


		// ===========================================================================
		/// \brief	Begin explicitly a safe enumeration
		// ===========================================================================
		public void BeginLockedEnumeration ()
		{
			// Creating the enumerator will get the lock
			safeEnumerator = new HistoryEnumerator<T> (this, scaler, idxStart, idxEnd, locker);
		}


		// ===========================================================================
		/// \brief	Terminate explicitly a safe enumeration
		// ===========================================================================
		public void EndLockedEnumeration ()
		{
			HistoryEnumerator<T> e;

			e = safeEnumerator;
			safeEnumerator = null;

			// Disposing the enumerator will release the lock
			e.Dispose ( );
		}


		// ===========================================================================
		/// \brief	Return the number of elements in the history
		// ===========================================================================
		public int Count ()
		{
			int n;
			lock (locker) { n = items.Count; }
			return n;
		}
	}
}
