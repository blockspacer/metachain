#pragma once

/*
this is the class to handle IP IOs
it's developed as template for better reusability. the template classes used need to have certain functions implemented for correct reading and writing of the contents
*/

template <class T>
class ipContainer
{
	private:
		string									m_strFileName;

	public:
												ipContainer();
												ipContainer(string strFileName);
												~ipContainer();

		void									setFileName(string strFileName) { m_strFileName = strFileName; };
		void									readContents();		
		void									writeContents();
		bool									entryExists(T *obj);

		// this vector holds all the IP information corresponding to the template class
		list< T >								lstIP;
};

/*
Template class implementation
*/
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

template <class T>
ipContainer<T>::ipContainer()
{
}

template <class T>
ipContainer<T>::ipContainer(string strFileName) :
	m_strFileName(strFileName)
{
}

template <class T>
ipContainer<T>::~ipContainer()
{

}

template <class T>
void ipContainer<T>::readContents()
{
	ifstream streamFile(m_strFileName);
#ifdef _DEBUG
	LOG_DEBUG("reading contents of file: " + m_strFileName, "IPC");
#endif

	for (string strLine; getline(streamFile, strLine); )
	{
		// renmove unwanted spaces from beginning and end
		boost::trim(strLine);

		// don't process empty lines
		if (strLine.length() == 0 || strLine.empty())
			continue;

		// if the first character is a # it's a comment and we skip this line
		if (strLine[0] == '#')
			continue;

#ifdef _DEBUG
		LOG_DEBUG("line content: " + strLine, "IPC");
#endif

		T tmp;
		if (tmp.init(strLine))
		{
			// we check for doubletts, sice we dont want that
			if (find(lstIP.begin(), lstIP.end(), tmp) != lstIP.end())
#ifdef _DEBUG
			{
				LOG_DEBUG("removing doublett: " + strLine, "IPC");
#endif
				continue;
#ifdef _DEBUG
			}
#endif
			lstIP.push_back(tmp);
		}
		else
		{
			LOG_ERROR("unable to add the following IP into our system: " + strLine, "IPC");
			continue;
		}
	}

#ifdef _DEBUG
	LOG_DEBUG("done reading contents of file: " + m_strFileName, "IPC");
	LOG_DEBUG("elements in the vector: " + to_string(lstIP.size()), "IPC");
#endif
}

template <class T>
void ipContainer<T>::writeContents()
{
#ifdef _DEBUG
	LOG_DEBUG("writing contents to file: " + m_strFileName, "IPC");
#endif

	ofstream streamOut;
	streamOut.open(m_strFileName, ios_base::out | ios_base::trunc);

	// create a date/time string
	basic_stringstream<char> wss;
	wss.imbue(locale(wcout.getloc(), new boost::posix_time::wtime_facet(L"%Y.%m.%d %H:%M:%S")));
	wss << boost::posix_time::second_clock::universal_time();

	// write standard header
	streamOut << "# generation of the file: " << wss.str() << endl;
	streamOut << "# it will be automatically updated through the TCT blockchain" << endl;
	streamOut << "# any manual changes will be overridden!" << endl << endl;

	for (list<T>::iterator it = lstIP.begin(); it != lstIP.end(); it++)
		streamOut << it->toString() << endl;

	streamOut.close();

#ifdef _DEBUG
	LOG_DEBUG("done writing contents to file: " + m_strFileName, "IPC");
#endif
}

template <class T>
bool ipContainer<T>::entryExists(T *obj)
{
	for (list<T>::iterator it = lstIP.begin(); it != lstIP.end(); it++)
	{
		if (*it == *obj)
			return true;
	}
	return false;
}