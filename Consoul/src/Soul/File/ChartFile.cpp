#include "pch.h"
#include "ChartFile.h"

#include <fstream>

#include "Log.h"

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
			bool inSyncSection = false;
			bool inNoteSection = false;

			while (!file.eof())
			{
				std::getline(file, line, '\n');

				if (inSongSection)
				{
					if (line.find("}") != std::string::npos)
					{
						inSongSection = false;
						continue;
					}

					// Get Data in file
					LoadMetaData(line);
				}
				else if (inSyncSection)
				{
					if (line.find("}") != std::string::npos)
					{
						inSyncSection = false;
						continue;
					}

					// Get Sync in file
					LoadSyncData(line);
				}
				else if (inNoteSection)
				{
					if (line.find("}") != std::string::npos)
					{
						Log::LogInfo("Loaded " + std::to_string(m_Notes.size()) + " notes");
						return;
					}

					// Get Note data in file
					LoadNoteData(line);
				}
				else if (line.find("[Song]") != std::string::npos)
				{
					// Start section
					std::getline(file, line, '\n');
					inSongSection = true;
				}
				else if (line.find("[SyncTrack]") != std::string::npos)
				{
					// Start section
					std::getline(file, line, '\n');
					inSyncSection = true;
				}
				else if (line.find("ExpertSingle") != std::string::npos)
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
		std::stringstream stream(lineCopy);

		NoteData note;

		// Store note's timestamp
		stream >> note.TimeStamp;

		// Store whether note is a sustain or not
		std::string temp;
		stream >> temp;
		stream >> temp;

		if (temp == "S")
			return;

		// Store note's color
		int col;
		stream >> col;

		switch (col)
		{
		case 0:
			note.Color = ChartFile::NoteColor::Green;
			break;
		case 1:
			note.Color = ChartFile::NoteColor::Red;
			break;
		case 2:
			note.Color = ChartFile::NoteColor::Yellow;
			break;
		case 3:
			note.Color = ChartFile::NoteColor::Blue;
			break;
		case 4:
			note.Color = ChartFile::NoteColor::Orange;
			break;
		default:
			return;
		}

		// Store note's sustain length
		stream >> note.Length;

		if (note.Length == 0)
			note.Sustain = false;
		else
			note.Sustain = true;

		// For preventing 2 notes from appearing one the same line.... Turns out that's exactly
		// what we want
		//if (m_Notes.size() > 0 && m_Notes.back().TimeStamp == note.TimeStamp)
		//	m_Notes.back().Color = (NoteColor)(m_Notes.back().Color | note.Color);
		//else
		m_Notes.push(note);
	}

	void ChartFile::LoadSyncData(const std::string& line)
	{
		std::string lineCopy = line;
		std::stringstream stream(lineCopy);
		EventData event;
		
		stream >> event.TimeStamp;

		std::string type;
		stream >> type;
		stream >> type;

		if (type.find("B") != std::string::npos)
			stream >> event.BPM;
		else
			return;

		m_Events.push(event);
	}
}