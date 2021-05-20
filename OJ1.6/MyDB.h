/*************************************************************************
    > File Name: mydb.h
    > Author:fengxin 
    > Mail:903087053@qq.com 
    > Created Time: 2017年07月21日 星期五 15时17分17秒
 ************************************************************************/

#ifndef _MYDB_H
#define _MYDB_H
#include<iostream>
#include<string>
#include<vector>
#include<mysql/mysql.h>
using namespace std;

class MyDB
{
    public:
    MyDB();
    ~MyDB();
    bool initDB(string host,string user,string pwd,string db_name); //连接mysql
    bool exeSQL(string sql);   //执行sql语句
    string selectSQL(string sql); //select语句
    bool execSelect(string sql,vector<string> &vec);   //执行sql语句
    private:
    MYSQL *mysql;          //连接mysql句柄指针
    MYSQL_RES *result;    //指向查询结果的指针
    MYSQL_ROW row;       //按行返回的查询信息
};


#endif
