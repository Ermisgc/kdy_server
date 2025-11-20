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