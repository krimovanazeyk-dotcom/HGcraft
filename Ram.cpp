#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <cmath>

// Apple platformalarının bloklanması qorunur
#if defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE || TARGET_OS_MAC || TARGET_OS_IOS
        #error "XƏTA: Bu oyun Apple cihazları üçün dəstəklənmir!"
    #endif
#endif

// Dünya və Chunk Sabitləri
const int WORLD_WIDTH = 2000;   // X oxu sərhədi
const int WORLD_LENGTH = 3000;  // Z oxu sərhədi
const int CHUNK_SIZE = 16;      // Hər chunk 16x16 blok ölçüsündədir

// Chunk Koordinat Struktur (Hash map üçün açar rolunu oynayır)
struct ChunkCoord {
    int x, z;

    // İki koordinatın bərabərliyini yoxlamaq üçün
    bool operator==(const ChunkCoord& other) const {
        return x == other.x && z == other.z;
    }
};

// Hash Funksiyası (unordered_map üçün sürətli axtarış təmin edir)
struct HashChunkCoord {
    size_t operator()(const ChunkCoord& c) const {
        return std::hash<int>()(c.x) ^ (std::hash<int>()(c.z) << 1);
    }
};

// Tək bir Chunk-ı təmsil edən sinif
class Chunk {
private:
    int cx, cz;
public:
    Chunk(int x, int z) : cx(x), cz(z) {
        // Real oyunda burada həmin chunk-ın blok məlumatları yaradılır
    }

    ~Chunk() {
        // Chunk RAM-dan silinərkən çağırılır
    }

    int getX() const { return cx; }
    int getZ() const { return cz; }
};

// Chunk Meneceri (Yükləmə və Silmə işlərini idarə edir)
class ChunkManager {
private:
    // Aktiv olaraq RAM-da saxlanılan chunk-lar (Açar: Chunk koordinatı, Dəyər: Chunk obyekti)
    std::unordered_map<ChunkCoord, std::unique_ptr<Chunk>, HashChunkCoord> loadedChunks;
    
    int renderDistance = 4; // Oyunçunun ətrafında aktiv saxlanılacaq chunk radiusu

public:
    // Oyunçu hərəkət etdikcə çağırılan əsas funksiya
    void updatePlayerPosition(float playerX, float playerZ) {
        // Oyunçunun dünya koordinatlarını Chunk koordinatlarına çeviririk
        int playerChunkX = static_cast<int>(playerX) / CHUNK_SIZE;
        int playerChunkZ = static_cast<int>(playerZ) / CHUNK_SIZE;

        // 1. Oyunçunun ətrafındakı chunk-ları yoxlayıb RAM-a yükləyirik
        for (int x = playerChunkX - renderDistance; x <= playerChunkX + renderDistance; ++x) {
            for (int z = playerChunkZ - renderDistance; z <= playerChunkZ + renderDistance; ++z) {
                
                // 2000x3000 xəritə sərhədləri daxilində olub olmadığını yoxlayırıq
                if (x >= -(WORLD_WIDTH / 2) / CHUNK_SIZE && x <= (WORLD_WIDTH / 2) / CHUNK_SIZE &&
                    z >= -(WORLD_LENGTH / 2) / CHUNK_SIZE && z <= (WORLD_LENGTH / 2) / CHUNK_SIZE) {
                    
                    ChunkCoord coord{x, z};
                    // Əgər həmin chunk hələ RAM-da yoxdursa, əlavə edirik
                    if (loadedChunks.find(coord) == loadedChunks.end()) {
                        loadedChunks[coord] = std::make_unique<Chunk>(x, z);
                        std::cout << "[Yükləndi] Chunk (" << x << ", " << z << ") RAM-a əlavə olundu.\n";
                    }
                }
            }
        }

        // 2. Uzaqlaşan (render məsafəsindən çıxan) chunk-ları RAM-dan silirik (Unload)
        for (auto it = loadedChunks.begin(); it != loadedChunks.end(); ) {
            int cx = it->first.x;
            int cz = it->first.z;

            // Əgər oyunçudan həddindən artıq uzaqlaşıbsa
            if (std::abs(cx - playerChunkX) > renderDistance || std::abs(cz - playerChunkZ) > renderDistance) {
                std::cout << "[Unload] Chunk (" << cx << ", " << cz << ") uzaqlaşdığı üçün RAM-dan təmizləndi.\n";
                it = loadedChunks.erase(it); // Smart pointer avtomatik olaraq yaddaşı azad edir
            } else {
                ++it;
            }
        }
    }

    size_t getLoadedChunkCount() const {
        return loadedChunks.size();
    }
};

int main() {
    std::cout << "HGCraft Chunk İdarəetmə Sistemi Başladıldı...\n";
    ChunkManager chunkManager;

    // Oyunçu mərkəzdədir (0, 0)
    std::cout << "\n--- Oyunçu (0, 0) nöqtəsindədir ---\n";
    chunkManager.updatePlayerPosition(0.0f, 0.0f);
    std::cout << "Aktiv RAM-dakı Chunk sayı: " << chunkManager.getLoadedChunkCount() << "\n";

    // Oyunçu uzaqdakı başqa bir koordinata hərəkət edir (məsələn, X=400, Z=500)
    std::cout << "\n--- Oyunçu (400, 500) nöqtəsinə qaçır ---\n";
    chunkManager.updatePlayerPosition(400.0f, 500.0f);
    std::cout << "Aktiv RAM-dakı Chunk sayı: " << chunkManager.getLoadedChunkCount() << "\n";

    return 0;
}