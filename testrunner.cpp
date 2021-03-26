/******************************************************************************
 * File:         testrunner.cpp
 *
 * Author:       David Haolong Lee
 * Created:      03/26/21
 * Description:  Compile and execute this file without arguments to run all
 *               the test files specified above. To run specific tests,
 *               execute this file with the relative paths to the test files.
 *               (i.e. ./testruner <path/to/testfile> <path/to/testfile2> ...)
 *               In addition to that, some lines below that handles the print
 *               statements are commented out, so you can uncomment them based
 *               on your need (read the comments below for more info).
 * Notes about test files (from Yunrui's PA4 notes):
 *               "It would be nice to have some tests format to be automatically
 *               read by your driver program!"
 * For example:  "+ 1" means "insert(make_pair(1, 1))"
 *               "+ -3" means "insert(make_pair(-3, -3))"
 *               "- 1" means "remove(1)"
 *               "- -3" means "remove(-3)"
 *               Check InsertNominal.txt and InsertRotateLeft.txt for more info
 *****************************************************************************/
#include "avlbst.h"
#include "bst.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string YELLOW = "\033[1;33m";
const string BLUE = "\033[1;34m";
const string MAGENTA = "\033[1;35m";
const string CYAN = "\033[1;36m";
const string WHITE = "\033[1;37m";
const string RESET = "\033[0m";

// edit the variables below to the relative paths to your test files
// i.e. the variables below represent file "tests/InsertNominal.txt"
// and "tests/InsertRotateLeft.txt"

const string TESTDIR = "tests/";
// if you are not putting the test files in a separate directory, uncomment the
// line below and comment the line above.
/* const string TESTDIR = "./" */
const vector<string> TESTFILES = {
        "InsertNominal.txt",
        "InsertNominal2.txt",
        "InsertNominal3.txt",
        "InsertNominal4.txt",
        "InsertOverwrite.txt",
        "InsertRotateLeft.txt",
        "RemoveEmpty.txt",
        "RemoveNominal.txt",
        "RemoveNominal2.txt",
        "RemoveRotateLeft.txt",
        "RemoveRotateRight.txt",
        "RemoveTargetNotFound.txt",
        "RemoveTest1.txt",
};

void printOverallResultsForAllFiles(map<string, bool>);

template<class Key, class Value>
bool doKeyCheck(AVLTree<Key, Value>& avl, BinarySearchTree<Key, Value>& bst, map<Key, Value>& m);

int main(int argc, char* argv[]) {

    // runs all test or specific tests based on the number of arguments
    vector<string> testfiles;
    if (argc < 2) {  // run all tests
        for (string path : TESTFILES) {
            testfiles.push_back(TESTDIR + path);
        }
    } else {  // run specific tests
        for (int i = 1; i < argc; i++) {
            testfiles.push_back(argv[i]);
        }
    }

    map<string, bool> testResultsMap;
    for (string filepath : testfiles) {
        cout << YELLOW << "TESTING: " << filepath << " [" << testResultsMap.size() + 1 << "/" << testfiles.size()
             << "] " << RESET << endl;

        ifstream ifile(filepath);
        if (ifile.fail()) {
            cout << "cannot open file" << endl;
            return -1;
        }

        // parse the testing file
        vector<pair<char, int>> cmds;
        char operation;
        int data = 0;
        while (ifile >> operation >> data) {
            cmds.push_back(make_pair(operation, data));
        }

        // All operation below are done respectively on a map, BST, and AVLTree,
        // so you can refer to the STL map for the resulting tree after the
        // operation.
        map<int, int> m;
        BinarySearchTree<int, int> bst;
        AVLTree<int, int> avl;
        for (pair<char, int> cmd : cmds) {
            cout << endl;
            if (cmd.first == '+') {
                cout << BLUE << "INSERTING: " << cmd.second << ", " << cmd.second << RESET << endl;
                m.insert(make_pair(cmd.second, cmd.second));
                bst.insert(make_pair(cmd.second, cmd.second));
                avl.insert(make_pair(cmd.second, cmd.second));
            } else if (cmd.first == '-') {
                cout << BLUE << "REMOVING: " << cmd.second << RESET << endl;
                m.erase(cmd.second);
                bst.remove(cmd.second);
                avl.remove(cmd.second);
            }

            // uncomment these lines to print BinarySearchTree
            /* cout << "=============BST" << cmd.first << "(" << cmd.second << ")" */
            /*      << "=============" << endl; */
            /* bst.print(); */

            // uncomment these lines to print AVLTree
            cout << "=============AVL" << cmd.first << "(" << cmd.second << ")"
                 << "=============" << endl;
            avl.print();

            // checks that I wrote for the trees
            bool allChecksPassed = true;

            // check the keys of the AVLTree with STL map
            allChecksPassed = allChecksPassed && doKeyCheck(avl, bst, m);

            // add your custom checks below

            // log test results for current file
            testResultsMap[filepath] = allChecksPassed;
        }

        cout << YELLOW << "COMPLETED TESTING: " << filepath << RESET << endl;
        cout << endl;
        printOverallResultsForAllFiles(testResultsMap);
    }

    cout << GREEN << "Testrunner finished; " << testResultsMap.size() << "/" << testfiles.size() << " ran. "
         << "Happy Debugging!" << RESET << endl;
    return 0;
}

void printOverallResultsForAllFiles(map<string, bool> resultsMap) {
    for (map<string, bool>::iterator it = resultsMap.begin(); it != resultsMap.end(); ++it) {
        if (it->second) {
            cout << GREEN << (it->first) << ": Passed!" << RESET << endl;
        } else {
            cout << RED << (it->first) << ": Failed!" << RESET << endl;
        }
    }
}

template<class Key, class Value>
bool doKeyCheck(AVLTree<Key, Value>& avl, BinarySearchTree<Key, Value>& bst, map<Key, Value>& m) {
    bool flag = true;
    AVLTree<int, int>::iterator avlIter = avl.begin();
    map<int, int>::iterator mIter = m.begin();
    while (mIter != m.end() && avlIter != avl.end()) {
        if ((*mIter).first != (*avlIter).first) {
            flag = false;
            cout << RED << "Key not equal: " << (*mIter).first << " != " << (*avlIter).first << RESET << endl;
        }
        ++mIter;
        ++avlIter;
    }
    if (flag) {
        cout << GREEN << "Key Check: Passed!" << RESET << endl;
    } else {
        cout << RED << "Key Check: Failed!" << RESET << endl;
    }
    return flag;
}
