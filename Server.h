#include "CV.h"
#include "Lock.h"

class Server {
 private:
  Lock lock;
  CV cv;

  int available;
  int alloc[10];
  int maximum[10];
  bool isFinished[10];

 public:
  Server();
  ~Server();

  int getAlloc(int phil);
  int getMax(int phil);

  void setMax(int phil, int value);

  bool isSafe();
  void request(int threadID, int numChops);
  bool wouldBeSafe(int threadID, int numChops);
};
