#include <stdio.h>
#include <winsock2.h>
#include <mysql.h>
#include <time.h>
/*数据库连接用宏*/
#define HOST "localhost"
#define USERNAME "root"
#define PASSWORD ""
#define DATABASE "yunbi"

void query(char sql[]);//连接mysql
void timetemp(char*time,char wannatime[]);//将时间转化为与Mysql中字符格式相同的字符串
void getmonth(char*time,char mon[]);//获取标准化的月份
void get_similartime(char wannatime[],char similar_time[]);//获取精确到分钟的时间
int main()
{
    //获取本地时间
    char wannatime[20]={""};
    time_t t;
    t=time(NULL);
    char*time=NULL;
    time=ctime(&t);
    timetemp(time,wannatime);
    char similar_time[23]={""};
    get_similartime(wannatime,similar_time);
    printf("%s\n",similar_time);
    //获取实时数据
    char query[200]={"select * from btccny where date_time in(select Max(date_time) from btccny where date_time in(select date_time from btccny where date_time like "};
    strcat(query,similar_time);
    query_sql(query);
    //query="select * from btccny where date_time in(select Max(date_time) from btccny where date_time in(select date_time from btccny where date_time like ";
    //query_sql(query,similar_time);
    return 0;
}
void get_similartime(char wannatime[],char similar_time[])
{
    char x[2]={"'"};
    strncpy(similar_time,x,1);
    strncat(similar_time,wannatime,16);
    char m[2]={"%"};
    char y[2]={")"};
    strcat(similar_time,m);
    strncat(similar_time,x,1);
    strcat(similar_time,y);
    strcat(similar_time,y);
}
void timetemp(char*time,char wannatime[])
{
    //char wannatime[20]={""};
    strncpy(wannatime,time+20,4);//得到2017

    char s[]="-";
    char a[]=" ";
    strcat(wannatime,s);
    char mon[3]={""};
    getmonth(time,mon);
    strcat(wannatime,mon);//得到月份
    //printf("%s",wannatime);
    strcat(wannatime,s);
    strncat(wannatime,time+8,2);//得到日期
    strcat(wannatime,a);
    strncat(wannatime,time+11,8);//得到时间
    //printf("%s",wannatime);
    return wannatime;
}
void getmonth(char*time,char mon[])
{
    char mon1[4]={""};
    //char mon[3]={""};
    strncpy(mon1,time+4,3);
    char Jan[]={"Jan"};
    char Feb[]={"Feb"};
    char Mar[]={"Mar"};
    char Apr[]={"Apr"};
    char May[]={"May"};
    char Jun[]={"Jun"};
    char Jul[]={"Jul"};
    char Aug[]={"Aug"};
    char Sep[]={"Sep"};
    char Oct[]={"Oct"};
    char Nov[]={"Nov"};
    char Dec[]={"Dec"};

    //printf("%s",mon1);
    if(strcmp(mon1,Jan)==0)
    {
        char b[]="01";
        strncpy(mon,b,2);
        //return mon;
    }
    else if(strcmp(mon1,Feb)==0)
    {
        char b[]="02";
        strcpy(mon,b);
        //return mon;
    }
    else if(strcmp(mon1,Mar)==0)
    {
        char b[]="03";
        strcpy(mon,b);
        //return mon;
    }
    else if(strcmp(mon1,Apr)==0)
    {
        char b[]="04";
        strcpy(mon,b);
        //return mon;
    }
    else if(strcmp(mon1,May)==0)
    {
        char b[]="05";
        strcpy(mon,b);
        //return mon;
    }
    else if(strcmp(mon1,Jun)==0)
    {
        char b[]="06";
        strcpy(mon,b);
        //return mon;
    }
    else if(strcmp(mon1,Jul)==0)
    {
        char b[]={"07"};
        strncpy(mon,b,2);
        //return mon;
    }
    else if(strcmp(mon1,Aug)==0)
    {
        char b[]="08";
        strcpy(mon,b);
        //return mon;
    }
    else if(strcmp(mon1,Sep)==0)
    {
        char b[]="09";
        strcpy(mon,b);
        //return mon;
    }
    else if(strcmp(mon1,Oct)==0)
    {
        char b[]="10";
        strcpy(mon,b);
        //return mon;
    }
    else if(strcmp(mon1,Nov)==0)
    {
        char b[]="11";
        strcpy(mon,b);
        //return mon;
    }
    else if(strcmp(mon1,Dec)==0)
    {
        char b[]="12";
        strcpy(mon,b);
        //return mon;
    }
}
void query_sql(char sql[])
{
    MYSQL my_connection; /*这是一个数据库连接*/
    int res; /*执行sql語句后的返回标志*/
    MYSQL_RES *res_ptr; /*指向查询结果的指针*/
    MYSQL_FIELD *field; /*字段结构指针*/
    MYSQL_ROW result_row; /*按行返回的查询信息*/
    int row, column; /*查询返回的行数和列数*/
    int i, j;
    /*初始化mysql连接my_connection*/
    mysql_init(&my_connection);
    /*建立mysql连接*/
    if (NULL != mysql_real_connect(&my_connection, HOST, USERNAME, PASSWORD,DATABASE, 0, NULL, CLIENT_FOUND_ROWS))  /*连接成功*/
    {
        //printf("数据库查询query_sql连接成功！\n");
        /*设置查询编码为gbk，以支持中文*/
        mysql_query(&my_connection, "set names gbk");
        res = mysql_query(&my_connection, sql);
        if (res)   /*执行失败*/
        {
            printf("Error： mysql_query !\n");
            /*关闭连接*/
            mysql_close(&my_connection);
        }
        else     /*现在就代表执行成功了*/
        {
            /*将查询的結果给res_ptr*/
            res_ptr = mysql_store_result(&my_connection);
            /*如果结果不为空，就把结果print*/
            if (res_ptr)
            {
                /*取得結果的行数和*/
                column = mysql_num_fields(res_ptr);
                row = mysql_num_rows(res_ptr);
                printf("查询到 %d 行 \n", row);
                /*输出結果的字段名*/
                for (i = 0; field = mysql_fetch_field(res_ptr); i++)
                    printf("%10s ", field->name);
                printf("\n");
                /*按行输出結果*/
                for (i = 1; i < row+1; i++)
                {
                    result_row = mysql_fetch_row(res_ptr);
                    for (j = 0; j < column; j++)
                        printf("%10s ", result_row[j]);
                    printf("\n");
                }
            }
            /*不要忘了关闭连接*/
            mysql_close(&my_connection);
        }
    }
    else
    {
        printf("数据库连接失败");
    }
}
