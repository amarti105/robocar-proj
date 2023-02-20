#include <ros.h>
#include <std_msgs/Int16.h>

const int EN1 = 2;
const int EN2 = 4;
const int EN3 = 7;
const int En4 = 8;

const int In1 = 5;
const int IN2 = 6;

ros::NodeHandle nh;
void motorrCallback(const std_msgs::Int16& cmd_msg)
{
  if (cmd_msg.data > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  analogWrite(EN1, abs(cmd_msg.data));
}

void motorlCallback(const std_msgs::Int16& cmd_msg)
{
  if (cmd_msg.data > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  }
  analogWrite(EN2, abs(cmd_msg.data));
}

ros::Subscriber<std_msgs::Int16> motor1_sub("motor1", motorrCallback);
ros::Subscriber<std_msgs::Int16> motor2_sub("motor2", motorlCallback);

void setup()
{
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(EN2, OUTPUT);
  pinMode(EN3, OUTPUT);
  pinMode(EN4, OUTPUT);

  nh.initNode();
  nh.subscribe(motor1_sub);
  nh.subscribe(motor2_sub);
}

void loop()
{
  nh.spinOnce();
}
