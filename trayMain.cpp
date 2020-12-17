#include <assert.h>
#include <stdio.h>
#include <time.h>

#include <iostream>

#include "Tray.h"
#include "thread.h"

/**
To compile:
g++ -Wall -D_POSIX_PTHREAD_SEMANTICS trayMain.cpp Tray.cpp CV.cpp Lock.cpp
thread.c -lpthread
*/

void dine(int n);
void wait(int seconds);

#define PHILOSOPHERS 10

thread_t philosophers[PHILOSOPHERS];
int philosophersRequests[PHILOSOPHERS];
int philosophersCurrent[PHILOSOPHERS];

Tray tray;

int main(int argc, char **argv) {
  tray.setTotalChopsticks(
      PHILOSOPHERS);  // Initializing the number of chopsticks in the tray
  srand(time(NULL));  // Seeding the random function
  for (int i = 0; i < PHILOSOPHERS; i++) {  // For each philosopher
    philosophersRequests[i] =
        (rand() % (6 - 1)) + 2;  // Number of chopsticks a philosopher needs
    philosophersCurrent[i] = 0;  // Philosopher i starts with 0 chopsticks

    thread_create(&philosophers[i], &dine,
                  i);  // Philosopher with dine function
    printf("Philosopher %d sat down and will want %d chopsticks.\n", i + 1,
           philosophersRequests[i]);
    int timeWait = (rand() % 20) + 1;  // Random time to wait
    wait(timeWait);                    // Waiting that time
  }
  for (int i = 0; i < PHILOSOPHERS; i++) {  // For each philosopher
    thread_join(philosophers[i]);           // Until each philosopher finishes
  }
  printf("Everyone is finished!\n");
}

/**
Function for philosopher "i" to dine
*/
void dine(int i) {
  int numChop =
      (rand() % (philosophersRequests[i] - philosophersCurrent[i] - 1)) +
      2;  // Getting first number of chopsticks, at least 2
  for (int j = 0; j < numChop; j++) {  // For as many chopsticks as they want
    tray.pickUp();                     // Pick up a single chopstick
    philosophersCurrent[i]++;  // Incriment how many chopsticks they have
  }
  int timeWait = (rand() % 20) + 1;  // Time to wait
  wait(timeWait);                    // Wait
  printf("Philosopher %d is requesting %d chopsticks.\n", i + 1, numChop);
  while (philosophersCurrent[i] <
         philosophersRequests[i]) {  // While they have a need
    numChop = (rand() % (philosophersRequests[i] - philosophersCurrent[i])) +
              1;                         // Get a random number of chopsticks
    for (int j = 0; j < numChop; j++) {  // For each chopstick to be picked up
      tray.pickUp();                     // Pick up a single chopstick
      philosophersCurrent[i]++;  // Incriment how many chopsticks they have
    }
    printf("Philosopher %d is requesting %d chopsticks.\n", i + 1, numChop);
    timeWait = (rand() % 20) + 1;  // Time to wait
    wait(timeWait);                // Waiting
  }                                // After they've gotten all their chopsticks
  for (int j = 0; j < philosophersRequests[i];
       j++) {                  // For each chopscick they requested
    tray.putDown();            // Put down a chopstick
    philosophersCurrent[i]--;  // Decriment how many chopsticks they have
    printf("Philosopher %d is putting down a chopstick.\n", i + 1);
  }
}

void wait(int seconds) {  // Function to wait
  clock_t endwait;
  endwait = clock() + seconds * CLOCKS_PER_SEC;  //
  while (clock() < endwait) {
  }
}