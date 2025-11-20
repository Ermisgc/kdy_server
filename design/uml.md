@startuml Database Architecture

!define abstract abstract
!define interface interface

package "应用层" {
  class UserService {
    + getUserProfile(userId: int): UserProfile
    + createUser(user: User): bool
  }
  
  class Application {
    + main()
  }
}

package "数据访问层" {
  class UserRepository {
    - db: IDatabase
    + getAllUsers(): vector<User>
    + getUserById(id: int): User
    + createUser(user: User): bool
  }
  
  class ProductRepository {
    - db: IDatabase
    + getProducts(): vector<Product>
  }
}

package "抽象接口层" <<interface>> {
  interface IDatabase {
    + connect(host, user, password, database): bool
    + disconnect(): void
    + executeQuery(sql: string): IResultSet
    + executeUpdate(sql: string): int
    + prepareStatement(sql: string): IPreparedStatement
    + beginTransaction(): void
    + commit(): void
    + rollback(): void
  }
  
  interface IResultSet {
    + next(): bool
    + getInt(column: string): int
    + getString(column: string): string
    + getDouble(column: string): double
  }
  
  interface IPreparedStatement {
    + setInt(index: int, value: int): void
    + setString(index: int, value: string): void
    + setDouble(index: int, value: double): void
    + execute(): bool
    + executeQuery(): IResultSet
    + executeUpdate(): int
  }
}

package "具体实现层" {
  class MySQLDatabase {
    - driver: MySQL_Driver
    - connection: Connection
    + connect(...): bool
    + disconnect(): void
    + executeQuery(...): IResultSet
    + prepareStatement(...): IPreparedStatement
  }
  
  class MySQLResultSet {
    - resultSet: sql::ResultSet
    + next(): bool
    + getInt(column: string): int
    + getString(column: string): string
  }
  
  class MySQLPreparedStatement {
    - stmt: sql::PreparedStatement
    + setInt(...): void
    + setString(...): void
    + executeQuery(): IResultSet
  }
}

package "工厂层" {
  class DatabaseFactory {
    + createDatabase(type: DatabaseType): IDatabase
  }
  
  enum DatabaseType {
    MYSQL
    POSTGRESQL
    SQLITE
  }
}

package "数据模型" {
  class User {
    - id: int
    - name: string
    - email: string
    - age: int
    + getId(): int
    + getName(): string
  }
  
  class Product {
    - id: int
    - name: string
    - price: double
  }
}

' 依赖关系
UserService --> UserRepository
Application --> UserService
Application --> DatabaseFactory

' 组合关系
UserRepository *--> IDatabase
ProductRepository *--> IDatabase

' 实现关系
MySQLDatabase ..|> IDatabase
MySQLResultSet ..|> IResultSet
MySQLPreparedStatement ..|> IPreparedStatement

' 关联关系
IDatabase --> IResultSet
IDatabase --> IPreparedStatement
MySQLDatabase --> MySQLResultSet
MySQLDatabase --> MySQLPreparedStatement

' 创建关系
DatabaseFactory --> IDatabase
DatabaseFactory --> DatabaseType

' 使用关系
UserRepository --> User
ProductRepository --> Product

note right of IDatabase
  多态核心接口
  支持多种数据库实现
  便于单元测试
end note

note right of DatabaseFactory
  工厂模式
  统一创建逻辑
  易于扩展新数据库类型
end note

note right of UserRepository
  Repository模式
  封装数据访问逻辑
  分离业务与数据层
end note

@enduml