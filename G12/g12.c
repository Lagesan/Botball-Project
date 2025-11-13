#include <kipr/wombat.h>
#include <stdio.h>

const int LEFT_MOTOR = 3;
const int RIGHT_MOTOR = 0;
const int LINE_SENSOR = 0;
const int FRONT_BUMP = 7;

const int BLACK_VAL = 3400;
const int WHITE_VAL = 900;
const int THRESHOLD = (BLACK_VAL + WHITE_VAL) / 2;

const int BASE_SPEED = 50;
const int K_NUM = 1;
const int K_DEN = 50;

const int LOST_LINE_LOW = WHITE_VAL + 200;
const int LOST_LINE_HIGH = BLACK_VAL - 200;

const int SERVO_GRAB_OPEN = 1000;
const int SERVO_GRAB_CLOSE = 2000;
const int SERVO_LIFT_DOWN = 1000;
const int SERVO_LIFT_UP = 2000;

void clamp_and_drive(int left, int right) {
    if(left > 100) left = 100;
    if(left < -100) left = -100;
    if(right > 100) right = 100;
    if(right < -100) right = -100;
    motor(LEFT_MOTOR, left);
    motor(RIGHT_MOTOR, right);
}

void stop_drive() {
    ao();
}

void grab_object() {
    set_servo_position(1, SERVO_GRAB_OPEN); msleep(300);
    set_servo_position(0, SERVO_LIFT_DOWN); msleep(300);
    set_servo_position(1, SERVO_GRAB_CLOSE); msleep(400);
    set_servo_position(0, SERVO_LIFT_UP); msleep(400);
}

int search_for_line() {
    motor(LEFT_MOTOR, BASE_SPEED);
    motor(RIGHT_MOTOR, -BASE_SPEED);
    int t;
    for(t = 0; t < 600; t += 50) {
        msleep(50);
        int a = analog(LINE_SENSOR);
        if(a > THRESHOLD - 200 && a < THRESHOLD + 200) {
            ao();
            return 1;
        }
    }
    motor(LEFT_MOTOR, -BASE_SPEED);
    motor(RIGHT_MOTOR, BASE_SPEED);
    for(t = 0; t < 1200; t += 50) {
        msleep(50);
        int a = analog(LINE_SENSOR);
        if(a > THRESHOLD - 200 && a < THRESHOLD + 200) {
            ao();
            return 1;
        }
    }
    ao();
    return 0;
}

void line_follow_step() {
    int reading = analog(LINE_SENSOR);
    int error = reading - THRESHOLD;
    int adj = (error * K_NUM) / K_DEN;

    int left_speed = BASE_SPEED - adj;
    int right_speed = BASE_SPEED + adj;
    clamp_and_drive(left_speed, right_speed);
}

int main() {
    printf("Single-sensor line follower start\n");
    enable_servos();
    set_servo_position(1, SERVO_GRAB_OPEN);
    set_servo_position(0, SERVO_LIFT_UP);
    msleep(200);

    while(1) {
        if(digital(FRONT_BUMP)) {
            printf("Front bump pressed - target reached\n");
            stop_drive();
            msleep(100);
            grab_object();
            break;
        }

        int reading = analog(LINE_SENSOR);
        if(reading > (THRESHOLD - 700) && reading < (THRESHOLD + 700)) {
            line_follow_step();
        } else {
            stop_drive();
            int found = search_for_line();
            if(!found) {
                motor(LEFT_MOTOR, -BASE_SPEED/2);
                motor(RIGHT_MOTOR, -BASE_SPEED/2);
                msleep(400);
                ao();
                if(!search_for_line()) {
                    printf("Line lost - abort\n");
                    stop_drive();
                    break;
                }
            }
        }
        msleep(20);
    }

    // printf("Returning to start (reverse)\n");
    // motor(LEFT_MOTOR, -BASE_SPEED);
    // motor(RIGHT_MOTOR, -BASE_SPEED);
    // msleep(1200);
    // ao();

    // set_servo_position(0, SERVO_LIFT_DOWN); msleep(300);
    // set_servo_position(1, SERVO_GRAB_OPEN); msleep(300);
    // set_servo_position(0, SERVO_LIFT_UP); msleep(200);

    stop_drive();
    disable_servos();
    printf("Mission complete\n");
    return 0;
}