/**
 * @brief 数据库连接池，单例模式，支持动态扩容
 * @example 使用一般为：DBConnectionPool::instance().getConnection()->execute(...);
 */
#ifndef KDY_DB_CONNECTION_POOL_H
#define KDY_DB_CONNECTION_POOL_H
#include <memory>
#include <queue>
#include <mutex>
#include "db.h"

NAMESPACE_BEGIN { namespace db{
class DBConnectionPool {
private:
    std::string _host;
    int _pool_size;
    uint16_t _port;
    std::queue<DBSession *> _connections;
    std::mutex _mutex;
    
    explicit DBConnectionPool(const std::string& host = MySQL_DEFAULT_HOST, uint16_t port = MySQL_DEFAULT_PORT, int poolSize = MySQL_CONNPOOL_SIZE);
    ~DBConnectionPool() noexcept {}
    
public:
    static DBConnectionPool & instance();
    std::shared_ptr<DBSession> getConnection();
};

}}
#endif