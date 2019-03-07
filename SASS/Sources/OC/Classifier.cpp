/*
 * Classifier.cpp
 * Created by: Joseph Walters, Trent Sellers 
 */

/* Standard headers */
#include <stdio.h>

/* System headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

/* SASS-specific headers */
#include <Sources/OC/Classifier.h>

ti_sysbios_knl_Semaphore_Handle buttonsSem = Semaphore_create(0, NULL, NULL);

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
    Directions direction;
    direction = static_cast<Directions>((int)args);

    Classifier* classifier = get_instance(direction);

    while(1)
    {
        printf("Running Classifier thread...\n");

        Task_yield();
    }
}

void *Classifier::watchman(void *args)
{
    while(1)
    {
        bool status = Semaphore_pend(buttonsSem, BIOS_WAIT_FOREVER);
        if (status == TRUE)
        {
            // Make classifier thread
            printf("Creating Classifier thread...\n");
            // Semaphore_post(buttonsSem);
        }

        Task_yield();
    }
}

void Classifier::callback_hwi(uint_least8_t index)
{
    Semaphore_post(buttonsSem);
    // printf("Captured HWI from Switch 1!\n");
}
