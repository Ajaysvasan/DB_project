#ifndef _DBMS_H
#define _DBMS_H 
#include<vector>
#include<string>
#include<string>
#include<unordered_map>
class Commands{
    private:  
        std::string curr_db;
        void createDatabase(std::string database_name);
        void createTable(std::string table_name,std::string curr_database,std::unordered_map<std::string,std::string>attributes);
        void insert(std::string table_name, std::string curr_database, std::vector<std::string> values);       
        void deleteEntry();
        void display();
        void modify();
        void alter();
        void use(std::string curr_db);
        void change(std::string change_db);
    public:
        void execute(std::string query);
};

#endif