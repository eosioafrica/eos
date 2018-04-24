#pragma once

#include <string>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

#include <cstring>
#include <cstdlib>
#include <iostream>


namespace elected_producer_status {
   enum elected_producer_status_enum
   {

      produce = 0,
      do_not_produce = 1,
      status_quiry_exception = 2,
      interprocess_exception = 3,
      nothing_to_read_exception = 4,
   };
}

class cluster {

private:

    // To anyone reading this, please just assume that this is a client process 
    // there is a seperate process that write these read values
    // into local memory. I will explain more as I find time to document stuff.
    // To implement please see https://www.boost.org/doc/libs/1_66_0/doc/html/interprocess/sharedmemorybetweenprocesses.html


    // this function is a skeleton for a more formed mechanism that must include signing of messages shared between processes.
    int get_latest_instruction_from_service(){
    
        using namespace boost::interprocess;

    	try {
        	// opening an existing shared memory object
        	shared_memory_object sharedmem2 (open_only, "can_i_produce", read_only);

        	// map shared memory object in current address space
        	mapped_region mmap (sharedmem2, read_only);

        	// need to type-cast since get_address returns void*
        	char *str_mem = static_cast<char*> (mmap.get_address());

            auto shared_memory_string = std::string(str_mem);

            // implement message signing and see if the instruction was created using the certs of this server.
            if(shared_memory_string == "yes"){
            
                return elected_producer_status::produce;
            }


            if(shared_memory_string == "no"){
            
                return elected_producer_status::do_not_produce;
            }

            return elected_producer_status::nothing_to_read_exception;

    	} catch (interprocess_exception& e) {
            
        	std::cout << e.what( ) << std::endl;

            return elected_producer_status::interprocess_exception;
    	}
    }



public:
    
     bool can_i_produce(){
     
        return get_latest_instruction_from_service() == elected_producer_status::produce;
     }
};
