#include <ACB_ARM.h> // Add the Robot arm libraries
ACB_ARM ARM;

void setup() {
  //base,izq,pinz,dere
  ARM.ARM_init(5, 16, 17, 18); // The parameters are four Servo pins
  ARM.JoyStick_init(33, 34,32, 36, 39, 35); // Joystick initialization

  Serial.begin(9600); // Initialize serial communication at 9600 baud
  Serial.println("Robot Arm Initialized");
}

void loop() {
  // Simulate a clean console with a clear section divider
  //Serial.println("\n========================================");

  ARM.get_JoyStick();
  //prints();

  // Control arm based on joystick input
  if (ARM.JoyY1 < 50) { // Chassis left
    ARM.chassis_angle = ARM.chassis_angle + 1;
    ARM.JoyChassisCmd(ARM.chassis_angle);
  }
  if (ARM.JoyY1 > 3500) { // Chassis right
    ARM.chassis_angle = ARM.chassis_angle - 1;
    ARM.JoyChassisCmd(ARM.chassis_angle);
  }
  if (ARM.JoyX1 < 50) { // Shoulder down
    ARM.shoulder_angle = ARM.shoulder_angle + 1;
    ARM.JoyShoulderCmd(ARM.shoulder_angle);
  }
  if (ARM.JoyX1 > 4000) { // Shoulder up
    ARM.shoulder_angle = ARM.shoulder_angle - 1;
    ARM.JoyShoulderCmd(ARM.shoulder_angle);
  }
  if (ARM.JoyX2 < 50) { // Elbow up
    ARM.elbow_angle = ARM.elbow_angle + 1;
    ARM.JoyElbowCmd(ARM.elbow_angle);
  }
  if (ARM.JoyX2 > 4000) { // Elbow down
    ARM.elbow_angle = ARM.elbow_angle - 1;
    ARM.JoyElbowCmd(ARM.elbow_angle);
  }
  if (ARM.JoyY2 > 4000 & ARM.claws_angle<120) { // Claws open
    ARM.claws_angle = ARM.claws_angle + 1;
    ARM.JoyClawsCmd(ARM.claws_angle);
  }
  if (ARM.JoyY2 < 50 ) { // Claws close
    ARM.claws_angle = ARM.claws_angle - 1;
    ARM.JoyClawsCmd(ARM.claws_angle);
  }


  // Add a small delay for readability
  //delay(50);
}
void prints() {
  // Imprimir los ángulos de los servos y los valores de los joysticks en el monitor serie
  Serial.print("Servo X1 Angle: ");
  Serial.print(ARM.claws_angle);
  Serial.print("\tServo Y1 Angle: ");
  Serial.print(ARM.elbow_angle);
  Serial.print("\tServo X2 Angle: ");
  Serial.print(ARM.shoulder_angle);
  Serial.print("\tServo Y2 Angle: ");
  Serial.println(ARM.chassis_angle);

}
