#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

// 1. Ölçülər (Dimensions)
enum class Dimension {
    OVERWORLD,
    NETHER,
    END
};

// 2. Xəritə Sərhədləri (2000x3000) və Optimizasiya
class WorldBorder {
private:
    const int MAX_X = 2000;
    const int MAX_Z = 3000;

public:
    bool isWithinBounds(int x, int z) const {
        int halfX = MAX_X / 2;
        int halfZ = MAX_Z / 2;
        // Xəritənin mərkəzi (0,0) olmaqla sərhədlərin yoxlanılması
        return (x >= -halfX && x <= halfX && z >= -halfZ && z <= halfZ);
    }
};

// 3. Portal Sistemi
class PortalManager {
public:
    Dimension teleport(Dimension currentDimension) {
        switch (currentDimension) {
            case Dimension::OVERWORLD:
                std::cout << "[Portal] Nether olcusune kecilir...\n";
                return Dimension::NETHER;
            case Dimension::NETHER:
                std::cout << "[Portal] Esas dunyaya qayidilir...\n";
                return Dimension::OVERWORLD;
            case Dimension::END:
                std::cout << "[Portal] The End-den cixis edilir...\n";
                return Dimension::OVERWORLD;
        }
        return currentDimension;
    }
};

// 4. Əşya Çevirilməsi (Crafting System)
class CraftingSystem {
private:
    // Reseptlərin yaddaşda saxlanılması (hash map vasitəsilə sürətli axtarış)
    std::unordered_map<std::string, std::string> recipes;

public:
    void registerRecipe(const std::string& ingredients, const std::string& result) {
        recipes[ingredients] = result;
    }

    std::string craft(const std::string& ingredients) {
        auto it = recipes.find(ingredients);
        if (it != recipes.end()) {
            return it->second;
        }
        return "Heç nə alınmadı (Air)";
    }
};

int main() {
    // RAM-a qənaət üçün smart pointerlərdən istifadə olunur
    auto border = std::make_unique<WorldBorder>();
    auto portalManager = std::make_unique<PortalManager>();
    auto craftingSystem = std::make_unique<CraftingSystem>();

    // Crafting resepti əlavə edək
    craftingSystem->registerRecipe("4*Wood", "Crafting Table");

    // Xəritə sərhədi testi
    int playerX = 500;
    int playerZ = -1200;

    if (border->isWithinBounds(playerX, playerZ)) {
        std::cout << "Oyunçu sərhədlər daxilindədir (" << playerX << ", " << playerZ << ")\n";
    } else {
        std::cout << "Sərhəddən kənardasınız!\n";
    }

    // Portal keçidi testi
    Dimension currentDim = Dimension::OVERWORLD;
    currentDim = portalManager->teleport(currentDim); // Nether-ə keçid

    // Əşya düzəltmə testi
    std::string result = craftingSystem->craft("4*Wood");
    std::cout << "Əldə olunan əşya: " << result << "\n";

    return 0;
}