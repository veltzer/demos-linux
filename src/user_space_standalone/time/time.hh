#ifndef TIME_H
#define TIME_H

class Time {
	public:
		void setTime(int hours, int minutes, int seconds);
		void Tick();
		void printStd();
		void printMilitary();

		// setters and getters...
		void setHours(int hours);
		int getHours();
		void setMinutes(int minutes);
		int getMinutes();
		void setSeconds(int seconds);
		int getSeconds();
	private:
		int m_hours,m_minutes,m_seconds;
};

#endif // TIME_H
