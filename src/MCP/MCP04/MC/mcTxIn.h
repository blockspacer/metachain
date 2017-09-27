#pragma once

/*********************************************************************
* Copyright (c) 2017 TCT DEVs	                                     *
* Distributed under the GPLv3.0 software license					 *
* contact us before using our code									 *
**********************************************************************/

#ifndef __MCP04_MC_TXIN_H__
#define __MCP04_MC_TXIN_H__ 1

#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/version.hpp>
#include "../../MCP03/txIn.h"
#include "../../MCp03/txOutRef.h"
#include "mcActions.h"

namespace MCP04
{
	namespace MetaChain
	{

#define CURRENT_MC_TXIN_VERSION 1

		class mcTxIn : public MCP03::txIn
		{
			private:
				// required to have serialization overrides
				friend class ::boost::serialization::access;

				// serialization
				template<class Archive>
				void											save(Archive &ar, const unsigned int version) const
				{
					// note: version is always stored last
					if (version == 1)
					{
						//ar << uint16tVersion << eAction;
						switch (eAction)
						{
							case ACTION::CREATE_SUBCHAIN:		ar << *(createSubchain*)pPayload;		break;
						}
					}
				}
				template<class Archive>
				void											load(Archive &ar, const unsigned int version) const
				{
					// note: version is always stored last
					if (version == 1)
						ar << txPrev << strSignature;
				}
				BOOST_SERIALIZATION_SPLIT_MEMBER()

			public:
				// enum that defines the action of this tx
				enum ACTION
				{
					CREATE_SUBCHAIN = 1
				};

																mcTxIn(uint16_t Version = CURRENT_MC_TXIN_VERSION);
																mcTxIn(uint16_t Version, ACTION eAction );
																~mcTxIn();


				// variables
				uint16_t										uint16tVersion;
				ACTION											eAction;
				void *											pPayload;

				// we don't have a previons tx in the MC scheme since we're not having in and outgoing tx. we have single commands that get validated
				const std::shared_ptr<MCP03::txOutRef>			txPrev = nullptr;

				// getter
				std::string										toString();
				uint32_t										getSize();
		};
	}
}

BOOST_CLASS_VERSION(MCP04::MetaChain::mcTxIn, CURRENT_MC_TXIN_VERSION)
#endif