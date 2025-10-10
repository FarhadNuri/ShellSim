#include "FileSystem.h"

FileSystem::FileSystem() {
    // Create root directory
    root = new FileNode("root", "folder");
    currentDir = root;
    previousDir = root;
    
    // Create home directory
    homeDir = new FileNode("home", "folder", root);
    root->children.push_back(homeDir);
    root->childMap["home"] = homeDir;
    
    printHeader("File System Simulator Started");
    cout << "Welcome! You are in the root directory." << endl;
    cout << "Available commands: mkdir, touch, cd, ls, rm, cat, pwd, echo, cp, mv, find, tree, exit" << endl;
    printBox("Type 'help' for detailed command information");
}

FileSystem::~FileSystem() {
    deleteNode(root);
}

void FileSystem::deleteNode(FileNode* node) {
    if (node == nullptr) return;
    
    // Delete all children first
    for (FileNode* child : node->children) {
        deleteNode(child);
    }
    delete node;
}

void FileSystem::printBox(string content) {
    int length = content.length();
    
    // Top border
    cout << "+";
    for (int i = 0; i < length + 2; i++) cout << "-";
    cout << "+" << endl;
    
    // Content with side borders
    cout << "| " << content << " |" << endl;
    
    // Bottom border
    cout << "+";
    for (int i = 0; i < length + 2; i++) cout << "-";
    cout << "+" << endl;
}

void FileSystem::printHeader(string title) {
    cout << endl;
    cout << "+======================================+" << endl;
    cout << "|        " << title;
    
    // Add spaces to center the title
    int spaces = 22 - title.length();
    for (int i = 0; i < spaces; i++) cout << " ";
    
    cout << "|" << endl;
    cout << "+======================================+" << endl;
    cout << endl;
}

bool FileSystem::isValidName(string name) {
    if (name.empty()) return false;
    if (name == "." || name == "..") return false;
    
    // Check for invalid characters
    for (char c : name) {
        if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || c == '<' || c == '>' || c == '|') {
            return false;
        }
    }
    return true;
}

void FileSystem::mkdir(string name) {
    if (!isValidName(name)) {
        printBox("Error: Invalid folder name!");
        return;
    }
    
    // Check if already exists
    if (currentDir->childMap.find(name) != currentDir->childMap.end()) {
        FileNode* existing = currentDir->childMap[name];
        if (existing->type == "folder") {
            printBox("Error: Folder '" + name + "' already exists!");
        } else {
            printBox("Error: File '" + name + "' already exists!");
        }
        return;
    }
    
    // Create new folder
    FileNode* newFolder = new FileNode(name, "folder", currentDir);
    currentDir->children.push_back(newFolder);
    currentDir->childMap[name] = newFolder;
    
    printBox("Created folder: " + name);
}

void FileSystem::touch(string name) {
    if (!isValidName(name)) {
        printBox("Error: Invalid file name!");
        return;
    }
    
    // Check if already exists
    if (currentDir->childMap.find(name) != currentDir->childMap.end()) {
        FileNode* existing = currentDir->childMap[name];
        if (existing->type == "file") {
            printBox("Error: File '" + name + "' already exists!");
        } else {
            printBox("Error: Folder '" + name + "' already exists!");
        }
        return;
    }
    
    // Create new file
    FileNode* newFile = new FileNode(name, "file", currentDir);
    currentDir->children.push_back(newFile);
    currentDir->childMap[name] = newFile;
    
    printBox("Created file: " + name);
}

void FileSystem::cd(string name) {
    if (name == "..") {
        // Go to parent directory
        if (currentDir->parent != nullptr) {
            previousDir = currentDir;
            currentDir = currentDir->parent;
            printBox("Moved to parent directory");
        } else {
            printBox("Already at root directory!");
        }
        return;
    }
    
    if (name == ".") {
        printBox("Already in current directory");
        return;
    }
    
    if (name == "-") {
        // Go to previous directory
        if (previousDir != currentDir) {
            FileNode* temp = currentDir;
            currentDir = previousDir;
            previousDir = temp;
            printBox("Switched to previous directory");
        } else {
            printBox("No previous directory to switch to!");
        }
        return;
    }
    
    if (name == "/") {
        // Go to root directory
        previousDir = currentDir;
        currentDir = root;
        printBox("Changed to root directory");
        return;
    }
    
    if (name == "~") {
        // Go to home directory
        previousDir = currentDir;
        currentDir = homeDir;
        printBox("Changed to home directory");
        return;
    }
    
    // Check if trying to cd into the same directory
    if (name == currentDir->name) {
        printBox("Already inside directory: " + name);
        return;
    }
    
    // Find the directory
    if (currentDir->childMap.find(name) == currentDir->childMap.end()) {
        printBox("Error: Directory '" + name + "' not found!");
        return;
    }
    
    FileNode* target = currentDir->childMap[name];
    if (target->type != "folder") {
        printBox("Error: '" + name + "' is not a directory!");
        return;
    }
    
    previousDir = currentDir;
    currentDir = target;
    printBox("Changed directory to: " + name);
}

void FileSystem::ls() {
    // Build full path for display
    string path = "";
    FileNode* current = currentDir;
    vector<string> pathParts;
    
    // Build path from current to root
    while (current != nullptr) {
        pathParts.push_back(current->name);
        current = current->parent;
    }
    
    // Reverse to get correct order
    path = "/";
    for (int i = pathParts.size() - 1; i >= 0; i--) {
        if (pathParts[i] != "root") {
            path += pathParts[i] + "/";
        }
    }
    
    if (path.length() > 1 && path.back() == '/') {
        path.pop_back();
    }
    
    printHeader("Contents of: " + path);
    
    if (currentDir->children.empty()) {
        printBox("Directory is empty");
        return;
    }
    
    cout << "+-----------------+------+-------------------------+" << endl;
    cout << "| Name            | Type | Created                 |" << endl;
    cout << "+-----------------+------+-------------------------+" << endl;
    
    for (FileNode* child : currentDir->children) {
        cout << "| ";
        
        // Name (15 chars)
        string displayName = child->name;
        if (displayName.length() > 15) {
            displayName = displayName.substr(0, 12) + "...";
        }
        cout << displayName;
        for (int i = displayName.length(); i < 15; i++) cout << " ";
        
        cout << " | ";
        
        // Type (4 chars)
        string typeDisplay = (child->type == "folder") ? "DIR" : "FILE";
        cout << typeDisplay;
        for (int i = typeDisplay.length(); i < 4; i++) cout << " ";
        
        cout << " | ";
        
        // Created time (23 chars)
        string timeDisplay = child->createdAt;
        if (timeDisplay.length() > 23) {
            timeDisplay = timeDisplay.substr(0, 20) + "...";
        }
        cout << timeDisplay;
        for (int i = timeDisplay.length(); i < 23; i++) cout << " ";
        
        cout << " |" << endl;
    }
    
    cout << "+-----------------+------+-------------------------+" << endl;
}

void FileSystem::rm(string name) {
    // Check if file/folder exists in current directory only
    if (currentDir->childMap.find(name) == currentDir->childMap.end()) {
        printBox("Error: '" + name + "' not found in current directory!");
        printBox("You must be inside the folder containing the file to delete it.");
        return;
    }
    
    FileNode* target = currentDir->childMap[name];
    
    // Double check that the target's parent is the current directory
    if (target->parent != currentDir) {
        printBox("Error: Cannot delete '" + name + "' - not in current directory!");
        return;
    }
    
    // Remove from children vector
    for (int i = 0; i < currentDir->children.size(); i++) {
        if (currentDir->children[i] == target) {
            currentDir->children.erase(currentDir->children.begin() + i);
            break;
        }
    }
    
    // Remove from map
    currentDir->childMap.erase(name);
    
    // Delete the node and all its children
    deleteNode(target);
    
    printBox("Deleted: " + name);
}

void FileSystem::cat(string name) {
    if (currentDir->childMap.find(name) == currentDir->childMap.end()) {
        printBox("Error: File '" + name + "' not found!");
        return;
    }
    
    FileNode* target = currentDir->childMap[name];
    if (target->type != "file") {
        printBox("Error: '" + name + "' is not a file!");
        return;
    }
    
    printHeader("File Contents: " + name);
    if (target->content.empty()) {
        printBox("(File is empty)");
    } else {
        cout << "+";
        for (int i = 0; i < 50; i++) cout << "-";
        cout << "+" << endl;
        cout << target->content << endl;
        cout << "+";
        for (int i = 0; i < 50; i++) cout << "-";
        cout << "+" << endl;
    }
}

void FileSystem::pwd() {
    string path = "";
    FileNode* current = currentDir;
    vector<string> pathParts;
    
    // Build path from current to root
    while (current != nullptr) {
        pathParts.push_back(current->name);
        current = current->parent;
    }
    
    // Reverse to get correct order
    path = "/";
    for (int i = pathParts.size() - 1; i >= 0; i--) {
        if (pathParts[i] != "root") {
            path += pathParts[i] + "/";
        }
    }
    
    if (path.length() > 1 && path.back() == '/') {
        path.pop_back();
    }
    
    printBox("Current path: " + path);
}

void FileSystem::echo(string content, string filename, bool append) {
    // Check if file exists
    FileNode* target = nullptr;
    if (currentDir->childMap.find(filename) != currentDir->childMap.end()) {
        target = currentDir->childMap[filename];
        if (target->type != "file") {
            printBox("Error: '" + filename + "' is not a file!");
            return;
        }
    } else {
        // Create new file
        target = new FileNode(filename, "file", currentDir);
        currentDir->children.push_back(target);
        currentDir->childMap[filename] = target;
    }
    
    if (append) {
        target->content += content + "\n";
        printBox("Content appended to: " + filename);
    } else {
        target->content = content + "\n";
        printBox("Content written to: " + filename);
    }
    
    target->size = target->content.length();
}

void FileSystem::cp(string source, string dest) {
    // Find source
    if (currentDir->childMap.find(source) == currentDir->childMap.end()) {
        printBox("Error: Source '" + source + "' not found!");
        return;
    }
    
    // Check if destination already exists
    if (currentDir->childMap.find(dest) != currentDir->childMap.end()) {
        printBox("Error: Destination '" + dest + "' already exists!");
        return;
    }
    
    FileNode* sourceNode = currentDir->childMap[source];
    FileNode* newNode = new FileNode(dest, sourceNode->type, currentDir);
    newNode->content = sourceNode->content;
    newNode->size = sourceNode->size;
    
    // If it's a folder, copy children recursively (simplified version)
    if (sourceNode->type == "folder") {
        for (FileNode* child : sourceNode->children) {
            FileNode* childCopy = new FileNode(child->name, child->type, newNode);
            childCopy->content = child->content;
            childCopy->size = child->size;
            newNode->children.push_back(childCopy);
            newNode->childMap[child->name] = childCopy;
        }
    }
    
    currentDir->children.push_back(newNode);
    currentDir->childMap[dest] = newNode;
    
    printBox("Copied '" + source + "' to '" + dest + "'");
}

void FileSystem::mv(string oldName, string newName) {
    // Find source
    if (currentDir->childMap.find(oldName) == currentDir->childMap.end()) {
        printBox("Error: '" + oldName + "' not found!");
        return;
    }
    
    // Check if destination already exists
    if (currentDir->childMap.find(newName) != currentDir->childMap.end()) {
        printBox("Error: '" + newName + "' already exists!");
        return;
    }
    
    FileNode* node = currentDir->childMap[oldName];
    node->name = newName;
    
    // Update the map
    currentDir->childMap.erase(oldName);
    currentDir->childMap[newName] = node;
    
    printBox("Renamed '" + oldName + "' to '" + newName + "'");
}

void FileSystem::find(string name) {
    printHeader("Search Results for: " + name);
    vector<string> results;
    
    // Simple recursive search function
    function<void(FileNode*, string)> searchNode = [&](FileNode* node, string path) {
        if (node->name.find(name) != string::npos) {
            results.push_back(path + "/" + node->name + " (" + node->type + ")");
        }
        
        for (FileNode* child : node->children) {
            searchNode(child, path + "/" + node->name);
        }
    };
    
    searchNode(root, "");
    
    if (results.empty()) {
        printBox("No files or folders found matching: " + name);
    } else {
        for (const string& result : results) {
            cout << "  " << result << endl;
        }
    }
}

void FileSystem::tree() {
    printHeader("Directory Tree");
    cout << "/" << endl;
    for (int i = 0; i < root->children.size(); i++) {
        bool isLast = (i == root->children.size() - 1);
        tree_helper(root->children[i], "", isLast);
    }
}

void FileSystem::tree_helper(FileNode* node, string prefix, bool isLast) {
    cout << prefix;
    cout << (isLast ? "+-- " : "|-- ");
    cout << node->name;
    if (node->type == "folder") cout << "/";
    cout << endl;
    
    string newPrefix = prefix + (isLast ? "    " : "|   ");
    
    for (int i = 0; i < node->children.size(); i++) {
        bool childIsLast = (i == node->children.size() - 1);
        tree_helper(node->children[i], newPrefix, childIsLast);
    }
}