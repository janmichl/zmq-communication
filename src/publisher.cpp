#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>
#include <vector>

#include <msgpack.hpp>
#include <zmq.hpp>

#define ZEROMQ_NUM_OF_THREADS_USED 1

namespace communication
{
    template <typename message_type>
    class Publisher
    {
        public:
            Publisher() : context_(ZEROMQ_NUM_OF_THREADS_USED),
                          publisher_(context_, ZMQ_PUB)
            {
                publisher_.bind("tcp://*:5563");
            }
            
            
            void publish(const char* topic, const message_type& message)
            {
                setTopic(std::string(topic));
                msgpack::sbuffer sbuf;
                serializeMessage(sbuf, message);
                send(sbuf);
            }
        
        
        private:
            zmq::context_t context_;
            zmq::socket_t  publisher_;


        private:
            void serializeMessage(msgpack::sbuffer& sbuf, const message_type& message)
            {
                // serialize it into simple buffer.
                msgpack::pack(sbuf, message);
            }
            

            bool send(const msgpack::sbuffer& content)
            {
                zmq::message_t message(content.size());
                memcpy(message.data(), content.data(), content.size());
                bool rc = publisher_.send(message);
                
                return(rc);
            }
            
            
            bool setTopic(const std::string& topic_name)
            {
                zmq::message_t message(topic_name.size());
                memcpy(message.data(), topic_name.data(), topic_name.size());
                bool rc = publisher_.send(message, ZMQ_SNDMORE);
                
                return(rc);
            }
    };
}


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
