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

// this should be templated
namespace communication
{
    class Publisher
    {
        public:
            Publisher() : context_(ZEROMQ_NUM_OF_THREADS_USED),
                          publisher_(context_, ZMQ_PUB)
            {
                //  Prepare our context and publisher
                publisher_.bind("tcp://*:5563");
            }


            void publish(const std::string& topic, const std::string& message)
            {
                sendToTopic(topic.data());
                send(message.data());
            }
            
            
            void publish(const std::string& topic, const std::vector<double>& vector)
            {
                sendToTopic(topic.data());
                
                msgpack::sbuffer sbuf;
                serializeMessage(sbuf, vector);

                send(sbuf);
            }
        
        
        private:
            zmq::context_t context_;
            zmq::socket_t  publisher_;


        private:
            // this should be templated
            void serializeMessage(msgpack::sbuffer& sbuf, const std::vector<double>& vector)
            {
                // serialize it into simple buffer.
                msgpack::pack(sbuf, vector);
            }
            

            bool send(const msgpack::sbuffer& content)
            {
                zmq::message_t message(content.size());
                memcpy(message.data(), content.data(), content.size());
                bool rc = publisher_.send(message);
                
                return(rc);
            }


            bool send(const std::string& content)
            {
                zmq::message_t message(content.size());
                memcpy(message.data(), content.data(), content.size());
                bool rc = publisher_.send(message);
                
                return(rc);
            }
            
            
            bool sendToTopic(const std::string& topic_name)
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
    communication::Publisher publisher;

    std::string topic   = "topic";
    //std::string message = "message";
    std::vector<double> message;
    message.push_back(10);
    message.push_back(20);

    for(;;)
    {
        publisher.publish(topic, message);
    }
    
    return(0);
}
