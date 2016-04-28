// JAVA IMPLEMENTATION OF THE IMPROVED PERLIN FUNCTION (see http://mrl.nyu.edu/~perlin/noise/)
// THE ORIGINAL JAVA IMPLEMENTATION IS COPYRIGHT 2002 KEN PERLIN

// Header guards.
#ifndef _PERLINNOISE_H_
#define _PERLINNOISE_H_

// Include files here.
#include <vector>
#include <iostream>
#include <cmath>
#include <random>
#include <numeric>
#include <algorithm>


class PerlinNoise
{

	public:
		// Methods.
		PerlinNoise();
		PerlinNoise(unsigned int seed);
		~PerlinNoise();
		double Noise(double x, double y, double z);

	private:
		// Attributes.
		std::vector<int> permutations;	// The permutations vector.

		// Methods.
		double Fade(double cuppa);
		double Lerp(double cuppa, double a, double b);
		double Gradient(int hash, double x, double y, double z);

};

#endif