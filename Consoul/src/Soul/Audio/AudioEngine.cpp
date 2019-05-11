#include "pch.h"
#include "AudioEngine.h"
#include "Log.h"

namespace Soul {
	irrklang::ISoundEngine* AudioEngine::m_Engine = irrklang::createIrrKlangDevice();
	irrklang::ISound* AudioEngine::m_Playing = nullptr;

	void AudioEngine::Play(const char* filePath)
	{
		m_Playing = m_Engine->play2D(filePath, false, false, true);
	}

	void AudioEngine::SetSpeed(int speed)
	{
		if (m_Playing != nullptr)
			m_Playing->setPlaybackSpeed(1.0f / (float)speed);
	}

	void AudioEngine::Stop()
	{
		if (m_Playing != nullptr)
			m_Playing->stop();
	}

	void AudioEngine::Pause()
	{
		if (m_Playing != nullptr)
			m_Playing->setIsPaused(true);
	}

	void AudioEngine::Resume()
	{
		if (m_Playing != nullptr)
			m_Playing->setIsPaused(false);
	}

	void AudioEngine::Update()
	{
		m_Engine->update();
	}

	void AudioEngine::CleanUp()
	{
		m_Engine->drop();
	}
}