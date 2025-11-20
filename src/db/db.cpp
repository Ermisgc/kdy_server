#include "db/db.h"
#include <mysql-cppconn/jdbc/mysql_driver.h>
#include <mysql-cppconn/jdbc/cppconn/exception.h>
#include <mysql-cppconn/jdbc/cppconn/statement.h>

NAMESPACE_BEGIN{ namespace db{
    QueryResult::QueryResult(std::unique_ptr<sql::ResultSet>& sql_result){
        _sql_result = std::move(sql_result);
    }
    
    QueryResult & QueryResult::operator>>(int & operand){
        operand = _sql_result ? _sql_result->getInt(_colomn_index++) : 0;
        return *this;
    }

    QueryResult & QueryResult::operator>>(double & operand){
        operand = _sql_result ? _sql_result->getDouble(_colomn_index++) : 0.0;
        return *this;
    }

    QueryResult & QueryResult::operator>>(std::string & operand){
        operand = _sql_result ? _sql_result->getString(_colomn_index++) : " ";
        return *this;
    }

    QueryResult & QueryResult::operator>>(bool & operand){
        operand = _sql_result ? _sql_result->getBoolean(_colomn_index++) : false;
        return *this;
    }

    bool QueryResult::next(){
        _colomn_index = 1;
        return _sql_result ? _sql_result->next() : false;
    }


    DBSession::DBSession(const std::string & host, uint16_t port){
        try{
            sql::Driver* driver = sql::mysql::get_mysql_driver_instance();
            std::string url = "tcp://" + host + ":" + std::to_string(port);

            static const std::string user = MySQL_USER_NAME;
            static const std::string password = MySQL_USER_PASSWORD;
            static const std::string database = MySQL_DATABASE_NAME;
            
            _connection = driver->connect(url, user, password);
            _connection->setSchema(database);
        } catch(const sql::SQLException & e){
            std::cerr << "MySQL Connection Error: " << e.what() << " code: " << e.getErrorCode() << " SQL Status: " << e.getSQLState() << std::endl;
            throw std::runtime_error("SQL Error");
        }
    } 

    DBSession::~DBSession(){
        if(_connection){
            delete _connection;
            _connection = nullptr;
        }
    }

    std::optional<std::unique_ptr<QueryResult>> DBSession::executeQuery(const std::string & query){
        try{
            std::unique_ptr<sql::Statement> stmt = std::unique_ptr<sql::Statement>(_connection->createStatement());
            std::unique_ptr<sql::ResultSet> result = std::unique_ptr<sql::ResultSet>(stmt->executeQuery(query));

            // // 获取元数据查看列信息
            // sql::ResultSetMetaData* meta = result->getMetaData();
            // int column_count = meta->getColumnCount();

            // std::cout << "=== 查询结果详情 ===" << std::endl;
            // std::cout << "列数: " << column_count << std::endl;

            // // 打印列名
            // std::cout << "列名: ";
            // for (int i = 1; i <= column_count; i++) {
            //     std::cout << meta->getColumnName(i) << " (" 
            //             << meta->getColumnTypeName(i) << ") | ";
            // }
            // std::cout << std::endl;

            // // 遍历所有行
            // int row_num = 0;
            // while (result->next()) {
            //     std::cout << "第 " << ++row_num << " 行: ";
            //     for (int i = 1; i <= column_count; i++) {
            //         try {
            //             std::string value = result->getString(i);
            //             std::cout << "[" << value << "] ";
            //         } catch (const sql::SQLException& e) {
            //             std::cout << "[ERROR: " << e.what() << "] ";
            //         }
            //     }
            //     std::cout << std::endl;
            // }

            // std::cout << "总行数: " << row_num << std::endl;

            return std::make_unique<QueryResult>(result);
        } catch(const sql::SQLException & e) {
            return std::nullopt;
        }
    } 

    int DBSession::executeUpdate(const std::string & sql){
        std::unique_ptr<sql::Statement> stmt = std::unique_ptr<sql::Statement>(_connection->createStatement());
        return stmt->executeUpdate(sql);
    }
}}