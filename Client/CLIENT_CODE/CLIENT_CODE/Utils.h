#pragma once

#include <string>

#include "globals.h"
#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/algorithm/string.hpp>


using namespace std;





/* 
General Utility functions for Client module 
*/
class Utils
{
public:

	
	/*  Convert uuid to str with no '-' characters
	
	Example: 
	88da877b-87fd-4017-8589-82ef89523878 -> 88da877b87fd4017858982ef89523878

	*/
	static std::string uuid_to_packed_str(const boost::uuids::uuid& uuid_instance);

	/* Convert  str to uuid object 

 	 Example:
	 88da877b87fd4017858982ef89523878 -> UUID(88da877b-87fd-4017-8589-82ef89523878)

	*/
	static boost::uuids::uuid str_to_uuid(const string& uuid);


	/*
	Convert raw bytes of uuid (16 bytes) to UUID valid string
	*/
	static std::string raw_bytes_to_uuid_str(uint8_t uuid_bytes[CLIENT_ID_SIZE_BYTES]);

};