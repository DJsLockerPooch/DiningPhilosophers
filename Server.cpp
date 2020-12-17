#include "Server.h"

#include <time.h>

#include <iostream>

/**
Function to return the phil'th allocation int
*/
int Server::getAlloc(int phil) {
  return alloc[phil];  // Return philosopher "phil"'s allocated resources
}

/**
Function to get the maximum number of chopsticks philosopher "phil" could
request
*/
int Server::getMax(int phil) {
  return maximum[phil];  // Returning the maximum value of philosopher "phil"
}

/**
Function to set the maximum number of chopsticks (value) philosopher "phil" will
request
*/
void Server::setMax(int phil, int value) {
  maximum[phil] = value;  // Set philosopher "phil"'s maximum value to "value"
}

/**
Function to determine and return if it is safe to continue
*/
bool Server::isSafe() {
  int toBeAvailable = available;   // Copy of available in Server
  int need[10];                    // How many each philosopher currently has
  bool finish[10];                 // If each philosopher can complete
  for (int i = 0; i < 10; i++) {   // For each philosopher
    if (isFinished[i] == false) {  // If the philosopher hasn't finished
      need[i] = maximum[i] - alloc[i];  // Get their need
    } else {                            // Otherwise
      need[i] = 0;                      //  They don't have a need anymore
    }
    finish[i] = false;  // Setting each as false
  }
  while (true) {            // Until we hit a return statement
    int philComplete = -1;  // Philosopher number whose need can be fulfilled
    for (int j = 0; j < 10; j++) {  // For each philosopher
      if ((finish[j] == false) && (need[j] <= toBeAvailable) &&
          (isFinished[j] == false)) {  // Philosopher's need
                                       // can be fulfilled
        philComplete = j;              // Take note of that philosophers number
      }
    }
    if (philComplete == -1) {  // If no philosopher's need can be fulfilled
      for (int i = 0; i < 10; i++) {  // Go through each philosopher
        if (finish[i] == false) {     // If any is false (we still have a need)
          if (isFinished[i] == 0) {   // Philosopher hasn't previously finished
            return false;             // Return false
          }
        }
      }
      return true;                  // Otherwise all philosopher's needs are met
    } else {                        // If a philosopher can finish
      finish[philComplete] = true;  // Philosopher has finished
      toBeAvailable += alloc[philComplete];  // Reallocate the chopsticks
    }
  }
}

/**
Function to request if philosopher "threadID" is safe to request their given
amount of chopsticks
*/
void Server::request(int threadID, int numChops) {
  lock.acquire();    // Acquire the lock
  assert(isSafe());  // Assert if it's safe
  printf("Philosopher %d requested %d out of %d chopsticks.\n", threadID + 1,
         numChops, maximum[threadID]);
  while (!wouldBeSafe(threadID, numChops)) {  // While it's not safe to proceed
    cv.wait(&lock);                           // Wait
  }
  if ((available - numChops) >= 0) {
    alloc[threadID] += numChops;  // Because at this point we're sure
                                  // it's safe, remove the resources
    available -= numChops;        // Remove the resources
  }
  assert(isSafe());  // Assert if it's safe again
  if (maximum[threadID] ==
      alloc[threadID]) {  // If the allocated # of chopsticks equals the max
                          // they requested
    available += maximum[threadID];  // Allocate their chopsticks
    isFinished[threadID] = true;     // They are finished with the meal
    cv.broadcast();                  // Broadcast they have completed
    printf("Philosopher %d is stuffed. They don't need anymore chopsticks.\n",
           threadID + 1);
  }
  lock.release();  // Release the lock
}

/**
Function to determine and return if the request will be safe or not
*/
bool Server::wouldBeSafe(int threadID, int numChops) {
  bool result = false;          // Starting with false
  available -= numChops;        // Removing the available resources
  alloc[threadID] += numChops;  // Reallocating the resources
  if (isSafe()) {               // Seeing if this is a safe state
    result = true;              // Because it is, we will reutrn true
  }
  available += numChops;  // Negating the removal of resources
  alloc[threadID] -=
      numChops;   // Negating the addition of reallocated resources
  return result;  // Returning the state of the request
}

Server::Server() {
  srand(time(NULL));  // Setting a seed for the rand() function
  available = 10;     // Initializing the number of available chopsticks
  for (int i = 0; i < 10; i++) {  // For each philosopher
    alloc[i] = 0;                 // They don't have any chopsticks to start
    isFinished[i] = false;        // None have finished their meal
  }
}