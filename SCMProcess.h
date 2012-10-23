#ifndef SCM_PROCESS_H
#define SCM_PROCESS_H

#include "Arduino.h"

class SCMProcess
{
  private:
  enum State {psSleeping, psRunning, psSuspended};
  typedef unsigned long Milliseconds;

  public:
  SCMProcess() : state(psSuspended) {}
  
  // This is the entry point for the scheduler to execute the process. 
  // The process is designed to start in a suspended state, and hibernate 
  // until a certain number of milliseconds have elapsed. 
  //
  // To respect this functionality, the implementing function in 
  // the descendant should first call this execute() and return immediately if 
  // it returns false. 
  //
  // bool MyProcess::execute() {
  //   if(!SCMProcess::execute()) return false;
  //
  // This function must respect the spirit of cooperatvie multitasking by 
  // returning in a timely manner. 
  virtual bool execute();


  State getState() { return state; }
  
  // Put the process into hibernation until a certain number of milliseconds
  // in the future. 
  // (Being cooperative, this simply means the next time execute() is called,
  // the process will not be able to run.)
  void hibernate(Milliseconds aMilliseconds);

  // The process will start in a suspended, non-running state. 
  // Once the descendant process class has been initialized, call resume().
  //
  // This may also be used for bringing the process out of hibernation.
  // (Being cooperative, this simply means the next time execute() is called,
  // the process will be able to run.)
  void resume();
  
  private:
  State state;

  // Hibernation variables
  Milliseconds startSleep;
  Milliseconds endSleep;

  // Reports if the current time is within the range defined by the hibernation
  // variables. 
  bool isSleepTime();

  // This reports whether the process can run or not. 
  // true = The process can run.
  // false = The process is suspended or hibernating and the wake time 
  // hasn't been reached.  
  bool tryWaking();
};

#endif // SCM_PROCESS_H
