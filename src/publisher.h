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
#include <vector>

#include <msgpack.hpp>
#include <zmq.hpp>

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
            
            
            void publish(const char* topic_name, const message_type& message)
            {
                sendTopicName(std::string(topic_name));
                msgpack::sbuffer serialized_message;
                serializeMessage(serialized_message, message);
                send(serialized_message);
            }
        
        
        private:
            zmq::context_t context_;
            zmq::socket_t  publisher_;


        private:
            void serializeMessage(msgpack::sbuffer& message_to_serialize, const message_type& message)
            {
                msgpack::pack(message_to_serialize, message);
            }
            

            void send(const msgpack::sbuffer& content)
            {
                zmq::message_t message(content.size());
                memcpy(message.data(), content.data(), content.size());
                publisher_.send(message);
            }
            
            
            void sendTopicName(const std::string& topic_name)
            {
                zmq::message_t message(topic_name.size());
                memcpy(message.data(), topic_name.data(), topic_name.size());
                publisher_.send(message, ZMQ_SNDMORE);
            }
    };
}
