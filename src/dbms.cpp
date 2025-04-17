#include "../include/dbms.h"
#include "../include/Parser.h"
#include<fstream>
#include<filesystem>
#include<unordered_map>
#include<iostream>
Tokenizer parser;
namespace fs = std:: filesystem;

void Commands::createDatabase(std::string database_name){
    std::string db_path = "./databases/" + database_name;
    if(fs::exists(db_path)){
        std::cerr<<"Command failed compile\nDatabase already exists"<<"\n";
        return;
    }
    fs::create_directories(db_path);
    std::cout<<"Command exected succefully";
}

void Commands::createTable(std::string table_name,std::string curr_database ,std::unordered_map<std::string,std::string>attributes){
    std::string db_path = "./databases/" + curr_database;
    std::string table_path = db_path + "/" + table_name + ".table";
    if(!fs::exists(db_path)){
        std::cerr<<"Command failed to compile\nDatabase doesn't exists"<<"\n";
        return;
    }
    if(fs::exists(table_path)){
        std::cerr<<"Command failed to compile\nTable "<<table_name<<" already exists"<<"\n";
        return;
    }
    std::ofstream tablefile(table_path);
    if(!tablefile){
        std::cerr<<"Error in creating the file"<<"\n";
        return;
    }
    for(const auto& arr:attributes){
        tablefile<<arr.first<<" "<<arr.second<<"\n";
    }
    tablefile.close();
    std::cout<<"Command executed successfully"<<"\n";
}

void Commands::insert(std::string table_name, std::string curr_database, std::vector<std::string> values) {
    std::string table_path = "./databases/" + curr_database + "/" + table_name + ".table";
    std::cout<<table_path<<"\n";
    if (!fs::exists(table_path)) {
        std::cerr << "Table does not exist.\n";
        return;
    }

    std::ofstream outfile(table_path, std::ios::app); // open in append mode
    if (!outfile) {
        std::cerr << "Error opening table file.\n";
        return;
    }

    for (size_t i = 0; i < values.size(); ++i) {
        outfile << values[i];
        if (i != values.size() - 1) outfile << " ";
    }
    outfile << "\n";
    outfile.close();

    std::cout << "Insert successful!\n";
}


void Commands::deleteEntry() {
    std::string table_path = "./databases/" + curr_db + "/" + parser.table_name + ".table";

    if (!fs::exists(table_path)) {
        std::cerr << "Table does not exist.\n";
        return;
    }

    std::ifstream infile(table_path);
    std::ofstream temp("temp.table");

    std::string line;
    bool deleted = false;

    while (getline(infile, line)) {
        if (line.find(parser.where_value) != std::string::npos) {
            deleted = true;
            continue; // Skip the matching line
        }
        temp << line << "\n";
    }

    infile.close();
    temp.close();

    fs::remove(table_path);
    fs::rename("temp.table", table_path);

    if (deleted)
        std::cout << "Entry deleted successfully.\n";
    else
        std::cout << "No matching entry found.\n";
}


void Commands::display() {
    std::string table_path = "./databases/" + curr_db + "/" + parser.table_name + ".table";

    if (!fs::exists(table_path)) {
        std::cerr << "Table does not exist.\n";
        return;
    }

    std::ifstream infile(table_path);
    std::string line;

    std::cout << "Contents of table " << parser.table_name << ":\n";
    while (getline(infile, line)) {
        std::cout << line << "\n";
    }
    infile.close();
}


void Commands::modify() {
    std::string table_path = "./databases/" + curr_db + "/" + parser.table_name + ".table";

    if (!fs::exists(table_path)) {
        std::cerr << "Table does not exist.\n";
        return;
    }

    std::ifstream infile(table_path);
    std::ofstream temp("temp.table");

    std::string line;
    bool updated = false;

    while (getline(infile, line)) {
        if (line.find(parser.where_value) != std::string::npos) {
            // Replace with new value
            size_t col_pos = line.find(parser.update_column);
            if (col_pos != std::string::npos) {
                // Replace the whole line with new values (simplified logic)
                temp << parser.update_column << " " << parser.update_value << "\n";
                updated = true;
                continue;
            }
        }
        temp << line << "\n";
    }

    infile.close();
    temp.close();

    fs::remove(table_path);
    fs::rename("temp.table", table_path);

    if (updated)
        std::cout << "Entry updated successfully.\n";
    else
        std::cout << "No matching entry found to update.\n";
}


void Commands::use(std::string curr_db){
    if(!this->curr_db.empty()){
        std::cerr<<"Database "<<this->curr_db<<" is already in use\n";
        return;
    }
    this->curr_db = curr_db;
    std::cout<<curr_db<<"\n";
}

void Commands::change(std::string change_db){
    this->curr_db = change_db;
    std::cout<<curr_db<<"\n";
}

void Commands::alter(){

}

void Commands::execute(std::string query){
    std::string database_name;
    std::string table_name;
    std::string curr_database;

    parser.getQuery(query);    
    parser.tokenize();
    parser.display();
    int command_type = parser.handleCommand();
    std::cout<<"Command type is "<<command_type<<"\n";

    switch(command_type){
        case 0:{ // CREATE DATABASE
            parser.validateCreateCommand();
            database_name = parser.database_name;
            createDatabase(database_name);
            parser.clear();
            break;
        }        
        case 1:{ // USE DATABASE
            std::vector<std::string> tk = parser.getTokens();
            parser.validUseCommand(tk[1]);
            std::cout<<"The database to use is "<<parser.curr_db<<"\n";
            use(parser.curr_db);
            std::cout<<"Command executed successfully\n";
            parser.clear();
            break;
        }
        case 2:{ // CREATE TABLE
            parser.validateCreateCommand();
            parser.displayTableAttributes();
            table_name = parser.getTokens()[2];
            std::cout<<"Table to be created "<<parser.table_name<<"\n";
            createTable(table_name, parser.curr_db, parser.getAttributes());
            parser.clear();
            break;
        }

        case 3:{ // INSERT INTO TABLE
            std::cout<<"Got executed\n";
            parser.display();
            parser.validInsertCommand();
            table_name = parser.table_name;
            std::cout<<"Table "<<table_name<<" in use\n";
            insert(table_name, curr_db, parser.getValues());
            parser.clear();
            break;
        }

        case 4:{ // CHANGE DATABASE
            parser.validChangeCommand();
            std::cout<<"Database to change "<<parser.curr_db<<"\n";
            change(parser.curr_db);
            parser.clear();
            break;
        }

        case 5:{ // SELECT
            parser.validSelectCommand();
            table_name = parser.table_name;
            std::cout << "Selecting from table: " << table_name << "\n";
            display();
            parser.clear();
            break;
        }

        case 6:{ // UPDATE
            parser.validUpdateCommand();
            table_name = parser.table_name;
            std::cout << "Modifying table: " << table_name << "\n";
            modify();
            parser.clear();
            break;
        }

        case 7:{ // DELETE
            parser.validDeleteCommand();
            table_name = parser.table_name;
            std::cout << "Deleting from table: " << table_name << "\n";
            deleteEntry();
            parser.clear();
            break;
        }

        default:{
            std::cerr << "Command failed to compile\nInvalid command\n";
            return;
        }
    }
}
