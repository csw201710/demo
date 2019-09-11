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


#ifdef SOLARIS
#include <sys/sockio.h>
#endif

#define MAXINTERFACES   16

int
main (int argc,char **argv)
{
   register int fd, intrface, retn = 0;
   struct ifreq buf[MAXINTERFACES];
   struct arpreq arp;
   struct ifconf ifc;

   if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0) {
      ifc.ifc_len = sizeof buf;
      ifc.ifc_buf = (caddr_t) buf;
      if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc)) {
         intrface = ifc.ifc_len / sizeof (struct ifreq);
         printf("interface num is intrface=%d/n/n/n",intrface);
         while (intrface-- > 0)
          {
            printf ("net device %s/n", buf[intrface].ifr_name);

/*Jugde whether the net card status is promisc  */
            if (!(ioctl (fd, SIOCGIFFLAGS, (char *) &buf[intrface]))) {
               if (buf[intrface].ifr_flags & IFF_PROMISC) {
                  puts ("the interface is PROMISC");
                  retn++;
               }
            } else {
               char str[256];

               sprintf (str, "cpm: ioctl device %s", buf[intrface].ifr_name);
               perror (str);
            }

/*Jugde whether the net card status is up       */
            if (buf[intrface].ifr_flags & IFF_UP) {
                puts("the interface status is UP");
               }
            else {
                puts("the interface status is DOWN");
            }

/*Get IP of the net card */
            if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))
                {
                 puts ("IP address is:");
                 puts(inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
                 puts("");
                   //puts (buf[intrface].ifr_addr.sa_data);
                }
            else {
               char str[256];

               sprintf (str, "cpm: ioctl device %s", buf[intrface].ifr_name);
               perror (str);
           }
/* this section can't get Hardware Address,I don't know whether the reason is module driver*/
//          ((struct sockaddr_in*)&arp.arp_pa)->;sin_addr=((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr;
#ifdef SOLARIS
            arp.arp_pa.sa_family = AF_INET;
            arp.arp_ha.sa_family = AF_INET;
            ((struct sockaddr_in*)&arp.arp_pa)->sin_addr.s_addr=((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr.s_addr;
            if (!(ioctl (fd, SIOCGARP, (char *) &arp)))
                {
                 puts ("HW address is:");

                 printf("%02x:%02x:%02x:%02x:%02x:%02x/n",
                                (unsigned char)arp.arp_ha.sa_data[0],
                                (unsigned char)arp.arp_ha.sa_data[1],
                                (unsigned char)arp.arp_ha.sa_data[2],
                                (unsigned char)arp.arp_ha.sa_data[3],
                                (unsigned char)arp.arp_ha.sa_data[4],
                                (unsigned char)arp.arp_ha.sa_data[5]);

                 puts("");
                 puts("");
                }


#else
#if 0
/*Get HW ADDRESS of the net card */
            if (!(ioctl (fd,  SIOCGENADDR, (char *) &buf[intrface])))
                {
                 puts ("HW address is:");

                 printf("%02x:%02x:%02x:%02x:%02x:%02x/n",
                                (unsigned char)buf[intrface].ifr_enaddr[0],
                                (unsigned char)buf[intrface].ifr_enaddr[1],
                                (unsigned char)buf[intrface].ifr_enaddr[2],
                                (unsigned char)buf[intrface].ifr_enaddr[3],
                                (unsigned char)buf[intrface].ifr_enaddr[4],
                                (unsigned char)buf[intrface].ifr_enaddr[5]);

                 puts("");
                 puts("");
                }
#endif
            if (!(ioctl (fd, SIOCGIFHWADDR, (char *) &buf[intrface])))
                {
                 puts ("HW address is:");

                 printf("%02x:%02x:%02x:%02x:%02x:%02x/n",
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[0],
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[1],
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[2],
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[3],
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[4],
                                (unsigned char)buf[intrface].ifr_hwaddr.sa_data[5]);

                 puts("");
                 puts("");
                }
#endif

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

    close(fd);
    return retn;
}

