//
// POSIX implementation is not mine.
// I have took it from http://win32eoposix.sourceforge.net/
//
#include <core/signal.h>
#include <core/exception.h>

#ifdef _WIN32
 #include <windows.h>
#else
 #include <sys/time.h>
 #include <pthread.h>
#endif

/* External functions */

#ifdef _WIN32
extern "C" __declspec(dllimport)
std::string handle_win_error();
#endif


/* Signal methods */

#ifndef _WIN32

namespace
{
	struct ListElement
	{
		pthread_mutex_t mutex;  // mutex for the conditional wait
		pthread_cond_t cond;
		ListElement* prev;
		ListElement* next;

		ListElement()
		{
			int err = pthread_mutex_init(&mutex, NULL);
			if (err != 0)
			{
				throw Exception(QString("pthread_mutex_init() failed: %1")
					.arg(QString::fromLocal8Bit(strerror(err))));
			}

			try
			{
				err = pthread_cond_init(&cond, NULL);
				if (err != 0)
				{
					throw Exception(QString("pthread_cond_init() failed: %1")
						.arg(QString::fromLocal8Bit(strerror(err))));
				}
			}
			catch (...)
			{
				pthread_mutex_destroy(&mutex);
				throw;
			}
		}

		~ListElement()
		{
			pthread_mutex_destroy(&mutex);
			pthread_cond_destroy(&cond);
		}
	};

	struct Event
	{
		ListElement* start;
		ListElement* end;
		pthread_mutex_t mutex;
		bool flag;

		Event()
			: start(NULL), end(NULL), flag(false)
		{
			int err = pthread_mutex_init(&mutex, NULL);
			if (err != 0)
			{
				throw Exception(QString("pthread_mutex_init() failed: %1")
					.arg(QString::fromLocal8Bit(strerror(err))));
			}
		}

		~Event()
		{
			assert(!start);
			pthread_mutex_destroy(&mutex);
		}

		void set()
		{
			MutexLocker lock1(mutex);
			flag = true;
			for (ListElement* p = start; p; p = p->next)
			{
				MutexLocker lock2(p->mutex);
				int err = pthread_cond_signal(&p->cond);
				if (err != 0)
				{
					throw Exception(QString("pthread_cond_signal() failed: %1")
						.arg(QString::fromLocal8Bit(strerror(err))));
				}
				if (p == end)
					break;
			}
		}

		bool wait(uint msecs)
		{
			bool result = true;
			ListElement e;

			MutexLocker lock1(mutex);
			add(&e);
			lock1.unlock();

			try
			{
				if (flag)
					flag = false;
				else
				{
					MutexLocker lock2(e.mutex);

					struct timespec ts;
					struct timeval tv;
					gettimeofday(&tv, NULL);
					ts.tv_nsec = tv.tv_usec * 1000L + ((long)msecs) * 1000000L;
					ts.tv_sec = tv.tv_sec;
					if (ts.tv_nsec >= 1000000000L)
					{
						ts.tv_sec += ts.tv_nsec / 1000000000L;
						ts.tv_nsec %= 1000000000L;
					}

					int err = pthread_cond_timedwait(&e.cond, &e.mutex, &ts);
					if (err == ETIMEDOUT)
						result = false;
					else if (err != 0)
					{
						throw Exception(QString("pthread_cond_timedwait() failed: %1")
							.arg(QString::fromLocal8Bit(strerror(err))));
					}

				}
			}
			catch (...)
			{
				try { lock1.relock(); } catch(...) { remove(&e); throw; }
				remove(&e);
				throw;
			}

			lock1.relock();
			remove(&e);

			return result;
		}

	private:
		class MutexLocker
		{
		public:
			MutexLocker(pthread_mutex_t& m)
				: mutex(m),
				  locked(false)
			{
				relock();
			}

			~MutexLocker()
			{
				if (locked)
					pthread_mutex_unlock(&mutex);
			}

			void unlock()
			{
				assert(locked);
				locked = false;
				pthread_mutex_unlock(&mutex);
			}

			void relock()
			{
				assert(!locked);
				int err = pthread_mutex_lock(&mutex);
				if (err != 0)
				{
					throw Exception(QString("pthread_mutex_lock() failed: %1")
						.arg(QString::fromLocal8Bit(strerror(err))));
				}
				locked = true;
			}

		private:
			pthread_mutex_t& mutex;
			bool locked;
		};

		void add(ListElement* e)
		{
			if (!start)
				start = e;
			else
			{
				end->next = e;
				e->prev = end;
			}
			end = e;
		}

		void remove(ListElement* e)
		{
			if (start == end)
				start = end = NULL;
			else if (start == e)
				start = start->next;
			else if (end == e)
				end = end->prev;
			else
			{
				e->prev->next = e->next;
				e->next->prev = e->prev;
			}
		}
	};
}

#endif

//
// Constructor
//
Signal::Signal()
{
#ifdef _WIN32
	m_Handle = CreateEvent(NULL, false, false, NULL);
	if (unlikely(!m_Handle))
		handle_win_error();
#else
	m_Handle = new Event;
#endif
}

//
// Destructor
//
Signal::~Signal()
{
#ifdef _WIN32
	CloseHandle((HANDLE)m_Handle);
#else
	delete (Event*)m_Handle;
#endif
}

//
// Emits the signal and wakes a single thread that is waiting for the signal
//
void Signal::signal()
{
#ifdef _WIN32
	SetEvent((HANDLE)m_Handle);
#else
	((Event*)m_Handle)->set();
#endif
}

//
// Waits for the signal to arrive
//
bool Signal::wait(uint msecs)
{
#ifdef _WIN32
	DWORD result = WaitForSingleObject((HANDLE)m_Handle, msecs);

	if (result == WAIT_OBJECT_0)
		return true;
	else if (result == WAIT_TIMEOUT)
		return false;
	else if (result == WAIT_ABANDONED)
		throw Exception("WaitForSingleObject() returned WAIT_ABANDONED");
	else
		handle_win_error();

	return false;
#else
	return ((Event*)m_Handle)->wait(msecs);
#endif
}
