#pragma once

#include <chrono>
#include <string>
#include <iostream>

class Timer
{
public:
	Timer(std::string timerName) { 
		start_time = std::chrono::high_resolution_clock::now();
		timer_name = timerName;
	}
	~Timer() {
		long long duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start_time).count();
		std::cout << "Timer " << timer_name << " Took: " << duration << std::endl;
	}

private:
	std::string timer_name;
	std::chrono::high_resolution_clock::time_point start_time;
};

