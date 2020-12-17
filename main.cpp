#include <assert.h>
#include <stdio.h>
#include <time.h>

#include <iostream>

#include "Server.h"
#include "thread.h"

/**
To compile:
g++ -Wall -D_POSIX_PTHREAD_SEMANTICS main.cpp Server.cpp CV.cpp Lock.cpp
thread.c -lpthread
*/

void dine(int n);
void wait(int seconds);

#define PHILOSOPHERS 10
#define CHOPSTICKS 10
// Tray code
// Tray sticks[1];

thread_t philosophers[PHILOSOPHERS];
int philosophersRequests[PHILOSOPHERS];
int philosophersCurrent[PHILOSOPHERS];

// Server code
Server *server;

int main(int argc, char **argv) {
  // Server code
  server = (Server *)server;
  server = new Server();

  // Tray code
  // sticks[0].setTotalChopsticks(CHOPSTICKS);

  srand(time(NULL));
  for (int i = 0; i < 10; i++) {  // For each philosopher
    philosophersRequests[i] =
        (rand() % (9 - 1)) + 2;  // Number of chopsticks a philosopher
                                 // will request between [2,10]
    philosophersCurrent[i] = 0;  // Philosopher i starts with 0 chopsticks
    printf(
        "Philosopher %d sat at the table, and want at most %d chopsticks for "
        "this meal.\n",
        i + 1, philosophersRequests[i]);

    thread_create(&philosophers[i], &dine,
                  i);  // Creating threads with dine function
  }
  for (int i = 0; i < 10; i++) {  // For each philosopher
    thread_join(
        philosophers[i]);  // Until each philosopher completes their dine
  }
  printf("Everyone is finished!\n");
}

/**
Function for philosopher "i" to dine
*/
void dine(int i) {
  server->setMax(i, philosophersRequests[i]);  // Telling the server the max

  // Getting chopsticks
  while (server->getAlloc(i) < server->getMax(i)) {
    int numChop = (rand() % (server->getMax(i) - server->getAlloc(i))) +
                  1;  // Random number from 1 to (max - current)
    server->request(i, numChop);
  }
}

void wait(int seconds) {
  clock_t endwait;
  endwait = clock() + seconds * CLOCKS_PER_SEC;
  while (clock() < endwait) {
  }
}