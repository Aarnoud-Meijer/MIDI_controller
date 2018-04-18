#ifndef ANALOG_h_
#define ANALOG_h_

#include "Arduino.h"
#include "../Settings/Settings.h"
#include "MIDI_Control_Element.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"

#if defined(__arm__)
#include <functional>
#endif

class Analog : public MIDI_Control_Element
{
public:
  Analog(pin_t analogPin, uint8_t controllerNumber, uint8_t channel); // Constructor
#if defined(__arm__)
  void map(std::function<int(int)> fn);
#else
  void map(int (*fn)(int));                                           // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()
#endif

private:
  void refresh(); // Read the analog input value, update the average, map it to a MIDI value, check if it changed since last time, if so, send Control Change message over MIDI

  pin_t analogPin;
  uint8_t controllerNumber, channel, oldVal = -1;

#if defined(__arm__)
  std::function<int(int)> analogMap = [](int raw){ return raw; };
#else
  int (*analogMap)(int) = identity; // function pointer to identity function f(x) → x
#endif

  static int identity(int x)
  { // identity function f(x) → x
    return x;
  }

#ifdef SINGLE_BYTE_AVERAGE
  uint8_t runningAverage(uint8_t value); // http://playground.arduino.cc/Main/RunningAverage
  uint8_t avValues[ANALOG_AVERAGE] = {};
  uint8_t avIndex = 0;
  unsigned int avSum = 0;
  uint8_t avCount = 0;
#else
  unsigned int runningAverage(unsigned int value); // http://playground.arduino.cc/Main/RunningAverage
  unsigned int avValues[ANALOG_AVERAGE] = {};
  size_t avIndex = 0;
  unsigned long avSum = 0;
  size_t avCount = 0;
#endif
};

#endif // ANALOG_h_
