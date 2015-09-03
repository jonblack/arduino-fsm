#include <Fsm.h>

void on_light_on_enter()
{
  Serial.println("Entering LIGHT_ON");
}

void on_light_on_exit()
{
  Serial.println("Exiting LIGHT_ON");
}

void on_light_off_enter()
{
  Serial.println("Entering LIGHT_OFF");
}

void on_light_off_exit()
{
  Serial.println("Exiting LIGHT_OFF");
}

void on_trans_light_on_light_off()
{
  Serial.println("Transitioning from LIGHT_ON to LIGHT_OFF");
}

void on_trans_light_off_light_on()
{
  Serial.println("Transitioning from LIGHT_OFF to LIGHT_ON");
}

State state_light_on(on_light_on_enter, &on_light_on_exit);
State state_light_off(on_light_off_enter, &on_light_off_exit);
Fsm fsm(&state_light_off);

void setup()
{
  Serial.begin(9600);

  fsm.add_transition(&state_light_on, &state_light_off, "flip_switch",
                     &on_trans_light_on_light_off);
  fsm.add_transition(&state_light_off, &state_light_on, "flip_switch",
                     &on_trans_light_off_light_on);
}

void loop()
{
  delay(2000);
  fsm.trigger("flip_switch");
  delay(2000);
  fsm.trigger("flip_switch");
}
