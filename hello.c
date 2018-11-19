#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int interval = 10; //seconds
bool running = true;
int pingCount = 0;
int successCount = 0;
int failCount = 0;

const char testarray[][255] = {
  "127.0.0.1",
  "10.0.7.1",
  "8.8.8.8"
};

void pingHost();

int main(){
  initscr();
  noecho();
  curs_set(FALSE);
  timeout(1000 * 10);

  while(running){
  clear();
  raw();

  printw("Pings: %d\tSucceeded:%d\tFailed:%d\tInterval:%d\tHosts:%d\n", pingCount, successCount, failCount, interval, sizeof(testarray) / sizeof(testarray[0]));

  for (size_t i = 0; i < sizeof(testarray) / sizeof(testarray[0]); i++){
    pingHost(testarray[i]);
  }
  printw("Press 'q' to quit");

  if (getch() == 'q'){
    running = false;
  }
}
  endwin();
  return 0;
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

  start_color();
  init_pair(1, 2, 0);
  init_pair(2, 1, 0);

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
