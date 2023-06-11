#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <any>
#include <cstddef>
#include <variant>

namespace torrent
{

class tracker final
{
public:
    struct metainfo
    {
        std::map<std::any, std::any> info;
        std::string announce;
        std::vector<std::any> announce_list;
        uint32_t creation_date;
        std::string comment;
        std::string created_by;
        std::string encoding;
    };

public:
    class tracker_request
    {
        public:
            struct parameters
            {
                uint64_t connection_id;
                uint32_t action;
                std::string info_hash;
                std::string peer_id;
                uint16_t port;
                uint64_t uploaded;
                uint64_t downloaded;
                uint64_t left;
                std::byte compact;
                bool no_peer_id;
                std::string event;

                enum events 
                { 
                    none = 0,
                    started = 1,
                    stopped = 2,
                    completed = 3
                };
                
                std::string ip;
                uint32_t numwant;
                uint32_t key;
                std::string tracker_id;
            };
    };

    class tracker_response
    {
        public:
            struct response
            {
                std::string failure_reason;
                std::string warning_message;
                uint32_t interval;
                uint32_t min_interval;
                std::string tracker_id;
                uint32_t complete;

                struct peers
                {
                    std::string peer_id;
                    std::string ip;
                    uint16_t port;
                };

            };
    };

    class tracker_scrape
    {

    };
};

}