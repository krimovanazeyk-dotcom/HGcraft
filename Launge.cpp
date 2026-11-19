#include <iostream>
#include <string>
#include <unordered_map>

// Dəstəklənən dillərin siyahısı
enum class Language {
    ENGLISH,
    RUSSIAN,
    GERMAN,
    SPANISH,
    FRENCH,
    TURKISH,
    ARABIC,
    AZERBAIJANI
};

class LocalizationManager {
private:
    Language currentLang;
    // [Dil][Açar söz] = Tərcümə
    std::unordered_map<int, std::unordered_map<std::string, std::string>> translations;

public:
    LocalizationManager(Language lang) : currentLang(lang) {
        loadTranslations();
    }

    void setLanguage(Language lang) {
        currentLang = lang;
    }

    void loadTranslations() {
        // --- ENGLISH ---
        translations[(int)Language::ENGLISH] = {
            {"out_of_bounds", "You have reached the 2000x3000 world border!"},
            {"portal_nether", "Entering the Nether..."},
            {"portal_overworld", "Returning to the Overworld..."},
            {"craft_failed", "Nothing happened (Air)"},
            {"play", "Play"},
            {"settings", "Settings"}
        };

        // --- RUSSIAN (Русский) ---
        translations[(int)Language::RUSSIAN] = {
            {"out_of_bounds", "Вы достигли границы мира 2000x3000!"},
            {"portal_nether", "Вход в Нижний мир..."},
            {"portal_overworld", "Возвращение в обычный мир..."},
            {"craft_failed", "Ничего не получилось (Воздух)"},
            {"play", "Играть"},
            {"settings", "Настройки"}
        };

        // --- GERMAN (Deutsch) ---
        translations[(int)Language::GERMAN] = {
            {"out_of_bounds", "Du hast die 2000x3000 Weltgrenze erreicht!"},
            {"portal_nether", "Betrete den Nether..."},
            {"portal_overworld", "Kehre in die Oberwelt zurueck..."},
            {"craft_failed", "Nichts passiert (Luft)"},
            {"play", "Spielen"},
            {"settings", "Einstellungen"}
        };

        // --- SPANISH (Español) -->
        translations[(int)Language::SPANISH] = {
            {"out_of_bounds", "¡Has llegado al límite del mundo 2000x3000!"},
            {"portal_nether", "Entrando al Nether..."},
            {"portal_overworld", "Regresando al mundo principal..."},
            {"craft_failed", "No pasó nada (Aire)"},
            {"play", "Jugar"},
            {"settings", "Ajustes"}
        };

        // --- FRENCH (Français) ---
        translations[(int)Language::FRENCH] = {
            {"out_of_bounds", "Vous avez atteint la limite du monde 2000x3000 !"},
            {"portal_nether", "Entrée dans le Nether..."},
            {"portal_overworld", "Retour dans le monde normal..."},
            {"craft_failed", "Rien ne s'est passé (Air)"},
            {"play", "Jouer"},
            {"settings", "Paramètres"}
        };

        // --- TURKISH (Türkçe) ---
        translations[(int)Language::TURKISH] = {
            {"out_of_bounds", "2000x3000 dünya sınırına ulaştın!"},
            {"portal_nether", "Nether'a giriliyor..."},
            {"portal_overworld", "Ana dünyaya dönülüyor..."},
            {"craft_failed", "Hiçbir şey olmadı (Hava)"},
            {"play", "Oyna"},
            {"settings", "Ayarlar"}
        };

        // --- ARABIC (العربية) ---
        translations[(int)Language::ARABIC] = {
            {"out_of_bounds", "لقد وصلت إلى حدود World 2000x3000!"},
            {"portal_nether", "جاري الدخول إلى النذر..."},
            {"portal_overworld", "العودة إلى العالم الأساسي..."},
            {"craft_failed", "لم يحدث شيء (هواء)"},
            {"play", "يلعب"},
            {"settings", "إعدادات"}
        };

        // --- AZERBAIJANI (Azərbaycan) ---
        translations[(int)Language::AZERBAIJANI] = {
            {"out_of_bounds", "2000x3000 dünya sərhədinə çatdınız!"},
            {"portal_nether", "Nether ölçüsünə daxil olunur..."},
            {"portal_overworld", "Əsas dünyaya qayıdılır..."},
            {"craft_failed", "Heç nə alınmadı (Air)"},
            {"play", "Oyna"},
            {"settings", "Tənzimləmələr"}
        };
    }

    std::string get(const std::string& key) {
        int langKey = (int)currentLang;
        if (translations[langKey].find(key) != translations[langKey].end()) {
            return translations[langKey][key];
        }
        return key; // Tərcümə tapılmasa açar sözün özünü qaytarır
    }
};

int main() {
    // 1. Əvvəlcə Azərbaycan dilində yoxlayaq
    LocalizationManager loc(Language::AZERBAIJANI);
    
    std::cout << "[AZ] " << loc.get("out_of_bounds") << "\n";
    std::cout << "[AZ] " << loc.get("play") << "\n\n";

    // 2. Dilini İngilis dilinə dəyişək
    loc.setLanguage(Language::ENGLISH);
    std::cout << "[EN] " << loc.get("out_of_bounds") << "\n";
    std::cout << "[EN] " << loc.get("play") << "\n\n";

    // 3. Dilini Türk dilinə dəyişək
    loc.setLanguage(Language::TURKISH);
    std::cout << "[TR] " << loc.get("out_of_bounds") << "\n";
    std::cout << "[TR] " << loc.get("play") << "\n";

    return 0;
}