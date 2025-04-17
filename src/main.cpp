#include "../include/Parser.h"
#include "../include/dbms.h"
#include<bits/stdc++.h> 
#include<string>
int main() {
    Commands cmd;
    std::string query;
    while(true){
        std::cout<<"Enter the query\n";
        std::getline(std::cin , query);
        if(query == "EXIT") break;
        cmd.execute(query);
    }
    return 0;
}
