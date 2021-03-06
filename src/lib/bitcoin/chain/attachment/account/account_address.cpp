/**
 * Copyright (c) 2011-2015 metaverse developers (see AUTHORS)
 *
 * This file is part of mvs-node.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin/chain/attachment/account/account_address.hpp>

#include <sstream>
#include <boost/iostreams/stream.hpp>
#include <bitcoin/bitcoin/utility/container_sink.hpp>
#include <bitcoin/bitcoin/utility/container_source.hpp>
#include <bitcoin/bitcoin/utility/istream_reader.hpp>
#include <bitcoin/bitcoin/utility/ostream_writer.hpp>
#include <json/minijson_writer.hpp>

namespace libbitcoin {
namespace chain {

account_address::account_address()
{
	reset();
}

account_address::account_address(std::string name, std::string prv_key, 
	std::string pub_key, uint32_t hd_index, uint64_t balance, std::string alias, std::string address) :
	name(name), prv_key(prv_key), pub_key(pub_key), hd_index(hd_index), balance(balance),
	alias(alias), address(address)
{
}

account_address::account_address(const account_address& other)
{
    name = other.name;
    prv_key = other.prv_key;
    pub_key = other.pub_key;
    hd_index = other.hd_index;
	balance = other.balance;
	alias = other.alias;
	address = other.address;
}
account_address account_address::factory_from_data(const data_chunk& data)
{
    account_address instance;
    instance.from_data(data);
    return instance;
}

account_address account_address::factory_from_data(std::istream& stream)
{
    account_address instance;
    instance.from_data(stream);
    return instance;
}

account_address account_address::factory_from_data(reader& source)
{
    account_address instance;
    instance.from_data(source);
    return instance;
}

bool account_address::is_valid() const
{
    return true;
}

void account_address::reset()
{	
    name = "";
    prv_key = "";
    pub_key = "";
    hd_index = 0;
	balance = 0;
	alias = "";
	address = "";
}

bool account_address::from_data(const data_chunk& data)
{
    data_source istream(data);
    return from_data(istream);
}

bool account_address::from_data(std::istream& stream)
{
    istream_reader source(stream);
    return from_data(source);
}

bool account_address::from_data(reader& source)
{
    reset();
    name = source.read_fixed_string(ADDRESS_NAME_FIX_SIZE);
    prv_key = source.read_fixed_string(ADDRESS_PRV_KEY_FIX_SIZE);
	pub_key = source.read_fixed_string(ADDRESS_PUB_KEY_FIX_SIZE);
    hd_index = source.read_4_bytes_little_endian();
	balance = source.read_8_bytes_little_endian();
	alias = source.read_fixed_string(ADDRESS_ALIAS_FIX_SIZE);
	address = source.read_fixed_string(ADDRESS_ADDRESS_FIX_SIZE);
    return true;	
}

data_chunk account_address::to_data() const
{
    data_chunk data;
    data_sink ostream(data);
    to_data(ostream);
    ostream.flush();
    //BITCOIN_ASSERT(data.size() == serialized_size());
    return data;
}

void account_address::to_data(std::ostream& stream) const
{
    ostream_writer sink(stream);
    to_data(sink);
}

void account_address::to_data(writer& sink) const
{
    sink.write_fixed_string(name, ADDRESS_NAME_FIX_SIZE);
	sink.write_fixed_string(prv_key, ADDRESS_PRV_KEY_FIX_SIZE);
	sink.write_fixed_string(pub_key, ADDRESS_PUB_KEY_FIX_SIZE);
	sink.write_4_bytes_little_endian(hd_index);
	sink.write_8_bytes_little_endian(balance);
	sink.write_fixed_string(alias, ADDRESS_ALIAS_FIX_SIZE);
	sink.write_fixed_string(address, ADDRESS_ADDRESS_FIX_SIZE);
}

uint64_t account_address::serialized_size() const
{
    return name.size() + prv_key.size() + pub_key.size() + 4 + 8 + alias.size() + address.size() + 4 + 4; // 4 "string length" byte
}

std::string account_address::to_string() 
{
    std::ostringstream ss;

    ss << "\t name = " << name << "\n"
		<< "\t prv_key = " << prv_key << "\n"
		<< "\t pub_key = " << pub_key << "\n"
		<< "\t hd_index = " << hd_index << "\n"
		<< "\t balance = " << balance << "\n"
		<< "\t alias = " << alias << "\n"
		<< "\t address = " << address << "\n";

    return ss.str();
}
void account_address::to_json(std::ostream& output) 
{
	minijson::object_writer json_writer(output);
	json_writer.write("name", name);
	json_writer.write("prv_key", prv_key);
	json_writer.write("pub_key", pub_key);
	json_writer.write("hd_index", hd_index);
	json_writer.close();
}

const std::string& account_address::get_name() const
{ 
    return name;
}
void account_address::set_name(const std::string& name)
{ 
     this->name = name;
}

const std::string& account_address::get_prv_key() const
{ 
    return prv_key;
}
void account_address::set_prv_key(const std::string& prv_key)
{ 
     this->prv_key = prv_key;
}

const std::string& account_address::get_pub_key() const
{ 
    return pub_key;
}
void account_address::set_pub_key(const std::string& pub_key)
{ 
     this->pub_key = pub_key;
}

uint32_t account_address::get_hd_index() const
{ 
    return hd_index;
}
void account_address::set_hd_index(uint32_t hd_index)
{ 
     this->hd_index = hd_index;
}

uint64_t account_address::get_balance() const
{ 
    return balance;
}
void account_address::set_balance(uint64_t balance)
{ 
     this->balance = balance;
}

const std::string& account_address::get_alias() const
{ 
    return alias;
}
void account_address::set_alias(const std::string& alias)
{ 
     this->alias = alias;
}

const std::string& account_address::get_address() const
{ 
    return address;
}
void account_address::set_address(const std::string& address)
{ 
     this->address = address;
}

} // namspace chain
} // namspace libbitcoin
