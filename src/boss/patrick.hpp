#pragma once

#include "boss.hpp"

namespace los {
	class Patrick : public Boss {
		public:
			Patrick(SDL_Renderer*);
			~Patrick();
		public:
			void update(const double, const float, const float) override;
	};
};
