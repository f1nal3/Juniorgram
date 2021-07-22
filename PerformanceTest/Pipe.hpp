#pragma once
#include <memory>
#include <string_view>

#include <cstdio>

#ifdef _WIN32

#define openPipe  _popen
#define closePipe _pclose

#elif

#define openPipe  popen
#define closePipe pclose

#endif // 

namespace PerformanceTest
{
	class Pipe
	{
	private:

		std::unique_ptr<FILE, decltype(&closePipe)> _pipe;

	public:

		Pipe(const std::string_view& command);

	public:

		void write(const std::string_view& data);
		std::string read();

	};
}
