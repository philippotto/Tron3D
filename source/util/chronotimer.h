#pragma once
// STD
#include <chrono>

namespace troen
{
namespace util
{
	/*! This class provides an abstraction over std::chrono for elapsed time (including start/stop/reset functionality).*/
	class ChronoTimer
	{
	public:
		ChronoTimer(
			const bool start = true
			, const bool autoUpdate = true);

		virtual ~ChronoTimer();

		void setAutoUpdating(const bool autoUpdate);
		const bool autoUpdating() const;

		const bool paused() const;

		void update() const;
		const long double elapsed() const;

		void start();
		void pause();
		void stop();
		void reset();

	protected:
		bool m_paused;
		bool m_auto;

	protected:
		typedef std::chrono::high_resolution_clock clock;
		typedef clock::time_point time_point;

		typedef std::chrono::duration<long double, std::milli> milli;

		time_point m_t0;
		time_point m_tp; // time_point of last pausing

		mutable time_point m_t1;

		long double m_offset;
		mutable long double m_elapsed;
	};
} // end namespace util
} // end namespace troen