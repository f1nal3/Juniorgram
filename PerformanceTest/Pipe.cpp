#include "Pipe.hpp"

namespace PerformanceTest
{
	Pipe::Pipe(const std::string_view& command)
		: _pipe{ openPipe(command.data(), "w"), &closePipe } {}

	void Pipe::write(const std::string_view& data)
	{
		fputs(data.data(), _pipe.get());
	}

	std::string Pipe::read()
	{
		char tempBuffer[255];
		std::string realBuffer;

		while (!feof(_pipe.get()))
		{
			fgets(tempBuffer, 255, _pipe.get());
			realBuffer += tempBuffer;
		}

		return realBuffer;
		//fflush(_pipe.get());
	}
}
