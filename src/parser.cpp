#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include<sstream>
#include <dirent.h>
#include <sys/types.h>
#include<algorithm>
#include<unordered_map>
#include "../include/Parser.h"


Tokenizer::Tokenizer(){
    std::cout<<"Getting all databases \n";
    DIR *dir;
    struct dirent *en;
    dir = opendir("databases");
    if(dir){
        while((en = readdir(dir)) != NULL){
            std::string name = en->d_name;
            if(name != "." && name != "..") {
                list_databases.push_back(name);
            }
        }
        closedir(dir);
    }
}

void Tokenizer::getQuery(std::string query){
    this->query = query;
    }
void Tokenizer::tokenize() {
    std::string current;
    for(char c : query){
        if(isspace(c)){
            if(!current.empty()){
                tokens.push_back(current);
                current.clear();
            }
        }else if(c == '(' || c == ')' || c == ',' || c == ';'|| c == '=' || c == '*' || c=='\''){
            if(!current.empty()){
                tokens.push_back(current);
                current.clear();
            }
            tokens.push_back(std::string(1,c));
        }else{
            current+=tolower(c);
        }
    }
}
int Tokenizer::validAttrType(std::string attr_type){
    // Using DATA_TYPE:: just for clarity you can use the values just by usig INT etc
    if(attr_type == "int") return DATA_TYPE::INT;
    else if(attr_type == "varchar") return DATA_TYPE::STRING;
    else if(attr_type == "bool") return DATA_TYPE::BOOL;
    else if(attr_type == "bigint") return DATA_TYPE::BIGINT;
    else if(attr_type == "smallint") return DATA_TYPE::SMALLINT;
    else if(attr_type == "float") return DATA_TYPE::FLOAT;
    else if(attr_type == "real") return DATA_TYPE::REAL;
    else return DATA_TYPE::INVALID;
}
    

Command_type Tokenizer::handleCommand(){
    const std::string& cmd = tokens[0];
    const std::string& obj = tokens[1];
    if(tokens.size() < 3  && cmd != "use") return Command_type::UNKNOWN;

    if(cmd == "create" && obj == "database") return Command_type::CREATE_DATABASE;
    else if(cmd == "create" && obj == "table") return Command_type::CREATE_TABLE;
    else if(cmd == "insert") return Command_type::INSERT;
    else if(cmd == "update") return Command_type::UPDATE_;
    else if(cmd == "select") return Command_type::SELECT;
    else if(cmd ==  "delete") return Command_type::DELETE_;
    else if(cmd == "use") return Command_type::USE;
    else if(cmd == "change") return Command_type::CHANGE; 
    else return Command_type::UNKNOWN;
}

bool Tokenizer::validTableName(){
    return true;
}

bool Tokenizer::validDataBaseName(){
    return true;
}

bool Tokenizer::validateCreateCommand(){
    // For Create command create table table_name(id int , name varchar, age int) or create database my_db;
    if(tokens.size() < 3){
        std::cerr<<"Invalid Syntax"<<"\n";
        exit(EXIT_FAILURE);
    }

    int command_type = handleCommand();
    int size = tokens.size();
    if(command_type == CREATE_DATABASE && tokens[size-1] == ";"){
        database_name = tokens[2];
        return true;
    }else if(command_type == CREATE_TABLE && tokens[size - 1] == ";"){        
        table_name = tokens[2];
        std::cout<<"Table to be created "<<table_name<<"\n";
        int curr = 3;
        if(tokens[curr++] != "("){
            std::cerr << "Expected '('\n"<<"\n";
            return false;
        }
        while(curr < size-1){
            if(tokens[curr] == ")") break;
            if(curr + 2 >= size || tokens[curr] == "," || tokens[curr] == "("){
                curr++;
                continue;
            }
            std::string attr_name = tokens[curr++];
            std::string attr_type = tokens[curr++];
            if(validAttrType(attr_type)){
                attributes[attr_name] = attr_type;
                if(tokens[curr] == ",") curr++;
            }else{
                std::cerr<<"Invalid data type"<<"\n";
                exit(EXIT_FAILURE);
            }
        }
        if(tokens[curr] != ")"){
            std::cerr << "Expected ')' before ';'\n";
            return false;
        }
        return true;
    }
    return false;
}

bool Tokenizer::validInsertCommand(){
    table_name = tokens[2];
    size_t pos = query.find("values");
    size_t lparen = query.find("(",pos);
    size_t rparen = query.find(")",lparen);
    std::string val_str = query.substr(lparen+1 , rparen - lparen - 1);
    std::stringstream ss(val_str);
    std::string value;
    while(std::getline(ss,value,',')){
        value.erase(remove_if(value.begin(),value.end(),isspace),value.end());
        values.push_back(value);
    }
    return true;
}

std::vector<std::string>Tokenizer::getValues(){
    return values;
}

bool Tokenizer::validUpdateCommand() {
    if (tokens.size() < 6 || tokens[0] != "update" || tokens[2] != "set") return false;
    table_name = tokens[1];

    std::string set_expr = tokens[3];
    size_t eq_pos = set_expr.find('=');
    if (eq_pos == std::string::npos) return false;
    update_column = set_expr.substr(0, eq_pos);
    update_value = set_expr.substr(eq_pos + 1);

    if (tokens[4] != "WHERE") return false;
    std::string where_expr = tokens[5];
    eq_pos = where_expr.find('=');
    if (eq_pos == std::string::npos) return false;
    where_column = where_expr.substr(0, eq_pos);
    where_value = where_expr.substr(eq_pos + 1);

    return true;
}


bool Tokenizer::validSelectCommand() {
    if (tokens.size() != 4) return false;
    if (tokens[0] != "select" || tokens[1] != "*" || tokens[2] != "from") return false;

    table_name = tokens[3];
    return true;
}


bool Tokenizer::validDeleteCommand() {
    if (tokens.size() != 6 || tokens[0] != "delete" || tokens[1] != "from" || tokens[3] != "where") return false;

    table_name = tokens[2];

    std::string cond_expr = tokens[4] + tokens[5];
    size_t eq_pos = cond_expr.find('=');
    if (eq_pos == std::string::npos) return false;

    where_column = cond_expr.substr(0, eq_pos);
    where_value = cond_expr.substr(eq_pos + 1);

    return true;
}


bool Tokenizer::validUseCommand(std::string curr_database){
    // use command -> use <database_name>;
    if(tokens.size()<2){
        std::cerr<<"Invalid command";
        return false;
    }
    std::string db_name = tokens[1];
    std::cout<<"The db to use is"<<db_name<<"\n";
    auto it = std::find(list_databases.begin(),list_databases.end(),db_name);
    if(it != list_databases.end()){
        std::cout<<"database found\n";
        curr_db = db_name;
        return true;
    }
    return false;
}

bool Tokenizer::validChangeCommand(){
    // change command -> change <database_name>;
    if(tokens.size()<2){
        std::cerr<<"Invalid command";
        return false;
    }
    std::string db_name = tokens[1];
    std::cout<<"The db to change is"<<db_name<<"\n";
    auto it = std::find(list_databases.begin(),list_databases.end(),db_name);
    if(it != list_databases.end()){
        std::cout<<"database found\n";
        curr_db = db_name;
        std::cout<<"Databased changed to"<<curr_db<<"\n";
        return true;
    }
    return false;
}

std::string Tokenizer::getTableName(){
    return table_name;
}
//Create Database 
// use database 
std::string Tokenizer::getDatabaseName(){
    int command_type = handleCommand();
    if(command_type == CREATE_DATABASE){
        return tokens[2];
    }else if(command_type == USE){
        return tokens[1];
    }else{
        std::cerr<<"Invalid Query\n";
        return "";
    }
}

std::vector<std::string> Tokenizer::getTokens(){
    return tokens;
}

std::unordered_map<std::string,std::string> Tokenizer::getAttributes(){
    return Tokenizer::attributes;
}

void Tokenizer::clear(){
    std::cout<<"Command executed successfully\n";
    tokens.clear();
    attributes.clear();
}

void Tokenizer::display(){
    std::cout<<"The entered query (after processing) is:"<<"\n";
    for(const std::string& token:tokens){
        std::cout<<token<<" ";
    }
    std::cout<<"\n";
}

void Tokenizer::displayTableAttributes(){
    for(const auto& values:attributes){
        std::cout<<values.first<<" -> "<<values.second<<"\n";
    }
    std::cout<<"\n";
}

void Tokenizer::displayDatabases(){
    if(list_databases.empty()){
        std::cerr<<"messed up\n";
    }
    for(const auto& value : list_databases){
        std::cout << "\n" << value << "\n";
    }
}

    // Just in case if the tokens array is not cleared 
Tokenizer::~Tokenizer(){
    clear();
    query.clear();
}
