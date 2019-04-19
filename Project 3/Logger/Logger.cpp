/////////////////////////////////////////////////////////////////////
// Logger.cpp - log text messages to std::ostream                  //
// ver 1.0                                                         //
//-----------------------------------------------------------------//
// Jim Fawcett (c) copyright 2015                                  //
// All rights granted provided this copyright notice is retained   //
//-----------------------------------------------------------------//
// Language:    C++, Visual Studio 2015                            //
// Application: Several Projects, CSE687 - Object Oriented Design  //
// Author:      Ammar Salman, Syracuse University                  //
//              assalman@syr.edu                                   //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////

#include <functional>
#include <fstream>
#include <windows.h>
#include "Logger.h"
#include "../Utilities/StringUtilities/StringUtilities.h"

using namespace Logging;

//----< send text message to std::ostream >--------------------------

void Logger::write(const std::string& msg)
{
  if(_ThreadRunning)
    _queue.enQ(msg);
}
void Logger::title(const std::string& msg, char underline)
{
  std::string temp = "\n  " + msg + "\n " + std::string(msg.size() + 2, underline);
  //write(temp);
}
//----< attach logger to existing std::ostream >---------------------

bool Logger::attach(std::ostream* pOut) 
{ 
  if (_ThreadRunning) return false;
  _streams.push_back(pOut); 
  return true;
}

//-----< detach existing logger to stop writing >--------------------
bool Logger::detach(std::ostream * pOut)
{
  if (_ThreadRunning) return false;
	auto stream = std::find(_streams.begin(), _streams.end(), pOut);
	if(stream != _streams.end())
		_streams.erase(stream);
  return true;
}
//----< start logging >----------------------------------------------

void Logger::start()
{
  if (_ThreadRunning)
    return;
  _ThreadRunning = true;
  std::function<void()> tp = [=]() {
    while (true)
    {
      std::string msg = _queue.deQ();
      if (msg == "quit")
      {
        _ThreadRunning = false;
        break;
      }
	  for (auto stream : _streams)
		  *stream << msg;
    }
  };
  std::thread thr(tp);
  thr.detach();
}
//----< stop logging >-----------------------------------------------

void Logger::stop(const std::string& msg)
{
  if (_ThreadRunning)
  {
    if(msg != "")
      write(msg);
    write("quit");  // request thread to stop
    while (_ThreadRunning)
      /* wait for thread to stop*/
      ;
  }
}
//----< has logger been started? >-----------------------------------

bool Logger::running()
{
	return _ThreadRunning;
}
void Logger::pause(bool doPause)
{
	if (doPause)
	{
		_Paused = true;
		::SuspendThread(_pThr->native_handle());
	}
	else
	{
		_Paused = false;
		::ResumeThread(_pThr->native_handle());
	}
}
//----< is logger currently paused? >--------------------------------

bool Logger::paused()
{
	return _Paused;
}
//----< wait for logging to empty input queue >----------------------

void Logger::flush()
{
	if (_ThreadRunning && !_Paused)
	{
		while (_queue.size() > 0)
			;
		for (auto pStream : streams_)
			pStream->flush();
	}
}
//----< wait for logger to finish >----------------------------------

void Logger::wait()
{
	if (_ThreadRunning && _pThr->joinable())
		_pThr->join();
}
//----< stop logging thread >----------------------------------------

Logger::~Logger()
{
  stop(); 
}

struct Cosmetic
{
  ~Cosmetic() { std::cout << "\n\n"; }
} cosmetic;

#ifdef TEST_LOGGER

//using Util = Utilities;

int main()
{
  //Util::Title("Testing Logger Class");
  Logger log;
  log.attach(&std::cout);
  log.write("\n  won't get logged - not started yet");
  log.start();
  log.title("Testing Logger Class", '=');
  log.write("\n  one");
  log.write("\n  two");
  log.write("\n  fini");
  log.stop();
  log.write("\n  won't get logged - stopped");
  log.start();
  log.write("\n  starting again");
  log.write("\n  and stopping again");
  log.stop("\n  terminating now");

  StaticLogger<1>::attach(&std::cout);
  StaticLogger<1>::start();
  StaticLogger<1>::write("\n");
  StaticLogger<1>::title("Testing StaticLogger class");
  StaticLogger<1>::write("\n  static logger at work");
  Logger& logger = StaticLogger<1>::instance();
  logger.write("\n  static logger still at work");
  logger.stop("\n  stopping static logger");
}

#endif
