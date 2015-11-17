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

#define MYPORT  8886
#define QUEUE   20
#define BUFFER_SIZE 1024
 
char s[100000];//存放用户名、密码、预共享密钥
FILE * fp;
//定义数组存放用户的帐号
char acc[100000][30] ;
int account_number=0;
char miwen[64];

int main()
{
	void recopy_empty_file();
	void add_account_and_password();
	void encrypt_account_and_password();
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

	//定义计算用户帐号的数量
	int sum = 0; 
	//判断是否连接成功
	int conn;
	
	///客户端套接字
	char buffer[BUFFER_SIZE];
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);

	//重新拷贝空文件
	recopy_empty_file();
	//添加用户名、密码、预共享密钥
	add_account_and_password();
	char tmp6[254];
	sprintf(tmp6,"ipsec restart");
	system(tmp6);

	while(1)
	{	
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
				encrypt_account_and_password();
				send(conn, miwen, 64, 0);//通过sochet向客户端发送密文
				//fputs(buf, stdout);

			}else{
				printf("time is diffrent\n");
				shijian = p->tm_hour;//重新定义时间

				recopy_empty_file();
				add_account_and_password();

				char tmp5[254];
				sprintf(tmp5,"ipsec restart");
				system(tmp5);

				account_number=0;
				encrypt_account_and_password();
				send(conn, miwen, 64, 0);//通过sochet向客户端发送密文
				//fputs(buf, stdout);

			}

		
		}else if(strcmp(buffer,"key2\n")==0  ||  strcmp(buffer,"key2")==0)
                {
                        char l[]="key2ok\n";
                        send(conn, l, 50, 0);
                        fputs(l, stdout);

		}else if(strcmp(buffer,"listen\n")==0 || strcmp(buffer,"listen")==0){
			char ll[]="ok\n";
			send(conn, ll, 50, 0);
			fputs(ll, stdout);
		}else if(strcmp(buffer,"timess\n")==0 || strcmp(buffer,"timess")==0){
                        //初始化本地时间
                        time_t timep;
                        struct tm *p;
                        time(&timep);
                        p =localtime(&timep);

                        //shijian  = p->tm_hour;
                        printf("now min is---time-----%d\n",p->tm_min);
                        if(p->tm_min >= shijian + 1)
                        {
                                char lll[]="timessok\n";
                                send(conn, lll, 50, 0);
                                fputs(lll, stdout);
                        }else{
                                char llll[]="timessnook\n";
                                send(conn, llll, 50, 0);
                                fputs(llll, stdout);

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

//重新拷贝空文件
void recopy_empty_file()
{
	char tmp0[254];
	sprintf(tmp0,"cp ./new/data  ./data");
	system(tmp0);

	char tmp1[254];
	sprintf(tmp1,"cp ./new/ipsec.secrets  ./ipsec.secrets");
	system(tmp1);
}

//添加用户名、密码、预共享密钥
void add_account_and_password()
{
	char pool[]=
	{
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

	int sum0;   	
	for(sum0=0;sum0<10;sum0++)
	{
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

		char tmp2[254];
		sprintf(tmp2,"%s \%%any : XAUTH \\\"%s\\\"",account,pwd);
		printf("----------%s\n",tmp2);

		char tmp3[254];
		sprintf(tmp3,"echo %s >> ipsec.secrets",tmp2);
		system(tmp3);

		char tmp4[254];
		sprintf(tmp4,"cp ipsec.secrets /etc");
		system(tmp4);

	        char buf[254];
	        char prsec[10] = {"cl123456"};
	        sprintf(buf,"%s,%s",account,pwd);
		sprintf(s,"%s,%s",buf,prsec);
		strcpy(acc[sum0] , s);

		//打开存储用户账户的文档
		if((fp=fopen("data","a+"))==NULL)
		{
			printf("-----cannot open this file on the in------\n");
			exit(0);
		}else{
			printf("--------open this file on the in-------");
		}

		fputs(acc[sum0],fp);
		fputs("\n",fp);
		fclose(fp);

		printf("-------用户名、密码、预共享密钥=%s\n",acc[sum0]);

	}
}

//发送一个密文
void encrypt_account_and_password()
{
	sprintf(s,"%s",acc[account_number]);
	account_number++;//逐渐提取用户密码预共享密钥
	if(account_number == 10)
	{
		account_number=0;
	}
	//把字符串拆分成每个都是8位的字符串
	int jjj;
	jjj = strlen(s);
	printf("printff-------acc[account_number]=%s\n",acc[account_number]);
	printf("printff-------sssssssss=%s\n",s);
	printf("printff-------fdddddf%d\n",jjj);
	//char s[50]="userName,Password";
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
		int i;
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
	//main1(MyMessage,MesHex);
	main1(s1,dddd);
	//printf("Encrypted!----buf-----:%s\n",buf);

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
}
