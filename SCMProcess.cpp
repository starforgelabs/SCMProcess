#include "Arduino.h"
#include <SCMProcess.h>

bool SCMProcess::execute()
{
  return tryWaking();
}

void SCMProcess::hibernate(Milliseconds aMilliseconds)
{
  if(aMilliseconds == 0) return;

  Milliseconds startSleep = millis();
  endSleep = startSleep + aMilliseconds;

  state = psSleeping;
}

bool SCMProcess::isSleepTime()
{
  Milliseconds now = millis();

  // millis() wraps every 50 days. 
  if(endSleep > startSleep) 
    // Hibernation time doesn't wrap around to zero.
    return (now >= startSleep) && (now < endSleep);
  else
    // Hibernationt time wraps around to zero.
    return (now >= startSleep) || (now < endSleep);
}

void SCMProcess::resume()
{
  state = psRunning;
}
  
bool SCMProcess::tryWaking()
{
  switch(state)
  {
    case psSuspended: return false;
    case psRunning: return true;
    case psSleeping:
      if(!isSleepTime()) state = psRunning;
      return state == psRunning;
  }
}

