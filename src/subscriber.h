/**
    @file
    @author Jan Michalczyk 

    @brief
*/

#pragma once

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
            
            
            void receive(message_type& deserialized_message)
            {
                //remove topic name from message
                receiveFromSocket();

                msgpack::sbuffer serialized_message;
                receiveFromSocket(serialized_message);
                
                // deserialize it.
                msgpack::object_handle oh = msgpack::unpack(serialized_message.data(), serialized_message.size());

                // print the deserialized object.
                msgpack::object obj = oh.get();

                // convert it into statically typed object.
                obj.convert(deserialized_message);
            }


        private:
            zmq::context_t context_;
            zmq::socket_t  subscriber_;


        private:
            void receiveFromSocket(msgpack::sbuffer& serialized_message)
            {
                zmq::message_t message;
                subscriber_.recv(&message);
                serialized_message.write(static_cast<const char *>(message.data()), message.size());
            }
            
            
            std::string receiveFromSocket()
            {
                zmq::message_t message;
                subscriber_.recv(&message);
                
                return(std::string(static_cast<const char*>(message.data()), message.size()));
            }
    };
}
