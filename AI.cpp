#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cmath>

// Apple platformalarının bloklanması (İstəyinizə görə qorunur)
#if defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE || TARGET_OS_MAC || TARGET_OS_IOS
        #error "XƏTA: Bu oyun Apple cihazları üçün dəstəklənmir!"
    #endif
#endif

// Varlıqların (Entities) növləri
enum class EntityType {
    HUMAN_NPC,   // Süni intellektli oyunçular/insanlar
    VILLAGER,    // Kəndlilər
    ANIMAL,      // İnək, qoyun, toyuq, at, ördək, balıq
    ZOMBIE,      // Zombi
    SKELETON,    // Skelet
    CREEPER,     // Kreeper
    ENDERMAN     // Enderman
};

// AI Davranış Vəziyyətləri
enum class AIState {
    IDLE,        // Sakit dayanma (Uzaqdadırsa RAM-a qənaət üçün bu rejimdə olur)
    WANDER,      // Ətrafa dolaşma
    ATTACK,      // Hücum etmə / Təqib etmə
    EXPLODE,     // Kriper üçün partlayış
    TELEPORT     // Enderman üçün teleportasiya
};

class Entity {
protected:
    std::string name;
    EntityType type;
    float x, y, z; // Xəritədəki koordinatları
    float health;
    AIState currentState;

public:
    Entity(std::string name, EntityType type, float x, float y, float z, float health)
        : name(name), type(type), x(x), y(y), z(z), health(health), currentState(AIState::IDLE) {}

    virtual ~Entity() = default;

    // RAM və CPU-ya qənaət edən AI yeniləmə funksiyası
    virtual void updateAI(float playerX, float playerZ) {
        // Oyunçu ilə varlıq arasındakı məsafənin hesablanması
        float distance = std::sqrt(std::pow(playerX - x, 2) + std::pow(playerZ - z, 2));
        
        // Əgər oyunçudan 32 blokdan uzaqdadırsa, AI işləmir (CPU-nu yormur)
        if (distance > 32.0f) {
            currentState = AIState::IDLE;
            return;
        }

        // Varlığın növünə görə süni intellekt davranışı
        switch (type) {
            case EntityType::ZOMBIE:
            case EntityType::SKELETON:
                if (distance < 16.0f) {
                    currentState = AIState::ATTACK;
                    std::cout << "[" << name << "] Oyuncunu görüb təqib edir!\n";
                } else {
                    currentState = AIState::WANDER;
                }
                break;

            case EntityType::CREEPER:
                if (distance < 3.0f) {
                    currentState = AIState::EXPLODE;
                    std::cout << "[" << name << "] SSSSSS... Partlayış baş verir!\n";
                } else if (distance < 10.0f) {
                    currentState = AIState::ATTACK;
                    std::cout << "[" << name << "] Oyuncuya tərəf səssizcə yaxınlaşır...\n";
                }
                break;

            case EntityType::ENDERMAN:
                if (distance < 8.0f) {
                    currentState = AIState::TELEPORT;
                    std::cout << "[" << name << "] Gözlərinə baxıldı! Anidən teleport oldu.\n";
                }
                break;

            case EntityType::VILLAGER:
            case EntityType::HUMAN_NPC:
                currentState = AIState::WANDER;
                std::cout << "[" << name << "] Kənddə gəzir və ya ticarət edir.\n";
                break;

            case EntityType::ANIMAL:
                currentState = AIState::WANDER;
                // Heyvanlar (inək, qoyun, toyuq, at, ördək, balıq) sakitcə otlayır/üzür
                break;
        }
    }

    std::string getName() const { return name; }
};

int main() {
    // Sürətli və az RAM yeyən yaddaş idarəetməsi üçün vector və smart pointerlər
    std::vector<std::unique_ptr<Entity>> worldEntities;

    // Oyuna müxtəlif varlıqlar əlavə edək
    worldEntities.push_back(std::make_unique<Entity>("AI Oyunçu (Murad)", EntityType::HUMAN_NPC, 10.0f, 0.0f, 10.0f, 20.0f));
    worldEntities.push_back(std::make_unique<Entity>("Kəndli", EntityType::VILLAGER, 12.0f, 0.0f, 14.0f, 20.0f));
    worldEntities.push_back(std::make_unique<Entity>("İnək", EntityType::ANIMAL, 5.0f, 0.0f, 5.0f, 10.0f));
    worldEntities.push_back(std::make_unique<Entity>("Zombi", EntityType::ZOMBIE, 15.0f, 0.0f, 12.0f, 20.0f));
    worldEntities.push_back(std::make_unique<Entity>("Kreeper", EntityType::CREEPER, 13.0f, 0.0f, 13.0f, 20.0f));
    worldEntities.push_back(std::make_unique<Entity>("Enderman", EntityType::ENDERMAN, 25.0f, 0.0f, 25.0f, 40.0f));

    // Oyunçunun cari koordinatları
    float playerX = 12.0f;
    float playerZ = 12.0f;

    std::cout << "--- HGCraft AI Sistemi İşə Düşdü ---\n";
    for (const auto& entity : worldEntities) {
        entity->updateAI(playerX, playerZ);
    }

    return 0;
}