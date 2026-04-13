#pragma once
#include "../libs/boostpp/vector.hpp"
#include "pixel.hpp"

namespace board::animate {

      using sprite_n = std::size_t; /* Number of diodes in sprite */

      /* Sprites Note. They do not natively have collision detection that only gets handeled explicitly. (Init with pixel count) */
      template <sprite_n n>
      using sprite = boost::fixed_vector<display::diode_data, n, sprite_n>;
} // namespace board::animate