#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <cstdlib>

// Apple platformalarının bloklanması qorunur
#if defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE || TARGET_OS_MAC || TARGET_OS_IOS
        #error "XƏTA: Bu oyun Apple cihazları üçün dəstəklənmir!"
    #endif
#endif

// Dünya Ölçüləri
const int WORLD_WIDTH = 2000;   // X oxu üzrə sərhəd
const int WORLD_LENGTH = 3000;  // Z oxu üzrə sərhəd

// Blok növləri
enum class BlockType {
    AIR,
    GRASS,
    DIRT,
    STONE,
    WATER,
    WOOD,
    LEAVES
};

// Relyef və Xəritə Generasiya Sistemi
class TerrainGenerator {
public:
    // 1. Hündürlük Xəritəsi (Dağlar və Düzənliklər)
    int calculateHeight(int x, int z) {
        // Sadə riyazi dalğa düsturu ilə dağlar və təpələr yaradırıq
        float height = 64.0f + std::sin(x * 0.01f) * 15.0f + std::cos(z * 0.01f) * 15.0f;
        
        // Hündür dağ zonaları (məsələn, xəritənin müəyyən hissəsində)
        if (x > 500 && x < 1000) {
            height += std::sin(x * 0.05f) * 30.0f; // Daha dik dağlar
        }
        
        return static_f<int>(height);
    }

    // 2. Göllərin Yaranması
    bool isLake(int x, int z) {
        // Müəyyən koordinatlarda su çökəklikləri yoxlanılır
        float lakeCheck = std::sin(x * 0.02f) * std::cos(z * 0.02f);
        return (lakeCheck > 0.7f); // Su səth şərti
    }

    // 3. Mağaraların Yaranması (Yer altındakı boşluqlar)
    bool isCave(int x, int y, int z) {
        // Yeraltı tunellər və mağaralar üçün riyazi səs-küy simulyasiyası
        if (y < 45) { // Yalnız yerin altında olur
            float caveNoise = std::sin(x * 0.1f) + std::cos(y * 0.1f) + std::sin(z * 0.1f);
            return (caveNoise > 1.2f && caveNoise < 1.4f);
        }
        return false;
    }

    // 4. Ağacların və Kəndlərin Yerləşdirilməsi
    void generateSurfaceStructures(int x, int z, int groundY) {
        // Hər 50 blokdan bir kənd mərkəzi yoxlanılır
        if (x % 250 == 0 && z % 250 == 0) {
            std::cout << "[Kənd] Koordinatda kənd yaradıldı: (" << x << ", " << groundY << ", " << z << ")\n";
        }
        
        // Təsadüfi və ya riyazi olaraq ağacların səpələnməsi
        if ((x * z) % 17 == 0 && !isLake(x, z)) {
            std::cout << "[Ağac] Meşə/Ağac əkildi: (" << x << ", " << groundY + 1 << ", " << z << ")\n";
        }
    }

    // Dünyanın müəyyən bölgəsini analiz edən əsas funksiya
    void generateChunkSlice(int startX, int startZ) {
        std::cout << "\n--- Chunk Generasiya Olunur (Baslanğıc: X=" << startX << ", Z=" << startZ << ") ---\n";
        
        // RAM-a qənaət üçün yalnız sərhəd daxilindəki nöqtələr hesablanır
        for (int x = startX; x < startX + 16 && x < WORLD_WIDTH / 2; ++x) {
            for (int z = startZ; z < startZ + 16 && z < WORLD_LENGTH / 2; ++z) {
                int groundY = calculateHeight(x, z);
                
                if (isLake(x, z)) {
                    // Su hövzəsi
                    // std::cout << "Su bloku: " << x << ", " << groundY << ", " << z << "\n";
                } else {
                    // Normal torpaq və ağaclar/kəndlər
                    generateSurfaceStructures(x, z, groundY);
                }
            }
        }
        std::cout << "Chunk uğurla generasiya olundu və RAM-da optimizasiya edildi.\n";
    }
};

int main() {
    std::cout << "HGCraft 2000x3000 Xəritə Sistemİ Başladılır...\n";
    
    auto terrainGen = std::make_unique<TerrainGenerator>();

    // Oyunçunun ətrafındakى ilk bölgəni (Chunk) yaradırıq
    terrainGen->generateChunkSlice(0, 0);

    return 0;
}