/*
 *  Hardware overview (update port numbers in the config section below):
 *    Motor 0 -> left wheel    Motor 1 -> right wheel
 *    Servo 1 -> arm up/down   (0 = lowest, 2047 = highest)
 *    Servo 0 -> claw open/close  (600 = closed, 0 = fully open)
 */

#include <kipr/wombat.h>

int iabs(int x)
{
    return x < 0 ? -x : x;
}

#define PORT_MOTOR_L 0
#define PORT_MOTOR_R 1
#define PORT_SERVO_ARM 1
#define PORT_SERVO_CLAW 0

#define ARM_LOW 100
#define ARM_MID 800
#define ARM_HIGH 1900

#define CLAW_OPEN 0
#define CLAW_GRIP 450
#define CLAW_CLOSE 600
#define SPD_EX 1000
#define SPD_FAST 80
#define SPD_MID 55
#define SPD_SLOW 35

void move_time(int speed, int time_ms)
{
    motor(PORT_MOTOR_L, -speed);
    motor(PORT_MOTOR_R, -speed);
    msleep(time_ms);
    ao();
}

void move_ticks(int speed, int ticks)
{
    clear_motor_position_counter(PORT_MOTOR_L);
    while (iabs(get_motor_position_counter(PORT_MOTOR_L)) < ticks)
    {
        motor(PORT_MOTOR_L, -speed);
        motor(PORT_MOTOR_R, -speed);
    }
    ao();
}

void pivot_left(int speed, int ticks)
{
    clear_motor_position_counter(PORT_MOTOR_R);
    while (iabs(get_motor_position_counter(PORT_MOTOR_R)) < ticks)
    {
        motor(PORT_MOTOR_L, 0);
        motor(PORT_MOTOR_R, -speed);
    }
    ao();
}

void pivot_right(int speed, int ticks)
{
    clear_motor_position_counter(PORT_MOTOR_L);
    while (iabs(get_motor_position_counter(PORT_MOTOR_L)) < ticks)
    {
        motor(PORT_MOTOR_L, -speed);
        motor(PORT_MOTOR_R, 0);
    }
    ao();
}

void spin_left(int speed, int ticks)
{
    clear_motor_position_counter(PORT_MOTOR_R);
    while (iabs(get_motor_position_counter(PORT_MOTOR_R)) < ticks)
    {
        motor(PORT_MOTOR_L, speed);
        motor(PORT_MOTOR_R, -speed);
    }
    ao();
}

void spin_right(int speed, int ticks)
{
    clear_motor_position_counter(PORT_MOTOR_L);
    while (iabs(get_motor_position_counter(PORT_MOTOR_L)) < ticks)
    {
        motor(PORT_MOTOR_L, -speed);
        motor(PORT_MOTOR_R, speed);
    }
    ao();
}

void spin_left_time(int speed, int time_ms)
{
    motor(PORT_MOTOR_L, speed);
    motor(PORT_MOTOR_R, -speed);
    msleep(time_ms);
    ao();
}

void spin_right_time(int speed, int time_ms)
{
    motor(PORT_MOTOR_L, -speed);
    motor(PORT_MOTOR_R, speed);
    msleep(time_ms);
    ao();
}

void pause(int ms)
{
    ao();
    msleep(ms);
}

void arm_enable()
{
    enable_servo(PORT_SERVO_ARM);
    enable_servo(PORT_SERVO_CLAW);
    msleep(200);
}

void arm_disable()
{
    disable_servo(PORT_SERVO_ARM);
    disable_servo(PORT_SERVO_CLAW);
}

void arm_to(int pos, int wait_ms)
{
    set_servo_position(PORT_SERVO_ARM, pos);
    msleep(wait_ms);
}

void claw_to(int pos, int wait_ms)
{
    set_servo_position(PORT_SERVO_CLAW, pos);
    msleep(wait_ms);
}

void claw_open()
{
    claw_to(CLAW_OPEN, 600);
}

void claw_close()
{
    claw_to(CLAW_GRIP, 700);
}

void arm_down()
{
    arm_to(ARM_LOW, 800);
}

void arm_mid()
{
    arm_to(ARM_MID, 700);
}

void arm_high()
{
    arm_to(ARM_HIGH, 800);
}

void pick_up()
{
    claw_open();
    arm_down();
    msleep(300);
    claw_close();
    msleep(400);
    arm_high();
}

void put_down()
{
    arm_down();
    msleep(300);
    claw_open();
    msleep(400);
    arm_high();
}

void put_down_on_box(int box_height)
{
    arm_to(box_height, 800);
    msleep(200);
    claw_open();
    msleep(500);
    arm_high();
}

void task1()
{
    printf("Task1 X02g: Large Loop\n");
    arm_enable();
    claw_close();
    pivot_right(SPD_MID, 1);
    pause(300);
    move_ticks(SPD_FAST, 7500); // below the green(right)
    pause(300);

    pivot_left(SPD_MID, 1600); // face upward
    pause(300);

    move_ticks(SPD_FAST, 2350); // 7~5
    pause(300);

    pivot_left(SPD_MID, 1400);
    pause(300);
    claw_open();
    move_ticks(SPD_FAST, 3580); // 5~1
    pause(300);

    pivot_left(SPD_MID, 1220); // face downward
    pause(300);
    move_ticks(SPD_SLOW, 400);
    pause(300);
    claw_close();
    pause(300);
    move_ticks(SPD_SLOW, 200);
    pause(300);

    pivot_right(SPD_MID, 1520);
    pause(300);
    move_ticks(SPD_FAST, 900);
    printf("Task1 Done\n");
}

void task2()
{
    printf("Task2 X09d: Extreme Slalom\n");
    claw_close();
    spin_right(SPD_MID, 470);
    pause(300);
    move_ticks(SPD_FAST, 2700);
    pause(300);

    spin_left(SPD_MID, 500);
    pause(300);

    spin_right(SPD_MID, 50);
    pause(300);
    move_ticks(SPD_FAST, 900);
    pause(300);
    spin_left(SPD_MID, 250);
    pause(300);
    move_ticks(SPD_FAST, 5800);
    pause(300);

    spin_right(SPD_MID, 300);
    pause(300);
    move_ticks(SPD_FAST, 1800);
    pause(300);
    spin_right(SPD_MID, 300);
    pause(300);
    move_ticks(SPD_FAST, 1500);
    pause(300);
    spin_right(SPD_MID, 290);
    pause(300);
    move_ticks(SPD_FAST, 1000);
    pause(300);
    spin_right(SPD_MID, 250);
    pause(300);
    move_ticks(SPD_FAST, 2200);
    pause(300);
    spin_right(SPD_MID, 400);
    pause(300);
    move_ticks(SPD_FAST, 6200);
    pause(300);
    spin_left(SPD_MID, 400);
    pause(300);

    move_ticks(SPD_FAST, 1000);
    pause(300);
    spin_left(SPD_MID, 400);
    pause(300);
    move_ticks(SPD_FAST, 1800);
    pause(300);
    spin_right(SPD_MID, 400);
    pause(300);
    move_ticks(SPD_FAST, 1000);
    pause(300);
    printf("Task2 Done\n");
}

void task3()
{
    printf("Task3 J05b: Can Collection\n");

    int BOX_HEIGHT = 970;

    arm_enable();
    claw_open();
    arm_down();
    pause(500);
    pivot_left(SPD_MID, 100);
    move_ticks(SPD_FAST, 2200);
    pause(300);
    pick_up();
    pause(100);

    pivot_right(SPD_MID, 280);
    pause(300);
    move_ticks(SPD_FAST, 4400);
    pause(300);
    pivot_left(SPD_MID, 200);
    pause(300);
    put_down_on_box(BOX_HEIGHT);
    pause(400);

    arm_high();
    pause(100);
    move_ticks(-SPD_MID, 1500);
    pause(300);
    arm_down();
    pause(300);
    spin_right(SPD_MID, 850);
    pause(300);
    move_ticks(SPD_MID, 900);
    pause(300);
    pick_up();
    pause(100);
    spin_left(SPD_MID, 800);
    pause(100);
    move_ticks(SPD_FAST, 1800);
    pause(100);
    spin_left(SPD_MID, 300);
    pause(100);
    put_down_on_box(BOX_HEIGHT);
    pause(400);
    move_ticks(-SPD_MID, 500);
    pause(300);
    spin_right(SPD_MID, 200);
    pause(100);
    move_ticks(-SPD_MID, 6500);
    pause(300);

    printf("Task3 Done\n");
}

void task4()
{
    printf("Task4 G01c: Precise Transport\n");
    int BOX_HEIGHT = 1680;
    arm_enable();
    claw_open();
    arm_down();
    msleep(500);

    pause(100);
    move_ticks(SPD_FAST, 1400);
    pause(300);
    pivot_left(SPD_EX, 1700); // turn 90 degrees
    pause(300);
    move_ticks(SPD_FAST, 800);
    pause(300);
    pivot_left(SPD_MID, 1374);
    pause(300);
    // start
    move_ticks(SPD_FAST, 3665);
    pause(300);
    arm_to(BOX_HEIGHT, 100);
    pause(500);
    msleep(1000);
    claw_close();
    pause(500);
    arm_high();
    // back
    move_ticks(-SPD_FAST, 300);
    pause(300);
    pivot_right(SPD_EX, 2660);
    pause(300);
    move_ticks(SPD_EX, 1400);
    pause(300);
    pivot_right(SPD_EX, 300);
    pause(300);
    move_ticks(SPD_EX, 2600);
    pause(300);
    pivot_right(SPD_EX, 1630); // turn 90 degrees
    pause(300);
    move_ticks(SPD_EX, 870);
    pause(300);
    pivot_right(SPD_EX, 800); // turn 90 degrees
    pause(300);
    move_ticks(SPD_EX, 300);
    pause(300);
    arm_down();
    msleep(300);
    claw_open();
    msleep(400);
    move_ticks(-SPD_EX, 300);
    pause(300);
    pivot_left(SPD_EX, 1630); // turn 90 degrees
    pause(300);
    move_ticks(SPD_FAST, 800);
    pause(300);
    pivot_left(SPD_MID, 1352);
    pause(300);
    printf("Task4 Done\n");
}

void task6()
{
    printf("Task4 G01c: Precise Transport\n");
    int BOX_HEIGHT = 1664;
    arm_enable();
    claw_open();
    arm_to(BOX_HEIGHT, 100);
    pause(500);
    pivot_right(SPD_EX, 2);
    pause(300);
    move_ticks(SPD_FAST, 2880);
    pause(300);
    claw_close();
    pause(300);
    arm_high();
    pause(700);
    move_ticks(-SPD_FAST, 2050);
    pause(300);
    pivot_right(SPD_EX, 2000); // turn 90 degrees
    pause(300);
    arm_down();
    pause(300);
    claw_open();
    pause(200);
    move_ticks(-SPD_FAST, 800);
    pause(300);
    pivot_left(SPD_FAST, 800);
    pause(300);
    arm_to(BOX_HEIGHT, 100);
    pause(500);
    move_ticks(SPD_EX, 500);
    pause(300);
    pivot_left(SPD_FAST, 1400);
    pause(300);
    move_ticks(SPD_EX, 2120);
    pause(300);
    pivot_right(SPD_EX, 114);
    pause(300);
    move_ticks(SPD_EX, 50);
    claw_close();
    pause(300);
    arm_high();
    pause(300);
    pivot_left(SPD_FAST, 70);
    pause(300);
    move_ticks(-SPD_EX, 3100);
    pause(300);
    pivot_left(SPD_FAST, 3000);
    pause(300);
    move_ticks(SPD_EX, 500);
    pause(300);
    arm_down();
    pause(300);
    claw_open();
    pause(300);

    printf("Task5 Done\n");
}

void task5()
{
    printf("Task4 G01c: Precise Transport\n");
    int BOX_HEIGHT = 1480;
    arm_enable();
    claw_open();
    arm_to(BOX_HEIGHT, 100);
    pause(500);
    pivot_right(SPD_EX, 2);
    pause(300);
    move_ticks(SPD_FAST, 4150);
    pause(300);
    claw_close();
    pause(300);
    arm_high();
    pause(700);
    move_ticks(-SPD_FAST, 3800);
    pause(300);
    pivot_right(SPD_EX, 1800);
    arm_to(50, 100);
    pause(200);
    claw_open();
    pause(200);

    printf("Task5 Done\n");
}
void task5_2()
{
    printf("Task4 G01c: Precise Transport\n");
    int BOX_HEIGHT = 1480;
    arm_enable();
    claw_open();
    arm_to(BOX_HEIGHT, 100);
    pause(500);
    pivot_right(SPD_EX, 2);
    pause(300);
    move_ticks(SPD_FAST, 8950);
    pause(300);
    pivot_left(SPD_EX, 1800);
    pause(300);
    move_ticks(SPD_EX, 850);
    pause(300);
    pivot_left(SPD_EX, 600);
    pause(300);
    move_ticks(SPD_EX, 300);
    pause(300);
    claw_close();
    pause(300);
    arm_high();
    pause(700);
    move_ticks(-SPD_FAST, 1800);
    pause(300);
    pivot_left(SPD_EX, 1500);
    pause(300);
    move_ticks(SPD_EX, 2400);
    pause(300);
    pivot_right(SPD_EX, 1500);
    pause(300);
    move_ticks(SPD_EX, 8000);
    pause(300);
    pivot_left(SPD_EX, 200);
    pause(300);
    arm_to(50, 100);
    pause(200);
    claw_open();
    pause(200);

    printf("Task5 Done\n");
}

int main()
{
    printf("=== Botball PRE Program Start ===\n");
    // task1(); // Task 1
    // task2(); // Task 2
    // task3(); // Task 3
    // task4(); // Task 4
    task5_2();

    printf("=== Program End ===\n");
    return 0;
}