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
    class Subscriber
    {
        public:
            Subscriber(const std::string& topic) : context_(ZEROMQ_NUM_OF_THREADS_USED),
                                                   subscriber_(context_, ZMQ_SUB)
            {
                //  Prepare our context and subscriber
                subscriber_.connect("tcp://localhost:5563");
                subscriber_.setsockopt(ZMQ_SUBSCRIBE, topic.data(), topic.size());
            }


            std::string receive()
            {
                return(receiveFromSocket());
            }
            
            
            void receive(std::vector<double>& vector)
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
                obj.convert(vector);
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
    communication::Subscriber subscriber(std::string("topic"));
    
    std::vector<double> message;
    while(true)
    {
        //std::string message = subscriber.receive();
        subscriber.receive(message);
        std::cout << "Received: " << message[0] << " " << message[1] << std::endl;
    }
    
    return(0);
}
