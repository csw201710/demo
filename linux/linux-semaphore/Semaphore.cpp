/**
 **	\file Semaphore.cpp
 **	\date  2007-04-13
 **	\author grymse@alhem.net
**/
/*
Copyright (C) 2007-2011  Anders Hedstrom

This library is made available under the terms of the GNU GPL, with
the additional exemption that compiling, linking, and/or using OpenSSL 
is allowed.

If you would like to use this library in a closed-source application,
a separate license agreement is available. For information about 
the closed-source license agreement for the C++ sockets library,
please visit http://www.alhem.net/Sockets/license.html and/or
email license@alhem.net.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#include "Semaphore.h"


#ifdef SOCKETS_NAMESPACE
namespace SOCKETS_NAMESPACE {
#endif


// ---------------------------------------------------------------
#ifdef _WIN32

Semaphore::Semaphore(value_t start_val)
{
	m_handle = ::CreateSemaphore((LPSECURITY_ATTRIBUTES)NULL, start_val, 1, (LPCTSTR)NULL);
}


Semaphore::~Semaphore()
{
	::CloseHandle(m_handle);
}


int Semaphore::Post()
{
	return (::ReleaseSemaphore(m_handle, 1, (LPLONG)NULL) != 0) ? 0 : -1;
}


int Semaphore::Wait()
{
	return (WaitForSingleObject(m_handle, INFINITE) == WAIT_OBJECT_0) ? 0 : -1;
}


int Semaphore::TryWait()
{
	return -1; // %! not implemented
}


int Semaphore::GetValue(int& i)
{
	return 0; // %! not implemented
}

// ---------------------------------------------------------------
#else
/*
int sem_init(sem_t *sem, int pshared, unsigned int value);
其中sem是要初始化的信号量，pshared表示此信号量是在进程间共享还是线程间共享，value是信号量的初始值。
如果 pshared 的值为 0，那么信号量将被进程内的线程共享
*/
Semaphore::Semaphore(value_t start_val)
{
	sem_init(&m_sem, 0, start_val);
}

/*
int sem_destroy(sem_t *sem);,其中sem是要销毁的信号量。只有用sem_init初始化的信号量才能用sem_destroy销毁。
*/
Semaphore::~Semaphore()
{
	sem_destroy(&m_sem);
}

/*
int sem_post(sem_t *sem); 释放信号量，让信号量的值加1。相当于V操作。
*/
int Semaphore::Post()
{
	return sem_post(&m_sem);
}

/*
int sem_wait(sem_t *sem);等待信号量，如果信号量的值大于0,将信号量的值减1,立即返回。如果信号量的值为0,则线程阻塞。相当于P操作。成功返回0,失败返回-1。
*/
int Semaphore::Wait()
{
	return sem_wait(&m_sem);
}


int Semaphore::TryWait()
{
	return sem_trywait(&m_sem);
}


int Semaphore::GetValue(int& i)
{
	return sem_getvalue(&m_sem, &i);
}

#endif


#ifdef SOCKETS_NAMESPACE
} // namespace SOCKETS_NAMESPACE {
#endif

