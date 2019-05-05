#pragma once

#include "Soul/File/ChartFile.h"

// A single note
class Note
{
public:
	Note(Soul::ChartFile::NoteColor color)
	{
		switch (color)
		{
		case Soul::ChartFile::Green:
			m_StartXPos = m_XPos = 67;
			m_FinalXPos = 45;
			m_AssociatedKey = Soul::A;
			break;
		case Soul::ChartFile::Red:
			m_StartXPos = m_XPos = 73;
			m_FinalXPos = 62;
			m_AssociatedKey = Soul::S;
			break;
		case Soul::ChartFile::Yellow:
			m_StartXPos = m_XPos = m_FinalXPos = 79;
			m_AssociatedKey = Soul::D;
			break;
		case Soul::ChartFile::Blue:
			m_StartXPos = m_XPos = 85;
			m_AssociatedKey = Soul::F;
			m_FinalXPos = 96;
			break;
		case Soul::ChartFile::Orange:
			m_StartXPos = m_XPos = 91;
			m_AssociatedKey = Soul::Space;
			m_FinalXPos = 113;
			break;
		}
	}

	void Update(float percentDone)
	{
		m_XPos = percentDone * (float)(m_FinalXPos - m_StartXPos) + m_StartXPos;
	}

	inline const int GetXPos() const { return (int)m_XPos; }
	inline const Soul::Keys GetAssociatedKey() const { return m_AssociatedKey; }
private:
	float m_StartXPos, m_XPos, m_FinalXPos;
	Soul::Keys m_AssociatedKey;
};

// A group of notes on the same hitframe
class Group
{
public:
	Group(Soul::ChartFile::NoteData data)
		: m_Data(data),
		m_StartYPos(11), m_YPos(11), m_FinalYPos(44)
	{
		if (data.Color & Soul::ChartFile::Green)
			m_Notes.emplace_back(Note(Soul::ChartFile::Green));
		if (data.Color & Soul::ChartFile::Red)
			m_Notes.emplace_back(Note(Soul::ChartFile::Red));
		if (data.Color & Soul::ChartFile::Yellow)
			m_Notes.emplace_back(Note(Soul::ChartFile::Yellow));
		if (data.Color & Soul::ChartFile::Blue)
			m_Notes.emplace_back(Note(Soul::ChartFile::Blue));
		if (data.Color & Soul::ChartFile::Orange)
			m_Notes.emplace_back(Note(Soul::ChartFile::Orange));
	}

	void Update(int bottomTimeStamp, int topTimeStamp, int hitLineHeight)
	{
		// Interpolate position between start and end x's and y's
		float percentDone = ((float)(topTimeStamp - m_Data.TimeStamp) / (float)(topTimeStamp - bottomTimeStamp))
			- ((float)hitLineHeight / 33.0f);

		m_YPos = percentDone * (float)(m_FinalYPos - m_StartYPos) + m_StartYPos;

		for (Note& note : m_Notes)
			note.Update(percentDone);
	}

	inline const std::vector<Note>& GetNotes() const { return m_Notes; };
	inline const int GetYPos() const { return (int)m_YPos; }
private:
	float m_StartYPos, m_YPos, m_FinalYPos;
	Soul::ChartFile::NoteData m_Data;
	std::vector<Note> m_Notes;
};