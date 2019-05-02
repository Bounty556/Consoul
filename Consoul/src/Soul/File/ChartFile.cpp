#include "pch.h"
#include "ChartFile.h"

#include <fstream>

#include "Log.h"
#include "Utility.h"

namespace Soul {
	ChartFile::ChartFile(const std::string& filePath)
		: SongFile()
	{
		LoadFile(filePath);
	}

	void ChartFile::LoadFile(const std::string& filePath)
	{
		std::ifstream file(filePath + ".chart");

		if (file.is_open())
		{
			Log::LogInfo("Successfully loaded file " + filePath);

			std::string line;

			bool inSongSection = false;
			bool inNoteSection = false;

			while (!file.eof())
			{
				std::getline(file, line, '\n');

				if (inSongSection)
				{
					if (line == "}")
					{
						inSongSection = false;
						continue;
					}

					// Get Data in file
					LoadMetaData(line);
				}
				else if (inNoteSection)
				{
					if (StartsWith(line.c_str(), "}"))
					{
						Log::LogInfo("Loaded " + std::to_string(m_Notes.size()) + " notes");
						return;
					}

					// Get Note data in file
					LoadNoteData(line.c_str());
				}
				else if (StartsWith(line.c_str(), "[Song]"))
				{
					// Start section
					std::getline(file, line, '\n');
					inSongSection = true;
				}
				else if (StartsWith(line.c_str(), "[ExpertSingle]"))
				{
					// Notes section
					std::getline(file, line, '\n');
					inNoteSection = true;
				}
			}
		}
		else
		{
			Log::LogError(std::string("Could not load file ") + filePath);
		}
	}

	void ChartFile::LoadMetaData(const std::string& line)
	{
		if (line.find("Name =") != std::string::npos)
		{
			std::string lineCopy = line.substr(line.find_first_of('\"'));
			lineCopy = lineCopy.substr(0, line.find_last_of('\"'));
			m_SongName = lineCopy;
		}
		else if (line.find("Artist =") != std::string::npos)
		{
			std::string lineCopy = line.substr(line.find_first_of('\"'));
			lineCopy = lineCopy.substr(0, line.find_last_of('\"'));
			m_ArtistName = lineCopy;
		}
		else if (line.find("Charter =") != std::string::npos)
		{
			std::string lineCopy = line.substr(line.find_first_of('\"'));
			lineCopy = lineCopy.substr(0, line.find_last_of('\"'));
			m_CharterName = lineCopy;
		}
		else if (line.find("Resolution =") != std::string::npos)
		{
			std::string lineCopy = line.substr(line.find_last_of(' '));
			m_ChartResolution = std::atoi(lineCopy.c_str());
		}
	}

	void ChartFile::LoadNoteData(const std::string& line)
	{
		std::string lineCopy = line;
		std::vector<int> data;
		std::stringstream stream(lineCopy);

		std::string temp;
		int found;
		while (!stream.eof())
		{
			stream >> temp;
			if (std::stringstream(temp) >> found)
				data.push_back(found);

			temp = "";
		}

		auto it = data.begin();

		int timeStamp = *it;
		Note note = (Note)*(it + 1);
		Note noteValue = None;

		switch (note)
		{
		case 0:
			noteValue = Green;
			break;
		case 1:
			noteValue = Red;
			break;
		case 2:
			noteValue = Yellow;
			break;
		case 3:
			noteValue = Blue;
			break;
		case 4:
			noteValue = Orange;
			break;
		default:
			noteValue = None;
		}

		if (m_Notes.size() > 0 && m_Notes.back().first == timeStamp)
			m_Notes.back().second = (Note)(m_Notes.back().second | noteValue);
		else
			m_Notes.push(std::pair<int, Note>(timeStamp, noteValue));
	}
}