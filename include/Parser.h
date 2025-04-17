#pragma once
#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <vector>
#include <string>
#include<iostream>
#include<unordered_map>
#include <dirent.h>
#include <sys/types.h>

enum Command_type {
    UNKNOWN = -1,
    CREATE_DATABASE,
    USE,
    CREATE_TABLE,
    INSERT,
    CHANGE,
    SELECT,
    UPDATE_,
    DELETE_,
};

enum DATA_TYPE {
    INVALID,
    INT,
    STRING,
    FLOAT,
    NULL_,
    SMALLINT,
    REAL,
    BIGINT,
    BOOL
};

class Tokenizer {
private:
    std::string query;
    std::vector<std::string> tokens;
    std::unordered_map<std::string, std::string> attributes;
    std::vector<std::string>values;
    std::vector<std::string> list_databases;
public:
std::string where_column,update_column,update_value,where_value;
Tokenizer();
    std::string table_name, database_name;
    std::string curr_db;

    void getQuery(std::string query);
    void tokenize();
    void clear();
    void display();
    int validAttrType(std::string attr_type);
    Command_type handleCommand();
    bool validateCreateCommand();
    bool validTableName();
    bool validDataBaseName();
    bool validInsertCommand();
    bool validUpdateCommand();
    bool validSelectCommand();
    bool validDeleteCommand();
    bool validUseCommand(std::string curr_database);
    bool validChangeCommand();
    std::vector<std::string> getTokens();
    std::vector<std::string> getValues();
    std::string getTableName();
    std::string getDatabaseName();
    std::unordered_map<std::string,std::string> getAttributes();
    void displayTableAttributes();
    void displayDatabases();

    ~Tokenizer();
};
#endif