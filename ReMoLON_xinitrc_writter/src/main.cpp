/*
 * Copyright (c) 2019 CCS/UPM - GMRV/URJC.
 *
 * Authors: Nadir Román Guerrero <nadir.roman@urjc.es>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
 * by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
 
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
  xinit << "#!/usr/bin/env bash \n";
  xinit << "export XSESSION_PID=\"$$\"\n";
  xinit << "gnome-terminal &\n";
  xinit << "cd " << remotooDir << "\n";
  xinit << "./remotoo " << session << " " << user << " " << userIP << " " << webPort << " " << sockPort << " " << rtcPort << " $XSESSION_PID &\n";
  xinit << "exec gnome-session\n";
  xinit.flush();
  xinit.close();

  return system("xinit -- :4");
}
