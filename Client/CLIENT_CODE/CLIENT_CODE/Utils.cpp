#include "Utils.h"



std::string Utils::uuid_to_packed_str(const boost::uuids::uuid& uuid_instance)
{
	string uuid_str = boost::lexical_cast<std::string>(uuid_instance);
	boost::erase_all(uuid_str, "-");

	return uuid_str;
}

/***********************************************************************************************************/

boost::uuids::uuid Utils::str_to_uuid(const string& uuid)
{
	// Need to insert dashes to make it valid uuid str
	string valid_uuid_str = uuid;
	valid_uuid_str.insert(8, 1, '-');
	valid_uuid_str.insert(13, 1, '-');
	valid_uuid_str.insert(18, 1, '-');
	valid_uuid_str.insert(23, 1, '-');

	return boost::lexical_cast<boost::uuids::uuid>(valid_uuid_str);
}

/***********************************************************************************************************/

std::string Utils::raw_bytes_to_uuid_str(uint8_t uuid_bytes[CLIENT_ID_SIZE_BYTES])
{
	boost::uuids::uuid tmp_uuid;
	memcpy(tmp_uuid.data, uuid_bytes, CLIENT_ID_SIZE_BYTES);

	return uuid_to_packed_str(tmp_uuid);
}
