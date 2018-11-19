#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

int interval = 120;
bool running = true;
int count = 0;

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

  while(running){
  clear();
  raw();

  printw("Count: %d\n", count);

  for (size_t i = 0; i < sizeof(testarray) / sizeof(testarray[0]); i++){
    pingHost(testarray[i]);
  }
  printw("Press 'q' to quit");
  count++;

  if (getch() == 'q'){
    running = false;
  }
}
  endwin();
  return 0;
}

void pingHost(char host[]){
  char cmd[255] = "ping -q -c 1 ";
  char hst[255] = "10.0.7.2";
  char dst[255] = " > /dev/null";

  strcat(cmd, host);
  strcat(cmd, dst);

  start_color();
  init_pair(1, 2, 7);
  init_pair(2, 1, 0);

  int ret = system(cmd);
  if (ret == 0){
    attron(COLOR_PAIR(1));
    printw("%s:\tSuccess, %d\t\t\n", host, ret);
    attroff(COLOR_PAIR(1));
  }else {
    attron(COLOR_PAIR(2));
    printw("%s:\tError\n", host);
    attroff(COLOR_PAIR(2));
  }
}
