#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>
#include<event.h>
#include<signal.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<mysql/mysql.h>

#define MAXFD 100

struct event*  fd_event[MAXFD];//存描述副和指针的对应关系

MYSQL mysql;

typedef struct Data
{
    char *value;//存放字符串
}Data;

typedef struct Seqlist
{
    Data *str;
    int cutsize;//当前长度，以及存放逗号个数
    int maxsize;//最大容量
}*PSeqlist,Seqlist;

PSeqlist Init_list()
{
    PSeqlist L;
    L = (PSeqlist )(malloc (sizeof(PSeqlist)));
    L->str = (Data*)(malloc(sizeof(Data)*10));
    L->str->value = (char*)(malloc(sizeof(char)*20));
    L->cutsize = 1;
    L->maxsize = 10;
    return L;
}

int IsFull(PSeqlist L)
{
    return L->cutsize > L->maxsize;
}

void Extend(PSeqlist L) //扩容
{
    L->str = (Data*)realloc(L->str,sizeof(Data)*L->cutsize);
    L->maxsize = L->cutsize;
	
}

PSeqlist function(char *buff)//字符串切割
{
    PSeqlist L;
    L=Init_list();
    int i =0;
    for( ;i<strlen(buff);i++)
    {
	if(buff[i]==',')
	   L->cutsize++;
	
    }
    if(IsFull(L))
    {
		Extend(L);
    }
    char *ptr;
    ptr = strtok(buff,",");

    i = 0;
    while(ptr != NULL)
    {
		L->str[i].value = ptr;
   		i++;
		ptr = strtok(NULL,","); 
    }
    return L;
}
int create_socket();
void User_register(int sockfd,PSeqlist L);//注册
void User_Login(int sockfd,PSeqlist L);//用户登录
void Car_Add(int sockfd,PSeqlist L);//车牌增加
void Car_Delete(int sockfd,PSeqlist L);//车牌删除
void Car_Change(int sockfd,PSeqlist L);//车牌修改
void Car_Search_num(int sockfd,PSeqlist L);//车牌号查询
void User_Delete(int sockfd,PSeqlist L);//用户删除
void User_Search_id(int sockfd,PSeqlist L);//用户id查询
void User_Search_num(int sockfd,PSeqlist L);//用户电话查询
void User_Change_num(int sockfd,PSeqlist L);//用户电话修改
void User_Mail_change(int sockfd,PSeqlist L);//用户邮箱修改
void User_Password_change(int sockfd,PSeqlist L);//用户密码
void recv_cb(int fd,short event,void *arg);
void listen_cb(int fd,short event,void *arg);
void Admistrator_Register(int sockfd,PSeqlist L);//管理员注册
void Admistrator_Delete(int sockfd,PSeqlist L);//管理员注销
void Admistrator_Login(int sockfd,PSeqlist L);//管理员登录
void Adminstrator_Password_Update(int sockfd,PSeqlist);//修改管理员密码
void Parking_Add(int sockfd,PSeqlist L);//增加停车场
void Parking_Delete(int sockfd,PSeqlist L);//删除停车场
void Parking_Update(int sockfd,PSeqlist L);//更新停车场信息
void Parking_Select_Id(int sockfd,PSeqlist L);//根据id查询停车场信息
void Resolve(int sockfd,PSeqlist L);//解析函数
void User_Parking_in(int sockfd,PSeqlist L);//用户端查询停车场所有信息
void User_Parking_m(int sockfd,PSeqlist L);//用户端根据车牌号查询自己的停车金额
void User_Parking_t(int sockfd,PSeqlist L);//用户端根据车牌号查询自己的停车时间
void User_Parking_Tinit(int sockfd,PSeqlist L);//用户停车时的初始化时间
void U_Capacity_add(int sockfd,PSeqlist L);//出车
void Parking_Capacity_Update(int sockfd,PSeqlist L);//修改停车场容量
void Select_User_Carid(int sockfd,PSeqlist L);


void Resolve(int sockfd,PSeqlist L)
{
   // PSeqlist L;  
   // L=Init_list(); //初始化顺序表
   // L = function(buff);//切割字符串存在顺序表中 其中L->str[0].value为函数符号
	
    if(!strcmp(L->str[0].value,"UL"))
	User_Login(sockfd,L);//用户登录 
    if(!strcmp(L->str[0].value,"UR"))
	User_register(sockfd,L);//用户注册
    if(!strcmp(L->str[0].value,"CA"))
	Car_Add(sockfd,L);//车辆增加
    if(!strcmp(L->str[0].value,"CD"))
	Car_Delete(sockfd,L);//车辆注销
    if(!strcmp(L->str[0].value,"CS"))
	Car_Search_num(sockfd,L);//车辆车牌号查询
    if(!strcmp(L->str[0].value,"CU"))
	Car_Change(sockfd,L);//车辆修改 
    if(!strcmp(L->str[0].value,"UD"))
	User_Delete(sockfd,L);//用户删除 
    if(!strcmp(L->str[0].value,"USi"))
	User_Search_id(sockfd,L);//用户id查询
    if(!strcmp(L->str[0].value,"USn"))
	User_Search_num(sockfd,L);//用户电话查询
    if(!strcmp(L->str[0].value,"UCn"))
	User_Change_num(sockfd,L);//用户电话修改
    if(!strcmp(L->str[0].value,"MN"))
	User_Mail_change(sockfd,L);//用户邮箱修改
    if(!strcmp(L->str[0].value,"UPc"))
	User_Password_change(sockfd,L);//用户密码修改  
    if(!strcmp(L->str[0].value,"AL"))
	Admistrator_Login(sockfd,L);//管理员登录
    if(!strcmp(L->str[0].value,"AR"))
	Admistrator_Register(sockfd,L);//管理员注册
	if(!strcmp(L->str[0].value,"APC"))
    Adminstrator_Password_Update(sockfd,L);//管理员密码修改
	if(!strcmp(L->str[0].value,"CID"))
    Select_User_Carid(sockfd,L);
    if(!strcmp(L->str[0].value,"PI"))
	Parking_Add(sockfd,L);    //增加停车场
    if(!strcmp(L->str[0].value,"PD"))
	Parking_Delete(sockfd,L);    // 删除停车场
    if(!strcmp(L->str[0].value,"PU"))
	Parking_Update(sockfd,L);    // 更新停车场
    if(!strcmp(L->str[0].value,"PS"))
	Parking_Select_Id(sockfd,L);  //根据Id查询停车场的信息*/
    if(!strcmp(L->str[0].value,"UPi"))
        User_Parking_in(sockfd,L);//用户端查询停车场所有信息，格式为P_name(第一行),P_capacity(第一行),P_site(第一行),P_charge(第一行),P_tel(第一行),P_name(第二行),P_capacity(第二行).....
    if(!strcmp(L->str[0].value,"UPm"))
        User_Parking_m(sockfd,L);//用户端根据车牌号查询自己的停车金额
    if(!strcmp(L->str[0].value,"UPt"))
        User_Parking_t(sockfd,L);//用户端根据车牌号查询自己的停车时间
	if(!strcmp(L->str[0].value,"UPT"))
		User_Parking_Tinit(sockfd,L);//用户停车时的初始化时间  需要客户端传递1.usrname 2.P_name   以及停车
	if(!strcmp(L->str[0].value,"UCa"))
		U_Capacity_add(sockfd,L);//出车并且删除P_parking表中的记录  需要客户端传递1.usrname
    
    if(L->str[0].value==NULL)
	exit(0);
}

int create_socket()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if ( sockfd == -1 )
    {
        return -1;
    }

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6500);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if ( res == -1 )
    {
        return -1;
    }

    listen(sockfd,5);
    return sockfd;
}

void fd_event_init()
{
    int i = 0;
    for( ;i < MAXFD; i++ )
    {
        fd_event[i] = NULL;
    }
}

void fd_event_add(int fd, struct event* ev)
{
    if ( fd >= 100 )
    {
        return;
    }

    fd_event[fd] = ev;
}

struct event* fd_event_find( int fd)
{
    if ( fd >= 100 )
    {
        return NULL;
    }

    return fd_event[fd];
}

void recv_cb(int fd,short event,void *arg)
{
    PSeqlist L;
    L=Init_list();
    if (event & EV_READ)
    {
		char buff[200] = {0};
		int num = recv(fd,buff,127,0);
		printf("%s\n",buff);
		L = function(buff);
		if(num == 0)
		{
			struct event *s = fd_event_find(fd);
			event_del(s);
			close(fd);
			return;
		}
    
    }
    Resolve(fd,L);
    
}
void listen_cb(int fd,short event,void *arg)
{
    struct event_base *base = (struct event_base*)arg;

    struct sockaddr_in caddr;
    int len = sizeof(caddr);//
    int c = accept(fd,(struct sockaddr*)&caddr,&len);
    if(c<0)
    {
		perror("accept error");
		return ;
    }
    printf("客户端描述符为 %d 的用户已连接服务器\n",c);    
    struct event *c_ev = event_new(base,c,EV_READ|EV_PERSIST,recv_cb,NULL);
    assert(c_ev != NULL);
   
   event_add(c_ev,NULL);
   fd_event_add(c,c_ev);
}
void User_register(int sockfd,PSeqlist L)//注册    +
{
    //printf("IM User_register\n");
    char sql_insert[200]={0};//保存插入语句     
    sprintf(sql_insert,"insert into person values('%s','%s','%s','%s','%s');",L->str[1].value,L->str[2].value,L->str[3].value,L->str[4].value,L->str[5].value);
    if(! mysql_real_query(&mysql,sql_insert,strlen(sql_insert)))
	send(sockfd,"yes",3,0);
    else
	send(sockfd,"no",2,0);
    
}
void User_Login(int sockfd,PSeqlist L)//登陆    +
{
    MYSQL_ROW row;
    MYSQL_RES *res_ptr;
    char sql_query[200]={0};//保存查询语句
    sprintf(sql_query,"select usrname,password from person where usrname='%s' AND password='%s';",L->str[1].value,L->str[2].value);
    int n = mysql_real_query(&mysql,sql_query,strlen(sql_query));
    if(n)
    {
	perror("Query failed");
    }
    else
    {
	res_ptr = mysql_use_result(&mysql);
	row = mysql_fetch_row(res_ptr);
    }
    if(row)
    {
	send(sockfd,"yes",3,0);
    }
    else
    {
	send(sockfd,"no",2,0);
    }
    mysql_free_result(res_ptr);

}
void Car_Add(int sockfd,PSeqlist L)//车牌增加     +
{
    
    char sql_query[200]={0};//保存查询语句
    sprintf(sql_query,"update  person set carid='%s' where usrname='%s';",L->str[1].value,L->str[2].value);
    if(!(mysql_real_query(&mysql,sql_query,strlen(sql_query))))
	send(sockfd,"yes",3,0);
    else
	send(sockfd,"no",2,0);
    
}
void Car_Delete(int sockfd,PSeqlist L)//车牌删除   +
{    
    char sql_query[200]={0};//保存查询语句
    sprintf(sql_query,"update person set carid='NULL' where carid='%s';",L->str[1].value);
    if(!(mysql_real_query(&mysql,sql_query,strlen(sql_query))))
	send(sockfd,"yes",3,0);
    else
	send(sockfd,"no",2,0);
    
}
void Car_Change(int sockfd,PSeqlist L)//车牌修改      +
{
    char sql_query[200]={0};//保存查询语句
    sprintf(sql_query,"update person set carid='%s' where phone='%s';",L->str[1].value,L->str[2].value);
   if(!(mysql_real_query(&mysql,sql_query,strlen(sql_query))))
	send(sockfd,"yes",3,0);
    else
	send(sockfd,"no",2,0);
}
void Car_Search_num(int sockfd,PSeqlist L)//车牌号查询
{
    MYSQL_ROW row;
    MYSQL_RES *res_ptr;
    char sql_query[200]={0};//保存查询语句
    sprintf(sql_query,"select carid from person where phone='%s';",L->str[1].value);
    int n = mysql_real_query(&mysql,sql_query,strlen(sql_query));
    if(n)
    {
	printf("yes\n");
    }
    else
    {
	res_ptr = mysql_use_result(&mysql);
	row = mysql_fetch_row(res_ptr);
    }
    
    send(sockfd,(char*)row,sizeof(row),0);
    printf("\n");
    
    mysql_free_result(res_ptr);
}


void User_Delete(int sockfd,PSeqlist L)//用户删除
{
    char sql_query[200]={0};//保存查询语句
    sprintf(sql_query,"delete from person where usrname='%s'",L->str[1].value);
    if((mysql_real_query(&mysql,sql_query,strlen(sql_query))))
	send(sockfd,"no",2,0);
    else
	send(sockfd,"yes",3,0);
}

void User_Search_id(int sockfd,PSeqlist L)//用户id查询
{
    char sql_query[200]={0};//保存查询语句
    sprintf(sql_query,"select phone,maik from person where mail='%s';",L->str[1].value);
    if((mysql_real_query(&mysql,sql_query,strlen(sql_query))))
	send(sockfd,"yes",3,0);
    else
	send(sockfd,"no",2,0);
}
void User_Search_num(int sockfd,PSeqlist L)//用户电话查询
{
    MYSQL_ROW row;
    MYSQL_RES *res_ptr;
    char sql_query[200]={0};//保存查询语句
    sprintf(sql_query,"select phone from person where usrname='%s';",L->str[1].value);
    int n = mysql_real_query(&mysql,sql_query,strlen(sql_query));
    if(n)
    {
	perror("Select failed");
    }
    else
    {
	res_ptr = mysql_store_result(&mysql);
        while(row = mysql_fetch_row(res_ptr))
	{
		send(sockfd,(char*)row[0],sizeof(row[0]),0);
	}			
	
        
    }
    
    mysql_free_result(res_ptr);
}

void User_Change_num(int sockfd,PSeqlist L)//用户电话修改    +
{
    char sql_query[200]={0};//保存查询语句
    sprintf(sql_query,"update  person set phone='%s' where phone='%s';",L->str[1].value,L->str[2].value);
    if((mysql_real_query(&mysql,sql_query,strlen(sql_query))))
	send(sockfd,"yes",3,0);
    else
	send(sockfd,"no",2,0);
}
void User_Mail_change(int sockfd,PSeqlist L)//用户邮箱    +
{
    char sql_query[200]={0};//保存查询语句
    sprintf(sql_query,"update  person set mail='%s' where mail='%s';",L->str[1].value,L->str[2].value);
    if((mysql_real_query(&mysql,sql_query,strlen(sql_query))))
	
	send(sockfd,"yes",3,0);
    else
	send(sockfd,"no",2,0);
}

void User_Password_change(int sockfd,PSeqlist L)//用户密码更改    +
{
    char sql_query[200]={0};//保存查询语句
    sprintf(sql_query,"update  person set password='%s' where password='%s';",L->str[1].value,L->str[2].value);
    if((mysql_real_query(&mysql,sql_query,strlen(sql_query))))

	send(sockfd,"yes",3,0);
    else
	send(sockfd,"no",2,0);
}



void Parking_Update(int sockfd,PSeqlist L)
{
     /*memset(buff,0,128);
     recv(fd,buff,127,0);
     const char* d=",";
     char *p_oldname=strtok(buff,d);
     char *p_newname=strtok(NULL,d);*/
     //printf("该客户端输入的旧的停车场名称为：%s,新的停车场名称为:%s",L->str[1].value,L->str[2].value);
     char sql_update[200] = {0};
     sprintf(sql_update,"update P_place set P_name='%s' where P_name='%s';",L->str[2].value,L->str[1].value);
     int res= mysql_real_query(&mysql,sql_update,strlen(sql_update));
     if(res)//删除数据库里面没有的数据还输出成功，所以有问题
         send(sockfd,"no",2,0);
     else
         send(sockfd,"yes",3,0);

}

void User_Parking_in(int sockfd,PSeqlist L)
{
	MYSQL_ROW row;
	MYSQL_RES *res_ptr;
	char a[128] = {0};
    int t;
	char sql_query[200]="select * from P_place;";//保存查询语句
	int n = mysql_real_query(&mysql,sql_query,strlen(sql_query));//运行指定为计数字符串的SQL查询
    if(n)
	{
		printf("Query failed\n");
		send(sockfd,"no",2,0);
    } 
    else
    {
		res_ptr = mysql_use_result(&mysql);//初始化逐行的结果集检索
	   
	        while(row = mysql_fetch_row(res_ptr))
		{
			for(t=0;t<mysql_num_fields(res_ptr);t++)
			{
				 printf("%s ",row[t]);
				 strcat(a,(char *)row[t]);//把数据库中的数据强转
				 strcat(a,",");
			}
		}
			
        printf("查询成功\n");
        
    }
   
	send(sockfd,a,strlen(a)-1,0);
     
    printf("\n");
 
    mysql_free_result(res_ptr);
}

void U_Capacity_cut(int sockfd,PSeqlist L)//进车
{
	MYSQL_RES *res_ptr;
	MYSQL_ROW row;
	char sql_query1[200]={0};//保存查询语句
	char sql_query2[200] = {0};

	sprintf(sql_query1,"select P_name from P_parking where usrname='%s';",L->str[1].value);//后面不加‘’就可以查询出结果加了就错误
	int n = mysql_real_query(&mysql,sql_query1,strlen(sql_query1));
	if(n)
	{
		send(sockfd,"no\n",3,0);
		perror("no P_name");
	}
	else
	{
		res_ptr = mysql_store_result(&mysql);
		row = mysql_fetch_row(res_ptr);
		mysql_free_result(res_ptr);
		sprintf(sql_query2,"update  P_place set P_capacity=P_capacity-1 where P_name='%s';",row[0]);
		if(!(mysql_real_query(&mysql,sql_query2,strlen(sql_query2))))
			send(sockfd,"yes\n",4,0);
		else
			send(sockfd,"no\n",3,0);
	}
}

void Delete_Parking_record(int sockfd,PSeqlist L)//删除停车记录
{
	char sql_query[200] = {0};
	sprintf(sql_query,"delete from P_parking where usrname = '%s';",L->str[1].value);
	int n = mysql_real_query(&mysql,sql_query,strlen(sql_query));
	if(n)
	{
		send(sockfd,"no\n",3,0);
	}
	else
	{
		send(sockfd,"yes\n",4,0);
	}

}
void U_Capacity_add(int sockfd,PSeqlist L)//出车
{
	MYSQL_RES *res_ptr;
	MYSQL_ROW row;
	char sql_query1[200]={0};//保存查询语句
	char sql_query2[200] = {0};

	sprintf(sql_query1,"select P_name from P_parking where usrname='%s';",L->str[1].value);//后面不加‘’就可以查询出结果加了就错误
	int n = mysql_real_query(&mysql,sql_query1,strlen(sql_query1));
	if(n)
	{
		send(sockfd,"no\n",3,0);
		perror("no P_name");
	}
	else
	{
		res_ptr = mysql_store_result(&mysql);
		row = mysql_fetch_row(res_ptr);
		mysql_free_result(res_ptr);
		sprintf(sql_query2,"update  P_place set P_capacity=P_capacity+1 where P_name='%s';",row[0]);
		if(!(mysql_real_query(&mysql,sql_query2,strlen(sql_query2))))
		{

			Delete_Parking_record(sockfd,L);
		}	

		else
			send(sockfd,"no\n",3,0);
	}
	
}

void User_Parking_Tinit(int sockfd,PSeqlist L)//用户停车时的初始化时间
{
	MYSQL_ROW row;
	MYSQL_RES *res_ptr;
	char sql_query1[200] = {0};
	char sql_query2[200] = {0};
	sprintf(sql_query1,"select carid from person where usrname = '%s';",L->str[1].value);
	int n = mysql_real_query(&mysql,sql_query1,strlen(sql_query1));
    if(n)
	{
		printf("Query failed\n");
		send(sockfd,"no\n",2,0);
    } 
    else
    {
		res_ptr = mysql_store_result(&mysql);//初始化逐行的结果集检索
  		row = mysql_fetch_row(res_ptr);//检索结果集的下一行
		mysql_free_result(res_ptr);
	}


	if(row)
    {
		sprintf(sql_query2,"insert into P_parking values('%s',0,0,'%s',now(),'%s');",row[0],L->str[2].value,L->str[1].value);
		int n1 = mysql_real_query(&mysql,sql_query2,strlen(sql_query2));
        if(n1)
		{
			printf("failed\n");
			//send(sockfd,"no\n",3,0);
			return;
		}
		else
		{
			printf("初始化成功！\n");
			//send(sockfd,"yes\n",4,0);
			U_Capacity_cut(sockfd,L);//进车
		}
		
        
    }
    else
	{
		printf("failed\n");
        send(sockfd,"no\n",3,0);
        return;
    }
	
}

void User_Parking_t(int sockfd,PSeqlist L)//用户端查询自己的停车时间
{
	MYSQL_ROW row,row1;
	MYSQL_RES *res_ptr ,*res_ptr1,*result;
	char sql_query1[200] = {0};//保存查询语句
	char sql_query2[200] = {0};
	char sql_query3[200] = {0};
	sprintf(sql_query1,"select Time_start from P_parking where usrname = '%s';",L->str[1].value);
	
	int n = mysql_real_query(&mysql,sql_query1,strlen(sql_query1));//运行指定为计数字符串的SQL查询
    if(n)
	{
		printf("Query failed\n");
		send(sockfd,"no\n",3,0);
    } 
    else
    {
		res_ptr = mysql_store_result(&mysql);//初始化逐行的结果集检索
		row = mysql_fetch_row(res_ptr);//检索结果集的下一行
	}	
	if(row)
	{
        mysql_free_result(res_ptr);     
	            
		sprintf(sql_query2,"update P_parking set Sum_time = TIMESTAMPDIFF(MINUTE,'%s',now()) where usrname = '%s';",row[0],L->str[1].value);
		int n1 = mysql_real_query(&mysql,sql_query2,strlen(sql_query2));
		if(n1)	
		{
			printf("Query failed\n");
			send(sockfd,"no\n",3,0);
		}
		else
		{
			sprintf(sql_query1,"select Sum_time from P_parking where usrname = '%s';",L->str[1].value);
			int n2 = mysql_real_query(&mysql,sql_query1,strlen(sql_query1));
			if(n2)	
			{
				printf("Query failed\n");
				send(sockfd,"no\n",3,0);
			}
			else
			{
				res_ptr1 = mysql_store_result(&mysql);//初始化逐行的结果集检索
				row1 = mysql_fetch_row(res_ptr1);
				printf("查询成功\n");
				send(sockfd,row[0],strlen(row[0]),0);
				mysql_free_result(res_ptr1);     
			}
		}
		
	}   
	
    else
    {
        printf("failed\n");
        send(sockfd,"no\n",3,0);
        return;
    }
    //mysql_free_result(res_ptr);     
         
}

void User_Parking_m(int sockfd,PSeqlist L)//用户端根据车牌号查询自己的停车金额
{
   // User_Parking_t(sockfd,L);
	MYSQL_ROW row,row1;
	MYSQL_RES *res_ptr;
	char sql_query1[200] = {0};//保存查询语句
	
	sprintf(sql_query1,"select P_charge from P_place where P_name = '%s';",L->str[1].value);
	
	int n = mysql_real_query(&mysql,sql_query1,strlen(sql_query1));//运行指定为计数字符串的SQL查询
    if(n)
	{
		printf("Query failed\n");
		send(sockfd,"no\n",3,0);
    } 
    else
    {
		res_ptr = mysql_store_result(&mysql);//初始化逐行的结果集检索
		row = mysql_fetch_row(res_ptr);//检索结果集的下一行
	}	
	if(row)
	{
       printf("sucess\n");
	   send(sockfd,row[0],strlen(row[0]),0);
	}   
    else
    {
        printf("failed\n");
        send(sockfd,"no\n",3,0);
    }
}

void Admistrator_Register(int sockfd,PSeqlist L)//管理员注册
{
   /* memset(buff,0,128);
    recv(fd,buff,127,0);
    const char* d=",";
    char *p_id;
    char *p_password;
    p_id=strtok(buff,d);
    p_password=strtok(NULL,d);*/
    printf("该客户端输入的电话号码为：%s",L->str[1].value);
    printf("该客户端输入的密码为:%s",L->str[2].value);
    char sql_insert[200] = {0};
    sprintf(sql_insert,"insert into P_adminstrator(P_phone,P_password) values('%s','%s');",L->str[1].value,L->str[2].value);
    //printf("%s\n",L->str[1].value);
    //printf("%s\n",L->str[2].value);
    
    int res= mysql_real_query(&mysql,sql_insert,strlen(sql_insert));
    if(res)
        send(sockfd,"no",2,0);
    else
        send(sockfd,"yes",3,0);
}
void Admistrator_Delete(int sockfd,PSeqlist L)//管理员注销
{
   /* memset(buff,0,128);
    recv(fd,buff,127,0);
    const char* d=",";
    char *p_id;
    char *p_password;
    p_id=strtok(buff,d);
    p_password=strtok(NULL,d);*/
    printf("该客户端输入的电话号码为：%s",L->str[1].value);
    printf("该客户端输入的密码为:%s",L->str[2].value);
    char sql_delete[200] = {0};
    sprintf(sql_delete,"delete from P_adminstrator where P_phone='%s' AND P_password='%s';",L->str[1].value,L->str[2].value);
    int res= mysql_real_query(&mysql,sql_delete,strlen(sql_delete));
    if(res)//删除数据库里面没有的数据还输出成功，所以有问题
        send(sockfd,"no",2,0);
    else
        send(sockfd,"yes",3,0);
}


//管理员密码修改
void Adminstrator_Password_Update(int sockfd,PSeqlist L)
{
  char sql_update[200] = {0};
  sprintf(sql_update,"update P_adminstrator set P_password='%s' where P_password='%s';",L->str[2].value,L->str[1].value);
 printf("旧密码：%s\n",L->str[1]);
printf("新密码:%s\n",L->str[2]); 
  int res = mysql_real_query(&mysql,sql_update,strlen(sql_update));
  if(res)
 send(sockfd,"no",2,0);
 else
 send(sockfd,"yes",3,0);
 
}
void Parking_Add(int sockfd,PSeqlist L)
{
    /* memset(buff,0,128);
     recv(fd,buff,127,0);
     const char* d=",";
     char *p_name=strtok(buff,d);
     char *p_capacity=strtok(NULL,d);
     char *p_site=strtok(NULL,d);
     char *p_charge=strtok(NULL,d);
     char *p_tel=strtok(NULL,d);*/
    printf("停车场名：%s\n",L->str[1].value);
    printf("停车场容量：\n",L->str[2].value);
    printf("停车场位置：\n",L->str[3].value);
    printf("收费:\n",L->str[4].value);
    printf("电话：\n",L->str[5].value);
   
    
     char sql_insert[200] = {0};

     sprintf(sql_insert,"insert into P_place(P_name,P_capacity,P_site,P_charge,P_tel)values('%s','%s','%s','%s','%s');",L->str[1].value,L->str[2].value,L->str[3].value,L->str[4].value,L->str[5].value);
     int res= mysql_real_query(&mysql,sql_insert,strlen(sql_insert));
     if(res)
         send(sockfd,"yes\n",3,0);
     else
         send(sockfd,"no\n",2,0);
}
void Parking_Delete(int sockfd,PSeqlist L)
{
   /* memset(buff,0,128);
    recv(fd,buff,127,0);
    const char* d=",";
    char *p_name;
    p_name=strtok(buff,d);*/
    printf("该客户端输入的停车场名称为：%s",L->str[1].value);
    char sql_delete[200] = {0};
    sprintf(sql_delete,"delete from P_place where P_name='%s';",L->str[1].value);
    int res= mysql_real_query(&mysql,sql_delete,strlen(sql_delete));
    if(res)//删除数据库里面没有的数据还输出成功，所以有问题
        send(sockfd,"no\n",2,0);
    else
        send(sockfd,"yes\n",3,0);
}

void Parking_Capacity_Update(int sockfd,PSeqlist L)//更改停车场容量
{
    char sql_update[200] = {0};
    sprintf(sql_update,"update P_place set P_capacity='%s'where P_capacity='%s';",L->str[2].value,L->str[1].value);
   int res = mysql_real_query(&mysql,sql_update,strlen(sql_update));
  if(res)
   send(sockfd,"no\n",2,0);
 else
  send(sockfd,"yes\n",3,0);
}
void Admistrator_Login(int sockfd,PSeqlist L)//管理员登录
{
	MYSQL_ROW row;
	MYSQL_RES *res_ptr;
	char sql_query[200]={0};//保存查询语句
   // printf("%s,%s\n",L->str[1].value,L->str[2].value);
	sprintf(sql_query,"select P_phone,P_password from P_adminstrator where P_phone='%s' and P_password=%s;",L->str[1].value,L->str[2].value);//后面不加‘’就可以查询出结果加了就错误
	int n = mysql_real_query(&mysql,sql_query,strlen(sql_query));
	if(n)
	{
     	perror("Query failed");
    }
     	else
     	{
     	res_ptr = mysql_use_result(&mysql);
     	row = mysql_fetch_row(res_ptr);
     	}
     	if(row)
     	{
     	//printf("登录成功\n");
     	printf("输出结果为:%s\n",row[1]);
     	send(sockfd,"yes",3,0);
     	}
     	else
     	{
            printf("failed\n");
     	send(sockfd,"no",2,0);
     	}
     	mysql_free_result(res_ptr);     
}

void Select_User_Carid(int sockfd,PSeqlist L)//根据车牌查询用户信息
{
    MYSQL_ROW row;
    MYSQL_RES *res_ptr;
    int t;
    char sql_query[200]={0};//保存查询语句
     printf("carid:%s\n",L->str[1].value);
     sprintf(sql_query,"select usrname,phone,mail from person where carid='%s';",L->str[1].value);
printf("%s",L->str[1].value);
    int n = mysql_real_query(&mysql,sql_query,strlen(sql_query));//运行指定为计数字符串的SQL查询
     if(n)
     {
     printf("Query failed\n");
     send(sockfd,"no",2,0);
     }
    else
     {
     res_ptr = mysql_use_result(&mysql);
     row = mysql_fetch_row(res_ptr);
     }
     if(row)
    {
      printf("登录成功\n");
      send(sockfd,"yes",3,0);
     }
     else
     {
        printf("failed\n");
        send(sockfd,"no",2,0);
         return;
     }
        char buff[128]={0};
        char a[128] = {0};
  for(t=0;t<mysql_num_fields(res_ptr);t++)
 {
      printf("%s ",row[t]);
      strcat(a,",");
      strcat(a,(char *)row[t]);//把数据库中的数据强转
 }                                                           send(sockfd,a,strlen(a),0);
     printf("%s",a);
     memset(a,0,strlen(a));                                                                                         mysql_free_result(res_ptr);

}
void Parking_Select_Id(int sockfd,PSeqlist L)//根据停车场名称查询信息
{
 	MYSQL_ROW row;
	MYSQL_RES *res_ptr;
     int t;
     char sql_query[200]={0};//保存查询语句
	printf("id:%s\n",L->str[1].value);
     sprintf(sql_query,"select * from P_place where  P_name='%s';",L->str[1].value);
    //查询，成功则发送yes
     int n = mysql_real_query(&mysql,sql_query,strlen(sql_query));//运行指定为计数字符串的SQL查询
     if(n)

     {
     printf("Query failed\n");
     send(sockfd,"no",2,0);
     } 
    else
     {
     res_ptr = mysql_use_result(&mysql);//初始化逐行的结果集检索
     row = mysql_fetch_row(res_ptr);//检索结果集的下一行
       
   }
	if(row)
     	{
          printf("登录成功\n");
 
          send(sockfd,"yes",3,0);
        
     	}
     	else
     	{
            printf("failed\n");
            send(sockfd,"no",2,0);
            return;
     	}
    char a[128] = {0};
     for(t=0;t<mysql_num_fields(res_ptr);t++)
     {
         printf("%s ",row[t]);
         strcat(a,",");
	 strcat(a,(char *)row[t]);//把数据库中的数据强转
     }
	send(sockfd,a,strlen(a),0);
    memset(a,0,strlen(a)); 
     printf("\n");
 
      mysql_free_result(res_ptr);
}
int main()
{
   
    mysql_init(&mysql);

    if(!mysql_real_connect(&mysql,"localhost","stu","111111","Parking",0,NULL,0))
	printf("error connect!\n");
    else
	printf("sucess connect!\n");
   /* if (!mysql_set_character_set(&mysql, "gbk"))
    {
        printf("New client character set: %s\n", mysql_character_set_name(&mysql));
    }*/
    fd_event_init();
    int sockfd = create_socket();
    if(sockfd == -1)
    {
	perror("error socket");
    }
    struct event_base *base = event_init();
    struct event *listen_ev = event_new(base,sockfd,EV_READ|EV_PERSIST,listen_cb,base);
 
    assert(listen_ev != NULL);

    event_add(listen_ev,NULL);
    event_base_dispatch(base);

    
    event_free(listen_ev);
    event_base_free(base);
    mysql_close(&mysql);
}
