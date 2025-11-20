/**
 * @file DataBase的客户端句柄，用于和DB进行交互写入，具体的表在DB里面已经建好了
 */

#ifndef KDY_DB_H
#define KDY_DB_H

#include <iostream>
#include <memory>
#include <string>
#include <optional>
#include <mysql-cppconn/jdbc/mysql_connection.h>
#include <mysql-cppconn/jdbc/cppconn/resultset.h>
#include "utils.h"
#include <concepts>

NAMESPACE_BEGIN { namespace db {
    /**
     * @brief 对MySQL询问的封装
     * @example 使用方式
     * QueryResult qr; 
     * int a; 
     * string b; 
     * double c; 
     * while(qr->next()) qr >> a >> b >> c;
     */
    class QueryResult{
        std::unique_ptr<sql::ResultSet> _sql_result;
        int _colomn_index = 1;
    public:
        QueryResult(std::unique_ptr<sql::ResultSet> & sql_result);
        
        QueryResult & operator>>(int & i);
        QueryResult & operator>>(double & );
        QueryResult & operator>>(std::string &);
        QueryResult & operator>>(bool &);

        /**
         * @brief 跳转到下一个数据行，如果已经是末行了，返回false
         * @example while(res->next()) { .... } 遍历每一行
         */
        bool next();
    };

    /**
     * @brief 定义api概念
     * @details 一个表的数据结构必须具备的条件
     */
    template<typename T>
    concept DatabaseMappable = requires(T obj, db::QueryResult * res) {
        {obj.insertToDB()} -> std::same_as<std::string>;  //insert插入语句对应的函数
        {obj.updateToDB()} -> std::same_as<std::string>;  //update语句
        {obj.deleteFromDB()} -> std::same_as<std::string>;//delete语句
        {T::fromDB(res)} -> std::same_as<T>;  //全量查询select * from user时采用的方式
        {T::tableName()} -> std::convertible_to<std::string>;
    };

    /**
     * @brief 一个与Database的会话，遵循RAII设计
     */
    class DBSession{
    private:
        sql::Connection * _connection;
    public:
        DBSession(const std::string & host=MySQL_DEFAULT_HOST, uint16_t port=MySQL_DEFAULT_PORT); 
        ~DBSession() noexcept;

        /**
         * @brief 同步查询，向Database发送一个查询语句，并返回结果
         */
        std::optional<std::unique_ptr<QueryResult>> executeQuery(const std::string & msg);

        /**
         * @brief 同步更新，向Database发送一个update请求，并返回结果
         */
        int executeUpdate(const std::string & sql);

        /**
         * @brief 查询所有行，select * from table_name
         */
        template<DatabaseMappable T>
        std::vector<T> selectAll() {
            std::vector<T> results;
            std::string sql = "SELECT * FROM " + T::tableName();

            auto res = executeQuery(sql).value();  //执行结果
            if(!res){
                std::cerr << "Query failed: " << sql << std::endl;
                return results;
            }

            while (res->next()) {
                T obj = T::fromDB(res.get());
                results.push_back(std::move(obj));
            }
            
            return results;
        }

        /**
         * @brief insert操作，insert into tables (field1, field2, ...) values (xxx, xxx)
         */
        template<DatabaseMappable T>
        inline bool insert(const T& obj) {
            std::string sql = obj.insertToDB();
            return executeUpdate(sql) > 0;  //执行结果
        }

        /**
         * @brief update操作，update tables set ...
         */
        template<DatabaseMappable T>
        inline bool update(const T& obj) {
            return executeUpdate(obj.updateToDB()) > 0;
        }

        //TODO:异步调用，计划采用C++20协程实现
    };
}}

#endif