#include "pch.h"
#include "AudioEngine.h"

namespace Soul {
	irrklang::ISoundEngine* AudioEngine::m_Engine = irrklang::createIrrKlangDevice();
	irrklang::ISound* AudioEngine::m_Playing = nullptr;

	void AudioEngine::Play(const char* filePath)
	{
		m_Playing = m_Engine->play2D(filePath, false);
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

	void AudioEngine::CleanUp()
	{
		m_Engine->drop();
	}
}