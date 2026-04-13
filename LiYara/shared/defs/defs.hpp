#pragma once
#include <cstdint>
#include <utility>

using x_axis = std::uint8_t;               /* Y-AXis */
using y_axis = std::uint8_t;               /* X-Axis */
using x_range = std::pair<x_axis, x_axis>; /* [START, END] */
using y_range = std::pair<x_axis, x_axis>; /* [START, END] */