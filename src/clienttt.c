#define _GNU_SOURCE
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <ifaddrs.h>
#include <linux/if.h>
#include <sys/ioctl.h>


char* getip()
    {
        //create an ifreq struct for passing data in and out of ioctl
        struct ifreq my_struct;
 
        //declare and define the variable containing the name of the interface
        char *interface_name="ens33";   //a very frequent interface name is "eth0";
 
        //the ifreq structure should initially contains the name of the interface to be queried. Which should be copied into the ifr_name field.
        //Since this is a fixed length buffer, one should ensure that the name does not cause an overrun
        size_t interface_name_len=strlen(interface_name);
 
        if(interface_name_len<sizeof(my_struct.ifr_name))
        {
            memcpy(my_struct.ifr_name,interface_name,interface_name_len);
            my_struct.ifr_name[interface_name_len]=0;
        }
        else
        {
            perror("Copy name of interface to ifreq struct");
            printf("The name you provided for the interface is too long...\n");
        }
 
        //provide an open socket descriptor with the address family AF_INET
        /* ***************************************************************
         * All ioctl call needs a file descriptor to act on. In the case of SIOCGIFADDR this must refer to a socket file descriptor. This socket must be in the address family that you wish to obtain (AF_INET for IPv4)
         * ***************************************************************
         */
 
        int file_descriptor=socket(AF_INET, SOCK_DGRAM,0);
 
        if(file_descriptor==-1)
        {
            perror("Socket file descriptor");
            printf("The construction of the socket file descriptor was unsuccessful.\n");
            return -1;
        }
 
        //invoke ioctl() because the socket file descriptor exists and also the struct 'ifreq' exists
        int myioctl_call=ioctl(file_descriptor,SIOCGIFADDR,&my_struct);
 
        if (myioctl_call==-1)
        {
            perror("ioctl");
            printf("Ooops, error when invoking ioctl() system call.\n");
            close(file_descriptor);
            return -1;
        }
 
        close(file_descriptor);
 
        /* **********************************************************************
         * If this completes without error , then the hardware address of the interface should have been returned in the  'my_struct.ifr_addr' which is types as struct sockaddr_in.
         * ***********************************************************************/
 
      //extract the IP Address (IPv4) from the my_struct.ifr_addr which has the type 'ifreq'
 
        /* *** Cast the returned address to a struct 'sockaddr_in' *** */
        struct sockaddr_in * ipaddress= (struct sockaddr_in *)&my_struct.ifr_addr;
       /* *** Extract the 'sin_addr' field from the data type (struct) to obtain a struct 'in_addr' *** */
      printf("IP Address is %s.\n", inet_ntoa(ipaddress->sin_addr));
      return inet_ntoa(ipaddress->sin_addr);
    }



void inscrire(int fd){
	char* hostname[128];
	gethostname(hostname,sizeof hostname);
	char *ip = getip();
	strcat(ip,"  :");
	strcat(ip,hostname);
	
	send(fd,ip,strlen(ip),0);
	
    	
	
}
	
int main(int argc, char *argv[])
{
    readFunc();
    
    
}
	

void readFunc(int argc, char *argv[]){

// La socket client
    int sockfd = 0;
    int  n = 0;
    // Le buffer pour recevoir la réponse du serveur
    char recvBuff[1024] = {0};
    // La structure avec les informations du serveur
    struct sockaddr_in serv_addr = {0};
    
    /*
     * Si l'IP du serveur n'a pas été passée en argument
     * le programme se termine
     */
    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    }
    
    // Création de la socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(7000);
    
    
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }
    
    
    // Connection au serveur
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        return 1;
    }
    
    
        inscrire(sockfd);
        
        FILE *out_file=fopen("list_serveur","a");
        fprintf(out_file,"adress et nom : %s",argv[1]);
    
    
    if(n < 0)
    {
        printf("\n Read error \n");
    }
    
    return 0;
}

