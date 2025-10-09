#include <kipr/wombat.h>

int forward(int ms) {
    ao();
    motor(3, 100); // left
    motor(0, 100); // right
    msleep(ms);
    ao();
    return 0;
}

int back(int ms) {
    ao();
    motor(3, -100); // left
    motor(0, -100); // right
    msleep(ms);
    ao();
    return 0;
}

void hold() {
	set_servo_position(1, 2100);
}

void drop() {
    set_servo_position(1, 500);
}

int lift(){
	set_servo_position(0, 2100);
}

int leftward(){
    motor(0, 100);
    msleep(1000);
    ao();
    return 0;
}

int rightward(){
    motor(3, 100);
    msleep(1000);
    ao();
    return 0;
}

int main()
{
    // forward(1000);
    // drop();
    // msleep(2000);
    // hold();
    // msleep(1000);
    // back(800);
    // msleep(500);
    // drop();
    // lift();
    leftward();
    return 0;
}
