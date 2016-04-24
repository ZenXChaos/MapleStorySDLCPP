
#include <random>

class GameUtils {
public:
	static int RandomInteger() {
		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(0, 100000); // guaranteed unbiased


		return uni(rng);
	}

	static int RandomIntegerRange(int min, int max) {
		std::random_device rd;     // only used once to initialise (seed) engine
		std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(min, max); // guaranteed unbiased


		return uni(rng);
	}
};