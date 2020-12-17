#include "CV.h"
#include "Lock.h"

class Tray {
 private:
  Lock lock;
  int numChopsticks;

  CV noChopsticks;
  CV allChopsticks;

  int totalChopsticks;

 public:
  Tray(){};
  ~Tray(){};
  int getNumChopsticks();
  void pickUp();
  void putDown();
  void setTotalChopsticks(int num);
};
