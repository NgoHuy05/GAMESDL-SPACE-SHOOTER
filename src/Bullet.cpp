class Bullet {
private:
    int x; 
    int y; 
    int speed; 
public:
    Bullet(int initialX, int initialY, int initialSpeed)
        : x(initialX), y(initialY), speed(initialSpeed) {}
    void move() {
        x += speed; 
    }
    int getX() const {
        return x;
    }
    int getY() const {
        return y;
    }
};
