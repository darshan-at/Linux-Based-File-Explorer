#include <bits/stdc++.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>

#define deb(x) cout<<#x<<" is "<<x<<endl;
#define deb1(x,y) cout<<x<<" "<<y<<endl;
#define CLEAR_SCREEN cout<<"\033[2J\033[1;1H";  //clears the screen
#define jump(x,y) printf("%c[%d;%dH",27,x,y);
using namespace std;

void print_list(vector<string>,int,int);
void enableRAW(char *);
int check_dir(string);
void list_dir(char *);
void display_info(string);
long long get_Dir_size(char *);
string human_readable(long long);
void looping(int,int);
int command_mode();

string split_command();
