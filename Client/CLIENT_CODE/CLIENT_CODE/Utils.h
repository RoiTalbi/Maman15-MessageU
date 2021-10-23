#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/algorithm/string.hpp>

using namespace std;




class Utils
{
public:


	/* Example: 
	
	88da877b-87fd-4017-8589-82ef89523878 -> 88da877b87fd4017858982ef89523878
	
	*/
	static std::string uuid_to_packed_str(const boost::uuids::uuid& uuid_instance)
	{
		string uuid_str = boost::lexical_cast<std::string>(uuid_instance);
		boost::erase_all(uuid_str, "-");

		return uuid_str;
	}

	/* Example:

	 88da877b87fd4017858982ef89523878 -> UUID(88da877b-87fd-4017-8589-82ef89523878)

	*/
	static boost::uuids::uuid str_to_uuid(const string& uuid)
	{
		// Need to insert dashes to make it valid uuid str
		string valid_uuid_str = uuid;
		valid_uuid_str.insert(8, 1, '-');
		valid_uuid_str.insert(12,1, '-');
		valid_uuid_str.insert(16, 1, '-');

		return boost::lexical_cast<boost::uuids::uuid>(valid_uuid_str);
	}






};