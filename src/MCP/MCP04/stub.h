#pragma once

/*********************************************************************
* Copyright (c) 2017 TCT DEVs	                                     *
* Distributed under the GPLv3.0 software license					 *
* contact us before using our code									 *
**********************************************************************/

#ifndef __MCP04_STUB_H__
#define __MCP04_STUB_H__ 1

#include "ChainInterface.h"
#include "../MCP03/Transaction.h"
#include "../MCP03/Block.h"

namespace MCP04
{
	// this is just a stub for reference
	// if you want to build your own proof of process, use this stub as a template
	// never use this class directly
#error Never include stub.h directly or use it!

	class STUB : public ChainInterface
	{
		protected:
			static const std::string			m_strName;

		public:
			static bool							registerFactory() { return ::MetaChain::getInstance().getStorageManager()->getSubChainManager()->registerFactory(m_strName, &createInstance); };
			static ChainInterface				*createInstance() { return new STUB(); };
	};
}
#endif