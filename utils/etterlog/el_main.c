/*
    etterlog -- log analyzer for ettercap log file 

    Copyright (C) ALoR & NaGA

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

    $Id: el_main.c,v 1.13 2004/04/09 08:14:22 alor Exp $
*/

#include <el.h>
#include <el_functions.h>
#include <ec_version.h>

#include <fcntl.h>

/* global options */
struct globals gbls;


/*******************************************/

int main(int argc, char *argv[])
{
   int ret;

   /* etterlog copyright */
   fprintf(stderr, "\n\033[01m\033[1m%s %s\033[0m copyright %s %s\n\n",
                      GBL_PROGRAM, EC_VERSION, EC_COPYRIGHT, EC_AUTHORS);
  
  
   /* allocate the global target */
   SAFE_CALLOC(GBL_TARGET, 1, sizeof(struct target_env));
  
   /* initialize to all target */
   GBL_TARGET->all_mac = 1;
   GBL_TARGET->all_ip = 1;
   GBL_TARGET->all_port = 1;
   
   /* getopt related parsing...  */
   parse_options(argc, argv);

   /* get the global header */
   ret = get_header(&GBL.hdr);
   if (ret == -EINVALID)
      FATAL_ERROR("Invalid log file");
   
   fprintf(stderr, "Log file version    : %s\n", GBL.hdr.version);
   fprintf(stderr, "Timestamp           : %s", ctime((time_t *)&GBL.hdr.tv.tv_sec));
   fprintf(stderr, "Type                : %s\n\n", (GBL.hdr.type == LOG_PACKET) ? "LOG_PACKET" : "LOG_INFO" );
  
   
   /* analyze the logfile */
   if (GBL.analyze)
      analyze();

   /* rewind the log file and skip the global header */
   gzrewind(GBL_LOG_FD);
   get_header(&GBL.hdr);
   
   /* display the connection table */
   if (GBL.connections)
      conn_table();

   /* not interested in the content... only analysis */
   if (GBL.analyze || GBL.connections)
      return 0;
   
   /* display the content of the logfile */
   display();
   
   return 0;
}



/* ANSI color escapes */

void set_color(int color)
{
   char str[8];
   
   sprintf(str, "\033[%dm", color);
   write(fileno(stdout), str, strlen(str));
}

/* reset the color to default */

void reset_color(void)
{
   write(fileno(stdout), EC_COLOR_END, 4);   
}


/* EOF */

// vim:ts=3:expandtab

