#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Admistrator_Login(int sockfd);//管理员登
void Admistrator_Register(int sockfd);//管理员注册
void Admistrator_Delete(int sockfd);//管理员注销
void Parking_Add(int sockfd);    //增加停车场
void Parking_Delete(int sockfd);    // 删除停车场
void Parking_Update(int sockfd);    // 更新停车场
//Parking_Select_Id(int sockfd);  //根据Id查询停车场的信息

void User_Register(int sockfd)
{

    char *username = (char*)malloc(sizeof(char)*20);
    char *password = (char*)malloc(sizeof(char)*20);
    char *carid = (char*)malloc(sizeof(char)*20);
    char *phone = (char*)malloc(sizeof(char)*20);
    char *mail = (char*)malloc(sizeof(char)*30);
    printf("请输入用户名：");
    scanf("%s",username);
    printf("请输入密码：");
    scanf("%s",password);
    printf("请输入车牌号：");
    scanf("%s",carid);
    printf("请输入电话号码：");
    scanf("%s",phone);
    printf("请输入邮箱：");
    scanf("%s",mail);
    char buff1[200]={0};
     sprintf(buff1,"UR,%s,%s,%s,%s,%s",username,password,carid,phone,mail);
    //printf("%s\n",buff1);
      send(sockfd,buff1,strlen(buff1),0);
      printf("sockfd=%d\n",sockfd);
      char buff2[128]={0};
      recv(sockfd,buff2,127,0);
      printf("ser:%s\n",buff2);
      free(username);
      free(password);
      free(carid);
      free(phone);
      free(mail);
}
 
int User_Login(int sockfd)
{
    char *username = (char*)malloc(sizeof(char)*20);
    char *password = (char*)malloc(sizeof(char)*20);
    printf("请输入用户名：");
    scanf("%s",username);
    printf("请输入密码：");
    scanf("%s",password);
    char buff1[200]={0};
    sprintf(buff1,"UL,%s,%s",username,password);
    send(sockfd,buff1,strlen(buff1),0);
    char buff2[128]={0};
    recv(sockfd,buff2,127,0);
    if(strncmp(buff2,"yes",3)==0)
    {
	printf("登录成功！\n");
	return 1;
    }
    if(strncmp(buff2,"no",2)==0)
    {
	printf("帐号或密码错误，请重新选择！\n");
	return 0;
    }
    free(username);
    free(password);
}
int Car_Add(int sockfd)
{
    char *carid = (char*)malloc(sizeof(char)*20);
    char *username = (char*)malloc(sizeof(char)*20);
    printf("请输入车牌号: ");
    scanf("%s",carid);
    printf("请输入用户名：");
    scanf("%s",username);
    char buff3[200] = {0};
    sprintf(buff3,"CA,%s,%s",carid,username);
    send(sockfd,buff3,strlen(buff3),0);
    char buff4[128]={0};
    recv(sockfd,buff4,127,0);
    printf("%s\n",buff4);
   // printf("添加成功！"\n);
    free(carid);
    free(username);
}
int Car_Delete(int sockfd)
{
    
    char *carid = (char*)malloc(sizeof(char)*20);
    printf("请输入要删除的车牌号: ");
    scanf("%s",carid);
    char buff5[200] = {0};
    sprintf(buff5,"CD,%s",carid);
    send(sockfd,buff5,strlen(buff5),0);
    char buff6[128]={0};
    recv(sockfd,buff6,127,0);
    printf("%s\n",buff6);
    free(carid);
}
int Car_Change(int sockfd)
{
    char *phone = (char*)malloc(sizeof(char)*20);
    char *carid = (char*)malloc(sizeof(char)*20);
    printf("请输入手机号： ");
    scanf("%s",phone);
    printf("请输入新的车牌号： ");
    scanf("%s",carid);
    char buff7[200] = {0};
    sprintf(buff7,"CU,%s,%s",carid,phone);
    send(sockfd,buff7,strlen(buff7),0);
    char buff8[128]={0};
    recv(sockfd,buff8,127,0);
    printf("%s\n",buff8);
    free(phone);
    free(carid);
}

int User_Delete(int sockfd)
{
    char *username =  (char*)malloc(sizeof(char)*20);
    printf("请输入用户名：");
    scanf("%s",username);
    char buff9[200] = {0};
    sprintf(buff9,"UD,%s",username);
    send(sockfd,buff9,strlen(buff9),0);
    char buff10[128]={0};
    recv(sockfd,buff10,127,0);
    printf("%s\n",buff10);
    free(username);
    exit(0);
}

int User_Search_id(int sockfd)
{
    char *mail =  (char*)malloc(sizeof(char)*20);
    printf("请输入邮箱：");
    scanf("%s",mail);
    char buff9[200] = {0};
    sprintf(buff9,"USi,%s",mail);
    send(sockfd,buff9,strlen(buff9),0);
    char buff10[128]={0};
    recv(sockfd,buff10,127,0);
    printf("%s\n",buff10);
    free(mail);
}

int User_Search_num(int sockfd)
{
    char *username = (char*)malloc(sizeof(char)*20);
    printf("请输入用户名：");
    scanf("%s",username);
    char buff1[200]={0};
    sprintf(buff1,"USn,%s",username);
    send(sockfd,buff1,strlen(buff1),0);
    char buff2[128]={0};
    recv(sockfd,buff2,127,0);
    printf("电话号码为：%s\n",buff2);
    free(username);
}


int User_Change_num(int sockfd)
{
    char *phone1 = (char*)malloc(sizeof(char)*20);
    char *phone2 = (char*)malloc(sizeof(char)*20);
    printf("请输入原手机号: ");
    scanf("%s",phone1);
    printf("请输入新手机号：");
    scanf("%s",phone2);
    char buff3[200] = {0};
    sprintf(buff3,"UCn,%s,%s",phone2,phone1);
    send(sockfd,buff3,strlen(buff3),0);
    char buff4[128]={0};
    recv(sockfd,buff4,127,0);
    printf("%s\n",buff4);
    free(phone1);
    free(phone2);
}  

int User_Mail_change(int sockfd)
{
    char *mail1 = (char*)malloc(sizeof(char)*20);
    char *mail2 = (char*)malloc(sizeof(char)*20);
    printf("请输入原邮箱: ");
    scanf("%s",mail1);
    printf("请输入新的邮箱：");
    scanf("%s",mail2);
    char buff3[200] = {0};
    sprintf(buff3,"MN,%s,%s",mail2,mail1);
    send(sockfd,buff3,strlen(buff3),0);
    char buff4[128]={0};
    recv(sockfd,buff4,127,0);
    printf("%s\n",buff4);
    free(mail1);
    free(mail2);
}  

int User_Password_change(int sockfd)
{
    char *Password1 = (char*)malloc(sizeof(char)*20);
    char *Password2 = (char*)malloc(sizeof(char)*20);
    printf("请输入原密码: ");
    scanf("%s",Password1);
    printf("请输入新密码：");
    scanf("%s",Password2);
    char buff3[200] = {0};
    sprintf(buff3,"UPc,%s,%s",Password2,Password1);
    send(sockfd,buff3,strlen(buff3),0);
    char buff4[128]={0};
    recv(sockfd,buff4,127,0);
    printf("%s\n",buff4);
    free(Password1);
    free(Password2);
}  
void  Admistrator_Register(int sockfd)//管理员注册
 {
     char *username = (char*)malloc(sizeof(char)*20);
     char *password = (char*)malloc(sizeof(char)*20);
     printf("请输入用户电话号码：");
     scanf("%s",username);
     printf("请输入密码：");
     scanf("%s",password);
     char buff1[200]={0};
     sprintf(buff1,"AR,%s,%s",username,password);
     send(sockfd,buff1,strlen(buff1),0);
     printf("%s\n",buff1);
     char buff2[128]={0};
     recv(sockfd,buff2,127,0);
     printf("ser:%s\n",buff2);
     if(strncmp(buff2,"yes",3)==0)
     {
         printf("登录成功！\n");
        // return 1;
     }
     if(strncmp(buff2,"no",2)==0)
     {
         printf("帐号或密码错误，请重新选择！\n");
        // return 0;
     }
     free(username);
     free(password);
 }

void  Admistrator_Delete(int sockfd)//管理员注销
 {
     char *username = (char*)malloc(sizeof(char)*20);
     char *password = (char*)malloc(sizeof(char)*20);
     printf("请输入用户电话号码：");
     scanf("%s",username);
     printf("请输入密码：");
     scanf("%s",password);
     char buff1[200]={0};
     sprintf(buff1,"AD,%s,%s",username,password);
     send(sockfd,buff1,strlen(buff1),0);
    // printf("%s\n",buff1);
     char buff2[128]={0};
     recv(sockfd,buff2,127,0);
     printf("ser:%s\n",buff2);
     free(username);
     free(password);
 }

int  Admistrator_Login(int sockfd)//管理员登录
 {
     char *username = (char*)malloc(sizeof(char)*20);
     char *password = (char*)malloc(sizeof(char)*20);
     printf("请输入用户电话号码：");
     scanf("%s",username);
     printf("请输入密码：");
     scanf("%s",password);
     char buff1[200]={0};
     sprintf(buff1,"AL,%s,%s",username,password);
     send(sockfd,buff1,strlen(buff1),0);
    // printf("%s\n",buff1);
     char buff2[128]={0};
     recv(sockfd,buff2,127,0);
     //printf("ser:%s\n",buff2);
     if(strncmp(buff2,"yes",3)==0)
     {
         printf("登录成功！\n");
         return 1;
     }
     if(strncmp(buff2,"no",2)==0)
     {
         printf("电话号或密码错误，请重新选择！\n");
         return 0;
     }
     free(username);
     free(password);
 }


void Parking_Select_Id(int sockfd) //根据名字查询停车场的信息
{
     char *p_name= (char*)malloc(sizeof(char)*20);
     printf("请输入用户名：");
     scanf("%s",p_name);
     char buff1[200]={0};
     sprintf(buff1,"PS,%s",p_name);
     send(sockfd,buff1,strlen(buff1),0);
    // printf("%s\n",buff1);
     char buff2[128]={0};
     recv(sockfd,buff2,127,0);//判断数据库是否查询成功
     char *p=strtok(buff2,",");
     printf("%s\n",p);
     p=strtok(NULL,",");
     while(p!=NULL)
    {
       printf("%s ",p);
       p=strtok(NULL,",");
    }
     printf("\n");
     
    //  printf("ser:%s\n",buff2);
 
     if(strncmp(buff2,"yes",3)==0)
     {
        // printf("查询成功！\n");
       //   memset(buff2,0,128);
       // recv(sockfd,buff2,127,0);
      //  printf("%s\n",buff2);
        // char buff3[128]={0};
        // recv(sockfd,buff3,127,0);
        // char*s=strtok(s," ");
        // while(s!=NULL)
      /*  {
          printf("%s ",s);
          s=strtok(NULL," ");
        }*/
       // printf("%s",buff3);

     }
     if(strncmp(buff2,"no",2)==0)
     {
         printf("帐号错误，请重新选择！\n");
     }
    // char buff3[128]={0};
    // int a = recv(sockfd,buff3,127,0);

     //printf("%s\n",buff3);
     free(p_name);
}


void  Parking_Increase(int sockfd)//增加停车场信息
 {
     char *username = (char*)malloc(sizeof(char)*20);
     char *capacity = (char*)malloc(sizeof(char)*20);
     char *site = (char*)malloc(sizeof(char)*20);
     char *charge = (char*)malloc(sizeof(char)*20);
     char *tel = (char*)malloc(sizeof(char)*20);
     printf("请输入停车场名称：");
     scanf("%s",username);
     printf("请输入可容纳车辆：");
     scanf("%s",capacity);
     printf("请输入位置：");
     scanf("%s",site);
     printf("请输入收费标准：");
     scanf("%s",charge);
     printf("请输入联系方式：");
     scanf("%s",tel);
     char buff1[200]={0};
     sprintf(buff1,"PI,%s,%s,%s,%s,%s",username,capacity,site,charge,tel);
     send(sockfd,buff1,strlen(buff1),0);
     printf("%s\n",buff1);
     char buff2[128]={0};
     recv(sockfd,buff2,127,0);
     printf("ser:%s\n",buff2);
     free(username);
     free(capacity);
     free(site);
     free(charge);
     free(tel);
 }

void  P_capacity(int sockfd)//更新车位息
 {
     //char *capacity = (char*)malloc(sizeof(char)*20);
     //char *P_name = (char*)malloc(sizeof(char)*20);
    // printf("判断停车场是否存在：请输入停车场电话名称：");
     //scanf("%s",P_name);
    // char buff[200]={0};
     //sprintf(buff,"PS,%s",P_name);
     //send(sockfd,buff,strlen(buff),0);
     //char buff3[128]={0};
     //recv(sockfd,buff3,127,0);
    // printf("%s\n",buff3); 
    // printf("请输入停车场名称：");
    // scanf("%s",P_name);
    // char buff1[200]={0};
    // sprintf(buff1,"PA,%s,%s",P_name,capacity);
    // send(sockfd,buff1,strlen(buff1),0);
    // printf("%s\n",buff1);
     //char buff2[128]={0};
     //recv(sockfd,buff2,127,0);
    // printf("ser:%s\n",buff2);
    char buff1[200] = {0};
    scanf("%s",buff1);
    send(sockfd,buff1,strlen(buff1),0);
    char buff2[200] = {0};
    recv(sockfd,buff2,127,0);
    printf("\n");
    printf("%s\n",buff2);
    // free(P_name);
     //free(capacity);
 }
void Parking_Delete(int sockfd)   // 删除停车场
{
     char *username = (char*)malloc(sizeof(char)*20);
     printf("请输入停车场名称：");
     scanf("%s",username);
     char buff1[200]={0};
     sprintf(buff1,"PD,%s",username);
     send(sockfd,buff1,strlen(buff1),0);
     printf("%s\n",buff1);
     char buff2[128]={0};
     recv(sockfd,buff2,strlen(buff2),0);
     printf("ser:%s\n",buff2);
     free(username);
}

void Parking_Update(int sockfd)//更新停车场
{
   char *oldname = (char*)malloc(sizeof(char)*20);
   char *newname = (char*)malloc(sizeof(char)*20);
   printf("请输入停车场旧名称：");
   scanf("%s",oldname);
   printf("请输入停车场新名称：");
   scanf("%s",newname);
   char buff1[128]={0};
   sprintf(buff1,"PU,%s,%s",oldname,newname);
     send(sockfd,buff1,strlen(buff1),0);
     printf("%s\n",buff1);
     char buff2[128]={0};
     recv(sockfd,buff2,127,0);
     printf("ser:%s\n",buff2);
     free(oldname);
     free(newname);  
}

int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    assert( sockfd != -1 );
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);//1024 
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int res = connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    assert( res != -1 );
    char buff[128] = {0};
    
   //看是用户还是管理员
   while(1)
  {
     printf("**********************************\n");
     printf(" 1-----------用户----------- \n");
     printf("2-----------管理员-----------\n");
     fgets(buff,128,stdin);
     setbuf(stdin,NULL);
      if(strncmp(buff,"1",1)==0)
    {
    int n=0;
      while(1)
    {
    printf("********************************************\n");
    printf("输入---------------功能---------------------\n");
    printf(" 1-----------用户注册------------\n");
    printf(" 2-----------用户登录--------------\n");
    printf("end-----------用户注销--------------\n");


    setbuf(stdin,NULL);
    fgets(buff,128,stdin);
    buff[strlen(buff)-1] = 0;

    if(strncmp(buff,"1",1)==0)
    {
        User_Register(sockfd);
    }
    
    if ( strncmp(buff,"end",3) == 0)
    {
        break;
    }
    
    if(strncmp(buff,"2",1)==0)
    {
	 n =User_Login(sockfd);

	if(n)
	{
	    while(1)
	    {
		printf("-------****-欢迎进入车辆界面 ****---------------------\n");
		printf("1-----------录入车牌------------\n");
		printf("2-----------删除车牌--------------\n");
		printf("3-----------修改车辆信息------------\n");
		printf("4-----------用户删除---------------------\n");
		printf("5-----------用户电话查询---------------------\n");
		printf("6-----------用户邮箱修改---------------\n");
		printf("7-----------用户密码修改----------------\n");
		printf("8-----------退出---------------------\n");
                printf("9----------查询空位信息-------------\n"); 
		setbuf(stdin,NULL);
		fgets(buff,128,stdin);
		buff[strlen(buff) -1 ] = 0;
		if( strncmp(buff,"1",1) == 0)
		{
		    Car_Add(sockfd);
		}
		if(strncmp(buff,"2",1)==0)
		{
		    Car_Delete(sockfd);
		}
		if(strncmp(buff,"3",1)==0)
		{
		    Car_Change(sockfd);
		}   
		if(strncmp(buff,"8",1)==0)
		{
		    exit(0);
		}
		if(strncmp(buff,"4",1)==0)
		{
		    User_Delete(sockfd);;
		}
		if(strncmp(buff,"5",1)==0)
		{
		    User_Search_num(sockfd);
		}
		if(strncmp(buff,"6",1)==0)
		{
		    User_Mail_change(sockfd);
	    }
		if(strncmp(buff,"7",1)==0)
                {
                   User_Password_change(sockfd);
                 }
                if(strncmp(buff,"9",1)==0)
                {

                  P_capacity(sockfd);
                }
        } 
}
 }
}       

    }
  else
{
    while(1)
    {
    setbuf(stdin,NULL);
    printf("********************************************\n");
    printf("输入---------------功能---------------------\n");
    printf("1-----------停车场管理员注册------------\n");
    printf("2-----------停车场管理员登录--------------\n");
    printf("3-----------管理员注销------------\n");
    printf("end---------退出管理员界面----------\n");
    
    fgets(buff,128,stdin);
    setbuf(stdin,NULL);
   // send(sockfd,buff,strlen(buff),0);
    if(strncmp(buff,"1",1)==0)
    {
        Admistrator_Register(sockfd);

    }
    if(strncmp(buff,"3",1)==0)
    {
      Admistrator_Delete(sockfd);
    // if(n==1)
    // printf("登录成功");
   //  else
     
    }


   if(strncmp(buff,"2",1)==0)
    {
	int n=Admistrator_Login(sockfd);
	if(n==1)
        {
   
     

 while(1)
        {
            printf("-------*欢迎进入停车场界面*-------\n");
            printf("1-------增加停车场信息-----------\n");
            printf("2-------修改停车场信息-----------\n");
            printf("3----根据停车场名字查询停车场信息---\n");
            printf("4------删除停车场信息------------\n");
            printf("bye----退出此界面---------------\n");
            setbuf(stdin,NULL);
            memset(buff,0,128);
            fgets(buff,128,stdin);
            send(sockfd,buff,strlen(buff),0);
            if(strncmp(buff,"1",1)==0)
            {
               Parking_Increase(sockfd);
            }
            
            if(strncmp(buff,"2",1)==0)
            {
               Parking_Update(sockfd);
            }
            if(strncmp(buff,"3",1)==0)
            {
	       Parking_Select_Id(sockfd);
              
            }

            if(strncmp(buff,"4",1)==0)
            {
                Parking_Delete(sockfd);
            }
           
            if(strncmp(buff,"bye",3)==0)
            {
                break;
            }
        }
        }
       }
    
    if ( strncmp(buff,"end",3) == 0 )
    { 
        break;
     }
   // send(sockfd,buff,strlen(buff),0);
    //memset(buff,0,128);
   // recv(sockfd,buff,127,0);
   // printf("buff=%s\n",buff);
 }
}
}

    close(sockfd);
    exit(0);
}
