#include <iostream>
#include <string>

#include "zhelpers.hpp"

namespace communication
{
    class Publisher
    {
        public:
            Publisher() : context_(1),
                          publisher_(context_, ZMQ_PUB)
            {
                //  Prepare our context and publisher
                publisher_.bind("tcp://*:5563");
            }


            void publish(const std::string& topic, const std::string& message)
            {
                std::string full_message = topic + message;
                s_send(publisher_, full_message.data());
            }
        
        
        private:
            zmq::context_t context_;
            zmq::socket_t  publisher_;
    };
}

int main()
{
    communication::Publisher publisher;

    std::string topic   = "topic";
    std::string message = "message";
    while(true)
    {
        publisher.publish(topic, message);
    }
    
    return 0;
}
