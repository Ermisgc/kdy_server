#include "db/db_connection_pool.h"
NAMESPACE_BEGIN{ namespace db {
    //静态函数，数据库连接池设计为单例模式
    DBConnectionPool & DBConnectionPool::instance(){
        static DBConnectionPool dcp;
        return dcp;
    }

    DBConnectionPool::DBConnectionPool(const std::string& host, uint16_t port, int poolSize):
        _host(host), _port(port), _pool_size(poolSize){
        for(int i = 0;i < poolSize; ++i){
            _connections.push(new DBSession(host, port));
        }
    }
    
    std::shared_ptr<DBSession> DBConnectionPool::getConnection(){
        std::lock_guard<std::mutex> locker(_mutex);
        if(_connections.empty()){
            try{
                _connections.push(new DBSession(_host, _port));  //动态扩容
            } catch (const std::runtime_error & e){
                return nullptr;
            }
        } 

        DBSession * ret = _connections.front();
        _connections.pop();

        return std::shared_ptr<DBSession>(ret, [this](DBSession * session) {
            std::lock_guard<std::mutex> locker(_mutex);
            _connections.push(session);
            if(_connections.size() > 2 * _pool_size){  
                while(_connections.size() > _pool_size){   //清理额外连接
                    DBSession * s = _connections.front();
                    delete s;
                    _connections.pop();
                }
            }
        });
    }
}}