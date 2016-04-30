#ifndef GAMEUTILS_H
#define GAMEUTILS_H
#include <random>

class GameUtils {
public:
	static int RandomInteger() {
		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(0, 100000); // guaranteed unbiased


		return uni(rng);
	}

	static std::string UniqID() {
		std::string del = "abcdefghijklmnopqrstuvwxyz";

		std::string fString = "";
		for (size_t i = 0; i < del.size(); i++) {
			std::random_device rd;     // only used once to initialise (seed) engine
			std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
			std::uniform_int_distribution<int> uni(0, del.size()-1); // guaranteed unbiased
			fString += del[uni(rng)];
		}

		return fString;
	}

	static int RandomIntegerRange(int min, int max) {
		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(min, max); // guaranteed unbiased


		return uni(rng);
	}
};
#endif
