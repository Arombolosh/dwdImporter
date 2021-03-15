/*	Authors: H. Fechner, A. Nicolai

	This file is part of the QtExt Library.
	All rights reserved.

	This software is copyrighted by the principle author(s).
	The right to reproduce the work (copy all or part of the source code),
	modify the source code or documentation, compile it to form object code,
	and the sole right to copy the object code thereby produced is hereby
	retained for the author(s) unless explicitely granted by the author(s).

*/

#ifndef QtExt_SleeperH
#define QtExt_SleeperH

#include <QThread>

namespace QtExt {

/*! \brief Contains some function for sleeping. Subclassed from QThread in order to make these functions public.*/
class Sleeper : public QThread
{
public:
	/*! Forces the current thread to sleep for usecs microseconds.*/
	static void usleep(unsigned long usecs) {
		QThread::usleep(usecs);
	}

	/*! Forces the current thread to sleep for msecs milliseconds.*/
	static void msleep(unsigned long msecs) {
		QThread::msleep(msecs);
	}

	/*! Forces the current thread to sleep for secs seconds.*/
	static void sleep(unsigned long secs){
		QThread::sleep(secs);
	}
};

} // end namespace QtExt

/*! \file QtExt_Sleeper.h
	\brief Contains the class Sleeper.
*/

#endif // QtExt_SleeperH
