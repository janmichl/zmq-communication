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
            Publisher(const char* topic) : context_(ZEROMQ_NUM_OF_THREADS_USED),
                                           publisher_(context_, ZMQ_PUB)
            {
                topic_ = topic;
                publisher_.bind("tcp://*:5563");
            }
            
            
            void publish(const message_type& message_to_publish)
            {
                sendPart(topic_);
                msgpack::sbuffer serialized_message_to_publish;
                serializeMessage(serialized_message_to_publish, message_to_publish);
                send(serialized_message_to_publish);
            }
        
        
        private:
            zmq::context_t context_;
            zmq::socket_t  publisher_;
            std::string    topic_;


        private:
            void serializeMessage(msgpack::sbuffer& serialized_message, const message_type& message_to_serialize)
            {
                msgpack::pack(serialized_message, message_to_serialize);
            }
            

            void send(const msgpack::sbuffer& content)
            {
                zmq::message_t message(content.size());
                memcpy(message.data(), content.data(), content.size());
                publisher_.send(message);
            }
            
            
            void sendPart(const std::string& message_part)
            {
                zmq::message_t message(message_part.size());
                memcpy(message.data(), message_part.data(), message_part.size());
                publisher_.send(message, ZMQ_SNDMORE);
            }
    };
}
