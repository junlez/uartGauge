
#define LED 13
#define SERIAL_BAUD 115200


typedef struct {
  uint32_t contact1;
  uint32_t contact2;
  uint32_t contact3;
  uint32_t contact4;
  const uint32_t delayUs = 1000;
  const uint8_t stepperGain = 1;
  const uint8_t stepperLowerLimit = 0;
  const uint8_t stepperUpperLimit = 159;
  const uint8_t stepperTrimCw = 240;
  const uint8_t stepperTrimCcw = 241;
} StepperX27168;

void StepperX27168_constructor(StepperX27168* const me, const uint32_t contact1, const uint32_t contact2, const uint32_t contact3, const uint32_t contact4)
{
  me->contact1 = contact1;
  me->contact2 = contact2;
  me->contact3 = contact3;
  me->contact4 = contact4;

  digitalWrite(me->contact1, LOW);
  digitalWrite(me->contact2, LOW);
  digitalWrite(me->contact3, LOW);
  digitalWrite(me->contact4, LOW);

  pinMode(me->contact1, OUTPUT);
  pinMode(me->contact2, OUTPUT);
  pinMode(me->contact3, OUTPUT);
  pinMode(me->contact4, OUTPUT);

  digitalWrite(me->contact1, LOW);
  digitalWrite(me->contact2, LOW);
  digitalWrite(me->contact3, LOW);
  digitalWrite(me->contact4, HIGH);
}

void StepperX27168_cw(StepperX27168* const me)
{
  digitalWrite(me->contact1, HIGH);
  delayMicroseconds(me->delayUs);

  digitalWrite(me->contact4, LOW);
  delayMicroseconds(me->delayUs);

  digitalWrite(me->contact2, HIGH);
  digitalWrite(me->contact3, HIGH);
  delayMicroseconds(me->delayUs);

  digitalWrite(me->contact1, LOW);
  delayMicroseconds(me->delayUs);

  digitalWrite(me->contact4, HIGH);
  delayMicroseconds(me->delayUs);

  digitalWrite(me->contact2, LOW);
  digitalWrite(me->contact3, LOW);
  delayMicroseconds(me->delayUs);
}

void StepperX27168_ccw(StepperX27168* const me)
{
  digitalWrite(me->contact2, HIGH);
  digitalWrite(me->contact3, HIGH);
  delayMicroseconds(me->delayUs);

  digitalWrite(me->contact4, LOW);
  delayMicroseconds(me->delayUs);

  digitalWrite(me->contact1, HIGH);
  delayMicroseconds(me->delayUs);

  digitalWrite(me->contact2, LOW);
  digitalWrite(me->contact3, LOW);
  delayMicroseconds(me->delayUs);

  digitalWrite(me->contact4, HIGH);
  delayMicroseconds(me->delayUs);

  digitalWrite(me->contact1, LOW);
  delayMicroseconds(me->delayUs);
}


static StepperX27168 gauge1;
static uint8_t target = 0;
static uint8_t current = 0;


// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(LED, OUTPUT);

  StepperX27168_constructor(&gauge1, 2, 3, 5, 4);

  Serial.begin(SERIAL_BAUD);
}


// the loop routine runs over and over again forever:
void loop()
{
  uint8_t newTarget;
  if(Serial.available())
  {
    newTarget = Serial.read();
//    Serial.write(newTarget);
    
    if((newTarget >= gauge1.stepperLowerLimit) && (newTarget <= gauge1.stepperUpperLimit))
    {
      target = gauge1.stepperGain * newTarget;
    }
    else if(newTarget == gauge1.stepperTrimCw)
    {
      if(current == target)
      {
      	StepperX27168_cw(&gauge1);
      }
      else
      {
      	target++;
      }
    }
    else if(newTarget == gauge1.stepperTrimCcw)
    {
      if(current == target)
      {
      	StepperX27168_ccw(&gauge1);
      }
      else
      {
      	target--;
      }
    }
  }

  if(current < target)
  {
    StepperX27168_cw(&gauge1);

    current++;
  }
  else if(current > target)
  {
    StepperX27168_ccw(&gauge1);

    current--;
  }
}
