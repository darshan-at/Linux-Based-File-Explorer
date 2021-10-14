#include "header.h"

vector<string>directories;
vector<char>command_input;
stack<string>back,forward_stack;
vector<string>arguments;
struct termios original_raw,new_raw;
struct winsize window_size;
int currentWindowRow,currentWindowWidth;
string curr_dir;
string home;
string my_command;

int first,last=25;
int x,y;
int ARROW_KEY = 27;
int UP_KEY = 65;
int DOWN_KEY=66;
int ENTER=10;
int BACKSPACE=127;
int RIGHT_KEY=67;
int LEFT_KEY=68;
	
int main()
{
	
	CLEAR_SCREEN;
	char pwd[256];
	getcwd(pwd,256);	//get present working dir
	curr_dir=pwd;
	home=curr_dir;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &window_size);	//get Window Size
	currentWindowRow=window_size.ws_row;
	currentWindowWidth=window_size.ws_col;
	//enter into RAW MODE
	char *temp;
	temp=&pwd[0];
	enableRAW(temp);
	CLEAR_SCREEN;
	return 0;
}

void list_dir(char *pwd)
{
	/*first=0;
	last=25;*/

	// Start
	CLEAR_SCREEN;
	
	directories.clear();

	struct dirent *entry;
	
	DIR *dir=opendir(pwd);
	if(dir==NULL)
	{	
		perror("dir");
	}
	else
	{
		
		while ((entry = readdir(dir))!=NULL)
		{	
			string s=curr_dir+"/"+(entry->d_name);
			
			directories.push_back(s);	
		}	
			
	}
	sort(directories.begin(),directories.end());
	
	//End
	/*last=min(min(window_size.ws_row-3,(int)directories.size()),last);
	
	print_list();*/
	closedir(dir);
}
void print_list(vector<string>directories,int s,int e)
{
	
	CLEAR_SCREEN;
	x=1;
	y=0;
	jump(x,y);
	
	for(int i=s;i<e;i++)
	{
		//char *entry=&directories[0];
		//long long dir_size=get_dir_size(entry);
		
		display_info(directories[i]);
	}
	x=1;
	y=0;
	jump(x,y);
}
long long get_Dir_size(char *dir_path)
{
	long long sum=0;

	struct dirent *entry;
	DIR *dir=opendir(dir_path);

	if(dir==NULL)
	{		
		perror(dir_path);
		EXIT_FAILURE;
	}
	else
	{
		struct stat fileInfo;
		stat(dir_path,&fileInfo);
		sum=sum+fileInfo.st_size;
		while ((entry = readdir(dir))!=NULL)
		{
			if(!strcmp(entry->d_name,"."))
			continue;
			else if(!strcmp(entry->d_name,".."))
			continue;
			char subdir[513];
			sprintf(subdir, "%s/%s",dir_path,entry->d_name);
			
			stat(subdir,&fileInfo);
			
			if(S_ISDIR(fileInfo.st_mode))
			{
				sum=sum+get_Dir_size(subdir);
			}
			else
			{
				sum=sum+fileInfo.st_size;
			}
			
		}	
			
	}
	closedir(dir);
	return sum;
}
string human_readable(long long size_bytes)
{
	float q=size_bytes;
	string a[]={"B","K","M","G","T","P","E","Z","Y"};
	int i,j=0;
	
	for(i=0;(i<=9 && q>=1024);q/=1024)
	{
		i++;
	}
	string t=to_string(q);
	for(j=0;j<t.length();j++)
	{
		if(t[j]=='.')
		{
			break;
		}
	}
	t=t.substr(0,j+2)+a[i];
	return t;
}
void display_info(string item)
{
	char *file_path;
	string temp;
	temp=item;
	file_path=&temp[0];
	struct stat fileInfo;
	stat(file_path,&fileInfo);
	int i,j=0;
	string permission="";
	
	long long size_bytes=fileInfo.st_size;
	for(i=item.length()-1;i>=0;i--)
	{
		if(item[i]=='/')
		{
			break;
		}
		j++;
	}
	item=item.substr(i+1,j);
	if(check_dir(temp))
	{
		permission.append("d");
		if(item==".")
		{}
		else if(item=="..")
		{}
		else{
			//cout<<file_path;
			size_bytes=get_Dir_size(file_path);
		}
	}
	else
	{
		permission.append("-");
	}
	string size_string=human_readable(size_bytes);
	
	if(fileInfo.st_mode & S_IRUSR)
	{
		permission.append("r");
	}
	else{
		permission.append("-");
	}
	if(fileInfo.st_mode & S_IWUSR)
	{
		permission.append("w");
	}
	else{
		permission.append("-");
	}
	if(fileInfo.st_mode & S_IXUSR)
	{
		permission.append("x");
	}
	else{
		permission.append("-");
	}
	if(fileInfo.st_mode & S_IRGRP)
	{
		permission.append("r");
	}
	else{
		permission.append("-");
	}
	if(fileInfo.st_mode & S_IWGRP)
	{
		permission.append("w");
	}
	else{
		permission.append("-");
	}
	if(fileInfo.st_mode & S_IXGRP)
	{
		permission.append("x");
	}
	else{
		permission.append("-");
	}
	if(fileInfo.st_mode & S_IROTH)
	{
		permission.append("r");
	}
	else{
		permission.append("-");
	}
	if(fileInfo.st_mode & S_IWOTH)
	{
		permission.append("w");
	}
	else{
		permission.append("-");
	}
	if(fileInfo.st_mode & S_IXOTH)
	{
		permission.append("x");
	}
	else{
		permission.append("-");
	}

	struct passwd *owner=getpwuid(fileInfo.st_uid);
	struct group *gr=getgrgid(fileInfo.st_gid);
	string o_name="",g_name="";
	if(owner!=0)
	{
		o_name=owner->pw_name;
	}
	if(gr!=0)
	{
		g_name=gr->gr_name;
	}

	
	if(item.length()>13)
	{
		string t;
		item=item.substr(0,13);
		item=item+"...";
	}
	else
	{
		item.resize(21,' ');
	}
	string mod_time=ctime(&fileInfo.st_mtime);
	size_string.resize(7,' ');
	cout<<item<<'\t'<<permission<<'\t'<<o_name<<"\t"<<g_name<<"\t"<<size_string<<"\t"<<mod_time;
	
}
void disableRAW()
{
	tcsetattr(STDIN_FILENO,TCSAFLUSH,&original_raw);
	CLEAR_SCREEN;
}
//Entering NON-CANONICAL Mode
void enableRAW(char *pwd)
{
	//list all directories with metadata
	list_dir(pwd);
	first=0;
	last=min(currentWindowRow-3,(int)directories.size());
	//Print the directories
	print_list(directories,first,last);
	//to get original setting
	tcgetattr(STDIN_FILENO,&new_raw);
	original_raw=new_raw;
	atexit(disableRAW);
	new_raw.c_lflag &=~ECHO;	//turning off the reflection of typed key on screen
	new_raw.c_lflag &=~ICANON;	//Turning off the Canonical Mode
	if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&new_raw)!=0)
	{
		fprintf(stderr,"Fail to achieve desired attribute");
	}
	looping(first,last);
	
		
	
}

void looping(int first,int last)
{	/*------------------------------LOOPING IN RAW MODE----------------------------*/
	unsigned char c;
	//int curpos=0;
	jump(1,0);
	
	y=0;
	int curpos=1;
	fflush(0);
	while(true)
	{
		cin.clear();
		ioctl(STDIN_FILENO, TIOCGWINSZ, &window_size);	//get Window Size
		if(window_size.ws_row!=currentWindowRow || window_size.ws_col!=currentWindowWidth)
		{
			char *entry=&curr_dir[0];
			currentWindowRow=window_size.ws_row;
			currentWindowWidth=window_size.ws_col;
			list_dir(entry);
			last=min(currentWindowRow-3,(int)directories.size());
			print_list(directories,0,last);
			curpos=1;
			first=0;
			continue;
		}
		jump(currentWindowRow-2,0);
		printf("Normal Mode");
		jump(curpos,y);
		c=cin.get();
		if(c==ARROW_KEY)
		{
			c=cin.get();
			c=cin.get();
			if(c==DOWN_KEY)
			{
				curpos++;
				//first++;
				//cout<<first+curpos;
				jump(curpos,y);
				
				if(first+curpos>last)
				{
					curpos--;
					//first--;
					jump(curpos,y);
					continue;
				}
				//last=min(window_size.ws_row-3,(int)directories.size());
				
			}
			else if(c==UP_KEY)
			{
				if(curpos==1)
				{
					continue;
				}
				curpos--;
				//first--;
				jump(curpos,y);
				/*if(first+curpos<=0)
				{
					curpos++;
					//first++;
					jump(curpos,y);
					continue;
				}*/
				//last=min(window_size.ws_row-3,(int)directories.size());
				
			}
			else if(c==LEFT_KEY)
			{
				//cout<<"trtr";
				string toGo;
				if(!back.empty())
				{
					toGo=back.top();
					back.pop();
					forward_stack.push(curr_dir);
					char *entry=&toGo[0];
					curr_dir=toGo;
					list_dir(entry);
					last=min(currentWindowRow-3,(int)directories.size());
					print_list(directories,0,last);
					curpos=1;
					first=0;
				}
			}
			else if(c==RIGHT_KEY)
			{
				string toGo;
				if(!forward_stack.empty())
				{
					toGo=forward_stack.top();
					forward_stack.pop();
					back.push(curr_dir);
					char *entry=&toGo[0];
					curr_dir=toGo;
					list_dir(entry);
					last=min(currentWindowRow-3,(int)directories.size());
					print_list(directories,0,last);
					curpos=1;
					first=0;
				}
			}
		}
		
		else if(c==ENTER)
		{
			string d_name=directories[first+curpos-1];
			int i,j=0;
			for(i=d_name.length()-1;i>=0;i--)
			{
				if(d_name[i]=='/')
				{
					break;
				}
				j++;
			}
			d_name=d_name.substr(i+1,j);
			
			if(d_name==".")
			{
				continue;
			}
			else
			{	
				
				if(d_name=="..")
				{	
					if(curr_dir==home)
					{
						continue;
					}
					int i;
					for(i=curr_dir.length()-1;i>=0;i--)
					{
						if(curr_dir[i]=='/')
						{
							break;
						}
					}
					curr_dir=curr_dir.substr(0,i);
					char *entry=&curr_dir[0];
					list_dir(entry);
					last=min(currentWindowRow-3,(int)directories.size());
					print_list(directories,0,last);
					curpos=1;
					first=0;
				}
				else
				{
					if(check_dir(directories[first+curpos-1]))
					{
						back.push(curr_dir);
						string temp=directories[first+curpos-1];
						char *entry=&temp[0];
						curr_dir=temp;
						list_dir(entry);
						last=min(currentWindowRow-3,(int)directories.size());
						print_list(directories,0,last);
						curpos=1;
						first=0;
						
					}
					else
					{
						char *t=new char[directories[first+curpos-1].length()+1];
						for(int j=0;j<directories[first+curpos-1].length();j++)
						{
							t[j]=directories[first+curpos-1][j];
						}
						pid_t pid=fork();
						if(pid==0)
						{
							int err=execl("/usr/bin/xdg-open","xdg-open",t,(char *)NULL);
							if(err==-1)
							{
								perror("Execl");
								exit(1);
							}
						}
						free(t);
					}
				}
			}
		}
		else if(c==127) //backspace
		{
			if(curr_dir==home)
			{
				continue;
			}
			back.push(curr_dir);
			int i;
			for(i=curr_dir.length()-1;i>=0;i--)
			{
				if(curr_dir[i]=='/')
				{
					break;
				}
			}
			curr_dir=curr_dir.substr(0,i);
			char *entry=&curr_dir[0];
			list_dir(entry);
			last=min(currentWindowRow-3,(int)directories.size());
			print_list(directories,0,last);
			curpos=1;
			first=0;
		}
		
		else if(c=='k' || c=='K') //scroll up ^
		{
			if(directories.size()<last || first==0)
			{
				//cout<<"f";
				continue;
			}
			first--;
			last--;
			print_list(directories,first,last); 
		}
		else if(c=='l' || c=='L') //Scroll Down 
		{
			if(directories.size()<=last)
			{
				//cout<<last;
				continue;
			}
			
			last++;
			first++;
			print_list(directories,first,last);
			
		}
		else if(c=='h' || c=='H')
		{
			back.push(curr_dir);
			char *entry=&home[0];
			curr_dir=home;
			list_dir(entry);
			last=min(currentWindowRow-3,(int)directories.size());
			print_list(directories,0,last);
			curpos=1;
			first=0;
		}
		else if(c=='q')
		{
			break;
		}
		else if(c==58)
		{
			
			int r=command_mode();
		}
		
	}
}

int check_dir(string path)
{
	
	char *entry=&path[0];
	struct stat fileInfo;
	
	if (stat(entry, &fileInfo) == -1) {
        perror(path.c_str());
        exit(EXIT_FAILURE);
    }
	if((fileInfo.st_mode & S_IFMT)==S_IFDIR)
	{		
		return 1;
	}
	else
	{
		return 0;
	}
}

//----------------------------------------------Command Mode--------------------------------

int command_mode()
{
	unsigned char c;
	x=currentWindowRow-2;
	y=1;
	jump(x,y);
	cout<<"\033[2K";		//clears the entire line
	cout<<"\033[38;5;50m";	//ESC[38;5;{ID}m sets background color and ID is 50 for bright blue
	cout<<":";
	
	cout<<"\033[0m";	//Resets color again to default
	cin.clear();
	
	y++;
	while(true)
	{
		c=cin.get();

		if(c=='q')
		{
			cout<<"\033[2K";
			return 1;
		}
		else if(c==ENTER)
		{
			//command_input.push_back('\n');
			my_command=split_command();
			my_command="create_file";

			if(my_command=="create_file")
			{
				//cout<<arguments.size();
				if(arguments.size()==2)
				{
					string filename=arguments[0];
					string filePath=getPath(filename,arguments[1]);
					//cout<<filePath;
					int res=createFile(filePath);
					if(res==0)
					{
						cout<<"Invalid Path/Filename Given";
					}
				}
				else
				{
					//INVALID Input
				}	
			}

			else
			{
				//INVALID Input
			}
			arguments.clear();
			my_command="";
			command_input.clear();
		}
		else if(c==BACKSPACE)
		{

			if(y==2)	//Dont Delete ':'
			{
				continue;
			}
			command_input.pop_back();
			y--;
			jump(x,y);
			cout<<"\033[0K";
		}
		else
		{
			cout<<c;
			command_input.push_back(c);
			y++;
			jump(x,y);
		}
	}
	return 1;
	

}
int createFile(string inp)
{
	char *entry=&inp[0];
	int res=open(entry,O_WRONLY | O_CREAT,(S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH| S_IXOTH));
	if(res<0)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}
string getPath(string filename,string arg)
{
	if(arg.length()==1 && arg[0]=='.')
	{
		return curr_dir+"/"+filename;
	}
	if((arg.length()==1 && arg[0]=='~') || (arg.length()==2 && arg[0]=='~' && arg[1]=='/'))
	{
		return home+"/"+filename;
	}
	else	//if(arg.length()>1)
	{
		if(arg[0]=='.' && arg[1]=='/')
		{
			return curr_dir+"/"+arg.substr(2)+"/"+filename;
		}
		else if(arg[0]=='~' && arg[1]=='/')
		{
			return home+"/"+arg.substr(2)+"/"+filename;
		}
		else if(arg[0]=='.' && arg[1]=='.' && arg[2]=='/')
		{
			string ab="";
			int i;
			for(i=curr_dir.length()-1;i>=0;i--)
			{
				if(curr_dir[i]=='/')
				{
					break;
				}
			}
			ab=curr_dir.substr(0,i);
			
			if(arg.length()==3)
			{
				return ab+"/"+filename;
			}
			else
			{
				return ab+"/"+arg.substr(3)+"/"+filename;
			}
		}
		else
		{
			if(arg.length()==1 && arg[0]=='/')
			{
				
				return home+"/"+filename;
			}
			else if(arg[0]=='/')
			{
				return home+arg+"/"+filename;
			}
			else
			{
				return curr_dir+"/"+arg+"/"+filename;
			}
			
		}
	}
}
string split_command()
{
	int i=0;
	string temp="";
	for(i=0;i<command_input.size();i++)
	{
		if(command_input[i]!=' ')
		{
			temp.append(1,command_input[i]);
		}
		if(command_input[i]==' ')
		{
			break;
		}
	}
	string param="";
	for(int j=i+1;j<(int)command_input.size();j++)
	{
		if(command_input[j]!=' ')
		{
			param.append(1,command_input[j]);
		}
		else
		{
			if(param.length()!=1)
			{
				if(param[param.length()-1]=='/')
				{
					param.pop_back();
				}
			}
			arguments.push_back(param);
			param="";
		}
	}
	if(param.length()!=1)
	{
		if(param[param.length()-1]=='/')
		{
			param.pop_back();
		}
	}
	arguments.push_back(param);
	return temp;
}