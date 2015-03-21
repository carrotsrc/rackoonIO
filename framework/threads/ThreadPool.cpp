/* Copyright 2015 Charlie Fyvie-Gauld
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published 
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "ThreadPool.h"
#include "WorkerThread.h"

using namespace RackoonIO;

ThreadPool::ThreadPool() {

}

ThreadPool::ThreadPool(int nThreads) {
	size = nThreads;
}

void ThreadPool::setSize(int nThreads) {
	size = nThreads;
}

int ThreadPool::getSize() {
	return size;
}

void ThreadPool::init(std::condition_variable *condition, std::mutex *mutex, PackagePump *pump) {
	for(int i = 0; i < size; i++)
		pool.push_back(new WorkerThread(condition, mutex, pump));
}

WorkerThread* ThreadPool::getThread(int index) {
	if(index >= size)
		return NULL;

	return pool[index];
}

WorkerThread* &ThreadPool::operator[] (int index) {
	return pool[index];
}

