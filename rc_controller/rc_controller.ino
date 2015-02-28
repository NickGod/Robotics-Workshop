#define RST P1_3
#define DQ P1_4
#define CLK P1_5

// Warning: LEFT and RIGHT MUST be triggered exclusively. If both are on,
// unexpected behavior (and potentially circuit damage) will happen!
#define LEFT P2_0
#define RIGHT P2_1

namespace Externals
{
    enum Instructions
    {
        STOP = 32,
        LEFT = 37,
        FORWARD = 38,
        RIGHT = 39,
        BACK = 40,
    };
};

namespace Internals
{
    enum Direction
    {
        LEFT = 0,
        CENTER = 1,
        RIGHT = 2,
    };

    enum Speeds
    {
        ZERO = 80,
        TOP = 255,
        BOTTOM = 0,
        STEPUP = 50,
        STEPDOWN = 50,
    };

};

static Internals::Direction Dir = Internals::CENTER;
static uint8_t speed = Internals::ZERO;

void moveLeft()
{
    switch (Dir)
    {
        case Internals::CENTER:
            Dir = Internals::LEFT;
            break;
        case Internals::RIGHT:
            Dir = Internals::CENTER;
            break;
    }
}

void moveRight()
{
    switch (Dir)
    {
        case Internals::CENTER:
            Dir = Internals::RIGHT;
            break;
        case Internals::LEFT:
            Dir = Internals::CENTER;
            break;
    }
}

uint8_t getSpeed()
{
    return speed;
}

void setSpeed(uint8_t s)
{
    speed = s;
}

void setup()
{
    // put your setup code here, to run once:
    pinMode(RST, OUTPUT);
    pinMode(DQ, OUTPUT);
    pinMode(CLK, OUTPUT);

    pinMode(LEFT, OUTPUT);
    pinMode(RIGHT, OUTPUT);

    sendTransaction(speed);

    Serial.begin(9600);
}


void sendBit(uint8_t b)
{
    digitalWrite(CLK, HIGH);
    delayMicroseconds(1);
    if (b&1)
    {
        digitalWrite(DQ, HIGH);
    }
    else
    {
        digitalWrite(DQ, LOW);
    }
    delayMicroseconds(1);
    digitalWrite(CLK, LOW);
    delayMicroseconds(1);
    digitalWrite(CLK, HIGH);
}

void sendByte(uint8_t b)
{
    for (int i = 7; i >= 0; i--)
    {
        sendBit((b & (1 << i)) >> i);
    }
}

void sendTransaction(uint8_t s)
{
    // Begin transaction
    digitalWrite(CLK, LOW);
    digitalWrite(RST, HIGH);
    delayMicroseconds(1);

    // Stack select bit
    sendBit(0);

    // Send first wiper position
    sendByte(s);

    // Send second wiper position
    sendByte(s);

    // End transaction
    digitalWrite(CLK, HIGH);
    delayMicroseconds(1);
    digitalWrite(RST, LOW);
}

void loop() 
{
    switch(Dir)
    {
        case Internals::LEFT:
            digitalWrite(RIGHT, LOW);
            digitalWrite(LEFT, HIGH);
            break;
        case Internals::RIGHT:
            digitalWrite(LEFT, LOW);
            digitalWrite(RIGHT, HIGH);
            break;
        case Internals::CENTER:
            digitalWrite(LEFT, HIGH);
            digitalWrite(RIGHT, HIGH);
            break;
    }

    sendTransaction(speed);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read(); 

    switch(inChar)
    {
        case Externals::LEFT:
            moveLeft();
            Serial.println("Left!");
            break;
        case Externals::FORWARD:
            setSpeed(constrain(getSpeed() - Internals::STEPUP, Internals::BOTTOM, Internals::TOP));
            Serial.println("Go Go Go!");
            Serial.print("Speed: " );
            Serial.println(getSpeed());
            break;
        case Externals::RIGHT:
            moveRight();
            Serial.println("Right!");
            break;
        case Externals::BACK:
            setSpeed(constrain(getSpeed() + Internals::STEPDOWN, Internals::BOTTOM, Internals::TOP));
            Serial.println("Slower!");
            Serial.print("Speed: " );
            Serial.println(getSpeed());
            break;
        case Externals::STOP:
            setSpeed(Internals::ZERO);
            Serial.println("Stop!");
            break;
    }
  }
}
