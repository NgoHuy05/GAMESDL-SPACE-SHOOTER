#include <fstream>

int readHighScore(const std::string& filename) {
    std::ifstream file(filename);
    int highScore = 0;
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
    return highScore;
}

void writeHighScore(const std::string& filename, int highScore) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}
