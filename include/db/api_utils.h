/**
 * @file api_utils.h
 * 本文件主要写一些与数据库相关的API的重要辅助函数，
 * 这主要是因为，每一个struct应该与MySQL表中的一个table对应，
 * 按照db.h文件中的DatabaseMapping概念定义struct，每个struct应至少提供以下几个方法：
 * 1. insertToDB，非静态方法，将数据插入到DB中
 * 2. updateToDB，非静态方法，将DB中的数据更新
 * 3. deleteFromDB，非静态方法，删除DB中的该条数据
 * 4. tableName，静态方法，返回数据库中与struct对应的table的名字
 * 5. fromDB，将select语句的返回结果序列化为struct
 * 本文件实现了一种反射机制，能够将变量名与变量的值通过宏映射起来
 */
#ifndef KDY_API_UTILS_H
#define KDY_API_UTILS_H

#include "db.h"
#include <algorithm>
#include <vector>
#include <sstream>

NAMESPACE_BEGIN{ namespace db {
    /**
     * @brief 将C++数据类型转化为MySQL字段数据
     * 将各个类型优雅地转化为插入MySQL时的字符串
     */
    template<typename T>
    inline std::string to_sql_value(const T & t){
        if constexpr(std::is_same_v<T, std::string>) return "\'" + t + "\'";
        else if constexpr (std::is_same_v<T, const char *>) return "\'" + std::string(t) + "\'";
        else if constexpr (std::is_arithmetic_v<T>) return std::to_string(t);
        else static_assert(std::is_same_v<T, void>, "Unsupported type for SQL conversion");
    } 

    /**
     * @brief 辅助函数，用来将同时将多个fields的值拼接为SQL插入语句对应的字符串
     * @details 比如说有三个变量:int a, string b, float c，它们转化为MySQL的插入为:insert into xxx (...) values (a, "b", c);
     * 这里返回values后面括号里的语句
     */
    template<typename ... T>
    std::string invoke_fields(T &... t){
        //传参为空的情况
        if constexpr(sizeof...(T) == 0) return "";
        
        std::string ret;

        //这里写一个折叠表达式，把所有变量根据类型拼接到折叠表达式上
        auto process = [&](auto & arg) -> void {
            ret += to_sql_value(arg);
            ret.push_back(',');
        };

        (process(t), ...);

        ret.pop_back();  //去掉后面的',';
        return ret;
    }

    /**
     * @brief 辅助函数，生成宏的可变参数__VA_ARGS__对应的变量vector
     */
    template<typename ... T>
    std::vector<std::string> fields_vector(T & ...t){
        //传参为空的情况
        if constexpr(sizeof...(T) == 0) return {};
        std::vector<std::string> res;
        res.reserve(sizeof...(t));

        auto process = [&](auto & arg)-> void {
            res.emplace_back(to_sql_value(arg));
        };

        (process(t), ...);
        return res;
    }

#ifndef TABLE_NAME
#define TABLE_NAME(name) \
    const std::string nonstatic_tableName = name; \
     \
    static std::string tableName() { \
        return name; \
    }
#endif

#ifndef INSERT_TO_DB

/**
 * @brief 出于便利设计的宏，根据字段实现insertToDB()方法
 */
#define INSERT_TO_DB(...) \
    std::string insertToDB() const{  \
        return "INSERT INTO " + nonstatic_tableName + " ( " + #__VA_ARGS__ + " ) VALUES ( " + invoke_fields(__VA_ARGS__) + " )"; \
    }    
#endif

#ifndef UPDATE_TO_DB
/**
 * @brief 根据字段实现update语句
 * @param ... 要更新的字段，最后一个参数作为WHERE条件的索引字段
 */
#define UPDATE_TO_DB(...) \
    std::string updateToDB(){ \
        /* 获取字段名字符串并分割 */ \
        std::string fieldNames = #__VA_ARGS__; \
        std::vector<std::string> fields; \
        std::istringstream iss(fieldNames); \
        std::string field; \
        while (std::getline(iss, field, ',')) { \
            field.erase(0, field.find_first_not_of(" ")); \
            field.erase(field.find_last_not_of(" ") + 1); \
            fields.push_back(field); \
        } \
        \
        if (fields.size() < 2) { \
            return "ERROR: UPDATE requires at least one field to update and one for condition"; \
        } \
        \
        /* 获取字段对应的值 */ \
        std::vector<std::string> values; \
        auto add_value = [&](auto&& value) { \
            values.push_back(to_sql_value(value)); \
        }; \
        \
        /* 使用折叠表达式按顺序收集值 */ \
        [&]<typename... T>(T&&... args) { \
            (add_value(std::forward<T>(args)), ...); \
        }(__VA_ARGS__); \
        \
        /* 构建UPDATE语句 */ \
        std::stringstream ss; \
        ss << "UPDATE " << nonstatic_tableName << " SET "; \
        \
        /* 前n-1个字段用于SET */ \
        for (size_t i = 0; i < fields.size() - 1; ++i) { \
            ss << fields[i] << " = " << values[i]; \
            if (i != fields.size() - 2) ss << ", "; \
        } \
        \
        /* 最后一个字段用于WHERE */ \
        ss << " WHERE " << fields.back() << " = " << values.back(); \
        return ss.str(); \
    }

#endif

#ifndef DELETE_FROM_DB
/**
 * @brief 根据字段实现update语句
 * @param ... 删除时的索引字段
 */
#define DELETE_FROM_DB(key) \
    std::string deleteFromDB(){ \
        return "DELETE FROM " + nonstatic_tableName + " WHERE " + #key + " = " + to_sql_value(key); \
    }
#endif
}}


#endif