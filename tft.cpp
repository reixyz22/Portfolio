#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <unordered_map>

const int MAX_CHAMPIONS = 64;
const int MAX_TRAITS = 27;

class Champion {
public:
    std::string name;
    int cost;
    std::bitset<MAX_TRAITS> traits;

    Champion(const std::string& n, int c, std::bitset<MAX_TRAITS> t)
        : name(n), cost(c), traits(t) {}
};

class Trait {
public:
    std::string name;
    std::vector<int> tiers;
    std::vector<double> values;

    Trait(const std::string& n, std::vector<int> t, std::vector<double> v)
        : name(n), tiers(std::move(t)), values(std::move(v)) {}
};

class Game {
    std::vector<Champion> champions;
    std::vector<Trait> traits;

    double calculateScore(const std::bitset<MAX_CHAMPIONS>& combination) {
        double score = 0.0;

        for (const auto& trait : traits) {
            int count = 0;
            for (size_t i = 0; i < champions.size(); ++i) {
                if (combination.test(i) && champions[i].traits.test(&trait - &traits[0])) {
                    ++count;
                }
            }

            for (size_t j = 0; j < trait.tiers.size(); ++j) {
                if (count >= trait.tiers[j]) {
                    score += trait.values[j];
                    break;
                }
            }
        }

        return score;
    }

    bool isValid(const std::bitset<MAX_CHAMPIONS>& combination) {
        int totalCost = 0;
        for (size_t i = 0; i < champions.size(); ++i) {
            if (combination.test(i)) {
                totalCost += champions[i].cost;
            }
        }

        // Example condition: total cost must not exceed 10
        return totalCost <= 10;
    }

    void findBestCombinationHelper(std::bitset<MAX_CHAMPIONS>& current, int index, double& maxScore, std::bitset<MAX_CHAMPIONS>& bestCombination) {
        if (index == champions.size()) {
            double currentScore = calculateScore(current);
            if (isValid(current) && currentScore > maxScore) {
                maxScore = currentScore;
                bestCombination = current;
            }
            return;
        }

        // Exclude current champion
        findBestCombinationHelper(current, index + 1, maxScore, bestCombination);

        // Include current champion
        current.set(index);
        findBestCombinationHelper(current, index + 1, maxScore, bestCombination);
        current.reset(index); // Backtrack
    }

public:
    Game(const std::vector<Champion>& champs, const std::vector<Trait>& tr)
        : champions(champs), traits(tr) {}

    std::bitset<MAX_CHAMPIONS> findOptimalCombination() {
        double maxScore = 0.0;
        std::bitset<MAX_CHAMPIONS> current, bestCombination;
        findBestCombinationHelper(current, 0, maxScore, bestCombination);
        std::cout << "Best Score: " << maxScore << std::endl;
        return bestCombination;
    }
};
int main() {
    std::vector<Champion> champs = {
        Champion("Alistar", 3, std::bitset<MAX_TRAITS>("000000100000010000010000000")),
        Champion("Annie", 2, std::bitset<MAX_TRAITS>("000000100000000000010000010")),
        Champion("Aphelios", 5, std::bitset<MAX_TRAITS>("000000100000000000100000100")),
        Champion("Ashe", 1, std::bitset<MAX_TRAITS>("000000000010000000000001000")),
        Champion("Aurelion Sol", 4, std::bitset<MAX_TRAITS>("000000000000000000000100000")),
        Champion("BelVeth", 4, std::bitset<MAX_TRAITS>("000000000000000000000100000")),
        Champion("Blitzcrank", 1, std::bitset<MAX_TRAITS>("100000000000000000000000001")),
        Champion("Camille", 2, std::bitset<MAX_TRAITS>("100000000000000100000000000")),
        Champion("Cho'Gath", 3, std::bitset<MAX_TRAITS>("000000000000000000000100000")),
        Champion("Draven", 2, std::bitset<MAX_TRAITS>("000000000000000100000100000")),
        Champion("Ekko", 4, std::bitset<MAX_TRAITS>("000001000000010000000100000")),
        Champion("Ezreal", 2, std::bitset<MAX_TRAITS>("000000000100000000000001000")),
        Champion("Fiddlesticks", 5, std::bitset<MAX_TRAITS>("000000000000000000000100010")),
        Champion("Fiora", 2, std::bitset<MAX_TRAITS>("000000100000000000010000000")),
        Champion("Galio", 1, std::bitset<MAX_TRAITS>("000000000000000000010000100")),
        Champion("Gangplank", 1, std::bitset<MAX_TRAITS>("000000000000100000010000000")),
        Champion("Janna", 5, std::bitset<MAX_TRAITS>("000000000000000000100000101")),
        Champion("Jax", 3, std::bitset<MAX_TRAITS>("000000000000000100000100000")),
        Champion("Jinx", 2, std::bitset<MAX_TRAITS>("010000000000000000000000010")),
        Champion("Kai'Sa", 3, std::bitset<MAX_TRAITS>("000000000010000000000001000")),
        Champion("Kayle", 1, std::bitset<MAX_TRAITS>("000000000100000000010000000")),
        Champion("LeBlanc", 3, std::bitset<MAX_TRAITS>("100000000000010000000100000")),
        Champion("Lee Sin", 2, std::bitset<MAX_TRAITS>("000000000000100000010010000")),
        Champion("Leona", 5, std::bitset<MAX_TRAITS>("000000000000010000010000100")),
        Champion("Lulu", 1, std::bitset<MAX_TRAITS>("000000000100000000010000000")),
        Champion("Lux", 1, std::bitset<MAX_TRAITS>("000000000010000000000100000")),
        Champion("Malphite", 2, std::bitset<MAX_TRAITS>("000000000000100000010000000")),
        Champion("Miss Fortune", 4, std::bitset<MAX_TRAITS>("010000000000000000000100000")),
        Champion("Mordekaiser", 5, std::bitset<MAX_TRAITS>("000000000010000000000100000")),
        Champion("Nasus", 1, std::bitset<MAX_TRAITS>("010000000000100000010000000")),
        Champion("Nilah", 3, std::bitset<MAX_TRAITS>("000000000010000000010000000")),
        Champion("Nunu", 5, std::bitset<MAX_TRAITS>("000000000100000000010000000")),
        Champion("Poppy", 1, std::bitset<MAX_TRAITS>("000000000100000000000010000")),
        Champion("Rammus", 3, std::bitset<MAX_TRAITS>("000000000000000000000100000")),
        Champion("Rell", 2, std::bitset<MAX_TRAITS>("000000000010000000000010000")),
        Champion("Renekton", 1, std::bitset<MAX_TRAITS>("000000000010000000000100000")),
        Champion("Riven", 3, std::bitset<MAX_TRAITS>("010000000000100000000010000")),
        Champion("Samira", 4, std::bitset<MAX_TRAITS>("000000000100000000100000100")),
        Champion("Sejuani", 4, std::bitset<MAX_TRAITS>("000000000010000000000100000")),
        Champion("Senna", 3, std::bitset<MAX_TRAITS>("000000000010000000000100000")),
        Champion("Sett", 4, std::bitset<MAX_TRAITS>("000000000000000100000010000")),
        Champion("Sivir", 2, std::bitset<MAX_TRAITS>("000000000000000000100000100")),
        Champion("Sona", 3, std::bitset<MAX_TRAITS>("000000000100000000010100000")),
        Champion("Soraka", 4, std::bitset<MAX_TRAITS>("100000000000000000010000000")),
        Champion("Sylas", 1, std::bitset<MAX_TRAITS>("010000000000000100000000000")),
        Champion("Syndra", 5, std::bitset<MAX_TRAITS>("000000000010000000010100000")),
        Champion("Taliyah", 4, std::bitset<MAX_TRAITS>("000000000010000000000100000")),
        Champion("Talon", 1, std::bitset<MAX_TRAITS>("000000100000000100000000000")),
        Champion("Urgot", 5, std::bitset<MAX_TRAITS>("000000000000000000000100000")),
        Champion("Vayne", 3, std::bitset<MAX_TRAITS>("010000000000000000010001000")),
        Champion("Velkoz", 3, std::bitset<MAX_TRAITS>("000000000000000000000100000")),
        Champion("Vi", 2, std::bitset<MAX_TRAITS>("000000000100000100000100000")),
        Champion("Viego", 4, std::bitset<MAX_TRAITS>("000000100000000100000000000")),
        Champion("Wukong", 1, std::bitset<MAX_TRAITS>("000000000000000100000010000")),
        Champion("Yasuo", 2, std::bitset<MAX_TRAITS>("000000000000000100000010000")),
        Champion("Yuumi", 2, std::bitset<MAX_TRAITS>("000000000010000000010010000")),
        Champion("Zac", 4, std::bitset<MAX_TRAITS>("000000000000000000000100000")),
        Champion("Zed", 4, std::bitset<MAX_TRAITS>("000000000010000100000010100")),
        Champion("Zoe", 3, std::bitset<MAX_TRAITS>("000000000010100000100000000")),
    };

    std::vector<Trait> gameTraits = {
        Trait("ADMIN", {2, 4, 6, 99}, {2.0, 4.0, 5.8}),
        Trait("AnimaSquad",{3,5,7,99},{2.0,3.0,4.0}),
        Trait("Arsenal",{1,99,99,99},{0.0}),
        Trait("Civilian",{1,2,3,99},{0.4,1.5,6.0}),
        Trait("Gadgeteen",{3,5,99,99},{3.5,4.0}),
        Trait("LaserCorps",{3,6,9,99},{8.0,12.0,7.0}),
        Trait("MechaPRIME",{3,5,99,99},{2.2,5.0}),
        Trait("OxForce",{2,4,6,8},{1.5,2.0,2.7,3.8}),
        Trait("StarGuardian",{3,5,7,9},{6.5,7.0,9.0,8.5}),
        Trait("Supers",{3,99,99,99},{2.0,2.0,1,0}),
        Trait("Threat",{1,99,99,99},{0.0}),
        Trait("Underground",{3,5,99,99},{6.5,8.0}),
        Trait("Ace",{1,4,99,99},{1.0,6.0}),
        Trait("Aegis",{2,3,4,5},{2.1,3.5,4.0,6.0}),
        Trait("Brawler",{2,4,6,8},{2.1,4.0,5.5,7.0}),
        Trait("Corrupted",{1,99,99,99},{0.0}),
        Trait("Defender",{2,4,6,99},{2.1,6.0,10.0}),
        Trait("Duelist",{2,4,6,8},{1.5,6.5,6.0,7.0}),
        Trait("Forecaster",{1,99,99,99},{0.0}),
        Trait("Hacker",{2,3,4,99},{2.5,2.9,4.0}),
        Trait("Heart",{2,4,6,99},{1.5,5.5,8.0}),
        Trait("Mascot",{2,4,6,8},{2.1,4.7,10.0,99.0}),
        Trait("Prankster",{2,3,99,99},{2.5,4.0}),
        Trait("Recon",{2,3,4,99},{2.0,2.5,3.0,8.0}),
        Trait("Renegade",{3,6,99,99},{6.5,8.0}),
        Trait("Spellslinger",{2,4,6,8},{1.75,5.5,8.0,10.0}),
        Trait("Sureshot",{2,4,99,99},{2.5,8.0}),
    };

    if (champs.empty() || gameTraits.empty()) {
        std::cout << "Champions or Traits are not initialized correctly." << std::endl;
        return 1;
    }

    std::cout << "Starting the game simulation..." << std::endl;
    Game game(champs, gameTraits);

    std::cout << "Finding the best combination..." << std::endl;
    std::bitset<MAX_CHAMPIONS> bestCombination = game.findOptimalCombination();

    if (bestCombination.none()) {
        std::cout << "No valid combination found." << std::endl;
    } else {
        // Output the best combination
        std::cout << "Best Combination: ";
        for (size_t i = 0; i < MAX_CHAMPIONS; ++i) {
            if (bestCombination.test(i)) {
                std::cout << champs[i].name << ", ";
            }
        }
        std::cout << std::endl;
    }
    return 0;
}
