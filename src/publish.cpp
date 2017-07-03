/**
    @file
    @author Jan Michalczyk 

    @brief
*/

#define ZEROMQ_NUM_OF_THREADS_USED 1

#include "publisher.h"

int main()
{
    communication::Publisher<std::vector<double> > publisher;

    std::vector<double> message;
    message.push_back(10);
    message.push_back(20);

    while(true)
    {
        publisher.publish("topic", message);
    }
    
    return(0);
}
