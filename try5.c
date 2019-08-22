//try1
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<pthread.h>
#include<string.h>


void view()
{
  int fd,n;
  char fname[100],buffer;
  printf("enter the file name\n");
  scanf("%s",&fname);
  fd=open(fname,O_RDONLY);
  if(fd>0)
  {
    system("clear");
    while(read(fd,&buffer,1)>0)
    {
      write(1,&buffer,1);
    }
  }
  else
  {
    printf("file does not exist\n");
  }
}
void upper(char a[])
{
  int l;
  l=strlen(a);
  for(int i=0;i<l;i++)
  {
    int n=a[i];
    if(n>90)//if lower case
    {
      n=n-32;
      a[i]=n;
    }
  }
}

int cmp(char a[], char b[])
{
  int al,bl,len,flag=0,lflag=0,i;
  al=strlen(a);
  bl=strlen(b);
  //printf("comparing %s and %s \n",a,b);
  //printf("length a is %d and b is %d \n",al,bl);
  upper(a);
  upper(b);
  if(al==(bl-1))
  lflag=1;
  if(lflag==1)
  {
    for(i=0;i<al-1;i++)
    {
      if(a[i]==b[i])
      flag=1;
      else
      flag=0;
      //printf("compared %c and %c flag is %d \n",a[i],b[i],flag);
    }
    return flag;
  }
  else
  {
    //printf("in else\n");
    memset(b,'\0',1024*sizeof(char));
    return 0;
  }
}


int search(char sname[])//for searching a book
{
  //system("clear");
  int sl=0,d1,i,j;
  int flag=1;
  char ext[]=".txt";
  char fl=1,buffer;
  char check[1024]={0};
  //char sname[100];
  char fname[5];//file name
  // printf("enter the name of book to search\n");
  // gets(sname);//book name
  printf("\n");
  sl=strlen(sname);
  char c=sname[0];
  int ck=c;
  if(ck>90)//if lower case
  {
    ck=ck-32;
    c=ck;
    sname[0]=c;
  }
  fname[0]=c;
  for(j=1;j<5;j++)//com   bining for file name
  fname[j]=ext[j-1];
  //printf("%s\n",fname);
  //strcat(fname,ext);
  d1=open(fname,O_RDONLY);
  for(i=0;i<sl;i++)
  if(sname[i]==' ')//replaceing space with underscore
    sname[i]='_';
  while(read(d1,&buffer,1)>0)
  {
    if(fl==1)//indicates new line
    {
      flag=cmp(sname,check);
      if(flag==1)
      break;
      fl=0;
      i=0;
    }
    if(buffer=='\n')//checking is next line is new
    {
      fl=1;
    }
    check[i++]=buffer;
  };
  flag=cmp(sname,check);//comparison to last word in file
  //printf("%d flag is \n",flag);
  if(flag==1)
  printf("available in library\n");
  else
  printf("not available in library\n");
  getchar();
  return 0;
}

void *rw(void *i)
{
  int f = (int *)i;
  char c = 65 + f;//creating capital letter
  char c2=c+32;//creating small letter
  char name[5];
  int n;
  char buffer;
  int fl=1,fc=0;//fl for line change,fc for checking to write
  char lo;//first letter of line
  name[0]=c;
  char ext[]=".txt";
  strcat(name,ext);//combining for file name
  int d1,d2;
  d1=open(name,O_WRONLY|O_CREAT,0777);
  lseek(d1,0,SEEK_END);
  d2=open("book.txt",O_RDONLY);
  while(read(d2,&buffer,1)>0)
  {
    if(fl==1)//that is new line
    {
      lo=buffer;
      fl=0;
      fc=0;
    }
    if(buffer=='\n')//checking is next line is new
    {
      fl=1;
    }

    if(lo==c||lo==c2)//checking first letter
    fc=1;

    if(buffer==' ')
    buffer='_';

    if(fc==1)
    {
      write(d1,&buffer,1);
    }
  };
}

void main()
{
  int choice,k,l;
  pthread_t t[26];
  int i=0;
  int ex;
  char sname[100],temp;//book list source
  while(1)
  {
      system("clear");
      printf("\t\t\t E-Library Management System\n\n\n\n");
      printf("Menu\n");
      printf("1. Enter Book List\n");
      printf("2. Search Book Availability \n");
      printf("3. View Book\n");
      printf("4. Exit\n");
      scanf("%d",&choice);

    switch (choice)
    {
        case 1:
                system("clear");
                for(i=0;i<26;i++)
                pthread_create(&t[i],NULL,&rw,(void*)i);
                for(i=0;i<26;i++)
                pthread_join(t[i],NULL);
                printf("Done\n");
                sleep(5);
                break;
        case 2:
          system("clear");
          printf("enter the name of book to search\n");
          scanf("%s",&sname);//book name
          search(sname);
          sleep(2);
          break;

        case 3: view();
                sleep(5);
                printf("\nPress 1 to exit\n");
                scanf("%d",&ex);
                break;
        case 4:
          exit(0);
          break;
          default:
            printf("Wrong Choice. Enter again\n");
            getchar();
    }
  };
}
