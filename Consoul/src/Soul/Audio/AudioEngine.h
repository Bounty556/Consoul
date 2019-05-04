#pragma once

#include "irrKlang.h"

namespace Soul {
	class AudioEngine
	{
	public:
		AudioEngine() = delete;

		static void Play(const char* filePath);
		static void Stop();
		static void Pause();
		static void Resume();

		static void CleanUp();
	private:
		static irrklang::ISoundEngine* m_Engine;
		static irrklang::ISound* m_Playing;
	};
}