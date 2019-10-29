//philip cho Sebastian Pokrywa
#include <string>
#include "GNode.hpp"
#include "LDisk.hpp"
#include "Tree.hpp"
#include <list>
#include <string>
#include <vector>
using namespace std;


Tree::Tree(int bs, int b){
    numBlocks = b;
    blockSize = bs;
    root = new GNode("./", 0, -1, NULL);
    currentDir = root;
    disk = new LDisk(b);
}  

void Tree::addNode(int size, std::string name){
    if(name.compare("./") == 0){
        return;
    }
    if(root->children.size() == 0){
        GNode * newNode = new GNode(name, 0, size, root);
        //not sure what to do for first parameter, totalsize, and blocksize
        newNode->file = new LFile(size, blockSize, disk);

        root->children.push_back(newNode);
        return;
    }
    
    GNode * curr = root;
    vector<string> findUpTo;
    int x = 0;
    //goes through string so that it can traverse through children
    for(int i = 0; i < name.size(); i++){
        if(name[i] == '/'){
            //doesn't capture first .//
            if(x > 1){
                string temp = name.substr(0,i);
                findUpTo.push_back(temp);
            }
            x++;
        }

    }
    int count = 0;
    //if empty, just append to the root folder
    //goes through children and finds the one that has the matching names
    if(findUpTo.empty() == false){
        while(1){
            for(std::list<GNode*>::iterator it = curr->children.begin(); it != curr->children.end(); ++it){
                if((*it)->name.compare(findUpTo[count]) == 0){
                    curr = *it;
                    count++;
                    break;
                }
                if(curr->children.size() == 0){
                    break;
                }
            }
        if(curr->children.size() == 0 || count == findUpTo.size()){
            break;
            }
        }
    }

    //adds new gNode and file
    //when entering in size, segfaulted, so hot fixed it to a value of 2
    GNode * newNode = new GNode(name, 0, size, curr);
//    newNode->file = new LFile(size, blockSize, disk);
    newNode->file = new LFile(2, blockSize, disk);
    curr->children.push_back(newNode);


}

//changes directory to specified, must include full string/path
void Tree::cd(std::string name){
    GNode * curr = root;
    vector<string> findUpTo;
    int x = 0;
    for(int i = 0; i < name.size(); i++){
        if(name[i] == '/'){
            if(x > 1){
                string temp = name.substr(0,i);
                findUpTo.push_back(temp);
            }
            x++;
        }
    }
    findUpTo.push_back(name);

    int count = 0;
    if(findUpTo.empty() == false){
        while(1){
            for(std::list<GNode*>::iterator it = curr->children.begin(); it != curr->children.end(); ++it){
                if((*it)->name.compare(findUpTo[count]) == 0){
                    curr = *it;
                    cout << curr->name << endl;
                    count++;
                    break;
                }
                if(curr->children.size() == 0){
                    break;
                }
            }
        if(curr->children.size() == 0 || count == findUpTo.size()){
            break;
            }
        }
    }
    currentDir = curr;
}
//changes currdir to parent
void Tree::cdOut(){
    if(currentDir == root){
        cout << "Current Directory is root" << endl;
    }
    else{
        currentDir = currentDir->parent;
    }

}
//lists out the current directory
void Tree::ls(){
    GNode * currDir = currentDir;
    for(std::list<GNode*>::iterator it = currDir->children.begin(); it != currDir->children.end(); ++it){
        cout << (*it)->name << "    ";
    }
    cout << endl;

}
//makes new directory
void Tree::mkdir(std::string name){
    GNode * currDir = currentDir;
    string curDirString = currDir->name;
    curDirString = curDirString + "/" + name;
    GNode * newNode = new GNode(curDirString, 0, -1, currDir);
    cout << curDirString << endl;
    //not sure what to do for first parameter, totalsize, and second parameter blocksize
    newNode->file = new LFile(-1, blockSize, disk);
    currDir->children.push_back(newNode);
}   
//creates new file with size 0
void Tree::create(std::string name){
    GNode * currDir = currentDir;
    string curDirString = currDir->name;
    curDirString = curDirString + "/" + name;
    //set size as 0 atm, perhaps can append 
    GNode * newNode = new GNode(curDirString, 0, 0, currDir);
    //not sure what to do for first parameter, totalsize, and second parameter blocksize
    newNode->file = new LFile(0, blockSize, disk);
    currDir->children.push_back(newNode);
}
//appends bytes to a file
void Tree::append(std::string name, int bytes){
    GNode * curr = Tree::lookUp(name);
    LFile *temp = curr->file;
    (*temp).append(bytes);
}
//removes bytes from a file
void Tree::remove(std::string name, int bytes){
    GNode * curr = Tree::lookUp(name);
    LFile *temp = curr->file;
    (*temp).remove(bytes);
}
//deletes a node 
void Tree::deleteNode(std::string name){
    GNode * curr = Tree::lookUp(name);
    GNode * removeNode = Tree::lookUp(name);
    curr = curr->parent;
    curr->children.remove(removeNode);
    delete removeNode;
    
}
//lists out the tree
void Tree::dir(){
    GNode * curr = root;
    vector<string> allNames;
    vector<GNode*> level;
    allNames.push_back(curr->name);
    for(std::list<GNode*>::iterator it = curr->children.begin(); it != curr->children.end(); ++it){
        level.push_back(*it);
    }
    while(level.size() != 0){
        cout << level[0]->name << endl;
        allNames.push_back((level[0]->name));
        if(level[0]->children.size() != 0){
            for(std::list<GNode*>::iterator it = level[0]->children.begin(); it != level[0]->children.end(); ++it){
                level.push_back((*it));
            }
        }
        level.erase(level.begin());
    }
    for(int i = 0; i < allNames.size(); i++){
        cout << allNames[i] << endl;
    }
}
//prints the disk
void Tree::printDisk(){
    (*disk).print(blockSize);
}
//prints all
void Tree::printFiles(){
    GNode * curr = root;
    vector<GNode*> level;
    for(std::list<GNode*>::iterator it = curr->children.begin(); it != curr->children.end(); ++it){
        level.push_back((*it));
    }
    while(!level.empty()){
        LFile * currFile = level[0]-> file;
        if((*currFile).getSize() != -1){
            (*currFile).print();
        }
        for(std::list<GNode*>::iterator it = level[0]->children.begin(); it != level[0]->children.end(); ++it){
            level.push_back((*it));
        }
        level.erase(level.begin());
    }

}


GNode * Tree::lookUp(std::string name){
GNode * curr = root;
    vector<string> findUpTo;
    int x = 0;
    for(int i = 0; i < name.size(); i++){
        if(name[i] == '/'){
            if(x > 1){
                string temp = name.substr(0,i);
                findUpTo.push_back(temp);
            }
            x++;
        }
    }
    findUpTo.push_back(name);
    int count = 0;
    if(findUpTo.empty() == false){
        while(1){
            for(std::list<GNode*>::iterator it = curr->children.begin(); it != curr->children.end(); ++it){
                if((*it)->name.compare(findUpTo[count]) == 0){
                    curr = *it;
                    count++;
                    break;
                }
                if(curr->children.size() == 0){
                    break;
                }
            }
        if(curr->children.size() == 0 || count == findUpTo.size()){
            break;
            }
        }
    }
    return curr;
}
Tree::~Tree(){
    GNode * curr = root;
    vector<GNode*> level;
    for(std::list<GNode*>::iterator it = curr->children.begin(); it != curr->children.end(); ++it){
        level.push_back((*it));
    }
    while(!level.empty()){
        GNode * temp;
        temp = level[0]; 
        for(std::list<GNode*>::iterator it = level[0]->children.begin(); it != level[0]->children.end(); ++it){
            level.push_back((*it));
        }
        level.erase(level.begin());
        delete temp;
    }

}