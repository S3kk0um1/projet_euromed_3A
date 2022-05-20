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
        //}
        //else if(pid==0) { // Le processus fils
            bzero(recvBuff,1025);
            recv(connfd,recvBuff,1025,0);
            
            printf("this is :%s\n",recvBuff);
		
            FILE *out_file=fopen("list_client","a");
            FILE *in_file=fopen("list_client","r");
		
            int theline=0;
            char line[1024];

            char stringg[1024];
            int j= snprintf(stringg,1024,"adress ip:%s",recvBuff);

            while(fgets(line,sizeof line,in_file) !=NULL){
            if(strcmp(line,stringg)!=0){
            theline=1;
            }
            else{theline=0;
            break;}

            }
            if(theline==1){
            fprintf(out_file,"adress ip:%s",recvBuff);
            }
            
            fclose(out_file);
            fclose(in_file);
            
            
             
            
                        
            
            close(connfd);
            close(listenfd);
        
    
}
