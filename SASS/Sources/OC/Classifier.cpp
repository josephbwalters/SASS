/* Standard headers */
#include <stdio.h>

/* SYS/BIOS headers */
#include <ti/sysbios/knl/Task.h>

/* Custom headers */
#include <Sources/OC/Classifier.h>

using namespace sources;
using namespace sources::oc;

Classifier::Classifier(Directions direction)
{

}

Classifier::~Classifier()
{

}

Classifier* Classifier::get_instance(Directions direction)
{
    switch (direction)
    {
    case Directions::NORTH:
        if (classifier_north == nullptr)
        {
            classifier_north = new Classifier(Directions::NORTH);
        }
        return classifier_north;

    case Directions::EAST:
        if (classifier_east == nullptr)
        {
            classifier_east = new Classifier(Directions::EAST);
        }
        return classifier_east;

    case Directions::SOUTH:
        if (classifier_south == nullptr)
        {
            classifier_south = new Classifier(Directions::SOUTH);
        }
        return classifier_south;

    case Directions::WEST:
        if (classifier_west == nullptr)
        {
            classifier_west = new Classifier(Directions::WEST);
        }
        return classifier_west;

    default:
        // TODO: Throw exception
        return nullptr;
    };
}

void *Classifier::classifier_thread(void *args)
{
    while(1)
    {
        printf("Running Classifier thread...\n");

        Task_yield();
    }
}
