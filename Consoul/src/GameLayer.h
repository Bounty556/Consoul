#pragma once

#include "Soul/Soul.h"

#include <deque>
#include <fstream>
#include <sstream>

#include "Notes.h"

class GameLayer : public Soul::Layer
{
public:
	GameLayer(Soul::ChartFile* chart)
		: Layer(160, 45),
		m_Highway(new char[160 * 45]),
		m_BottomTimeStamp(0), m_TopTimeStamp(0),
		m_HitLineHeight(7),
		m_Score(0), m_ComboCount(0), m_Multiplier(1),
		m_HighwayCompression(1.25f),
		m_BPM(120.0f),
		m_TimeSinceStart(0),
		m_CurrentDistance(0.0), m_PreviousDistance(0.0),
		m_Chart(chart)
	{
		Soul::InputManager::AddKey(Soul::A);
		Soul::InputManager::AddKey(Soul::S);
		Soul::InputManager::AddKey(Soul::D);
		Soul::InputManager::AddKey(Soul::F);
		Soul::InputManager::AddKey(Soul::Space);
		Soul::InputManager::AddKey(Soul::Up);
		Soul::InputManager::AddKey(Soul::Left);

		LoadHighway();
		Soul::AudioEngine::Play("Songs/Soria Moria/song.ogg");
	}

	~GameLayer()
	{
		delete[] m_Highway;
		delete m_Chart;
	}

	bool Update(double deltaTime) override
	{
		// Update to check for strumming
		CheckNoteHits();

		m_TimeSinceStart += deltaTime;

		float bps = m_BPM / 60.0f;

		m_CurrentDistance = m_TimeSinceStart * ((double)m_Chart->GetResolution() * bps);
		m_BottomTimeStamp = (int)(m_CurrentDistance + m_PreviousDistance);
		m_TopTimeStamp = (int)((m_TimeSinceStart + m_HighwayCompression) * ((float)m_Chart->GetResolution() * bps) + m_PreviousDistance);

		while (m_Chart->HasNextNote() && m_Chart->PeekNextNote().TimeStamp <= m_TopTimeStamp)
			m_Notes.push_front(Group(m_Chart->GetNextNote()));

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
		for (const Group& group : m_Notes)
		{
			int yPos = group.GetYPos();
			if (yPos >= 11 && yPos <= 44)
				for (const Note note : group.GetNotes())
					m_Draw[yPos * 160 + (int)(note.GetXPos())] = '#';
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
		if (!m_Notes.empty() &&
			(Soul::InputManager::WasKeyPressed(Soul::Up) || Soul::InputManager::WasKeyPressed(Soul::Left)))
		{
			bool hit = false;
			for (auto it = m_Notes.end(); it != m_Notes.begin();)
			{
				it--;

				int noteDistance = (45 - m_HitLineHeight) - it->GetYPos();
				if (noteDistance <= 3 && noteDistance >= -2)
				{
					short keysDown = 0;
					for (const Note note : it->GetNotes())
						keysDown += note.GetAssociatedKey();

					short actualKeysDown = Soul::InputManager::GetKeysDown();

					// We don't want to count pressing Enter or Pipe as note selection
					if (Soul::InputManager::IsKeyDown(Soul::Up))
						actualKeysDown -= Soul::Up;
					if (Soul::InputManager::IsKeyDown(Soul::Left))
						actualKeysDown -= Soul::Left;

					// Don't prevent a hit if this is a single note and the notes to the left of it are pressed
					if (it->GetNotes().size() == 1)
					{
						// Remove all notes to the left of this one if applicable
						if (Soul::InputManager::IsKeyDown((Soul::Keys)keysDown))
						{
							switch (keysDown)
							{
							case Soul::Space:
								actualKeysDown = Soul::Space;
								break;
							case Soul::F:
								if (!Soul::InputManager::IsKeyDown(Soul::Space))
									actualKeysDown = Soul::F;
								break;
							case Soul::D:
								if (!Soul::InputManager::IsKeyDown(Soul::Space) &&
									!Soul::InputManager::IsKeyDown(Soul::F))
									actualKeysDown = Soul::D;
								break;
							case Soul::S:
								if (!Soul::InputManager::IsKeyDown(Soul::Space) &&
									!Soul::InputManager::IsKeyDown(Soul::F) &&
									!Soul::InputManager::IsKeyDown(Soul::D))
									actualKeysDown = Soul::S;
								break;
							}
						}
					}

					// Note was hit
					if (keysDown == actualKeysDown)
					{
						m_Notes.erase(it);
						hit = true;

						break;
					}
				}
				else if (noteDistance > 3)
				{
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
	std::deque<Group> m_Notes;
	Soul::ChartFile* m_Chart;
};