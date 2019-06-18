#include "GSRSA.h"

#include<stdio.h>
#include<string.h>
#include <stdlib.h>   
#include <time.h>  
#include <math.h>
#include <malloc.h>

#include "GSStrUtil.hpp"

#define MAX_KEY_LEN	     500
#define RSA_MAX          100
#define START_POS        11

class CLargeInt  
{
public:
	CLargeInt();
	~CLargeInt();

	static void CreateRandom(CLargeInt &n, CLargeInt &q);
	static bool RSACreate( CLargeInt &p,CLargeInt & q,CLargeInt &e,CLargeInt &d,CLargeInt &n);	
	static void Encode(CLargeInt &n,CLargeInt &d, const char *pSource, int iSourceLen, char *pTarget, int &iTargetLen);
	static void Decode(CLargeInt &n,CLargeInt &d, const char *pSource, int iSourceLen, char *pTarget, int &iTargetLen);
	unsigned long _len;
	int _data[RSA_MAX]; 
};


int PackKey(char** pKey, int &nKey, CLargeInt &Key1, CLargeInt &Key2)
{
	unsigned long i = 0;
	int nKeyInt[MAX_KEY_LEN] = {0};
	for(i=0; i<MAX_KEY_LEN; i++)
	{
		nKeyInt[i] = rand();
	}
	unsigned long iPos = START_POS;
	nKeyInt[iPos++] = Key1._len;
	for(i = 0; i < Key1._len; i++)
	{
	//	iPos++; 
		nKeyInt[iPos++] = Key1._data[i];
	}
	nKeyInt[iPos++] = Key2._len;
	for(i = 0; i < Key2._len; i++)
	{
//		iPos++;
		nKeyInt[iPos++] = Key2._data[i];
	}

	nKey = sizeof(nKeyInt);
	*pKey = (char*)malloc(nKey);
	memcpy(*pKey, nKeyInt, nKey);

	return 0;
}

int ParserKey(const char* pKey, CLargeInt &Key1, CLargeInt &Key2)
{
	unsigned long i = 0;

	int nKeyInt[MAX_KEY_LEN] = {0};
	memcpy(nKeyInt, pKey, sizeof(nKeyInt));

	unsigned long iPos = START_POS;
	Key1._len = nKeyInt[iPos++];
	for(i = 0; i < Key1._len; i++)
	{
	//	iPos++;
		Key1._data[i] = nKeyInt[iPos++];
	}

	Key2._len = nKeyInt[iPos++];
	for(i = 0; i < Key2._len; i++)
	{
//		iPos++;
		Key2._data[i] = nKeyInt[iPos++];
	}
	
	return 0;
}

int GSRSA_CreateKey(char**pPublicKey,  int &nPublicKey,
					char**pPrivateKey, int &nPrivateKey)
{
	CLargeInt p,q;
	CLargeInt::CreateRandom(p,q);

	CLargeInt e, d, n;
	CLargeInt::RSACreate(p,q,e,d,n);

	PackKey(pPublicKey, nPublicKey, n, e);
	PackKey(pPrivateKey, nPrivateKey, n, d);

	return 0;
}


int GSRSA_EncodeData(const char* pPrivateKey, const char* pData, int nDataSize,
					 char** pOutputData, int& nOutputSize)
{
	CLargeInt e, n;

	int iRet = ParserKey(pPrivateKey, n, e);
	if (0 != iRet)
		return iRet;


	char* pEncodeData = (char*)malloc(1024*1024);
	if (NULL == pEncodeData)
	{
		return -1;
	}

	CLargeInt::Encode(n, e, pData, nDataSize, pEncodeData,  nOutputSize);

	*pOutputData = (char*)malloc(nOutputSize);
	if (NULL == * pOutputData)
	{
		return -1;
	}
	memcpy(*pOutputData, pEncodeData, nOutputSize);
	free(pEncodeData);

	return 0;
}

int GSRSA_DecodeData(const char* pPublicKey, const char* pData, int nDataSize,
					 char** pOutputData, int& nOutputSize)
{
	CLargeInt d, n;

	int iRet = ParserKey(pPublicKey, n, d);
	if (0 != iRet)
		return iRet;


	char* pDecodeData = (char*)malloc(1024*1024);
	if (NULL == pDecodeData)
	{
		return -1;
	}

	CLargeInt::Decode(n, d, pData, nDataSize, pDecodeData, nOutputSize);

	*pOutputData = (char*)malloc(nOutputSize);
	if (NULL == pDecodeData)
	{
		return -1;
	}
	memcpy(*pOutputData, pDecodeData, nOutputSize);
	free(pDecodeData);
	
	return 0;
}


int GSRSA_EncodeFile(char* pPrivateKey, char const* pSrcFile, const char* pDstFile)
{
	return -1;
}

int GSRSA_DecodeFile(char* pPublicKey, const char* pSrcFile, const char* pDstFile)
{
	return -1;
}





#define LEN sizeof(struct slink)
void sub(int a[RSA_MAX],int b[RSA_MAX] ,int c[RSA_MAX] );

struct slink
{ 
	int  bignum[RSA_MAX];
	struct slink *next;
};

int  cmp(int a1[RSA_MAX],int a2[RSA_MAX])
{ 
	int l1, l2;
	int i;
	l1=a1[RSA_MAX-1];
	l2=a2[RSA_MAX-1];
	if (l1>l2)
		return 1;
	if (l1<l2)
		return -1;
	for(i=(l1-1);i>=0;i--)
	{
		if (a1[i]>a2[i])
			return 1 ;
		if (a1[i]<a2[i])
			return -1;
	}
	return 0;
}

void mov(int a[RSA_MAX],int *b)
{
	int j;
	for(j=0;j<RSA_MAX;j++)
		b[j]=a[j];

	return ;
}


void mul(int a1[RSA_MAX],int a2[RSA_MAX],int *c)
{
	int i,j;
	int y;
	int x;
	int z;
	int w;
	int l1, l2;
	l1=a1[RSA_MAX-1];
	l2=a2[RSA_MAX-1];
	if (a1[RSA_MAX-2]=='-'&& a2[RSA_MAX-2]=='-')
		c[RSA_MAX-2]=0;
	else if (a1[RSA_MAX-2]=='-')
		c[RSA_MAX-2]='-';
	else if (a2[RSA_MAX-2]=='-')
		c[RSA_MAX-2]='-';
	for(i=0;i<l1;i++)
	{
		for(j=0;j<l2;j++)
		{
			x=a1[i]*a2[j];
			y=x/10;
			z=x%10;
			w=i+j;
			c[w]=c[w]+z;
			c[w+1]=c[w+1]+y+c[w]/10;
			c[w]=c[w]%10;
		}
	}
	w=l1+l2;
	if(c[w-1]==0)w=w-1;
	c[RSA_MAX-1]=w;
	return;
} 

void add(int a1[RSA_MAX],int a2[RSA_MAX],int *c)
{

	int i,l1,l2;
	int len,temp[RSA_MAX];
	int k=0;
	l1=a1[RSA_MAX-1];
	l2=a2[RSA_MAX-1];
	if((a1[RSA_MAX-2]=='-')&&(a2[RSA_MAX-2]=='-'))
	{
		c[RSA_MAX-2]='-';
	}
	else if (a1[RSA_MAX-2]=='-')
	{
		mov(a1,temp);
		temp[RSA_MAX-2]=0;
		sub(a2,temp,c);
		return;
	}
	else if (a2[RSA_MAX-2]=='-')
	{
		mov(a2,temp);
		temp[RSA_MAX-2]=0;
		sub(a1,temp,c);
		return;
	}

	if(l1<l2)len=l1;
	else len=l2;
	for(i=0;i<len;i++)
	{
		c[i]=(a1[i]+a2[i]+k)%10;
		k=(a1[i]+a2[i]+k)/10;
	}
	if(l1>len)
	{
		for(i=len;i<l1;i++)
		{
			c[i]=(a1[i]+k)%10;
			k=(a1[i]+k)/10;   
		}
		if(k!=0)
		{
			c[l1]=k;
			len=l1+1;
		}
		else len=l1;
	}
	else
	{
		for(i=len;i<l2;i++)
		{
			c[i]=(a2[i]+k)%10;
			k=(a2[i]+k)/10;   
		}
		if(k!=0)
		{
			c[l2]=k;
			len=l2+1;
		}
		else len=l2;
	}
	c[99]=len;
	return;
} 


void sub(int a1[RSA_MAX],int a2[RSA_MAX],int *c)
{
	int i,l1,l2;
	int len = 0,t1[RSA_MAX],t2[RSA_MAX];
	int k=0;
	l1=a1[RSA_MAX-1];
	l2=a2[RSA_MAX-1];
	if ((a1[RSA_MAX-2]=='-') && (a2[RSA_MAX-2]=='-'))
	{
		mov(a1,t1);
		mov(a2,t2);
		t1[RSA_MAX-2]=0;
		t2[RSA_MAX-2]=0;
		sub(t2,t1,c);
		return;
	}
	else if( a2[RSA_MAX-2]=='-')
	{
		mov(a2,t2);
		t2[RSA_MAX-2]=0;
		add(a1,t2,c);
		return;
	}
	else if (a1[RSA_MAX-2]=='-')
	{
		mov(a2,t2);
		t2[RSA_MAX-2]='-';
		add(a1,t2,c);
		return;
	}

	if(cmp(a1,a2)==1)
	{

		len=l2;
		for(i=0;i<len;i++)
		{
			if ((a1[i]-k-a2[i])<0)
			{
				c[i]=(a1[i]-a2[i]-k+10)%10;
				k=1;
			}
			else 
			{
				c[i]=(a1[i]-a2[i]-k)%10;
				k=0;
			}
		}


		for(i=len;i<l1;i++)
		{
			if ((a1[i]-k)<0)
			{
				c[i]=(a1[i]-k+10)%10;
				k=1;
			}
			else 
			{
				c[i]=(a1[i]-k)%10;
				k=0;
			} 
		}
		if(c[l1-1]==0)
		{
			len=l1-1;
			i=2;
			while (c[l1-i]==0)
			{
				len=l1-i;
				i++;
			}

		}

		else 
		{
			len=l1;
		}
	}
	else
		if(cmp(a1,a2)==(-1))
		{
			c[RSA_MAX-2]='-';
			len=l1;
			for(i=0;i<len;i++)
			{
				if ((a2[i]-k-a1[i])<0)
				{
					c[i]=(a2[i]-a1[i]-k+10)%10;
					k=1;
				}
				else 
				{
					c[i]=(a2[i]-a1[i]-k)%10;
					k=0;
				}
			}
			for(i=len;i<l2;i++)
			{
				if ((a2[i]-k)<0)
				{
					c[i]=(a2[i]-k+10)%10;
					k=1;
				}
				else 
				{
					c[i]=(a2[i]-k)%10;
					k=0;
				}   
			}
			if(c[l2-1]==0)
			{  
				len=l2-1;
				i=2;
				while (c[l1-i]==0)
				{
					len=l1-i;
					i++;
				}

			}

			else len=l2;

		}

		else if(cmp(a1,a2)==0)
		{
			len=1;
			c[len-1]=0;
		}
		c[RSA_MAX-1]=len;
		return;
}

void  mod(int a[RSA_MAX],int b[RSA_MAX],int  *c)
{	
	int d[RSA_MAX];
	mov (a,d);
	while (cmp(d,b)!=(-1))
	{
		sub(d,b,c);
		mov(c,d);
	}	
	return ;
}


void  divt(int t[RSA_MAX],int b[RSA_MAX],int  *c ,int *w)
{

	int a1,b1,i,j,m;
	int d[RSA_MAX],e[RSA_MAX],f[RSA_MAX],g[RSA_MAX],a[RSA_MAX];

	mov(t,a);
	for(i=0;i<RSA_MAX;i++)
		e[i]=0;
	for(i=0;i<RSA_MAX;i++)
		d[i]=0;
	for(i=0;i<RSA_MAX;i++) g[i]=0;
	a1=a[RSA_MAX-1];
	b1=b[RSA_MAX-1];
	if (cmp(a,b)==(-1))
	{
		c[0]=0;
		c[RSA_MAX-1]=1;
		mov(t,w);
		return;
	}
	else if (cmp(a,b)==0)
	{
		c[0]=1;
		c[RSA_MAX-1]=1;
		w[0]=0;
		w[RSA_MAX-1]=1;
		return;
	}
	m=(a1-b1);
	for(i=m;i>=0;i--)/*341245/3=341245-300000*1--->41245-30000*1--->11245-3000*3--->2245-300*7--->145-30*4=25--->25-3*8=1*/
	{
		for(j=0;j<RSA_MAX;j++)
			d[j]=0;
		d[i]=1;
		d[RSA_MAX-1]=i+1;
		mov(b,g);
		mul(g,d,e);


		while (cmp(a,e)!=(-1))
		{
			c[i]++;
			sub(a,e,f);
			mov(f,a);
		}

		for(j=i;j<RSA_MAX;j++)
			e[j]=0;

	}
	mov(a,w);
	if (c[m]==0) c[RSA_MAX-1]=m;
	else c[RSA_MAX-1]=m+1;

	return;
}

void mulmod(int a[RSA_MAX] ,int b[RSA_MAX] ,int n[RSA_MAX],int *m)
{
	int c[RSA_MAX],d[RSA_MAX];
	int i;
	for(i=0;i<RSA_MAX;i++)
		d[i]=c[i]=0;
	mul(a,b,c);
	divt(c,n, d,m);
	for(i=0;i<m[RSA_MAX-1];i++)
		printf("%d",m[m[RSA_MAX-1]-i-1]);
	printf("\nm  length is :  %d \n",m[RSA_MAX-1]);
}


void expmod(int a[RSA_MAX] ,int p[RSA_MAX] ,int n[RSA_MAX],int *m)
{
	int t[RSA_MAX],l[RSA_MAX],temp[RSA_MAX]; 
	int w[RSA_MAX],s[RSA_MAX],c[RSA_MAX],b[RSA_MAX],i;
	for(i=0;i<RSA_MAX-1;i++)
		b[i]=l[i]=t[i]=w[i]=0;
	t[0]=2;t[RSA_MAX-1]=1;
	l[0]=1;l[RSA_MAX-1]=1;
	mov(l,temp);
	mov(a,m);
	mov(p,b);

	while(cmp(b,l)!=0)
	{
		for(i=0;i<RSA_MAX;i++)
			w[i]=c[i]=0;

		divt(b,t,w,c);

		mov(w,b);

		if(cmp(c,l)==0) 
		{
			for(i=0;i<RSA_MAX;i++)
				w[i]=0;

			mul(temp,m,w);

			mov(w,temp);

			for(i=0;i<RSA_MAX;i++)
				w[i]=c[i]=0;

			divt(temp,n,w,c);

			mov(c,temp);
		}

		for(i=0;i<RSA_MAX;i++)
			s[i]=0;

		mul(m,m,s);

		for(i=0;i<RSA_MAX;i++)
			c[i]=0;

		divt(s,n,w,c);

		mov (c,m);

	}

	for(i=0;i<RSA_MAX;i++)
		s[i]=0;

	mul(m,temp,s);

	for(i=0;i<RSA_MAX;i++)
		c[i]=0;

	divt(s,n,w,c);

	mov (c,m);

	m[RSA_MAX-2]=a[RSA_MAX-2];

	return;

}


int   is_prime_san(int p[RSA_MAX] )
{

	int i,a[RSA_MAX],t[RSA_MAX],s[RSA_MAX],o[RSA_MAX]; 
	for(i=0;i<RSA_MAX;i++)
		s[i]=o[i]=a[i]=t[i]=0;
	t[0]=1;
	t[RSA_MAX-1]=1;
	a[0]=2;// { 2,3,5,7 }
	a[RSA_MAX-1]=1;

	sub(p,t,s);

	expmod ( a, s, p ,o);
	if ( cmp(o,t) != 0 ) 
	{
		return 0;
	}

	a[0]=3;
	for(i=0;i<RSA_MAX;i++)  o[i]=0;

	expmod ( a, s, p ,o);
	if ( cmp(o,t) != 0 ) 	   
	{
		return 0;
	}

	a[0]=5;
	for(i=0;i<RSA_MAX;i++)  o[i]=0;

	expmod ( a, s, p ,o);
	if ( cmp(o,t) != 0 ) 
	{	   
		return 0;
	}

	a[0]=7;
	for(i=0;i<RSA_MAX;i++)  o[i]=0;

	expmod ( a, s, p ,o);

	if ( cmp(o,t) != 0 ) 
	{

		return 0;
	}

	return 1;
}


int coprime(int e[RSA_MAX],int s[RSA_MAX])

{
	int a[RSA_MAX],b[RSA_MAX],c[RSA_MAX],d[RSA_MAX],o[RSA_MAX],l[RSA_MAX];
	int i;
	for(i=0;i<RSA_MAX;i++)
		l[i]=o[i]=c[i]=d[i]=0;
	o[0]=0;o[RSA_MAX-1]=1;
	l[0]=1;l[RSA_MAX-1]=1;
	mov(e,b);
	mov(s,a);
	do
	{
		if(cmp(b,l)==0)
		{
			return 1;
		}
		for(i=0;i<RSA_MAX;i++)
			c[i]=0;
		divt(a,b,d,c);
		mov(b,a);/*b--->a*/
		mov(c,b);/*c--->b*/

	}while(cmp(c,o)!=0);
	/*	printf("Ihey are  not coprime!\n");*/
	return 0;
}


void prime_random(int *p,int *q)
{
	int i,k;
	time_t t; 
	p[0]=1;
	q[0]=3;

	p[RSA_MAX-1]=10;
	q[RSA_MAX-1]=11;


	do
	{
		t=time(NULL);
		srand((unsigned long)t);
		for(i=1;i<p[RSA_MAX-1]-1;i++)
		{
			k=rand()%10;
			p[i]=k;
		}
		k=rand()%10;
		while (k==0)
		{
			k=rand()%10;
		}
		p[p[RSA_MAX-1]-1]=k;

		printf(">");

	}while((is_prime_san(p))!=1);
	do
	{
		t=time(NULL);
		srand((unsigned long)t);
		for(i=1;i<q[RSA_MAX-1];i++)
		{
			k=rand()%10;
			q[i]=k;
		}
		printf(">");
	}while((is_prime_san(q))!=1);

	return;
}

void  erand(int e[RSA_MAX],int m[RSA_MAX])
{
	int i,k;
	time_t t;
	e[RSA_MAX-1]=5;
	do
	{
		t=time(NULL);
		srand((unsigned long)t);
		for(i=0;i<e[RSA_MAX-1]-1;i++)
		{
			k=rand()%10;
			e[i]=k;
		}
		while((k=rand()%10)==0)
			k=rand()%10;
		e[e[RSA_MAX-1]-1]=k;
	}while(coprime( e, m)!=1);
}

void rsad(int e[RSA_MAX],int g[RSA_MAX],int *d)
{
	int   r[RSA_MAX],n1[RSA_MAX],n2[RSA_MAX],k[RSA_MAX],w[RSA_MAX];
	int     i,t[RSA_MAX],b1[RSA_MAX],b2[RSA_MAX],temp[RSA_MAX];
	mov(g,n1);
	mov(e,n2);
	for(i=0;i<RSA_MAX;i++)
		k[i]=w[i]=r[i]=temp[i]=b1[i]=b2[i]=t[i]=0;
	b1[RSA_MAX-1]=0;b1[0]=0;/*/b1=0;*/
	b2[RSA_MAX-1]=1;b2[0]=1;/*/b2=1;*/
	while(1)
	{

		for(i=0;i<RSA_MAX;i++)
			k[i]=w[i]=0;
		divt(n1,n2,k,w);/*/k=n1/n2;*/
		for(i=0;i<RSA_MAX;i++)
			temp[i]=0;
		mul(k,n2,temp);/*/temp=k*n2;*/
		for(i=0;i<RSA_MAX;i++)
			r[i]=0;
		sub(n1,temp,r);

		if((r[RSA_MAX-1]==1) && (r[0]==0))/*/r=0*/
		{
			break;
		}
		else
		{
			mov(n2,n1);/*/n1=n2;*/
			mov( r,n2);/*/n2=r;*/
			mov(b2, t);/*/t=b2;*/
			for(i=0;i<RSA_MAX;i++)
				temp[i]=0;
			mul(k,b2,temp);/*/b2=b1-k*b2;*/
			for(i=0;i<RSA_MAX;i++)
				b2[i]=0;
			sub(b1,temp,b2);
			mov(t,b1);
		}
	}

	for(i=0;i<RSA_MAX;i++)
		t[i]=0;
	add(b2,g,t);
	for(i=0;i<RSA_MAX;i++)
		temp[i]=d[i]=0;
	divt(t,g,temp,d);
}





unsigned long  rsa(unsigned long p,unsigned long q,unsigned long e)  
{
	unsigned long g,k,r,n1,n2,t;
	unsigned long b1=0,b2=1;

	g=(p-1)*(q-1);
	n1=g;
	n2=e;

	while(1)
	{
		k=n1/n2;
		r=n1-k*n2;
		if(r!=0)
		{
			n1=n2;
			n2=r;
			t=b2;
			b2=b1-k*b2;
			b1=t;
		}

		else
		{
			break;
		}

	}

	return (g+b2)%g;
}


CLargeInt::CLargeInt()
{
	memset(_data, 0, sizeof(int) * RSA_MAX);
	_len = RSA_MAX;
}


CLargeInt::~CLargeInt()
{

}

void CLargeInt::CreateRandom(CLargeInt &p, CLargeInt &q)
{
	prime_random(p._data, q._data);
}

bool CLargeInt::RSACreate(CLargeInt &p,CLargeInt &	q,CLargeInt &e,CLargeInt &d,CLargeInt &n)
{
	CLargeInt p1, q1, m;

	mul(p._data,q._data,n._data);

	mov(p._data, p1._data);
	p1._data[0]--;      
	mov(q._data, q1._data);
	q1._data[0]--;

	mul(p1._data, q1._data, m._data);
	erand(e._data, m._data);
	rsad(e._data, m._data, d._data);

	return true;
}

void CLargeInt::Encode(CLargeInt &n,CLargeInt &d, const char *pSource, int iSourceLen, char *pTarget, int &iTargetLen)
{
	int i,k,count,temp,c;
	struct slink  *p,*p1,*p2;
	struct slink  *h;
	h=p=p1=p2=(struct slink * )malloc(LEN);
	h=NULL;

	count=0;
	for (int iPos = 0; iPos < iSourceLen; iPos++)
	{  
		c=pSource[iPos];
		k=0;
		if(c<0)
		{
			c=abs(c);/*/把负数取正并且做一个标记*/
			p1->bignum[RSA_MAX-2]='0';
		}
		else
		{
			p1->bignum[RSA_MAX-2]='1';
		}

		while(c/10!=0)
		{
			temp=c%10;
			c=c/10;
			p1->bignum[k]=temp;
			k++;
		}
		p1->bignum[k]=c;
		p1->bignum[RSA_MAX-1]=k+1;
		count=count+1;
		if(count==1)
			h=p1;
		else p2->next=p1;
		p2=p1;
		p1=(struct slink * )malloc(LEN);
	}
	p2->next=NULL; 


	p=p1=(struct slink * )malloc(LEN);
	p=h;

	iTargetLen = 0;
	do 
	{ 
		expmod( p->bignum , d._data,n._data ,p1->bignum);

		pTarget[iTargetLen++] = (char ) p1->bignum[RSA_MAX-2];

		if ((p1->bignum[RSA_MAX-1]/10) ==0)
		{
			pTarget[iTargetLen++]=0+48;

			pTarget[iTargetLen++]= (char ) p1->bignum[RSA_MAX-1]+48;

		}
		else
		{
			pTarget[iTargetLen++]=(char )p1->bignum[RSA_MAX-1]/10+48;
			pTarget[iTargetLen++]=(char )p1->bignum[RSA_MAX-1]%10+48;
		}

		for(i=0;i<p1->bignum[RSA_MAX-1];i++)
		{
			pTarget[iTargetLen++]=(char )p1->bignum[i]+48;
		}
		p=p->next;
		p1=(struct slink * )malloc(LEN);
	}while(p!=NULL);


}

void CLargeInt::Decode(CLargeInt &n,CLargeInt &e, const char *pSource, int iSourceLen, char *pTarget, int &iTargetLen)
{

	struct slink *h,*p1,*p2;
	int i,j,k,c,count,temp = 0;
	i=0;
	j=3;
	count=0;
	h=p1=p2=(struct slink * )malloc(LEN);

	for (int iPos = 0; iPos < iSourceLen; iPos++)
	{  
		c=pSource[iPos];

		if(j==3)
		{
			p1->bignum[RSA_MAX-2]=c;
			j--;
		}
		else if(j==2)
		{
			temp=c-48;
			j--;
		}
		else if(j==1)
		{
			p1->bignum[RSA_MAX-1]=temp*10+c-48;
			j--;
		}
		else if (j==0)
		{
			p1->bignum[i]=c-48;
			i++;
			if(i==p1->bignum[RSA_MAX-1])
			{ 
				i=0;
				j=3;
				count++;
				if (count==1)
					h=p1;
				else p2->next=p1;
				p2=p1;
				p1=(struct slink * )malloc(LEN);
			}
		}
	}
	p2->next=NULL; 


	p2=(struct slink * )malloc(LEN);
	p1=h;
	k=0;

	iTargetLen = 0;
	do
	{
		for(i=0;i<RSA_MAX;i++)
			p2->bignum[i]=0;
		expmod( p1->bignum , e._data ,n._data ,p2->bignum);		  
		temp=p2->bignum[0]+p2->bignum[1]*10+p2->bignum[2]*RSA_MAX;
		if (( p2->bignum[RSA_MAX-2])=='0')
		{
			temp=0-temp;
		}	 
		pTarget[iTargetLen++]=(char )temp;
		k++;
		p1=p1->next;
		p2=(struct slink * )malloc(LEN);
	}while (p1!=NULL);

}



