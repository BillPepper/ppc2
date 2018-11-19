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

  int ret = system(cmd);
  if (ret == 0){
    init_color(COLOR_RED, 700, 0, 0);
    printw("%s:\tSuccess, %d\n", host, ret);
  }else {
    printw("%s:\tError\n", host);
  }
}
