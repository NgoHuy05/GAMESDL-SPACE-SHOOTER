class Character {
private:
    int x;
    int y;
    int speed;
    int screenWidth;
    int screenHeight;
    std::vector<Bullet> bullets;
    int score;
    int bestScore;
    int HP;
public:
    Character(int initialX, int initialY, int initialSpeed, int screenW, int screenH)
        : x(initialX), y(initialY), speed(initialSpeed), screenWidth(screenW), screenHeight(screenH), score(0), bestScore(0), HP(3) {}

    void moveUp() { y -= speed; }
    void moveDown() { y += speed; }
    void moveLeft() { x -= speed; }
    void moveRight() { x += speed; }

    void shoot() {
        int bulletX = x + 50;
        int bulletY = y;
        int bulletSpeed = 15;
        Bullet newBullet(bulletX, bulletY, bulletSpeed);
        bullets.push_back(newBullet);
    }

    void updatePosition() {
        if (x < 0) { x = 0; }
        if (x > screenWidth - 50) { x = screenWidth - 50; }
        if (y < 0) { y = 0; }
        if (y > screenHeight - 50) { y = screenHeight - 50; }

        std::vector<Bullet> remainingBullets;
        for (size_t i = 0; i < bullets.size(); ++i) {
            bullets[i].move();
            if (bullets[i].getX() <= screenWidth) {
                remainingBullets.push_back(bullets[i]);
            }
        }
        bullets = remainingBullets;
    }

    bool isCollidingWithEnemy(const Enemy& enemy) const {
        SDL_Rect characterRect = { x, y, 50, 50 };
        SDL_Rect enemyRect = { enemy.getX(), enemy.getY(), 50, 50 };
        return SDL_HasIntersection(&characterRect, &enemyRect);
    }

    bool isCollidingWithThreat(const Threat& threat) const {
        SDL_Rect characterRect = { x, y, 50, 50 };
        SDL_Rect threatRect = { threat.getX(), threat.getY(), 50, 50 };
        return SDL_HasIntersection(&characterRect, &threatRect);
    }

    int getX() const { return x; }
    void setX(int newX) { x = newX; }
    int getY() const { return y; }
    void setY(int newY) { y = newY; }

    const std::vector<Bullet>& getBullets() const { return bullets; }
    int getScore() const { return score; }
    void increaseScore(int amount) { score += amount; }
    void setScore(int newScore) { score = newScore; }
    int getBestScore() const { return bestScore; }
    void setBestScore(int newBestScore) { bestScore = newBestScore; }
    void updateBestScore() { if (score > bestScore) bestScore = score; }

    int getHP() const { return HP; }
    void setHP(int newHP) { HP = newHP; }
    void decreaseHP(int amount) { HP -= amount; }
};
