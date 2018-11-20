#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ppc2.h"

int interval = 10; //seconds
bool running = true;
int pingCount = 0;
int successCount = 0;
int failCount = 0;
int termLenght = 100;

const char testarray[][255] = {
  "127.0.0.1",
  "10.0.7.1",
  "8.8.8.8"
  //,"10.0.7.188"
};

int main(){
  initscr();
  noecho();
  curs_set(FALSE);
  timeout(1000 * 10);

  start_color();
  init_pair(1, 2, 0);
  init_pair(2, 1, 0);
  init_pair(3, 4, 0);

  while(running){
    clear();
    raw();

    printw("Persistent Ping Check 2\n");
    printSeperator();
    printPingStatistics();
    printSeperator();

    for (size_t i = 0; i < sizeof(testarray) / sizeof(testarray[0]); i++){
      pingHost(testarray[i]);
    }
  
    printSeperator();
    printw("Problem Hosts: -1\n");
    printw("Deactivated Hosts: -1\n");
    printSeperator();
    printw("Press '?' for help or 'q' to quit");

    if (getch() == 'q'){
     running = false;
    }
  }
  endwin();
  return 0;
}

void printPingStatistics(){
  attron(COLOR_PAIR(3));
  printw("Pings: %d\t", pingCount);
  attroff(COLOR_PAIR(3));

  attron(COLOR_PAIR(1));
  printw("Succeeded: %d\t", successCount);
  attroff(COLOR_PAIR(1));

  attron(COLOR_PAIR(2));
  printw("Failed: %d\t", failCount);
  attroff(COLOR_PAIR(2));

  attron(COLOR_PAIR(3));
  printw("Interval: %d\tHosts: %d\n", interval, sizeof(testarray) / sizeof(testarray[0]));
  attroff(COLOR_PAIR(3));
}

void printSeperator(){
  for (int i = 0; i < termLenght;i++){
    printw("-");
  }  
  printw("\n");
}

int getHostIndex(char host[]){
  for (size_t i = 0; i < sizeof(testarray) / sizeof(testarray[0]); i++){
    if (testarray[i] == host){
      return i;
    } 
  }
  return -1;
}

void pingHost(char host[]){
  char cmd[255] = "ping -q -c 1 ";
  char dst[255] = " > /dev/null";

  strcat(cmd, host);
  strcat(cmd, dst);

  pingCount++;

  int ret = system(cmd);
  if (ret == 0){
    successCount++;
    attron(COLOR_PAIR(1));
    printw("%d\t%s:\tSuccess, %d\t\t\n", getHostIndex(host) +1, host, ret);
    attroff(COLOR_PAIR(1));
  }else {
    failCount++;
    attron(COLOR_PAIR(2));
    printw("%d\t%s:\tError\n", getHostIndex(host) +1, host);
    attroff(COLOR_PAIR(2));
  }
}
