#pragma once

// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>

class CSemaphore
{
private:
	boost::condition_variable condition;
	boost::mutex mutex;
	int value;

public:
	CSemaphore(int init) : value(init) {}

	void wait()
	{
		boost::unique_lock<boost::mutex> lock(mutex);
		while (value < 1) {
			condition.wait(lock);
		}
		value--;
	}

	bool try_wait()
	{
		boost::unique_lock<boost::mutex> lock(mutex);
		if (value < 1)
			return false;
		value--;
		return true;
	}

	void post()
	{
		{
			boost::unique_lock<boost::mutex> lock(mutex);
			value++;
		}
		condition.notify_one();
	}
};

/** RAII-style semaphore lock */
class CSemaphoreGrant
{
private:
	CSemaphore* sem;
	bool fHaveGrant;

public:
	void Acquire()
	{
		if (fHaveGrant)
			return;
		sem->wait();
		fHaveGrant = true;
	}

	void Release()
	{
		if (!fHaveGrant)
			return;
		sem->post();
		fHaveGrant = false;
	}

	bool TryAcquire()
	{
		if (!fHaveGrant && sem->try_wait())
			fHaveGrant = true;
		return fHaveGrant;
	}

	void MoveTo(CSemaphoreGrant& grant)
	{
		grant.Release();
		grant.sem = sem;
		grant.fHaveGrant = fHaveGrant;
		fHaveGrant = false;
	}

	CSemaphoreGrant() : sem(NULL), fHaveGrant(false) {}

	CSemaphoreGrant(CSemaphore& sema, bool fTry = false) : sem(&sema), fHaveGrant(false)
	{
		if (fTry)
			TryAcquire();
		else
			Acquire();
	}

	~CSemaphoreGrant()
	{
		Release();
	}

	operator bool()
	{
		return fHaveGrant;
	}
};