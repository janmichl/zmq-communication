/**
    @file
    @author Jan Michalczyk 

    @brief
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <signal.h>

#include <msgpack.hpp>
#include <zmq.hpp>

#define ZEROMQ_NUM_OF_THREADS_USED 1

namespace communication
{
    template <typename message_type> 
    class Subscriber
    {
        public:
            Subscriber(const char* topic) : context_(ZEROMQ_NUM_OF_THREADS_USED),
                                                   subscriber_(context_, ZMQ_SUB)
            {
                std::string topic_str = std::string(topic);
                
                subscriber_.connect("tcp://localhost:5563");
                subscriber_.setsockopt(ZMQ_SUBSCRIBE, topic_str.data(), topic_str.size());
            }
            
            
            void receive(message_type& message)
            {
                //remove topic name from message
                receiveFromSocket();

                msgpack::sbuffer sbuf;
                receiveFromSocket(sbuf);
                
                // deserialize it.
                msgpack::object_handle oh = msgpack::unpack(sbuf.data(), sbuf.size());

                // print the deserialized object.
                msgpack::object obj = oh.get();
                //std::cout << obj << std::endl;

                // convert it into statically typed object.
                obj.convert(message);
            }


        private:
            zmq::context_t context_;
            zmq::socket_t  subscriber_;


        private:
            void receiveFromSocket(msgpack::sbuffer& sbuf)
            {
                zmq::message_t message;
                subscriber_.recv(&message);
                sbuf.write(static_cast<const char *>(message.data()), message.size());
            }
            
            
            std::string receiveFromSocket()
            {
                zmq::message_t message;
                subscriber_.recv(&message);

                return(std::string(static_cast<char*>(message.data()), message.size()));
            }
    };
}


int main()
{
    communication::Subscriber<std::vector<double> > subscriber("topic");
    
    std::vector<double> message;
    while(true)
    {
        subscriber.receive(message);
        std::cout << "Received: " << message[0] << " " << message[1] << std::endl;
    }
    
    return(0);
}
