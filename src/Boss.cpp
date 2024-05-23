class Boss {
    int x;
    int y;
    int speed;
    bool movingDown;
    int HP;
public:
    Boss(int initialX, int initialY, int initialSpeed, int initialHP)
        : x(initialX), y(initialY), speed(initialSpeed), movingDown(true), HP(initialHP) {}

   void move() {
        if (movingDown) {
            y += speed;
            if (y >= 480) {
                movingDown = false;
            }
        } else {
            y -= speed;
            if (y <= 0) {
                movingDown = true;
            }
        }
    }
    int getX() const {
        return x;
    }

    int getY() const {
        return y;
    }

    int getHP() const {
        return HP;
    }

    void decreaseHP(int amount) {
        HP -= amount;
    }
};
