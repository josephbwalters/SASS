class Vehicle
{
    Directions direction;
    float speed;

    Vehicle(Directions dir, float s)
    {
       direction = dir;
       speed = s;
    }

    void set_speed(float s)
    {
         speed = s;
    }

    Directions get_direction()
    {
        return direction;
    }

    float get_speed()
    {
        return speed;
    }
};


