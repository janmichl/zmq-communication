/**
    @file
    @author Jan Michalczyk 

    @brief
*/

#define ZEROMQ_NUM_OF_THREADS_USED 1

#include "publisher.h"

int main()
{
    try
    {
        communication::Publisher<std::vector<double> > publisher("topic", "127.0.0.1", "5563");

        std::vector<double> message;
        message.push_back(10);
        message.push_back(20);

        while(true)
        {
            publisher.publish(message);
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        exit(-1);
    }
    
    return(0);
}
