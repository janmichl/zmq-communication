#include <iostream>
#include <string>

#include "zhelpers.hpp"

namespace communication
{
    class Subscriber
    {
        public:
            Subscriber(const std::string& topic) : context_(1),
                                                   subscriber_(context_, ZMQ_SUB)
            {
                //  Prepare our context and subscriber
                subscriber_.connect("tcp://localhost:5563");
                subscriber_.setsockopt(ZMQ_SUBSCRIBE, topic.data(), topic.size());
            }


            std::string receive()
            {
                return(s_recv(subscriber_));
            }


        private:
            zmq::context_t context_;
            zmq::socket_t  subscriber_;
    };
}

int main ()
{
    communication::Subscriber subscriber(std::string("topic"));
    
    while(true)
    {
        std::string message = subscriber.receive();
        std::cout << "Received: " << message << std::endl;
    }
    
    return 0;
}
