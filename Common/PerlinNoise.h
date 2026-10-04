/*
================================================================================
    This file is part of the ICST AmbiPlugins.

    ICST AmbiPlugins are free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ICST AmbiPlugins are distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the ICSTAmbiPlugins.  If not, see <http://www.gnu.org/licenses/>.
================================================================================
*/

#pragma once
#include <cstdint>
#include <cmath>

// Minimal deterministic 1D gradient (Perlin-style) noise. Free functions only, no persistent
// object/state by design - noise1D(seed, t) is a pure function, so callers can recompute it fresh
// every call instead of accumulating it over time. That's what makes it trivially safe to use for
// animation that must behave correctly across seeks/loops: the value at a given t never depends on
// how many times (or in what order) it was previously evaluated.
namespace PerlinNoise
{
    namespace detail
    {
        // Pure-arithmetic hash of (seed, lattice point) - no lookup tables, no static state.
        // A standard integer avalanche mix (variant of MurmurHash3's finalizer).
        inline uint32_t hash(uint32_t seed, int32_t i)
        {
            uint32_t h = seed ^ static_cast<uint32_t>(i);
            h ^= h >> 16;
            h *= 0x85ebca6bu;
            h ^= h >> 13;
            h *= 0xc2b2ae35u;
            h ^= h >> 16;
            return h;
        }

        // Maps a lattice point's hash to a pseudo-random gradient (slope) in [-1, 1].
        inline double gradient(uint32_t seed, int32_t i)
        {
            uint32_t h = hash(seed, i);
            return (static_cast<double>(h) / static_cast<double>(0xFFFFFFFFu)) * 2.0 - 1.0;
        }

        // Perlin's quintic fade curve: smoother than linear/cubic interpolation, avoids visible
        // discontinuities in the derivative at lattice points.
        inline double fade(double f)
        {
            return f * f * f * (f * (f * 6.0 - 15.0) + 10.0);
        }
    }

    // Deterministic 1D gradient noise. Pure function of (seed, t). Returns a smooth value in
    // approximately [-1, 1].
    inline double noise1D(uint32_t seed, double t)
    {
        const auto i0 = static_cast<int32_t>(std::floor(t));
        const auto i1 = i0 + 1;
        const double frac = t - static_cast<double>(i0);

        const double g0 = detail::gradient(seed, i0);
        const double g1 = detail::gradient(seed, i1);

        // 1D gradient contribution: slope at each lattice point times the (signed) distance to it.
        const double d0 = g0 * frac;
        const double d1 = g1 * (frac - 1.0);

        const double u = detail::fade(frac);
        return d0 + u * (d1 - d0);
    }
}
