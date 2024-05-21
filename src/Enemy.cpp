class Enemy{
private:
    int x;
    int y;
    int speed;

public:
    Enemy(int initialX, int initialY, int initialSpeed)
        : x(initialX), y(initialY), speed(1.5) {}

    void move() {
        x -= speed;
    }

    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }
    bool isOffScreen() const {
        return x < 0;
    }

};
