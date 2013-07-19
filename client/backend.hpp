#ifndef OBELISK_CLIENT_BACKEND
#define OBELISK_CLIENT_BACKEND

#include <functional>
#include <unordered_map>
#include <system_error>
#include <zmq.hpp>

#include "message.hpp"

class backend_cluster
{
public:
    typedef std::function<void (const bc::data_chunk&)> response_handler;

    backend_cluster();

    void request(const std::string& command,
        const bc::data_chunk& data, response_handler handle);
    void update();

private:
    struct request_container
    {
        time_t timestamp;
        size_t retries_left;
        outgoing_message message;
    };

    typedef std::unordered_map<uint32_t, response_handler>
        response_handler_map;
    typedef std::unordered_map<uint32_t, request_container>
        request_retry_queue;

    bool process(const incoming_message& response);

    zmq::context_t context_;
    zmq::socket_t socket_;
    response_handler_map handlers_;
    request_retry_queue retry_queue_;
};

#endif

