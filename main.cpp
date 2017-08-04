#include "Scheduler.h"

#include "Ping.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>

#include <boost/asio.hpp>

#include <iostream>
#include <future>
#include <sstream>

namespace
{
    using clock = std::chrono::system_clock;
           
    template<typename Func>
    void measure(Func f, const std::string& name)
    {
       auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
       f();
       auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(clock::now());
       std::cout << name << ": " << (end - start).count() << std::endl;
    }
}

auto googleTask = Task{std::chrono::seconds(10), [](){
       auto request = []() -> void{
           curlpp::Cleanup myCleanup;
           std::ostringstream os;
           os << curlpp::options::Url(std::string("http://www.google.com"));
       };
       
       measure(request, "GoogleTask");
}};

auto pingTask = Task{std::chrono::seconds(20), [](){
    try
    {
        boost::asio::io_service io_service;
        pinger p(io_service, "google.com", [&](const ipv4_header& ipHeader, 
                                               const icmp_header& icmpHeader, 
                                               const posix_time::ptime& timeSent, 
                                               std::size_t length){
            posix_time::ptime now = posix_time::microsec_clock::universal_time();
            std::cout << length - ipHeader.header_length()
                << " bytes from " << ipHeader.source_address()
                << ": icmp_seq=" << icmpHeader.sequence_number()
                << ", ttl=" << ipHeader.time_to_live()
                << ", time=" << (now - timeSent).total_milliseconds() << " ms"
                << std::endl;
                io_service.stop();
        });
        
        io_service.run();
    }
    catch(const std::exception& e)
    {
        std::cout << "Error in ping task : " << e.what() << std::endl;
    }
}};

int main(int argc, char **argv) {
    Scheduler s;
    s.add(googleTask);
    s.add(pingTask);
    
    while(true){}
    return 0;
}
