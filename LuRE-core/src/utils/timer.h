#pragma once

#include <GLFW\glfw3.h>
#include <iostream>

namespace lumi {

	class Timer
	{
	private:
		unsigned int _frames, _updates;
		double _lastTime, _lastUpdateTime, _currentTime, _diff;

	public:

		Timer()
			: _frames(0), _updates(0), _lastUpdateTime(glfwGetTime()), _currentTime(0), _diff(0), _lastTime(glfwGetTime())
		{

		}

		inline bool update(double interval, float* elapsed_time)
		{

			_currentTime = glfwGetTime();
			*elapsed_time = _currentTime - _lastUpdateTime;

			if (_currentTime - _lastUpdateTime >= interval) {
				_updates++;
				_lastUpdateTime = _currentTime;
				return true;
			}

			return false;
		}

		inline bool tick()
		{
			_currentTime = glfwGetTime();

			_frames++;
			if (_currentTime - _lastTime >= 1.0 - _diff)
			{
				std::cout << "FPS: " << _frames << " ms: " << 1000.0/double(_frames) << " updates: " << _updates << "\r\n";
				_frames = 0;
				_updates = 0;
				_lastTime = _currentTime;
				_diff = glfwGetTime() - _lastTime;

				return true;
			}
			
			return false;
		}

	};

}