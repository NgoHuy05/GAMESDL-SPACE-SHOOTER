// Định nghĩa lớp Bullet
class Bullet {
private:
    int x; // Tọa độ x của đạn trên màn hình
    int y; // Tọa độ y của đạn trên màn hình
    int speed; // Tốc độ di chuyển của đạn

public:
    // Constructor khởi tạo vị trí và tốc độ ban đầu của đạn
    Bullet(int initialX, int initialY, int initialSpeed)
        : x(initialX), y(initialY), speed(initialSpeed) {}

    // Phương thức di chuyển đạn
    void move() {
        x += speed; // Di chuyển theo chiều dương của trục x (phía bên phải)
    }
    // Getter cho tọa độ x của đạn
    int getX() const {
        return x;
    }

    // Getter cho tọa độ y của đạn
    int getY() const {
        return y;
    }
};
