#include "../../stdafx.h"

void CService::Init()
{
	port = 0;
}

CService::CService()
{
	Init();
}

CService::CService(const CNetAddr& cip, unsigned short portIn) : CNetAddr(cip), port(portIn)
{
}

CService::CService(const struct in_addr& ipv4Addr, unsigned short portIn) : CNetAddr(ipv4Addr), port(portIn)
{
}

CService::CService(const struct in6_addr& ipv6Addr, unsigned short portIn) : CNetAddr(ipv6Addr), port(portIn)
{
}

CService::CService(const struct sockaddr_in& addr) : CNetAddr(addr.sin_addr), port(ntohs(addr.sin_port))
{
	assert(addr.sin_family == AF_INET);
}

CService::CService(const struct sockaddr_in6 &addr) : CNetAddr(addr.sin6_addr, addr.sin6_scope_id), port(ntohs(addr.sin6_port))
{
	assert(addr.sin6_family == AF_INET6);
}

CService::CService(const char *caHost, unsigned short usPort)
{
	string strHost = caHost;

	// lets check if it's a *, then it's quite easy
	if (strHost == "*")
	{
		struct in_addr bind_address;
		bind_address.s_addr = INADDR_ANY;
		::CService(bind_address, usPort);
	}
	else
	{
		// now we need to resolve the hostname into an IP address v4 or v6
		struct addrinfo aiHint;
		memset(&aiHint, 0, sizeof(struct addrinfo));

		aiHint.ai_socktype = SOCK_STREAM;
		aiHint.ai_protocol = IPPROTO_TCP;
		aiHint.ai_family = AF_UNSPEC;
#ifdef _WINDOWS
		aiHint.ai_flags = 0;
#else
		aiHint.ai_flags = AI_ADDRCONFIG;
#endif

		struct addrinfo *aiRes = NULL;
		int nErr = getaddrinfo(caHost, NULL, &aiHint, &aiRes);
		if (nErr)
		{
			LOG_ERROR("Error resolving hostname: " + strHost + " - Error No: " + to_string(nErr), "CService");
			throw exception("Error resolving hostname", nErr);
		}

		struct addrinfo *aiTrav = aiRes;
		while( aiTrav != NULL )
		{
			if (aiTrav->ai_family == AF_INET)
			{
				assert(aiTrav->ai_addrlen >= sizeof(sockaddr_in));
				::CService(CNetAddr(((struct sockaddr_in*)(aiTrav->ai_addr))->sin_addr), usPort);
			}

			if (aiTrav->ai_family == AF_INET6)
			{
				assert(aiTrav->ai_addrlen >= sizeof(sockaddr_in6));
				struct sockaddr_in6* s6 = (struct sockaddr_in6*) aiTrav->ai_addr;
				::CService(CNetAddr(s6->sin6_addr, s6->sin6_scope_id), usPort);
			}

			aiTrav = aiTrav->ai_next;
		}

		freeaddrinfo(aiRes);
	}
}

bool CService::SetSockAddr(const struct sockaddr *paddr)
{
	switch (paddr->sa_family) {
	case AF_INET:
		*this = CService(*(const struct sockaddr_in*)paddr);
		return true;
	case AF_INET6:
		*this = CService(*(const struct sockaddr_in6*)paddr);
		return true;
	default:
		return false;
	}
}

unsigned short CService::GetPort() const
{
	return port;
}

bool operator==(const CService& a, const CService& b)
{
	return (CNetAddr)a == (CNetAddr)b && a.port == b.port;
}

bool operator!=(const CService& a, const CService& b)
{
	return (CNetAddr)a != (CNetAddr)b || a.port != b.port;
}

bool operator<(const CService& a, const CService& b)
{
	return (CNetAddr)a < (CNetAddr)b || ((CNetAddr)a == (CNetAddr)b && a.port < b.port);
}

bool CService::GetSockAddr(struct sockaddr* paddr, socklen_t *addrlen) const
{
	if (IsIPv4()) {
		if (*addrlen < (socklen_t)sizeof(struct sockaddr_in))
			return false;
		*addrlen = sizeof(struct sockaddr_in);
		struct sockaddr_in *paddrin = (struct sockaddr_in*)paddr;
		memset(paddrin, 0, *addrlen);
		if (!GetInAddr(&paddrin->sin_addr))
			return false;
		paddrin->sin_family = AF_INET;
		paddrin->sin_port = htons(port);
		return true;
	}
	if (IsIPv6()) {
		if (*addrlen < (socklen_t)sizeof(struct sockaddr_in6))
			return false;
		*addrlen = sizeof(struct sockaddr_in6);
		struct sockaddr_in6 *paddrin6 = (struct sockaddr_in6*)paddr;
		memset(paddrin6, 0, *addrlen);
		if (!GetIn6Addr(&paddrin6->sin6_addr))
			return false;
		paddrin6->sin6_scope_id = scopeId;
		paddrin6->sin6_family = AF_INET6;
		paddrin6->sin6_port = htons(port);
		return true;
	}
	return false;
}

std::vector<unsigned char> CService::GetKey() const
{
	std::vector<unsigned char> vKey;
	vKey.resize(18);
	memcpy(&vKey[0], ip, 16);
	vKey[16] = port / 0x100;
	vKey[17] = port & 0x0FF;
	return vKey;
}

std::string CService::ToStringPort() const
{
	return strprintf("%u", port);
}

std::string CService::ToStringIPPort() const
{
	if (IsIPv4()) {
		return ToStringIP() + ":" + ToStringPort();
	}
	else {
		return "[" + ToStringIP() + "]:" + ToStringPort();
	}
}

std::string CService::ToString() const
{
	return ToStringIPPort();
}

void CService::SetPort(unsigned short portIn)
{
	port = portIn;
}