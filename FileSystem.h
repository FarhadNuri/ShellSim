#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <functional>

using namespace std;

// Simple struct for file/folder information
struct FileNode {
    string name;
    string type;  // "file" or "folder"
    int size;
    string createdAt;
    string content;  // File content
    FileNode* parent;
    vector<FileNode*> children;
    unordered_map<string, FileNode*> childMap;  // Quick lookup
    
    // Constructor for files and folders
    FileNode(string n, string t, FileNode* p = nullptr) {
        name = n;
        type = t;
        size = 0;
        content = "";
        parent = p;
        
        // Get current time as string
        time_t now = time(0);
        createdAt = ctime(&now);
        // Remove newline from ctime
        if (!createdAt.empty() && createdAt.back() == '\n') {
            createdAt.pop_back();
        }
    }
};

class FileSystem {
private:
    FileNode* root;
    FileNode* currentDir;
    FileNode* previousDir;
    FileNode* homeDir;
    
public:
    FileSystem();
    ~FileSystem();
    
    // Main commands
    void mkdir(string name);
    void touch(string name);
    void cd(string name);
    void ls();
    void rm(string name);
    void cat(string name);
    void pwd();
    void echo(string content, string filename, bool append);
    void cp(string source, string dest);
    void mv(string oldName, string newName);
    void find(string name);
    void tree();
    void tree_helper(FileNode* node, string prefix, bool isLast);
    
    // Helper functions
    void printBox(string content);
    void printHeader(string title);
    bool isValidName(string name);
    void deleteNode(FileNode* node);
};

#endif