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
		m_PreviousNoteHit(false),
		m_HighwayCompression(1.25f),
		m_BPM(120.0f),
		m_TimeSinceStart(0),
		m_CurrentDistance(0.0), m_PreviousDistance(0.0),
		m_Chart(chart)
	{
		m_HopoThreshold = m_Chart->GetResolution() / 4;

		// All keys that the game uses
		Soul::InputManager::AddKey(Soul::A);
		Soul::InputManager::AddKey(Soul::S);
		Soul::InputManager::AddKey(Soul::D);
		Soul::InputManager::AddKey(Soul::F);
		Soul::InputManager::AddKey(Soul::Space);
		Soul::InputManager::AddKey(Soul::Up);
		Soul::InputManager::AddKey(Soul::Left);

		// These are our note keys
		Soul::InputManager::AddNoteKey(Soul::A);
		Soul::InputManager::AddNoteKey(Soul::S);
		Soul::InputManager::AddNoteKey(Soul::D);
		Soul::InputManager::AddNoteKey(Soul::F);
		Soul::InputManager::AddNoteKey(Soul::Space);

		LoadHighway();
		Soul::AudioEngine::Play("Songs/Lira/song.ogg");
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
		{
			Group note(m_Chart->GetNextNote());

			if (!m_Notes.empty() &&
				note.GetTimeStamp() - m_Notes.front().GetTimeStamp() <= m_HopoThreshold)
			{
				// Check if the note in front of it is the same note
				auto previousNote = m_Notes.begin(); // This should be the note that is right in front of it

				if (!(note == *previousNote))
					note.SetHopo(); // Set note to be HOPO
			}

			m_Notes.push_front(note);
		}

		while (m_Chart->HasNextEvent() && m_Chart->PeekNextEvent().TimeStamp <= m_BottomTimeStamp)
		{
			m_BPM = m_Chart->GetNextEvent().BPM / 1000.0f; // Change the bpm

			m_PreviousDistance += m_CurrentDistance;
			m_CurrentDistance = 0;
			m_TimeSinceStart = 0;

			Soul::Log::LogInfo("BPM changed to: " + std::to_string(m_BPM));
		}

		// Update Notes
		for (Group& group : m_Notes)
			group.Update(m_BottomTimeStamp, m_TopTimeStamp, m_HitLineHeight);

		// Remove all notes that have been marked
		m_Notes.erase(std::remove_if(m_Notes.begin(), m_Notes.end(),
			[&](const Group& group)
		{
			if (group.IsHopo() && group.IsMarked() && group.GetYPos() > 40)
			{
				NoteHit();
				return true;
			}
			return false;
		}), m_Notes.end());

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
			{
				if (group.IsHopo())
				{
					if (!group.IsMarked())
						for (const Note note : group.GetNotes())
							m_Draw[yPos * 160 + (int)(note.GetXPos())] = '@';
				}
				else
				{
					for (const Note note : group.GetNotes())
						m_Draw[yPos * 160 + (int)(note.GetXPos())] = '#';
				}
			}
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
		using namespace Soul;
		if (!m_Notes.empty())
		{
			for (auto it = m_Notes.end(); it != m_Notes.begin();)
			{
				--it;

				// If we're in distance 
				int noteDistance = (45 - m_HitLineHeight) - it->GetYPos();
				if (noteDistance <= 3 && noteDistance >= -2)
				{
					// Check for hit HOPO notes
					if (it->IsHopo() && InputManager::IsInputChangeDetected() && m_PreviousNoteHit
						&& CorrectKeys(*it))
					{
						// Make sure we can only tap this if the previous note was hit
						auto previousNote = it;
						previousNote++;
						if (previousNote == m_Notes.end() ||
							(previousNote != m_Notes.end() && previousNote->IsMarked()))
						{
							it->SetMarked();
							return;
						}
					}
					
					// Check for hit notes
					if ((InputManager::WasKeyPressed(Up) || InputManager::WasKeyPressed(Left)))
					{
						if (CorrectKeys(*it))
						{
							m_Notes.erase(it);
							NoteHit();

							return;
						}

						NoteMissed();
						return;
					}
				}
			}
		}

		if ((InputManager::WasKeyPressed(Up) || InputManager::WasKeyPressed(Left)))
		{
			if (m_Notes.empty())
			{
				NoteMissed();
			}
			else
			{
				int noteDistance = (45 - m_HitLineHeight) - (m_Notes.end() - 1)->GetYPos();
				if (noteDistance > 3)
					NoteMissed();
			}
		}
	}

	bool CorrectKeys(Group& group)
	{
		short keysDown = 0;
		for (const Note note : group.GetNotes())
			keysDown += note.GetAssociatedKey();

		short actualKeysDown = Soul::InputManager::GetKeysDown();

		// We don't want to count pressing Up or Left as note selection
		if (Soul::InputManager::IsKeyDown(Soul::Up))
			actualKeysDown -= Soul::Up;
		if (Soul::InputManager::IsKeyDown(Soul::Left))
			actualKeysDown -= Soul::Left;

		// Don't prevent a hit if this is a single note and the notes to the left of it are pressed
		if (group.GetNotes().size() == 1)
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
			return true;

		return false;
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

		m_PreviousNoteHit = true;
	}

	void NoteMissed()
	{
		m_ComboCount = 0;
		m_Multiplier = 1;

		m_PreviousNoteHit = false;
	}

private:
	char* m_Highway;
	int m_BottomTimeStamp, m_TopTimeStamp;
	int m_HitLineHeight;
	int m_Score;
	int m_HopoThreshold;
	short m_ComboCount, m_Multiplier;
	bool m_PreviousNoteHit;
	float m_HighwayCompression;
	float m_BPM;
	double m_TimeSinceStart;
	double m_CurrentDistance, m_PreviousDistance;
	std::deque<Group> m_Notes;
	Soul::ChartFile* m_Chart;
};