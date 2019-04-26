#pragma once

#include "Layer.h"

namespace Soul {
	class ConsoleLayer : public Layer
	{
	public:
		ConsoleLayer(int width, int height);

		void Update(float deltaTime) override;
	};
}