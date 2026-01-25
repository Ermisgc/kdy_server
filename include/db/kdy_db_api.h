#ifndef KDY_KDY_DB_API_H
#define KDY_KDY_DB_API_H
#include "utils.h"
#include "db/api_utils.h"
#include "db/db_connection_pool.h"
#include <optional>

NAMESPACE_BEGIN{ namespace db{
    struct User{
        int id;
        int previlege;
        std::string user_name;
        std::string user_password;
        TABLE_NAME("users")
        INSERT_TO_DB(user_name, user_password, previlege)
        UPDATE_TO_DB(user_name, user_password, previlege, id)  //前三个更新，最后一个作为WHERE条件
        DELETE_FROM_DB(id)
        
        static User fromDB(QueryResult * res){
            User ret;
            QueryResult & qr = *res;
            qr >> ret.id >> ret.user_name >> ret.user_password >> ret.previlege;
            return ret;
        }
    };

    struct Displacement{
        std::string id;
        uint64_t ts;
        double xplacement;
        double yplacement;
        TABLE_NAME("point_data")
        INSERT_TO_DB(id, ts, xplacement, yplacement)
        std::string updateToDB() { assert(0); }  //禁止调用updateToDB函数

        std::string deleteFromDB(){ 
            return "DELETE FROM " + nonstatic_tableName + " WHERE id=" + to_sql_value(id) + " AND timestamp=" + to_sql_value(ts);
        }

        static Displacement fromDB(QueryResult * res){
            Displacement ret;
            QueryResult & qr = *res;
            qr >> ret.ts >> ret.xplacement >> ret.yplacement;
            return ret;            
        }

        DEFINE_JSON(Displacement, id, ts, xplacement, yplacement)

        static inline std::vector<Displacement> queryDuration(const std::string & id, uint64_t duration_seconds=60) {
            duration_seconds *= 1000;  //seconds to milliseconds
            std::string query = "SELECT ts, xplacement, yplacement FROM point_data WHERE id = " + to_sql_value(id) + 
            " AND ts >= UNIX_TIMESTAMP() * 1000 - " + to_sql_value(duration_seconds) + " ORDER BY ts;";
            auto ret = DBConnectionPool::instance().getConnection()->executeQuery(query);
            std::vector<Displacement> ans;
            if(ret.has_value()){
                auto & qr = ret.value();
                while (qr->next()) {
                    Displacement obj = Displacement::fromDB(qr.get());
                    ans.push_back(std::move(obj));
                }
            }
            return ans;
        }
    };
}}

#endif