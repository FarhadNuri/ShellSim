#include "FileSystem.h"

int main() {
    FileSystem fs;
    string command, argument;
    
    while (true) {
        cout << endl << "filesystem> ";
        cin >> command;
        
        if (command == "exit") {
            cout << "Goodbye!" << endl;
            break;
        }
        else if (command == "mkdir") {
            cin >> argument;
            fs.mkdir(argument);
        }
        else if (command == "touch") {
            cin >> argument;
            fs.touch(argument);
        }
        else if (command == "cd") {
            cin >> argument;
            fs.cd(argument);
        }
        else if (command == "ls") {
            fs.ls();
        }
        else if (command == "rm") {
            cin >> argument;
            fs.rm(argument);
        }
        else if (command == "cat") {
            cin >> argument;
            fs.cat(argument);
        }
        else if (command == "pwd") {
            fs.pwd();
        }
        else if (command == "echo") {
            string content, op, filename;
            cin >> content >> op >> filename;
            
            if (op == ">") {
                fs.echo(content, filename, false);
            } else if (op == ">>") {
                fs.echo(content, filename, true);
            } else {
                cout << "Usage: echo \"text\" > file.txt or echo \"text\" >> file.txt" << endl;
            }
        }
        else if (command == "cp") {
            string source, dest;
            cin >> source >> dest;
            fs.cp(source, dest);
        }
        else if (command == "mv") {
            string oldName, newName;
            cin >> oldName >> newName;
            fs.mv(oldName, newName);
        }
        else if (command == "find") {
            cin >> argument;
            fs.find(argument);
        }
        else if (command == "tree") {
            fs.tree();
        }
        else if (command == "help") {
            cout << endl;
            cout << "Available commands:" << endl;
            cout << "  mkdir <name>  - Create a new folder" << endl;
            cout << "  touch <name>  - Create a new file" << endl;
            cout << "  cd <name>     - Change directory (.. parent, - previous, / root, ~ home)" << endl;
            cout << "  ls            - List directory contents" << endl;
            cout << "  rm <name>     - Remove file or folder" << endl;
            cout << "  cat <name>    - Display file contents" << endl;
            cout << "  pwd           - Show current directory path" << endl;
            cout << "  echo \"text\" > file   - Write content to file" << endl;
            cout << "  echo \"text\" >> file  - Append content to file" << endl;
            cout << "  cp <src> <dst> - Copy file or folder" << endl;
            cout << "  mv <old> <new> - Move/rename file or folder" << endl;
            cout << "  find <name>    - Search for files/folders" << endl;
            cout << "  tree          - Show directory tree structure" << endl;
            cout << "  help          - Show this help message" << endl;
            cout << "  exit          - Exit the program" << endl;
        }
        else {
            cout << "Unknown command: " << command << endl;
            cout << "Type 'help' for available commands." << endl;
        }
    }
    
    return 0;
}