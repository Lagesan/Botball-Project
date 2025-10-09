#include <kipr/wombat.h>
#include <stdio.h>

void forward(int speed, double sec){
    motor(0, speed);
    motor(3, speed);
    msleep(sec * 1000);
    ao();
}

void turn_left(int speed, double sec){
    motor(0, -speed);
    motor(3, speed);
    msleep(sec * 1000);
    ao();
}

void turn_right(int speed, double sec){
    motor(0, speed);
    motor(3, -speed);
    msleep(sec * 1000);
    ao();
}

int main() {
    printf("Start 8-shape challenge!\n");

    forward(60, 1.5);
    turn_left(60, 0.8);
    forward(60, 3);
    turn_right(60, 0.8);
    forward(60, 3);
    turn_right(60, 0.9);
    forward(60, 3);
    turn_left(60, 0.8);
    forward(60, 1.2);
    turn_left(60, 0.8);
    forward(60, 1.5);
    turn_left(60, 0.8);
    forward(60, 1.2);
    turn_left(60, 0.6);
    forward(60, 1.5);
    turn_left(60, 1.2);
    forward(60, 4.5);
    turn_right(60, 1.5);
    forward(60, 1.5);
    

    ao();
    printf("Finished!\n");
    return 0;
}

// 警告：没调完，能走四分之三个八字