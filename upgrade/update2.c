#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/time.h>
#define MAX 1  
pthread_t thread[2];
pthread_mutex_t mut;
int number=0, i;
FILE * fp;
char localip[254];

#define ETH_NAME        "eth0"  
#define MYPORT  8885
#define MYPORT1  8884
#define QUEUE   20
#define BUFFER_SIZE 1024

int sock_cli;
char sendbuf[BUFFER_SIZE];
char recvbuf[BUFFER_SIZE];

void *thread1()
{
        printf ("thread1 : I'm thread 1\n");
        for (i = 0; i < MAX; i++)
        {
                printf("thread1 : number = %d\n",number);
                pthread_mutex_lock(&mut);
                number++;
                pthread_mutex_unlock(&mut);
                sleep(2);

		int sock;
		struct sockaddr_in sin;
		struct ifreq ifr;
		sock = socket(AF_INET, SOCK_DGRAM, 0);

		if (sock == -1){
		        perror("socket");
			exit(1);
		        //return -1;
		}

		//get ipaddr
		strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
		ifr.ifr_name[IFNAMSIZ - 1] = 0;

		if (ioctl(sock, SIOCGIFADDR, &ifr) < 0){
		        perror("ioctl");
			exit(1);
		        //return -1;
		}

		memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
	//      fprintf(stdout, "%s\n", inet_ntoa(sin.sin_addr));
		printf("ipaddr:%s\n", inet_ntoa(sin.sin_addr));

		char*e;
		char*f;
		char*g;
		char*h;
		//one
		e=strtok(inet_ntoa(sin.sin_addr),".");
		if(e)
		{
			printf("%s\n",e);
		}
			/*Asecond call to strtok using a NULL
			as the first parameter returns a pointer
			to the character following the token*/
		//two
		f=strtok(NULL,".");
		if(f)
	 	        printf("%s\n",f);

		//three
		g=strtok(NULL,".");
		if(g)
			printf("%s\n",g);

		//four
		h=strtok(NULL,".");
		if(h)
			printf("%s\n",h);

		sprintf(localip,"%s.%s.%s.%s",e,f,g,h);

		printf("The ip_addr :%s\n", localip);

		int u1;
		for(u1=0;u1<3;u1++)
		{
			sleep(5);
			///定义sockfd
			sock_cli = socket(AF_INET,SOCK_STREAM, 0);

			struct sockaddr_in servaddr;
			memset(&servaddr, 0, sizeof(servaddr));
			servaddr.sin_family = AF_INET;
			servaddr.sin_port = htons(MYPORT1);  ///服务器端口
			servaddr.sin_addr.s_addr = inet_addr(localip);  ///服务器ip

			///连接服务器，成功返回0，错误返回-1
			if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
			{
	       			perror("connect");
				printf("************error***********\n");
				if(u1==2){	
					printf("you need change up1!!!!!!!\n");
					char name[254];
					int sock;
					struct sockaddr_in sin;
					struct ifreq ifr;
					sock = socket(AF_INET, SOCK_DGRAM, 0);

					if (sock == -1){
						perror("socket");
						exit(1);
						//return -1;
					}
					//get mac
					strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
					ifr.ifr_name[IFNAMSIZ - 1] = 0;

					if (ioctl(sock,SIOCGIFHWADDR, &ifr) < 0){
						perror("ioctl");
						exit(1);
						//return -1;
					}

					memcpy(&sin, &ifr.ifr_addr, sizeof(sin));
					char mac[32];
					sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x",
					(unsigned char) ifr.ifr_hwaddr.sa_data[0],
					(unsigned char) ifr.ifr_hwaddr.sa_data[1],
					(unsigned char) ifr.ifr_hwaddr.sa_data[2],
					(unsigned char) ifr.ifr_hwaddr.sa_data[3],
					(unsigned char) ifr.ifr_hwaddr.sa_data[4],
					(unsigned char) ifr.ifr_hwaddr.sa_data[5]);
					printf("Mac:%s\n",mac);

					//定义数组存放name
					char acc[4][1000] ;
					char tmp[128];
					sprintf(tmp,"curl http://upd.id2u.cn/?r=up1'&'v=12345678'&'m=%s -s -o update1",mac);
					printf("ddddddddddddddddddd%s\n",tmp);
					system(tmp);

					//打开文件，读取服务器返回信息
					if((fp=fopen("update1","a+"))==NULL)
					{
						printf("-----cannot open this file on the out------\n");
						//exit(0);
					}else{
						printf("--------open this file on the out-------\n");
					}

					//
					int i=0;
					if(fgets(acc[i],1000,fp)!=NULL)
					{
						printf("read update1 file---%s\n",acc[i]);
						strcpy(name,acc[i]);
						printf("-----------name=%s\n",name);
					}
					fclose(fp);

				        char tmp1[254];
				        sprintf(tmp1,"cd ./u1 && wget %s",name);
				        system(tmp1);
				        char tmp2[54];
				        sprintf(tmp2,"cd ./u1/ && tar -zxvf ./* && cd ./patch/ && sh upgrade.sh");
				        system(tmp2);
				        char tmp3[54];
				        sprintf(tmp3,"cd ./u1/ && rm ./*.tar.gz && rm -rf ./patch");
				        system(tmp3);

				}
				continue;
	       			//exit(1);
			}

			//up1 is ok ?
			char up2[] = "up1\n";
			send(sock_cli, up2, strlen(up2),0); ///发送
			recv(sock_cli, recvbuf, sizeof(recvbuf),0); ///接收
			fputs(recvbuf, stdout);

		        char n3[6]={'\0'};
			printf("yikaishi n3=%s\n",n3);
			//strcpy(n3,'0');
		        strncpy(n3,recvbuf,sizeof(n3));
			printf("ssssssssizeof=%ld\n",sizeof(n3));
		        n3[sizeof(n3)-1]='\0';
			printf("chulihou n3=%s\n",n3);

			printf("uuuuuujjjj2222=%d\n",u1);

			if(strncmp(n3,"up1ok",5)==0){
				printf("up1 is ok\n");
				printf("you don't change up1\n");	

				memset(sendbuf, 0, sizeof(sendbuf));
				memset(recvbuf, 0, sizeof(recvbuf));
				close(sock_cli);
	
				break;
			}else{
		
				memset(sendbuf, 0, sizeof(sendbuf));
				memset(recvbuf, 0, sizeof(recvbuf));
				close(sock_cli);
				sleep(2);
			}
			/*if(u1==2){	
				printf("uuuujjjjj22222=%d\n",u1);
				printf("you need change up1\n");

			}*/
		}
        }
        printf("thread1 :主函数在等我完成任务吗？\n");
        pthread_exit(NULL);

}


void *thread2()
{
        printf("thread2 : I'm thread 2\n");
        for (i = 0; i < MAX; i++)
        {
                printf("thread2 : number = %d\n",number);
                pthread_mutex_lock(&mut);
                number++;
                pthread_mutex_unlock(&mut);
                sleep(1);
		///定义sockfd
		int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
	 
		///定义sockaddr_in
		struct sockaddr_in server_sockaddr;
		server_sockaddr.sin_family = AF_INET;
		server_sockaddr.sin_port = htons(MYPORT);
		server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	 
		///bind，成功返回0，出错返回-1
		if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
		{
			perror("bind");
			exit(1);
		}
	 
		///listen，成功返回0，出错返回-1
		if(listen(server_sockfd,QUEUE) == -1)
		{
			perror("listen");
			exit(1);
		}

		///客户端套接字
		char buffer[BUFFER_SIZE];
		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);

		int conn;


		while(1)
		{
			///成功返回非负描述字，出错返回-1
			conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
			if(conn<0)
			{
				perror("connect");
				exit(1);
			}
			memset(buffer,0,sizeof(buffer));
			int len = recv(conn, buffer, sizeof(buffer),0);

		        if(strcmp(buffer,"up2\n")==0  ||  strcmp(buffer,"up2")==0)
		        {
		                char l[]="up2ok\n";
		                send(conn, l, 50, 0);
		                fputs(l, stdout);
				break;
		        }else if(strcmp(buffer,"listen\n")==0 || strcmp(buffer,"listen")==0){
		                char ll[]="ok--ok--ok\n";
		                send(conn, ll, 50, 0);
		                fputs(ll, stdout);
		        }

		}
		close(conn);
		close(server_sockfd);
        }
        printf("thread2 :主函数在等我完成任务吗？\n");
        pthread_exit(NULL);
}

void thread_create(void)
{
        int temp;
        memset(&thread, 0, sizeof(thread));
        /*创建线程*/
        if((temp = pthread_create(&thread[0], NULL, thread1, NULL)) != 0)        
                printf("线程1创建失败!\n");
        else
                printf("线程1被创建\n");
        if((temp = pthread_create(&thread[1], NULL, thread2, NULL)) != 0)
                printf("线程2创建失败");
        else
                printf("线程2被创建\n");
}

void thread_wait(void)
{
        /*等待线程结束*/
        if(thread[0] !=0) {
                pthread_join(thread[0],NULL);
                printf("线程1已经结束\n");
        }
        if(thread[1] !=0) {
                pthread_join(thread[1],NULL);
                printf("线程2已经结束\n");
        }
}

int main()
{
        /*用默认属性初始化互斥锁*/
        pthread_mutex_init(&mut,NULL);
        printf("我是主函数哦，我正在创建线程，呵呵\n");
        thread_create();
        printf("我是主函数哦，我正在等待线程完成任务阿，呵呵\n");
        thread_wait();
        printf("两个线程已经结束\n");
        sleep(5);
        printf("结束\n");
        return 0;
}

