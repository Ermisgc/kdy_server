# MySQL里的表设计
MySQL的表的创建应该不由上位机控制，而是由开发者设计的。

本项目所使用的`database`取名为`kdy`:
```sql
create database kdy;   //如果没有的话要创建database
use kdy;
```

## 1. 用户表设计
涉及到用户可否登录的信息，以及用户的权限，表设计如下：
```sql
CREATE TABLE users (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_name VARCHAR(50),
    user_password VARCHAR(128),
    privilege TINYINT 
);
```
说白了就是有：`id`、`用户名`、`权限`和`privilege`四个字段。

`user_name`和`user_password`字段有字符数量上限，需要上位机进行检查。`privilege`是用户权限字段，为0表示用户，为1表示管理员，目前这么设计：
```bash
mysql> describe users;
+---------------+--------------+------+-----+---------+----------------+
| Field         | Type         | Null | Key | Default | Extra          |
+---------------+--------------+------+-----+---------+----------------+
| id            | int          | NO   | PRI | NULL    | auto_increment |
| user_name     | varchar(50)  | YES  |     | NULL    |                |
| user_password | varchar(128) | YES  |     | NULL    |                |
| privilege     | tinyint      | YES  |     | NULL    |                |
+---------------+--------------+------+-----+---------+----------------+
```

# 2. 测点表设计
创建表时的信息如下：
```sql
CREATE TABLE point_data (
    id VARCHAR(50) NOT NULL COMMENT '测点ID',
    ts BIGINT UNSIGNED NOT NULL COMMENT 'uint64_t时间戳，按插入顺序递增',
    xplacement DECIMAL(10, 4) COMMENT 'X位置',
    yplacement DECIMAL(10, 4) COMMENT 'Y位置',
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    
    PRIMARY KEY (id, ts),
    
    INDEX idx_ts (ts)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='测点数据表';
```

这里的`id`是测点ID，此外，时间戳不是MySQL生成的，而是C++程序写进去的。索引方面，建立了`id`和`ts`的联合索引作为主键，是为了方便进行范围查询：
```sql
SELECT ts, xplacement, yplacement
FROM point_data
WHERE id = '123'
  AND ts >= @current_ts - 60000
ORDER BY ts;
```
理论上这个查询建立覆盖索引`INDEX(id, ts, xplacement, yplacement)`是查询性能最优，但是要额外占空间，因此并不考虑这个覆盖索引。

此外，建立了一个额外的索引`INDEX idx_ts(ts)`，是为了便于根据范围清除旧数据。

```bash
mysql> describe point_data;
+------------+-----------------+------+-----+-------------------+-------------------+
| Field      | Type            | Null | Key | Default           | Extra             |
+------------+-----------------+------+-----+-------------------+-------------------+
| id         | varchar(50)     | NO   | PRI | NULL              |                   |
| ts         | bigint unsigned | NO   | PRI | NULL              |                   |
| xplacement | decimal(10,4)   | YES  |     | NULL              |                   |
| yplacement | decimal(10,4)   | YES  |     | NULL              |                   |
| created_at | timestamp       | YES  |     | CURRENT_TIMESTAMP | DEFAULT_GENERATED |
+------------+-----------------+------+-----+-------------------+-------------------+
5 rows in set (0.01 sec)
```


