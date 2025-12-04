#include <kipr/wombat.h>
#include <stdio.h>

/*
 硬件映射（按你提供）
 左轮电机    -> motor port 3
 右轮电机    -> motor port 0
 灰度传感器  -> analog port 0
 前碰撞传感器-> digital port 7
 抓握舵机    -> servo port 1
 上下舵机    -> servo port 0

 单探头巡线（中心放在车前中间）
*/

// ========== 可调参数 ==========
const int LEFT_MOTOR = 3;
const int RIGHT_MOTOR = 0;
const int LINE_SENSOR = 0;      // analog port
const int FRONT_BUMP = 7;       // digital port

// 灰度阈值（由你测得）
const int BLACK_VAL = 3400;
const int WHITE_VAL = 900;
const int THRESHOLD = (BLACK_VAL + WHITE_VAL) / 2; // 2150

const int BASE_SPEED = 50;      // 平稳模式建议 40~55，可调
// 比例系数 Kp = K_NUM / K_DEN，使用整数运算避免浮点
const int K_NUM = 1;
const int K_DEN = 50;           // adj = (reading - threshold) * K_NUM / K_DEN

// 丢线判断（当探测到偏离太多时触发“找线”）
const int LOST_LINE_LOW = WHITE_VAL + 200;   // 比白面高一点
const int LOST_LINE_HIGH = BLACK_VAL - 200;  // 比黑面低一点

// 舵机预设位置（按需调整）
const int SERVO_GRAB_OPEN = 1000;
const int SERVO_GRAB_CLOSE = 2000;
const int SERVO_LIFT_DOWN = 1000;
const int SERVO_LIFT_UP = 2000;

// ========== 帮助函数 ==========
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

// 简单夹取动作（示例）
void grab_object() {
    // 放下抓手 -> 关闭 -> 抬起
    set_servo_position(1, SERVO_GRAB_OPEN); msleep(300);
    set_servo_position(0, SERVO_LIFT_DOWN); msleep(300);
    set_servo_position(1, SERVO_GRAB_CLOSE); msleep(400);
    set_servo_position(0, SERVO_LIFT_UP); msleep(400);
}

// 丢线后原地旋转找线（先向右转一段时间，再左转）
int search_for_line() {
    // 右转短旋
    motor(LEFT_MOTOR, BASE_SPEED);
    motor(RIGHT_MOTOR, -BASE_SPEED);
    int t;
    for(t = 0; t < 600; t += 50) { // 总共 0.6s 检查多次
        msleep(50);
        int a = analog(LINE_SENSOR);
        if(a > THRESHOLD - 200 && a < THRESHOLD + 200) {
            ao();
            return 1; // 找到
        }
    }
    // 若未找到，改向左转探测
    motor(LEFT_MOTOR, -BASE_SPEED);
    motor(RIGHT_MOTOR, BASE_SPEED);
    for(t = 0; t < 1200; t += 50) { // 左转时间更长一些
        msleep(50);
        int a = analog(LINE_SENSOR);
        if(a > THRESHOLD - 200 && a < THRESHOLD + 200) {
            ao();
            return 1;
        }
    }
    ao();
    return 0; // 未找到
}

// 单探头比例巡线核心（每次循环调用）
void line_follow_step() {
    int reading = analog(LINE_SENSOR);
    // 计算偏差
    int error = reading - THRESHOLD; // positive -> reading higher (更黑)
    int adj = (error * K_NUM) / K_DEN; // 调整量，可以为正负

    // 左右速度差：左 = base - adj, 右 = base + adj
    int left_speed = BASE_SPEED - adj;
    int right_speed = BASE_SPEED + adj;
    clamp_and_drive(left_speed, right_speed);
}

// ========== 主程序 ==========
int main() {
    printf("Single-sensor line follower start\n");
    // 初始化舵机（根据需要）
    enable_servos();
    set_servo_position(1, SERVO_GRAB_OPEN);
    set_servo_position(0, SERVO_LIFT_UP);
    msleep(200);

    // 主循环：巡线直到前碰撞触发（认为找到目标）
    while(1) {
        // 先检查前碰撞（优先安全）
        if(digital(FRONT_BUMP)) {
            printf("Front bump pressed - target reached\n");
            stop_drive();
            msleep(100);
            grab_object(); // 执行夹取动作
            break;
        }

        int reading = analog(LINE_SENSOR);
        // 如果传感器读到了正常范围，按比例巡线
        if(reading > (THRESHOLD - 700) && reading < (THRESHOLD + 700)) {
            line_follow_step();
        } else {
            // 认为有可能丢线，进入找线逻辑
            stop_drive();
            int found = search_for_line();
            if(!found) {
                // 如果找不到，后退一点再试（避免卡住）
                motor(LEFT_MOTOR, -BASE_SPEED/2);
                motor(RIGHT_MOTOR, -BASE_SPEED/2);
                msleep(400);
                ao();
                // 再次尝试寻找
                if(!search_for_line()) {
                    // 仍然没找到就停止，防止跑远
                    printf("Line lost - abort\n");
                    stop_drive();
                    break;
                }
            }
        }
        msleep(20); // 循环节拍
    }

    // 夹好物体后，简单返回（这里用后退固定时间作为示例）
    printf("Returning to start (reverse)\n");
    motor(LEFT_MOTOR, -BASE_SPEED);
    motor(RIGHT_MOTOR, -BASE_SPEED);
    msleep(1200); // 根据场地距离调整
    ao();

    // 释放舵机（放下物体）
    set_servo_position(0, SERVO_LIFT_DOWN); msleep(300);
    set_servo_position(1, SERVO_GRAB_OPEN); msleep(300);
    set_servo_position(0, SERVO_LIFT_UP); msleep(200);

    // 结束
    stop_drive();
    disable_servos();
    printf("Mission complete\n");
    return 0;
}
