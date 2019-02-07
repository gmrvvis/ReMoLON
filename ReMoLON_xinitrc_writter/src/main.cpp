#include <fstream>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main(int argc, char ** argv)
{
  // remolon_xinitrc_writter <remotobinarydir> <sessionname> <username> <userIP> <webPort> <webSockPort> <webRTCPort>
  if (argc < 8)
  {
    std::cerr << "Usage remolon_xinitrc_writter <remotoobinarydir> <sessionname> <username> <userIP> <webPort> <webSockPort> <webRTCPort>" << std::endl;
    return -1;
  }

  char * remotooDir = argv[1];
  char * session = argv[2];
  char * user = argv[3];
  char * userIP = argv[4];
  char * webPort = argv[5];
  char * sockPort = argv[6];
  char * rtcPort = argv[7];

  struct passwd * userInfo = getpwnam( user );
  char buf [ 0xffff ];
  sprintf ( buf, "%s/.xinitrc", userInfo->pw_dir );

  ofstream xinit;
  xinit.open (buf, std::ios::out | std::ios::trunc);
  xinit << "gnome-terminal &\n";
  xinit << "cd " << remotooDir << "\n";
  xinit << "./remotoo " << session << " " << user << " " << userIP << " " << webPort << " " << sockPort << " " << rtcPort << " &\n";
  xinit << "exec gnome-session\n";
  xinit.flush();
  xinit.close();

  return system("xinit -- :4");
}
