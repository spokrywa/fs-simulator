//philip cho Sebastian Pokrywa

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "GNode.hpp"
#include "Tree.hpp"
#include "LDisk.hpp"

void useStatement(){
    std::cout<<"./driver -f input_file -d input_dir -s disk_size -b block_size"<<std::endl;
}

int main(int argc, char* argv[]){

    //PARSE ARGS
    std::string fileList = "";
    std::string dirList = "";
    int diskSize = -1;
    int blockSize = -1;
    char flag;

    while((flag = getopt(argc, argv, "f:d:s:b:")) != -1)
    {
        switch(flag)
        {
            case 'f':
                fileList = optarg;
                break;
            case 'd':
                dirList = optarg;
                break;
            case 's':
                diskSize = atoi(optarg);
                break;
            case 'b':
                blockSize = atoi(optarg);
                break;
            case '?':
                useStatement();
                exit(1);
            default:
                useStatement();
                exit(1);
        }
    }

    // ERROR CHECKING
    if(diskSize < 0 || blockSize < 0 || fileList.length() < 1 || dirList.length() < 1)
    {
        useStatement();
        exit(1);
    }

    //READING STREAMS
    std::ifstream fileStream(fileList);
    std::ifstream directoryStream(dirList);
    std::string line;
    size_t index=0;

    if (!directoryStream){
        std::cerr<< "Cannot open file " <<std::endl;
        exit(1);
    }

    int numBlocks= diskSize/blockSize;
    
    //CREATE THE DIRECTORY
    Tree directory= Tree(blockSize,numBlocks);
    
    //READ THE DIRECTORIES
    while(std::getline(directoryStream, line)){
        //ADD EACH DIRECTORY AS A NODE
        directory.addNode(-1,line);
    }
    directoryStream.close();

    if (!fileStream){
        std::cerr<< "Cannot open file" <<std::endl;
    }

    //READ THE FILES
    while (std::getline(fileStream,line)){
        //PARSE THE LINE INTO NAME AND SIZE
        //MAY VARY
        std::string name;
        int size=0;

        std::string buffer;
        std::stringstream stream(line);
        int ctr=0;

        while (stream >> buffer){
            if (ctr==6){
                size= atoi(buffer.c_str());
                //size = atoi(buffer);
                //std::cout << "Size: " << size << std::endl;
            }
            if (ctr == 10){
                name= buffer;
                //std::cout << "Name: " << name << std::endl;
            }
            ctr++;
        }
        // ADD EACH TO THE DIRECTORY
        directory.addNode(size,name);
    }
    fileStream.close();

    //HANDLE USER INPUT
    while(1){
        std::cout << "FileSystem/Shell: ";
        std::string command;
        std::string arg1;
        std::string arg2;
        getline(std::cin,command);

        std::stringstream stream(command);

        stream >> command;
        stream >> arg1;
        stream >> arg2;

        if (!command.compare("cd")){
            directory.cd(arg1);
        }

        else if(!command.compare("cd..")){
            directory.cdOut();
        }

        else if(!command.compare("ls")){
            directory.ls();
        }

        else if(!command.compare("mkdir")){
            if (arg1.compare("")){
                directory.mkdir(arg1);
            }
            else{
                std::cout << "Need more arguments" <<std::endl;
            }   
        }

        else if(!command.compare("create")){
            if (arg1.compare("")){
                directory.create(arg1);
            }
            else{
                std::cout << "Need more arguments" <<std::endl;
            }   
        }

        else if(!command.compare("append")){
            if (arg1.compare("") && arg2.compare("")){
                directory.append(arg1,stoi(arg2));
            }
            else{
                std::cout << "Need more arguments" <<std::endl;
            }   
        }

        else if(!command.compare("remove")){
            if (arg1.compare("") && arg2.compare("")){
                directory.remove(arg1,stoi(arg2));
            }
            else{
                std::cout << "Need more arguments" <<std::endl;
            }   
        }

        else if(!command.compare("delete")){
            directory.deleteNode(arg1); 
        }

        else if(!command.compare("exit")){
            std::cout << "Bye" << std::endl;
            //return 0;
            break;
        }

        else if(!command.compare("dir")){
            directory.dir();
        }

        else if(!command.compare("prfiles")){
            directory.printFiles();
        }

        else if(!command.compare("prdisk")){
            directory.printDisk();
        }

        else{
            std::cout << "Command does not exist" <<std::endl;
        }
    }
}
