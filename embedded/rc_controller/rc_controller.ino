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
        STEPUP = 100,
        STEPDOWN = 100,
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
}

void sendByte(uint8_t b)
{
    for (int i = 7; i >= 0; i--)
    {
    }
}

void sendTransaction(uint8_t s)
{
    // Begin transaction

    // Stack select bit

    // Send first wiper position

    // Send second wiper position

    // End transaction
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
        case 37:
            moveLeft();
            Serial.println("Left!");
            break;
        case 38:
            Serial.println("Go Go Go!");
            Serial.print("Speed: " );
            Serial.println(getSpeed());
            break;
        case 39:
            moveRight();
            Serial.println("Right!");
            break;
        case 40:
            Serial.println("Slower!");
            Serial.print("Speed: " );
            Serial.println(getSpeed());
            break;
        case 32:
            Serial.println("Stop!");
            break;
    }
  }
}
