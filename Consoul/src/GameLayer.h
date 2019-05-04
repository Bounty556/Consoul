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

	void Update(int bottomTimeStamp, int topTimeStamp, int hitLineHeight)
	{
		// Interpolate position between start and end x's and y's
		float percentDone = ((float)(topTimeStamp - note.TimeStamp) / (float)(topTimeStamp - bottomTimeStamp))
			- ((float)hitLineHeight / 33.0f);

		m_XPos = percentDone * (float)(m_FinalXPos - m_StartXPos) + m_StartXPos;
		m_YPos = percentDone * (float)(m_FinalYPos - m_StartYPos) + m_StartYPos;
	}

	inline const Soul::ChartFile::NoteData GetData() const { return note; }
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
		m_HitLineHeight(6),
		m_Score(0),
		m_ComboCount(0),
		m_Multiplier(1),
		m_HighwayCompression(1.25f),
		m_BPM(120.0f),
		m_TimeSinceStart(0),
		m_CurrentDistance(0.0),
		m_PreviousDistance(0.0),
		m_Chart(chart)
	{
		Soul::InputManager::AddKey(Soul::A);
		Soul::InputManager::AddKey(Soul::S);
		Soul::InputManager::AddKey(Soul::D);
		Soul::InputManager::AddKey(Soul::F);
		Soul::InputManager::AddKey(Soul::Space);

		LoadHighway();
		Soul::AudioEngine::Play("Songs/Trading Shadows/song.ogg");
	}

	~GameLayer()
	{
		delete[] m_Highway;
		delete m_Chart;
	}

	bool Update(double deltaTime) override
	{
		CheckNoteHits();

		m_TimeSinceStart += deltaTime;

		float bps = m_BPM / 60.0f;

		m_CurrentDistance = m_TimeSinceStart * ((double)m_Chart->GetResolution() * bps);
		m_BottomTimeStamp = (int)(m_CurrentDistance + m_PreviousDistance);
		m_TopTimeStamp = (int)((m_TimeSinceStart + m_HighwayCompression) * ((float)m_Chart->GetResolution() * bps) + m_PreviousDistance);

		while (m_Chart->HasNextNote() && m_Chart->PeekNextNote().TimeStamp <= m_TopTimeStamp)
			m_Notes.push_front(Note(m_Chart->GetNextNote()));

		while (m_Chart->HasNextEvent() && m_Chart->PeekNextEvent().TimeStamp <= m_BottomTimeStamp)
		{
			m_BPM = m_Chart->GetNextEvent().BPM / 1000.0f; // Change the bpm

			m_PreviousDistance += m_CurrentDistance;
			m_CurrentDistance = 0;
			m_TimeSinceStart = 0;

			Soul::Log::LogInfo("BPM changed to: " + std::to_string(m_BPM));
		}

		for (auto it = m_Notes.begin(); it != m_Notes.end(); ++it)
			it->Update(m_BottomTimeStamp, m_TopTimeStamp, m_HitLineHeight);

		while (!m_Notes.empty() && m_Notes.back().GetYPos() > 44)
		{
			m_Notes.pop_back();
			NoteMissed();
		}

		return true;
	}

	const char* Draw() const override
	{
		// Draw Highway template
		for (int i = 0; i < 160 * 45 - 1; ++i)
			m_Draw[i] = m_Highway[i];

		// Draw Notes
		for (auto it = m_Notes.begin(); it != m_Notes.end(); ++it)
		{
			Note note = *it;
			if (note.GetYPos() >= 11 && note.GetYPos() <= 44)
				m_Draw[note.GetYPos() * 160 + note.GetXPos()] = '#';
		}

		//Draw Combo Bar
		for (int i = 0; i < m_ComboCount; i++)
		{
			m_Draw[15 * 160 + 119 + i] = '#';
			m_Draw[16 * 160 + 119 + i] = '#';
		}

		// Draw Multiplier
		m_Draw[16 * 160 + 136] = std::to_string(m_Multiplier).c_str()[0];

		// Draw Score
		DrawToBuffer(std::to_string(m_Score).c_str(), 120, 12);

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

	void CheckNoteHits()
	{
		if (!m_Notes.empty() && Soul::InputManager::WasKeyPressed(Soul::A))
		{
			bool hit = false;
			for (auto it = m_Notes.end(); it != m_Notes.begin();)
			{
				it--;

				// Note hit
				if (it->GetData().Color == Soul::ChartFile::Green &&
					std::abs(it->GetYPos() - (45 - m_HitLineHeight)) <= 3)
				{
					m_Notes.erase(it);
					hit = true;
					break;
				}
			}

			if (hit)
				NoteHit();
			else
				NoteMissed();
		}
		if (!m_Notes.empty() && Soul::InputManager::WasKeyPressed(Soul::S))
		{
			bool hit = false;
			for (auto it = m_Notes.end(); it != m_Notes.begin();)
			{
				it--;

				if (it->GetData().Color == Soul::ChartFile::Red &&
					std::abs(it->GetYPos() - (45 - m_HitLineHeight)) <= 3)
				{
					m_Notes.erase(it);
					hit = true;
					break;
				}
			}

			if (hit)
				NoteHit();
			else
				NoteMissed();
		}
		if (!m_Notes.empty() && Soul::InputManager::WasKeyPressed(Soul::D))
		{
			bool hit = false;
			for (auto it = m_Notes.end(); it != m_Notes.begin();)
			{
				it--;

				if (it->GetData().Color == Soul::ChartFile::Yellow &&
					std::abs(it->GetYPos() - (45 - m_HitLineHeight)) <= 3)
				{
					m_Notes.erase(it);
					hit = true;
					break;
				}
			}

			if (hit)
				NoteHit();
			else
				NoteMissed();
		}
		if (!m_Notes.empty() && Soul::InputManager::WasKeyPressed(Soul::F))
		{
			bool hit = false;
			for (auto it = m_Notes.end(); it != m_Notes.begin();)
			{
				it--;

				if (it->GetData().Color == Soul::ChartFile::Blue &&
					std::abs(it->GetYPos() - (45 - m_HitLineHeight)) <= 3)
				{
					m_Notes.erase(it);
					hit = true;
					break;
				}
			}

			if (hit)
				NoteHit();
			else
				NoteMissed();
		}
		if (!m_Notes.empty() && Soul::InputManager::WasKeyPressed(Soul::Space))
		{
			bool hit = false;
			for (auto it = m_Notes.end(); it != m_Notes.begin();)
			{
				it--;

				if (it->GetData().Color == Soul::ChartFile::Orange &&
					std::abs(it->GetYPos() - (45 - m_HitLineHeight)) <= 3)
				{
					m_Notes.erase(it);
					hit = true;
					break;
				}
			}

			if (hit)
				NoteHit();
			else
				NoteMissed();
		}
	}

	void NoteHit()
	{
		// Check current combo and multiplier
		if (m_Multiplier == 4)
		{
			if (++m_ComboCount > 10)
				m_ComboCount = 10;
		}
		else
		{
			// Add to combo meter and adjust multiplier
			if (++m_ComboCount >= 10)
			{
				m_Multiplier++;
				m_ComboCount = 0;
			}
		}

		// Award points
		m_Score += 10 * m_Multiplier;
	}

	void NoteMissed()
	{
		m_ComboCount = 0;
		m_Multiplier = 1;
	}

private:
	char* m_Highway;
	int m_BottomTimeStamp, m_TopTimeStamp;
	int m_HitLineHeight;
	int m_Score;
	short m_ComboCount;
	short m_Multiplier;
	float m_HighwayCompression;
	float m_BPM;
	double m_TimeSinceStart;
	double m_CurrentDistance;
	double m_PreviousDistance;
	std::deque<Note> m_Notes;
	Soul::ChartFile* m_Chart;
};