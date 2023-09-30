#include "Replay.h"
#include <fstream>

bool Replay::load(std::string path)
{
	frames.clear();
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		return false;
	}

	size_t acSize = 0;
	size_t fcSize = 0;

	file.read((char*)&fps, sizeof(float));

	file.read((char*)&acSize, 4);
	file.read((char*)&fcSize, 4);

	frames.resize(fcSize);

    file.seekg(24 * acSize, std::ios::cur);

	file.read((char*)&frames[0], sizeof(FrameCapture) * fcSize);

	file.close();

    return true;
}