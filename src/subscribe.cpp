/**
    @file
    @author Jan Michalczyk 

    @brief
*/

#define ZEROMQ_NUM_OF_THREADS_USED 1

#include "subscriber.h"

int main()
{
    try
    {
        communication::Subscriber<std::vector<double> > subscriber("topic");
        
        std::vector<double> message;
        while(true)
        {
            subscriber.receive(message);
            std::cout << "Received: " << message[0] << " " << message[1] << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        exit(-1);
    }
    
    return(0);
}
