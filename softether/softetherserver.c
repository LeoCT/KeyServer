#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <errno.h>

#include"des.h"

#define ETH_NAME        "eth0" 

#define MYPORT  8887
#define QUEUE   20
#define BUFFER_SIZE 1024
 
int main()
{

        int sock;
        struct sockaddr_in sin;
        struct ifreq ifr;
        sock = socket(AF_INET, SOCK_DGRAM, 0);

        if (sock == -1){
                perror("socket");
                return -1;
        }

        //get ipaddr
        strncpy(ifr.ifr_name, ETH_NAME, IFNAMSIZ);
        ifr.ifr_name[IFNAMSIZ - 1] = 0;

        if (ioctl(sock, SIOCGIFADDR, &ifr) < 0){
                perror("ioctl");
                return -1;
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

	char ip[254];
	sprintf(ip,"%s.%s.%s.%s",e,f,g,h);

	printf("The ip_addr :%s\n", ip);


	///定义sockfd
	int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
	//AF_INET是针对Internet的,因而可以允许在远程主机之间通信
	//SOCK_STREAM表明我们用的是TCP协议,这样会提供按顺序的,可靠,双向,面向连接的比特流.

     
	///定义sockaddr_in
	struct sockaddr_in server_sockaddr;
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(MYPORT);
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
 
	//int bind(int sockfd, struct sockaddr *my_addr, int addrlen)
	//sockfd:是由server_sockfd调用返回的文件描述符.
	//addrlen:是sockaddr结构的长度.
	//my_addr:是一个指向sockaddr的指针. 在中有 sockaddr的定义
	//struct sockaddr{
	//unisgned short as_family;
	//char sa_data[14];
	//};
	//使用另外一个结构(struct server_sockaddr) 来代替


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
	
	//定义时间并初始化，用作前一天的用户清理
	int shijian;

	time_t timep;
	struct tm *p;
	time(&timep);
	p =localtime(&timep);

	shijian  = p->tm_hour;
	printf("now hours is---conn-----%d\n",p->tm_hour);
	printf("now min is---conn-----%d\n",p->tm_min);

	//定义数组存放用户的帐号
	char acc[254254][10] ;
	//定义计算用户帐号的数量
	int sum = 0; 
	int conn;
	
	///客户端套接字
	char buffer[BUFFER_SIZE];
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);


	FILE * fp;


	//打开文件
	if((fp=fopen("greensky-server.ovpn","r"))==NULL)
	{
		printf("-----cannot open this file on the out------");
		//exit(0);
		char filename[254];
		sprintf(filename,"cd /work/softether/ && vpncmd /SERVER %s /adminhub:DEFAULT /PASSWORD:server /CMD OpenVpnMakeConfig file.zip",ip);
		system(filename);

		char filename1[254];
		sprintf(filename1,"cd /work/softether/ && chmod 777 file.zip && unzip file.zip && mv greensky*l3.ovpn greensky-server.ovpn");
		system(filename1);

		char filename2[254];
		sprintf(filename2,"cd /work/softether/ && rm readme.* && rm file.zip && rm greensky*l2.ovpn");
		system(filename2);
	}else{
		printf("--------open this file on the out-------");
	}

	//get cert to AllString char;
	char acc1[250][254]={0};
	char AllString[204800]={0};
	//打开文件
	if((fp=fopen("greensky-server.ovpn","a+"))==NULL)
	{
	        printf("-----cannot open this file on the out------\n");
	        exit(0);
	}else{
	        printf("--------open this file on the out-------\n");
	}

	//判断文件是否含有<a> and </ca>
	int iiii=0;
	while(fgets(acc1[iiii],254,fp)!=NULL)
	{
	        char acc2[2];
	        acc2[1]='\0';
	        strncpy(acc2,acc1[iiii],1);
	        if((strcmp(acc2,"#")!=0) && (strcmp(acc2,";")!=0) && (strcmp(acc2,"\r")!=0))
	        {
	                printf("read dada file---%s\n",acc1[iiii]);
			strcat(AllString,acc1[iiii]);
	        }
	        iiii++;
	        //printf("read dada file---%s\n",acc1[iiii]);
		//strcat(AllString,acc1[iiii]);
	        //iiii++;
	}
	fclose(fp);
	printf("The following is a certificate:\n%s\n",AllString);

	//打开文件
	if((fp=fopen("data","a+"))==NULL)
	{
		printf("-----cannot open this file on the out------");
		exit(0);
	}else{
		printf("--------open this file on the out-------");
	}

	//判断文件里面是否有账户，并删除用户
	int ii=0;
	while(fgets(acc[ii],10,fp)!=NULL)
	{
		char tmp0[254];
		sprintf(tmp0,"vpncmd /SERVER %s /adminhub:DEFAULT /PASSWORD:server /CMD UserDelete %s",ip,acc[ii]);
		system(tmp0);

		printf("read dada file---%s",acc[ii]);
		ii++;
	}
	fclose(fp);
	//删除用户后重新拷贝空文件
	char tmp4[254];
	sprintf(tmp4,"cp ./cp/data  ./data");
	system(tmp4);


	while(1)
	{	
		//输出当前时间
		printf("now shijian is--------%d\n",shijian);


		///成功返回非负描述字，出错返回-1
		conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
		if(conn<0)
		{
        		perror("connect");
        		exit(1);
		}	

		//初始化本地时间
		time_t timep;
		struct tm *p;
		time(&timep);
		p =localtime(&timep);

		//shijian  = p->tm_hour;
		printf("now hours is---conn-----%d\n",p->tm_hour);
		printf("now min is---conn-----%d\n",p->tm_min);


        	memset(buffer,0,sizeof(buffer));
        	int len = recv(conn, buffer, sizeof(buffer),0);
        	/*if(strcmp(buffer,"exit\n")==0)
        		break;*/

		if(strcmp(buffer,"user\n")==0 || strcmp(buffer,"user")==0)
		{

			printf("now hours is----user----%d\n",p->tm_hour);
			printf("now min is----user----%d\n",p->tm_min);
			//判断时间是否为当天的时间，若是，不做处理；若不是，清除之前的帐号
			if(shijian == p->tm_hour)
			{
				printf("time is same\n");
			}else{
				printf("time is diffrent\n");
				shijian = p->tm_hour;
				int y;
				for(y=0; y<sum; y++)
				{
	                		char pp[254];
                			sprintf(pp,"vpncmd /SERVER %s /adminhub:DEFAULT /PASSWORD:server /CMD UserDelete %s",ip,acc[y]);
                			system(pp);
        	        		printf("=======time=======\n");
				}
				sum = 0;
				
			}

			//生成账户密码
			char pool[]={
			'0','1','2','3','4','5','6','7','8','9',
			'a','b','c','d','e','f','g','h','i','j',
        		'k','l','m','n','o','p','q','r','s','t',
        		'u','v','w','x','y','z','A','B','C','D',
        		'E','F','G','H','I','J','K','L','M','N',
        		'O','P','Q','R','S','T','U','V','W','X',
        		'Y','Z'
    			};//随机池
			srand(time(0));
    			char account[9];
			char pwd[9];
			account[8]='\0';//方便作为字符串输出处理
			pwd[8]='\0';//方便作为字符串输出处理
	   	
    			int j=0;
    			while(j!=8)
    			{
				account[j++]=pool[rand()%sizeof(pool)];
    			}
    			printf("帐号:%s\n",account);
	
			int i=0;
			while(i!=8)
			{
				pwd[i++]=pool[rand()%sizeof(pool)];
    			}
    			printf("密码:%s\n",pwd);
			//执行linux命令，创建随机用户并为之创建密码
			char tmp[254];
			sprintf(tmp,"vpncmd /SERVER %s /adminhub:DEFAULT /PASSWORD:server /CMD UserCreate %s /GROUP:none /REALNAME:none /NOTE:vpncmd",ip,account);
			system(tmp);

			char tmp2[254];
        		sprintf(tmp2,"vpncmd /SERVER %s /adminhub:DEFAULT /PASSWORD:server /CMD UserPasswordSet %s /PASSWORD:%s",ip,account,pwd);
        		system(tmp2);
			char buf[254];
			char prsec[10] = {"cl123456"};
			sprintf(buf,"%s,%s",account,pwd);
			strcpy(acc[sum] , account);


			//把字符串拆分成每个都是8位的字符串
			char s[254];
			int jjj;
			jjj = strlen(s);
			sprintf(s,"%s,%s",buf,prsec);
			printf("printff-------ffffff%s\n",s);
			printf("printff-------fdddddf%d\n",jjj);
			//char s[50]="userName,Password";
				int iii;
				char s1[9];
				char s2[9];
				char s3[9];
				char s4[9];
				char sn[3];
				char ss[9];
				
				s1[8]='\0';
				s2[8]='\0';
				s3[8]='\0';
				s4[8]='\0';
				sn[2]='\0';
				ss[8]='\0';
				for(i=0;i<8;i++)//one
				{		
					if(i<strlen(s))
					{
						printf("%c",s[i]);
						sprintf(ss,"%c",s[i]);
						s1[i] = *ss;
					}
					else printf("%c",' ');
				}
				printf("\n");


				for(i=0;i<8;i++)//two
				{
					if(i+8<strlen(s))
					{
			
						printf("%c",s[i+8]);
						sprintf(ss,"%c",s[i+8]);
						s2[i] = *ss;
					}
					else printf("%c",' ');
				}
				printf("\n");

				for(i=0;i<8;i++)//three
				{
					if(i+16<strlen(s))
					{
			
						printf("%c",s[i+16]);
						sprintf(ss,"%c",s[i+16]);
						s3[i] = *ss;
					}
					else printf("%c",' ');
				}
				printf("\n");

				for(i=0;i<8;i++)//four
				{
					if(i+24<strlen(s))
					{
			
						printf("%c",s[i+24]);
						sprintf(ss,"%c",s[i+24]);
						s4[i] = *ss;
					}
					else printf("%c",' ');
				}
				printf("\n");


			for(i=0;i<2;i++)
			{
				sprintf(ss,"%c",s4[i]);
				sn[i] = *ss;

			}

			int s11;s11 = strlen(s1);
			printf("-----%s-----%d\n",s1,s11);
			int s21;s21 = strlen(s2);
			printf("-----%s-----%d\n",s2,s21);
			int s31;s31 = strlen(s3);
			printf("-----%s-----%d\n",s3,s31);
			int s41;s41 = strlen(s4);
			printf("-----%s-----%d\n",s4,s41);


			int eee=0;

			//char MyMessage[16]={"as1df345"};
			char dddd[48]={0}; 
			char dddd1[48]={0}; 
			char dddd2[48]={0};
			char dddd3[48]={0};
			char miwen[64];
			//main1(MyMessage,MesHex);
			main1(s1,dddd);
			printf("Encrypted!----buf-----:%s\n",buf);

			printf("Your Message is Encrypted!:\n");  // 信息已加密one
			printf("Encrypted!:%s\n",dddd);
			for(eee=0;eee<64;eee++)           
			{
				printf("%c",dddd[eee]);
			}
			printf("\n");
			printf("\n");
	
			main1(s2,dddd1);
			printf("Your Message2222222 is Encrypted!:\n");  // 信息已加密two
			printf("Encrypted22222222!:%s\n",dddd1);
			for(eee=0;eee<64;eee++)           
			{
				printf("%c",dddd1[eee]);
			}
			printf("\n");
			printf("\n");

			main1(s3,dddd2);
			printf("Your Message2222222 is Encrypted!:\n");  // 信息已加密three
			printf("Encrypted22222222!:%s\n",dddd2);
			for(eee=0;eee<64;eee++)           
			{
				printf("%c",dddd2[eee]);
			}
			printf("\n");
			printf("\n");

			main1(sn,dddd3);
			printf("Your Message2222222 is Encrypted!:\n");  // 信息已加密four
			printf("Encrypted22222222!:%s\n",dddd3);
			for(eee=0;eee<64;eee++)           
			{
				printf("%c",dddd3[eee]);
			}
			printf("\n");
			printf("\n");
			sprintf(miwen,"%s%s%s%s",dddd,dddd1,dddd2,dddd3);//发送密文
			printf("*****miwen*****%s\n",miwen);


			//打开存储用户账户的文档
			if((fp=fopen("data","a+"))==NULL)
			{
				printf("-----cannot open this file on the in------");
				exit(0);
			}else{
				printf("--------open this file on the in-------");
			}

			fputs(acc[sum],fp);
			fputs("\n",fp);
			fclose(fp);
			
			printf("-------acc[sum]=%s\n",acc[sum]);

         		send(conn, miwen, 64, 0);
			fputs(buf, stdout);
			sum++;
			printf("-------sum=%d\n",sum);
		
		}else if(strcmp(buffer,"key1\n")==0  ||  strcmp(buffer,"key1")==0){//softether is ok ?
                        char l[]="key1ok\n";
                        send(conn, l, 50, 0);
                        fputs(l, stdout);
		}else if(strcmp(buffer,"listen\n")==0 || strcmp(buffer,"listen")==0){//client listen
                        //定义时间并初始化，判断是否有用户连接
                        time_t timep;
                        struct tm *p;
                        time(&timep);
                        p =localtime(&timep);
                        shijian  = p->tm_min;
                        printf("now min is---listen-----%d\n",p->tm_min);

			char ll[]="ok\n";
			send(conn, ll, 50, 0);
			fputs(ll, stdout);
		}else if(strcmp(buffer,"time\n")==0 || strcmp(buffer,"time")==0){//client is  connecting ?
                        //初始化本地时间
                        time_t timep;
                        struct tm *p;
                        time(&timep);
                        p =localtime(&timep);

                        //shijian  = p->tm_hour;
                        printf("now min is---time-----%d\n",p->tm_min);
                        if(p->tm_min >= shijian + 5)
                        {
                                char lll[]="timeok\n";
                                send(conn, lll, 50, 0);
                                fputs(lll, stdout);
                        }else{
                                char llll[]="timenook\n";
                                send(conn, llll, 50, 0);
                                fputs(llll, stdout);

                        }

                }else if(strcmp(buffer,"cert\n")==0 || strcmp(buffer,"cert")==0){//send cert
			char strkey[]="</key>";
			if(strstr(AllString,strkey)==NULL){
				printf("Get Certificate Again\n");
				char acc4[250][254]={0};
				//AllString[204800]={0};
				memset(AllString,0,sizeof(AllString));
		        	//打开文件
		        	if((fp=fopen("greensky-server.ovpn","a+"))==NULL)
		        	{
		        	        printf("-----cannot open this file on the out------\n");
		        	        exit(0);
		        	}else{
		        	        printf("--------open this file on the out-------\n");
		        	}

		        	//判断文件是否含有<a> and </ca>
		        	iiii=0;
		        	while(fgets(acc4[iiii],254,fp)!=NULL)
		        	{
                        	        char acc3[2];
                        	        acc3[1]='\0';
                        	        strncpy(acc3,acc4[iiii],1);
                        	        if((strcmp(acc3,"#")!=0) && (strcmp(acc3,";")!=0) && (strcmp(acc3,"\r")!=0))
                        	        {
                        	                printf("read dada file---%s\n",acc4[iiii]);
						strcat(AllString,acc4[iiii]);
                        	        }
                        	        iiii++;
		        	        //printf("read dada file---%s\n",acc1[iiii]);
					//strcat(AllString,acc1[iiii]);
		        	        //iiii++;
		        	}
		        	fclose(fp);
			
			}else{
				printf("AllString is complete\n");
                        	send(conn, AllString, 20480, 0);
				printf("%s\n",AllString);
                        	//fputs(acc1, stdout);
				
			}

                }else{
                        char lerror[]="error\n";
                        send(conn, lerror, 50, 0);
                        fputs(lerror, stdout);
                }


	}
	close(conn);
        close(server_sockfd);
	return 0;
}
