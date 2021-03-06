#pragma once

/*********************************************************************
* Copyright (c) 2017 TCT DEVs	                                     *
* Distributed under the GPLv3.0 software license					 *
* contact us before using our code									 *
**********************************************************************/

#ifndef __MCP03_TXOUTREF_H__
#define __MCP03_TXOUTREF_H__ 1

#include <string>
#include <cstdint>
#include <limits>
#include <boost/serialization/access.hpp>
#include <boost/serialization/version.hpp>
#include "../../uint256.h"

namespace MCP03
{

#define CURRENT_TXOUTREF_VERSION 1

	class txOutRef
	{
		private:
			// required to have serialization overrides
			friend class ::boost::serialization::access;

			// serialization
			template<class Archive>
			void								serialize(Archive &ar, const unsigned int version)
			{
				// note: version is always stored last
				if (version == 1)
					ar & hash & uint16tPos;
			}
	
		public:
											txOutRef();
											~txOutRef();

			uint256							hash;
			uint16_t						uint16tPos;

			// simple getter and setter
			virtual bool					isEmpty() { return (hash.IsNull() && uint16tPos == (std::numeric_limits<uint16_t>::max)()); }
			virtual std::string				toString();
			virtual uint32_t				getSize() { return sizeof(hash) + sizeof(uint16tPos); };

			// operators
			virtual bool					operator<(const txOutRef& ref);
			virtual bool					operator==(const txOutRef& ref);
			virtual bool					operator!=(const txOutRef& ref);
	};
}

BOOST_CLASS_VERSION(MCP03::txOutRef, CURRENT_TXOUTREF_VERSION)

#endif