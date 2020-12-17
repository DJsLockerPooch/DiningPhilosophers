#include "Tray.h"

#include <stdio.h>
/**
 * Function to return the number of chopsticks are in the tray
 * */
int Tray::getNumChopsticks() { return numChopsticks; }

/**
 * Function to pick up a single chopstick (or wait if none are available)
 * */
void Tray::pickUp() {
  lock.acquire();               // Acquire the lock
  while (numChopsticks == 0) {  // If the number of chopsticks is 0
    noChopsticks.wait(&lock);   // Their are no chopsticks, so wait
  }                             // After there is a chopstick available
  numChopsticks--;              // Decriment by one chopstick
  allChopsticks.broadcast();    // Broadcast that a chopstick is available
  lock.release();               // Release the lock
}

/**
 * Function to put down a chopstick (or wait if all are already down)
 * */
void Tray::putDown() {
  lock.acquire();  // Acquire the lock
  while (numChopsticks ==
         totalChopsticks) {     // While no chopsticks are picked up
    allChopsticks.wait(&lock);  // All are in the tray, so wait
  }                             // After one has been picked up
  numChopsticks++;              // Incriment how many chopsticks in the tray
  noChopsticks.broadcast();     // Broadcast a chopstick is available
  lock.release();               // Release the lock
}

/**
 * Function to set the initial number of chopsticks at the start and to give
 * */
void Tray::setTotalChopsticks(int num) {
  numChopsticks = num;
  totalChopsticks = num;
}
