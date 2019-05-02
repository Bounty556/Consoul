#pragma once

#include "SongFile.h"

namespace Soul {
	class ChartFile : public SongFile
	{
	public:
		ChartFile(const std::string& filePath);

	protected:
		void LoadFile(const std::string& filePath) override;

	private:
		void LoadMetaData(const std::string& line);
		void LoadNoteData(const std::string& line);
	};
}