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


char *loadfile(char *name,char *file){
	FILE *x=fopen(name,"r");
	int i=0;
	char *c=0;
	
	while((c=fgetc(x))!=EOF){
		
		file[i]=c;
		i++;
		}
	
	
	
	fclose(x);
	
	return file;
}


void sendfile(char *ip,char *file){
	printf("\n in sendfile\n");
	int sendfd = 0;
   	int  n = 0;
	
 
   	struct sockaddr_in serv_addr = {0};
   	if((sendfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Error : Could not create socket \n");
 		return 1;
    }
    
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(7001);
    
    
	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
	{
        	printf("\n inet_pton error occured\n");
        	return 1;
    }
    
    
    // Connection au serveur
	if(connect(sendfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
		printf("\n Error : Connect Failed \n");
		return 1;
    }
    printf("connected\n");
    
    if (send(sendfd,file,strlen(file),0)==-1){
    	printf("send fail");
    }
    printf("\n this is:%s\n",file);
    printf("in sendfile\n");
	//receiiive
    //recv(sendfd,file,48000,0);
    
    close(sendfd);
}





int main(int argc, char *argv[])
{
    writeFunc();
}


void writeFunc(void){
   // La socket serveur
    int listenfd = 0;
    // La socket client (récupérée avec accept())
    int connfd = 0;
    // La structure avec les informations du serveur
    struct sockaddr_in serv_addr = {0};
    // Le buffer pour envoyer les données
    char recvBuff[1025] = {0};
    
    // Création de la socket serveur
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    //Initialisation de la structure sockaddr
    serv_addr.sin_family = AF_INET;
    //Accepte les connexions depuis n'importe quelle adresse
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // Le port sur lequel la socket va écouter
    serv_addr.sin_port = htons(7000);
    
    // Association de la socket avec la structure sockaddr
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    //La socket écoute pour des connexions
    listen(listenfd, 10);
    
    // Récupération du nom de la machine
    char hostname[128];
    gethostname(hostname, sizeof hostname);
    
    int pid = 0;
    
        // Accepte la connexion d'une socket client
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        
        // Exécution d'un fork pour gérer la connexion
       // if((pid=fork())==-1) {
         //   printf("erreur\n");
           // close(connfd);
        //}
        //else if(pid>0) { // Le processus père
          //  close(connfd);
        //}      |                                    ~^  ~~~~~

        //else if(pid==0) { // Le processus fils
            bzero(recvBuff,1025);
            recv(connfd,recvBuff,1025,0);
            
            printf("this is :%s\n",recvBuff);
            FILE *out_file=fopen("list_client","a");
            fprintf(out_file,"adress ip:%s",recvBuff); 


            char file[48000];
	    bzero(file,48000);           
            //char *file1;
            //file1=loadfile("code.txt",file1);
            
            //printf("contenu file 1:%s",file1);
            char *file1=loadfile("disksize.sh",file);
            sendfile("192.168.244.198",file1);
            printf("file after send %s:\n",file1);
		
            close(connfd);
            close(listenfd);
            //shutdown(connfd,2);
    
}
