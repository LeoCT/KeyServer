#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void To2Bin(char p[8],int b[64])    //将字节转换成二进制流
{
	int i,k=0;

	for(i=0;i<8;i++) 
	{
		int j=0x80;
		for(;j;j>>=1)  
		{
			if(j&p[i]) 
			{
				b[k++]=1;     
			}
			else   
			{
				b[k++]=0;   
			}
		}
	}    
}

int IP_Table[64] =            //初始置换(IP)

{    
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7,
    56, 48, 40, 32, 24, 16, 8, 0,
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6
}; 

    
int E_Table[] = {			    	//扩展变换E
	   31,  0,  1,  2,  3,  4,
		3,  4,  5,  6,  7,  8,
		7,  8, 9, 10, 11, 12,
	   11, 12, 13, 14, 15, 16,
	   15, 16, 17, 18, 19, 20,
	   19, 20, 21, 22, 23, 24,
	   23, 24, 25, 26, 27, 28,
	   27, 28, 29, 30, 31,  0
};
                                   
int S_Box[8][4][16]  = {												//8个s盒
					{
						{14, 4,13, 1, 2,15,11, 8, 3,10, 6,12, 5, 9, 0, 7},
						{ 0,15, 7, 4,14, 2,13, 1,10, 6,12,11, 9, 5, 3, 8},
						{ 4, 1,14, 8,13, 6, 2,11,15,12, 9, 7, 3,10, 5, 0},
						{15,12, 8, 2, 4, 9, 1, 7, 5,11, 3,14,10, 0, 6,13}
					},

					{
						{15, 1, 8,14, 6,11, 3, 4, 9, 7, 2,13,12, 0, 5,10},
						{ 3,13, 4, 7,15, 2, 8,14,12, 0, 1,10, 6, 9,11, 5},
						{ 0,14, 7,11,10, 4,13, 1, 5, 8,12, 6, 9, 3, 2,15},
						{13, 8,10, 1, 3,15, 4, 2,11, 6, 7,12, 0, 5,14, 9}
					},

					{
						{10, 0, 9,14, 6, 3,15, 5, 1,13,12, 7,11, 4, 2, 8},
						{13, 7, 0, 9, 3, 4, 6,10, 2, 8, 5,14,12,11,15, 1},
						{13, 6, 4, 9, 8,15, 3, 0,11, 1, 2,12, 5,10,14, 7},
						{ 1,10,13, 0, 6, 9, 8, 7, 4,15,14, 3,11, 5, 2,12}
					},

					{
						{ 7,13,14, 3, 0, 6, 9,10, 1, 2, 8, 5,11,12, 4,15},
						{13, 8,11, 5, 6,15, 0, 3, 4, 7, 2,12, 1,10,14, 9},
						{10, 6, 9, 0,12,11, 7,13,15, 1, 3,14, 5, 2, 8, 4},
						{ 3,15, 0, 6,10, 1,13, 8, 9, 4, 5,11,12, 7, 2,14}
					},

					{
						{ 2,12, 4, 1, 7,10,11, 6, 8, 5, 3,15,13, 0,14, 9},
						{14,11, 2,12, 4, 7,13, 1, 5, 0,15,10, 3, 9, 8, 6},
						{4, 2, 1, 11,10,13, 7, 8,15, 9,12, 5, 6, 3, 0,14},
						{11,8,12, 7, 1,14, 2,13,  6,15, 0, 9,10, 4, 5, 3}
					},

					{
						{12, 1,10,15, 9, 2, 6, 8, 0,13, 3, 4,14, 7, 5,11},
						{10,15, 4, 2, 7,12, 9, 5, 6, 1,13,14, 0,11, 3, 8},
						{ 9,14,15, 5, 2, 8,12, 3, 7, 0, 4,10, 1,13,11, 6},
						{ 4, 3, 2,12, 9, 5,15,10,11,14, 1, 7, 6, 0, 8,13}
					},

					{
						{ 4,11, 2,14,15, 0, 8,13, 3,12, 9, 7, 5,10, 6, 1},
						{13, 0,11, 7, 4, 9, 1,10,14, 3, 5,12, 2,15, 8, 6},
						{ 1, 4,11,13,12, 3, 7,14,10,15, 6, 8, 0, 5, 9, 2},
						{ 6,11,13, 8, 1, 4,10, 7, 9, 5, 0,15,14, 2, 3,12}
					},

					{
						{13, 2, 8, 4, 6,15,11, 1,10, 9, 3,14, 5, 0,12, 7},
						{ 1,15,13, 8,10, 3, 7, 4,12, 5, 6,11, 0,14, 9, 2},
						{ 7,11, 4, 1, 9,12,14, 2, 0, 6,10,13,15, 3, 5, 8},
						{ 2, 1,14, 7, 4,10, 8,13,15,12, 9, 0, 3, 5, 6,11}
					}
				};

int IP_1_Table[64] =						//逆初始置换IP^-1
{ 
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25,
    32, 0, 40, 8, 48, 16, 56, 24
};

int P_Table[32] =				//置换运算P
{
	15,6,19,20,
	28,11,27,16,   
    0,14,22,25,
	4,17,30,9,   
    1,7,23,13,
	31,26,2,8,   
    18,12,29,5,
	21,10,3,24
}; 

int PC_1[56] =
 {
	56,48,40,32,24,16,8,   //密钥置换PC_1
    0,57,49,41,33,25,17,   
    9,1,58,50,42,34,26,   
    18,10,2,59,51,43,35,   
    62,54,46,38,30,22,14,   
    6,61,53,45,37,29,21,   
    13,5,60,52,44,36,28,   
    20,12,4,27,19,11,3
};   
  
int PC_2[48] =			//密钥置换PC_2
{
	13,16,10,23,0,4,
	2,27,14,5,20,9,
	22,18,11,3,25,7,
	15,6,26,19,12,1,   
    40,51,30,36,46,54,
	29,39,50,44,32,47,
	43,48,38,55,33,52,
	45,41,49,35,28,31
};  

void Replacement(int arry1[],int arry2[],int arry3[],int num)			//置换函数(初始IP，逆初始IP，
{
	int i,tmp;
	for(i=0;i<num;i++)
	{
		tmp=arry2[i];
		arry3[i]=arry1[tmp];
	}
}

int move_times[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};       //对左移位的规定

void lif_move(int arry1[],int arry2[],int n)     //左移位实现函数
{
	int i;
	for(i=0;i<28;i++)
	{
		arry2[i]=arry1[(n+i)%28];
	}
}

int K[16][48];			//存放16轮子密钥
int c[64];				//存放明文或密文
int L[17][32],R[17][32];  //存放加密过程中左右部分
char dddd[48]={0};
char ffff[48]={0};

void SubKey(int K0[64])			//子密钥产生函数
{
	int i;
	int K1[56],K2[56];
	int C[17][28],D[17][28];

	Replacement(K0,PC_1,K1,56);	//密钥置换PC_1
	for(i=0;i<28;i++)		//将PC_1输出的56比特分为左右两部分
	{
		C[0][i]=K1[i];
		D[0][i]=K1[i+28];
	}
	i=0;
	while(i<16)
	{
		int j;
		lif_move(C[i],C[i+1],move_times[i]);
		lif_move(D[i],D[i+1],move_times[i]);
		for(j=0;j<28;j++)
		{
			K2[j]=C[i+1][j];
			K2[j+28]=D[i+1][j];
		}
		Replacement(K2,PC_2,K[i],48);		//密钥置换PC_2
		i++;
	}
	/*printf("\n子密钥生成过程中，左边生成的值:");
	for(i=0;i<17;i++)
	{
		int j;
		printf("\nC[%d]:",i);
		for(j=0;j<28;j++)
		{
			if(j%7==0)
			{
				printf(" ");
			}
			printf("%d",C[i][j]);
		}
	}
	printf("\n子密钥生成过程中，右边生成的值:");
	for(i=0;i<17;i++)
	{
		int j;
		printf("\nD[%d]:",i);
		for(j=0;j<28;j++)
		{
			if(j%7==0)
			{
				printf(" ");
			}
			printf("%d",D[i][j]);
		}
	}*/
}

void S_compress(int arry[],int shc[])	//S盒压缩变换，其中数组shc存放经过s盒的结果
{
	int h,l;			//行，列
	int sha[8];            //存放经过s盒的十进制结果
	int i,j;
	int temp[4];

	for(i=0;i<8;i++) //s盒压缩变换
	{
		h=arry[(1+(i*6))-1]*2 + arry[(6+(i*6))-1];
		l =arry[(2+(i*6))-1]*8 + arry[(3+(i*6))-1]*4 + arry[(4+(i*6))-1]*2 + arry[(5+(i*6))-1]; 
		sha[i]=S_Box[i][h][l];
	} 
	for(i=0;i<8;i++)
	{
	   for(j=3;j>=0;j--)
	   {
		   temp[j]=sha[i]%2;
		   sha[i]=sha[i]/2;
	   }
	   for(j=0;j<4;j++)
	   {
		   shc[4*i+j]=temp[j];
	   }
    }
	/*printf("\n第%d次s盒的输出:",m++);
	for(i=0;i<32;i++)
	{
		if(i%8==0)
		{
			printf(" ");
		}
		printf("%d",shc[i]);
	}*/
}

void To10(int a[], int b[],int n)//二进制转十进制
{
	int i,j;
	int temp;
	int arry[16][4];

	for(i=0;i<n/4;i++)
	{
		for(j=0;j<4;j++)
		{
			arry[i][j]=a[4*i+j];
		}
	}
	for (i=0;i<n/4;i++)
	{
		temp=arry[i][0]*8+arry[i][1]*4+arry[i][2]*2+arry[i][3]*1;
		/*for(j=3;j>=0;j--)
		{
			if(arry[i][j]==1)
			{
				t=1;
				for(k=0;k<3-j;k++)
				{
					t=t*2;
				}
				temp+=t;
			}
		}*/
		b[i]=temp;
	}	
}

void To102(int a[], int b[],int n)//二进制转十进制
{
	int i,j;
	int temp;
	int arry[8][8];
	int t=1,k;

	for(i=0;i<n/8;i++)
	{
		for(j=0;j<8;j++)
		{
			arry[i][j]=a[8*i+j];
		}
	}
	for (i=0;i<n/8;i++)
	{
		temp=0;
		for(j=7;j>=0;j--)
		{
			if(arry[i][j]==1)
			{
				t=1;
				for(k=0;k<7-j;k++)
				{
					t=t*2;
				}
				temp+=t;
			}
		}
		b[i]=temp;
	}	
}

void F_Function(int a[32],int b[32],int n)		//F函数
{
	int i;
	int tmp[48];
	int tep[32];

	Replacement(a,E_Table,tmp,48);			//扩展变换E
	/*printf("\n第%d轮E盒扩展结果:",n);
	for(i=0;i<48;i++)
	{
		if(i%8==0)
			printf(" ");
		printf("%d",tmp[i]);
	}*/
	for(i=0;i<48;i++)					//与子密钥异或
	{
		tmp[i] ^= K[n][i];
	}
	/*printf("\n进入S盒的48比特:");
	for(i=0;i<48;i++)
	{
		if(i%6==0)
		{
			printf(" ");
		}
		printf("%d",tmp[i]);
	}*/
	S_compress(tmp,tep);              //压缩变换S

	Replacement(tep,P_Table,b,32);   //置换运算P
	/*printf("\n第%d次P置盒输出:",l++);
	for(i=0;i<32;i++)
	{
		if(i%8==0)
			printf(" ");
		printf("%d",b[i]);
	}*/
	/*printf("\nf[%d]的输出结果:",n);
	for(i=0;i<32;i++)
	{
		if(i%8==0)
		{
			printf(" ");
		}
		printf("%d",b[i]);
	}*/
}

void Encryption(int m0[64],int c1[64])
{
	int i,k;
	int arry[32];
	int c0[64],m1[64];

	Replacement(m0,IP_Table,m1,64);  //初始置换IP
	/*printf("\n初始置换:");
	for(i=0;i<64;i++)
	{
		if(i%8==0)
			printf(" ");
		printf("%d",m1[i]);
	}*/
	for(i=0;i<32;i++)
	{
		L[0][i]=m1[i];
		R[0][i]=m1[i+32];
	}
	k=1;
	while(k<17)
	{
		F_Function(R[k-1],arry,k-1);
		for(i=0;i<32;i++)
		{
			L[k][i]=R[k-1][i];
			R[k][i]=L[k-1][i]^arry[i];
		}
		k++;
	}
	for(i=0;i<32;i++)
	{
		c0[i]=R[16][i];
		c0[i+32]=L[16][i];
	}
	Replacement(c0,IP_1_Table,c1,64);   //逆初始置换
}

void changeKey(int a[16][48])
{
	int i,j;
	int tmp[16][48];
	
	for(i=0;i<16;i++)
	{
		for(j=0;j<48;j++)
		{
			tmp[i][j]=a[i][j];
		}
	}
	for(i=0;i<16;i++)
	{
		for(j=0;j<48;j++)
		{
			K[i][j]=tmp[15-i][j];
		}
	}

}
void Decryption(int c1[],int m[])
{
	int c0[64],t[64];
	int i,k;
	int arry[32];

	changeKey(K);
	/*printf("\n交换后的密钥:\n");
	for(i=0;i<16;i++)
	{
		printf("\n");
		for(j=0;j<48;j++)
		{
			if(j%8==0)
			{
				printf(" ");
			}	
			printf("%d",K[i][j]);
		}
	}*/

	Replacement(c1,IP_Table,c0,64);		//初始IP
	for(i=0;i<32;i++)
	{
		L[0][i]=c0[i];
		R[0][i]=c0[i+32];
	}
	k=1;
	while(k<17)
	{
		F_Function(R[k-1],arry,k-1);
		for(i=0;i<32;i++)
		{
			L[k][i]=R[k-1][i];
			R[k][i]=L[k-1][i]^arry[i];
		}
		k++;
	}
	for(i=0;i<32;i++)
	{
		t[i]=R[16][i];
		t[i+32]=L[16][i];
	}
	Replacement(t,IP_1_Table,m,64);   //逆初始置换
}



int main1(char s1[8],char dddd[48])
{
	int num,i;
	int t = 0;
	char s2[10]={"12345678"};
	int m[64];  //m存放二进制明文/密文
	int d[64];  //存放二进制密钥
	int s[8];

	//while(strlen(s1)!=17)
	//{
		//printf("请输入明文:");
		//scanf("%s",s1);
	//}
	To2Bin(s1,m);				//将明文转换成二进制流
	/*while(strlen(s2)!=8)
	{
		printf("请输入密钥(8):");
		scanf("%s",s2);
	}*/
	To2Bin(s2,d);				//将密钥转换成二进制
	/*printf("\n初始二进制明文:");
	for(i=0;i<64;i++)
	{
		printf("%d",m[i]);
	}*/
	/*printf("\n初始二进制密钥:");
	for(i=0;i<64;i++)
	{
		printf("%d",d[i]);
	}*/
	SubKey(d);
	/*printf("\n16轮子密钥如下:");
	for(i=0;i<16;i++)
	{
		printf("\nK[%d]:",i+1);
		for(j=0;j<48;j++)
		{
			if(j%8==0)
				printf(" ");
			printf("%d",K[i][j]);
		}
	}*/
	
	Encryption(m,c);
	//printf("-----------%s\n",c);
	/*printf("\n16次迭代左结果:");
	for(i=0;i<17;i++)
	{
		printf("\nL[%d]:",i);
		for(j=0;j<32;j++)
		{
			if(j%8==0)
		{
			printf(" ");
		}
			printf("%d",L[i][j]);
		}
	}
	printf("\n16次迭代右结果:");
	for(i=0;i<17;i++)
	{
		printf("\nR[%d]:",i);
		for(j=0;j<32;j++)
		{
				if(j%8==0)
		{
			printf(" ");
		}
			printf("%d",R[i][j]);
		}
	}*/

	int ii;
	int a[12],b[12],dd[3][16];
	int p[64],q[64];
	
	for(ii=0;ii<32;ii++)
	{
		p[ii]=L[15][ii];
		p[32+ii]=R[15][ii];
		q[ii]=R[16][ii];
		q[ii+32]=L[16][ii];
	}
	To10(K[14],a,48);
	To10(K[15],b,48);
	To10(c,dd[0],64);
	To10(p,dd[1],64);
	To10(q,dd[2],64);
	/*printf("\n15轮密钥:");
	for(i=0;i<12;i++)
	{
		printf("%x",a[i]);
	}
	printf("\n15轮结果:");
	for(i=0;i<16;i++)
	{
		printf("%X",d[1][i]);
	}
	printf("\n16轮密钥:");
	for(i=0;i<12;i++)
	{
		printf("%X",b[i]);
	}
	printf("\n16轮结果:");
	for(i=0;i<16;i++)
	{
		printf("%X",d[2][i]);
	}*/
	printf("\n最后结果:\n");
	char ddd[48]={0};

	for(ii=0;ii<16;ii++)
	{
		
		//ddd[i] = d[0][i];
		printf("%X",dd[0][ii]);
		sprintf(ddd,"%X",dd[0][ii]);
		//strcpy(dddd,ddd);
		dddd[ii] = *ddd;
	}
	printf("\n*****ddd*****%s\n",dddd);

	/*printf("\n二进制密文:");
	for(i=0;i<64;i++)
	{
		printf("%d",c[i]);
	}*/
	//scanf("%d",&t);
	/*if(t==0)
	{
		int s[8];
		int a[64];
		//printf("-----------%d\n",*c);
		Decryption(c,a);
		/*printf("\n解密后的二进制:");
		for(i=0;i<64;i++)
		{
			printf("%d",a[i]);
		}*/
		/*To102(a,s,64);
		printf("\n解密结果:");
		for(i=0;i<8;i++)
		{
			printf("%c",s[i]);
		}
	}
	printf("\n");*/

}

int main2(char dddd[48],char ffff[8])
{
	int i;
	int s[8];
	int a[64];
	//printf("-----------%d\n",*c);
	Decryption(c,a);
	/*printf("\n解密后的二进制:");
	for(i=0;i<64;i++)
	{
		printf("%d",a[i]);
	}*/
	To102(a,s,64);
	printf("\n解密结果:");
	char fff[48]={0};
	for(i=0;i<8;i++)
	{
		printf("%c",s[i]);
		sprintf(fff,"%c",s[i]);
		ffff[i] = *fff;
	}
	printf("\n*****fff*****%s\n",ffff);


}



