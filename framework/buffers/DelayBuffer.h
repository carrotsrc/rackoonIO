/*
 * Copyright (C) 2015  Charlie Fyvie-Gauld
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DELAYBUFFER_H
#define DELAYBUFFER_H
#include "common.h"

namespace RackoonIO {

namespace Buffers {
/** Buffer to act as a general delay line
 *
 * This buffer implements a general purpose delay line. It's
 * main usage is passing it periods of samples until its load
 * reaches it's capacity. When the buffer is flushed, it flushes
 * all the samples at once.
 *
 * General usage:
 *
 * Keep supplying it periods of samples until its state changes
 * from OK to WAIT, at which point is at capacity
 *
 * When you need the samples, flush it to get a pointer to all the samples
 */

/** @todo
 * Once samples are set to a standard data type (floating point) then
 * this buffer can be given a pointer to a cache object to flush into
 * directly instead of two memcpys to get it into cache
 */

template<typename T>
class DelayBuffer
{
public:
	/// The various states the buffer will take on
	enum State {
		OK, ///< The buffer can be supplied more data
		WAIT ///< The buffer is full and can't take any more
	};
	typedef State E;
	DelayBuffer(int);
	State supply(const T*, int);
	const T* flush();
	int getLoad();
	State hasCapacity(int pSize);

private:
	int bSize, load;
	T *buffer;
};

// Template Defintions

/** Initialise the buffer size
 *
 * The interal buffer is allocated with the specified size
 */
template<typename T>
DelayBuffer<T>::DelayBuffer(int size) {
	bSize = size;
	buffer = (T*) malloc(size * sizeof(T));
	load = 0;
}

/** Get the current sample load
 *
 * @return The number of samples in the buffer
 */
template<typename T>
int DelayBuffer<T>::getLoad() {
	return load;
}

/** Supply a sample period to the buffer
 *
 * Supply a period of pSize samples. The method
 * will return the state of the buffer
 *
 * @param period A pointer to the period of samples
 * @param pSize The number of samples in period
 * @return OK if frames were copied; WAIT if the buffer's load is at full capacity
 */
template<typename T>
typename DelayBuffer<T>::State
DelayBuffer<T>::supply(const T *period, int pSize) {
	if(load + pSize > bSize)
		return WAIT;

	memcpy(buffer+load, period, pSize * sizeof(T));
	load += pSize;
	return OK;
}

/** Flush all the samples out of the buffer
 *
 * This method will return a pointer to the samples
 * and reset the load to zero.
 *
 * @return A const pointer to the samples
 */
template<typename T>
const T* DelayBuffer<T>::flush() {
	load = 0;
	return buffer;
}
/** Check whether the buffer has enough room left
 *
 * @param pSize The size of the next period
 * @return OK if load is below capacity; WAIT if at full capacity
 */
template<typename T>
typename DelayBuffer<T>::State
DelayBuffer<T>::hasCapacity(int pSize) {
	if(load + pSize > bSize)
		return WAIT;

	return OK;
}

/** Check if buffer has enough capacity for sample period
 *
 * @param pSize The size of the period in frames
 * @return OK if there is capacity; WAIT if it can't take the load
 */
template<typename T>
typename DelayBuffer<T>::State
DelayBuffer<T>::hasCapacity(int pSize) {
	if(load + pSize > bSize)
		return WAIT;

	return OK;
}


} // Buffers

} // RackoonIO
#endif // DELAYBUFFER_H
