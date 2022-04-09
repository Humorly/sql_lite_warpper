#ifndef __SQL_LITE_WARPPER_H__
#define __SQL_LITE_WARPPER_H__

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <vector>

#include <iostream>
#include <string>
#include "log_writer.h"
#include "sql_operate_ipml.h"

class sql_lite_warpper : public sql_operate_ipml
{
public:
	// 构建连接
    sql_lite_warpper(std::string name)
	{
		if (QSqlDatabase::contains("qt_sql_default_connection"))
			db_ = QSqlDatabase::database("qt_sql_default_connection");
		else
		{
			// 建立连接
			db_ = QSqlDatabase::addDatabase("QSQLITE");
			// 设置名字
            db_.setDatabaseName(name.c_str());
		}
		// 打开
		open();
	}

    ~sql_lite_warpper() { close(); };

    // 打开数据库
	bool open(void) 
	{
		if (!db_.open())
		{
			qDebug() << "Error: Failed to connect database." << db_.lastError();
			return false;
		}
		return true;
	}

    // 是否存在
	bool exist(std::string table_name)
	{
		QSqlDatabase base_ = QSqlDatabase::database();
		if (base_.tables().contains(table_name.c_str()))
			return true;

		return false;
	}

	// 创建数据表
	bool create(const std::string& command) { return invoke(command); }
	bool insert(const std::string& command) { return invoke(command); }
	bool remove(const std::string& command) { return invoke(command); }
	bool update(const std::string& command) { return invoke(command); }

	// 增加 —> insert命令由command指出
	template <typename __type, typename ... params>
	bool insert(const std::string& command, std::tuple<__type, params...> val)
	{
		return __invoke_template(command, val);
	}

	// 删除 —> delete命令由command指出
	template <typename __type, typename ... params>
	bool remove(const std::string& command, std::tuple<__type, params...> val)
	{
		return __invoke_template(command, val);
	}

	// 增加 —> update命令由command指出
	template <typename __type, typename ... params>
	bool update(const std::string& command, std::tuple<__type, params...> val)
	{
		return __invoke_template(command, val);
	}

	// 查询
	template <typename __holder_type, typename __type, typename ... params>
	bool select(const std::string& command, __holder_type holder, std::vector<std::tuple<__type, params...>>& dest)
	{
		QSqlQuery sql_;
		// 构建命令
		QString command_ = QString(command.c_str());
		sql_.prepare(command_);

		// 合成参数
		synthesis(&sql_, holder);

		if (!sql_.exec())
			qDebug() << "Error: Fail to query table. " << sql_.lastError();
		else
		{
			// 提取列表
			while (sql_.next())
			{
				separation<std::tuple<__type, params...>, QSqlQuery, __type, params...>(dest, sql_);
			}
		}
		return true;
	}

    // 关数据库
	void close(void) { db_.close(); }

private:
    QSqlDatabase db_;

	// 执行语句
	bool invoke(const std::string& command)
	{
		QSqlQuery sql_;
		// 构建命令
		QString command_ = QString(command.c_str());
		sql_.prepare(command_);
		// 执行命令
		if (!sql_.exec())
		{
			qDebug() << "Error: Fail to create table. " << sql_.lastError();
			return false;
		}
		qDebug() << "Table created!";
		return true;
	}

	// 执行模板
	template <typename __type>
	bool __invoke_template(const std::string& command, __type& val)
	{
		QString command_ = QString(command.c_str());

		QSqlQuery sql_;
		sql_.prepare(command_);

		// 合成参数
		synthesis(&sql_, val);

		if (!sql_.exec())
		{
			std::string err = sql_.lastError().text().toStdString();
			qDebug() << "Error: Fail to insert data. " << sql_.lastError();
			return false;
		}
		return true;
	}
};

#endif //  __SQL_LITE_WARPPER_H__
