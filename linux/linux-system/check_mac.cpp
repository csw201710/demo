#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>

#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <unistd.h>

static int checkMac(const char* lic_mac){
   int fd, intrface;
   struct ifreq buf[16];
   struct ifconf ifc;
   int ret = -1;
   if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0) {
      ifc.ifc_len = sizeof (buf);
      ifc.ifc_buf = (caddr_t) buf;
      if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc)) {
         intrface = ifc.ifc_len / sizeof (struct ifreq);
         while (intrface-- > 0)
          {
            if (!(ioctl (fd, SIOCGIFHWADDR, (char *) &buf[intrface])))
                {
                  char mac_addr[64]={0};
                 sprintf(mac_addr,"%02x%02x%02x%02x%02x%02x",
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[0],
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[1],
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[2],
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[3],
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[4],
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[5]);
                  //printf("mac_addr: %s\n",mac_addr);
                  //printf("mac_addr: %s\n",lic_mac);
                  if(strcasecmp(mac_addr,lic_mac) == 0){
                    ret = 0 ; goto err;
                  }
                 
                }


            else {
               char str[256];

               sprintf (str, "cpm: ioctl device %s", buf[intrface].ifr_name);
               perror (str);
           }
        }
      } else
         perror ("cpm: ioctl");

   } else
      perror ("cpm: socket");

err:
    close(fd);
    return ret;
}

