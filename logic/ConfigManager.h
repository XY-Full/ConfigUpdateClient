#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>

class ConfigManager {
private:
    std::unordered_map<std::string, std::string> configMap;
    std::string configFilePath;

    void loadConfig() {
        std::ifstream configFile(configFilePath);
        if (!configFile.is_open()) {
            throw std::runtime_error("Failed to open config file: " + configFilePath);
        }

        std::string line;
        while (std::getline(configFile, line)) {
            std::istringstream iss(line);
            std::string key, value;
            if (std::getline(iss, key, '=') && std::getline(iss, value)) {
                configMap[key] = value;
            }
        }
    }

public:
    ConfigManager(const std::string& filePath) : configFilePath(filePath) {
        loadConfig();
    }

    std::string getConfig(const std::string& key) const {
        auto it = configMap.find(key);
        if (it != configMap.end()) {
            return it->second;
        }
        return "";
    }

    void setConfig(const std::string& key, const std::string& value) {
        configMap[key] = value;
    }

    bool saveConfig() {
        std::ofstream configFile(configFilePath);
        if (!configFile.is_open()) {
            return false;
        }

        for (const auto& pair : configMap) {
            configFile << pair.first << "=" << pair.second << std::endl;
        }
        return true;
    }
};

#endif // CONFIG_MANAGER_H
