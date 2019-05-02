#pragma once

#include <queue>

namespace Soul {
	class SongFile
	{
	public:
		enum Note : unsigned char
		{
			None = 0,
			Green = 1,
			Red = 2,
			Yellow = 4,
			Blue = 8,
			Orange = 16
		};

		SongFile()
			: m_SongName("Unknown"), m_ArtistName("Unknown"), m_CharterName("Unknown"),
			m_ChartResolution(0) { }

		Note GetNextNote()
		{
			if (m_Notes.size() == 0)
				return None;
			Note note = m_Notes.front().second;
			m_Notes.pop();
			return note;
		}

		inline const bool HasNextNote() const { return m_Notes.size() > 0; }
	protected:
		virtual void LoadFile(const std::string& filePath) = 0;

	protected:
		std::string m_SongName;
		std::string m_ArtistName;
		std::string m_CharterName;
		unsigned int m_ChartResolution;
		std::queue<std::pair<int, Note>> m_Notes; // Timestamp, Note value pair
	};
}