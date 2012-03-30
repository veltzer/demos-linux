#include"time.hh"
#include<stdio.h> // for printf(3)

void Time::setTime(int hours, int minutes, int seconds) {
	m_hours=hours;
	m_minutes=minutes;
	m_seconds=seconds;
}

void Time::Tick() {
	m_seconds++;
	if(m_seconds==60) {
		m_seconds=0;
		m_minutes++;
		if(m_minutes==60) {
			m_minutes=0;
			m_hours++;
			if(m_hours==24) {
				m_hours=0;
			}
		}
	}
}

void Time::printStd() {
	const char* part;
	int hours;
	if(m_hours>=12) {
		part="PM";
		hours=m_hours-12;
	} else {
		part="AM";
		hours=m_hours;
	}
	printf("%2d:%02d:%02d %s\n",
		hours,
		m_minutes,
		m_seconds,
		part
	);
}

void Time::printMilitary() {
	printf("%02d:%02d\n",
		m_hours,
		m_minutes
	);
}

// setters and getters...
void Time::setHours(int hours) {
	m_hours=hours;
}

int Time::getHours() {
	return m_hours;
}

void Time::setMinutes(int minutes) {
	m_minutes=minutes;
}

int Time::getMinutes() {
	return m_minutes;
}

void Time::setSeconds(int seconds) {
	m_seconds=seconds;
}

int Time::getSeconds() {
	return m_seconds;
}
