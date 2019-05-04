#pragma once

#include "Soul/Soul.h"

#include <deque>
#include <fstream>
#include <sstream>

class Note
{
public:
	Note() = delete;

	Note(Soul::ChartFile::NoteData data)
		: note(data)
	{
		m_StartYPos = m_YPos = 11;
		m_FinalYPos = 44;

		switch (data.Color)
		{
		case Soul::ChartFile::Green:
			m_StartXPos = m_XPos = 67;
			m_FinalXPos = 45;
			break;
		case Soul::ChartFile::Red:
			m_StartXPos = m_XPos = 73;
			m_FinalXPos = 62;
			break;
		case Soul::ChartFile::Yellow:
			m_StartXPos = m_XPos = m_FinalXPos = 79;
			break;
		case Soul::ChartFile::Blue:
			m_StartXPos = m_XPos = 85;
			m_FinalXPos = 96;
			break;
		case Soul::ChartFile::Orange:
			m_StartXPos = m_XPos = 91;
			m_FinalXPos = 113;
			break;
		}
	}

	void Update(int bottomTimeStamp, int topTimeStamp)
	{
		// Interpolate position between start and end x's and y's
		float percentDone = (float)(topTimeStamp - note.TimeStamp) / (float)(topTimeStamp - bottomTimeStamp);

		m_XPos = percentDone * (float)(m_FinalXPos - m_StartXPos) + m_StartXPos;
		m_YPos = percentDone * (float)(m_FinalYPos - m_StartYPos) + m_StartYPos;
	}

	inline const int GetXPos() const { return (int)m_XPos; }
	inline const int GetYPos() const { return (int)m_YPos; }
private:
	float m_StartXPos, m_StartYPos, m_XPos, m_YPos, m_FinalXPos, m_FinalYPos;
	Soul::ChartFile::NoteData note;
};

class GameLayer : public Soul::Layer
{
public:
	GameLayer(Soul::ChartFile* chart)
		: Layer(160, 45),
		m_Highway(new char[160 * 45]),
		m_BottomTimeStamp(0), m_TopTimeStamp(0),
		m_HighwayCompression(3.0f),
		m_TimeSinceStart(0),
		m_Chart(chart)
	{
		LoadHighway();
	}

	~GameLayer()
	{
		delete[] m_Highway;
		delete m_Chart;
	}

	bool Update(float deltaTime) override
	{
		m_TimeSinceStart += deltaTime;

		m_BottomTimeStamp = (int)(m_TimeSinceStart * m_Chart->GetResolution());
		m_TopTimeStamp = (int)((m_TimeSinceStart + m_HighwayCompression) * m_Chart->GetResolution());

		while (m_Chart->HasNextNote() && m_Chart->PeekNextNote().TimeStamp <= m_TopTimeStamp)
			m_Notes.push_front(Note(m_Chart->GetNextNote()));

		for (auto it = m_Notes.begin(); it != m_Notes.end(); ++it)
			it->Update(m_BottomTimeStamp, m_TopTimeStamp);

		while (!m_Notes.empty() && m_Notes.back().GetYPos() > 44)
			m_Notes.pop_back();

		return true;
	}

	const char* Draw() const override
	{
		for (int i = 0; i < 160 * 45 - 1; ++i)
			m_Draw[i] = m_Highway[i];

		for (auto it = m_Notes.begin(); it != m_Notes.end(); ++it)
		{
			m_Draw[(*it).GetYPos() * 160 + (*it).GetXPos()] = '#';
		}

		return m_Draw;
	}

private:
	void LoadHighway()
	{
		std::ifstream file("Highway.txt");

		if (file.is_open())
		{
			Soul::Log::LogInfo("Successfully loaded file Highway.txt");

			std::string line;
			int lineNum = 0;

			while (!file.eof())
			{
				std::getline(file, line, '\n');

				for (int i = 0; i < line.length(); ++i)
				{
					m_Draw[lineNum * 160 + i] = line[i];
					m_Highway[lineNum * 160 + i] = line[i];
				}

				lineNum++;
			}

			m_Draw[160 * 45 - 1] = '\0';
			m_Highway[160 * 45 - 1] = '\0';
		}
	}

private:
	char* m_Highway;
	int m_BottomTimeStamp, m_TopTimeStamp;
	float m_HighwayCompression;
	float m_TimeSinceStart;
	std::deque<Note> m_Notes;
	Soul::ChartFile* m_Chart;
};