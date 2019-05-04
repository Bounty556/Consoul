#pragma once

#include <queue>

namespace Soul {
	class SongFile
	{
	public:
		enum NoteColor : unsigned char
		{
			None = 0,
			Green = 1,
			Red = 2,
			Yellow = 4,
			Blue = 8,
			Orange = 16
		};

		struct NoteData
		{
			int TimeStamp;
			NoteColor Color;
			bool Sustain;
			int Length;
		};

		SongFile()
			: m_SongName("Unknown"), m_ArtistName("Unknown"), m_CharterName("Unknown"),
			m_ChartResolution(0) { }

		NoteData GetNextNote()
		{
			if (m_Notes.size() == 0)
			{
				return { 0, None, false, 0 };
			}
			NoteData note = m_Notes.front();
			m_Notes.pop();
			return note;
		}

		inline const NoteData& PeekNextNote() const { return m_Notes.front(); }
		inline const bool HasNextNote() const { return m_Notes.size() > 0; }
		inline const std::string& GetSongName() const { return m_SongName; }
		inline const std::string& GetArtistName() const { return m_ArtistName; }
		inline const std::string& GetCharterName() const { return m_CharterName; }
		inline const unsigned int GetResolution() const { return m_ChartResolution; }
	protected:
		virtual void LoadFile(const std::string& filePath) = 0;

	protected:
		std::string m_SongName;
		std::string m_ArtistName;
		std::string m_CharterName;
		unsigned int m_ChartResolution;
		std::queue<NoteData> m_Notes; // Timestamp, Note value pair
	};
}