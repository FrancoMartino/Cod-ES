#ifndef MotorPAP_h
#define MotorPAP_h

class MotorPAP
{
public:
  MotorPAP(int number_of_steps, int motor_pin_1, int motor_pin_2);
  MotorPAP(int number_of_steps, int motor_pin_1, int motor_pin_2,
           int motor_pin_3, int motor_pin_4);
  MotorPAP(int number_of_steps, int motor_pin_1, int motor_pin_2,
           int motor_pin_3, int motor_pin_4,
           int motor_pin_5);

  void ajustarVelocidad(long whatSpeed);

  void paso(int number_of_steps);

  int version(void);

private:
  void stepMotor(int this_step);

  int direction;
  unsigned long step_delay;
  int number_of_steps;
  int pin_count;
  int step_number;

  int motor_pin_1;
  int motor_pin_2;
  int motor_pin_3;
  int motor_pin_4;
  int motor_pin_5;

  unsigned long last_step_time;
};

#endif