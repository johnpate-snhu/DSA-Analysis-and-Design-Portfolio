//============================================================================
// Name        : ProjectTwo.cpp
// Author      : John Pate
// Version     : 1.0
// Copyright   : Copyright 2023 SNHU COCE
// Description : Cs 300 Project Two
//============================================================================


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

// Course class to hold course data
class Course {
public:
    string courseNumber;
    string title;
    vector<string> prerequisites;

    Course(string num, string t) : courseNumber(num), title(t) {}

    // Function to add prerequisite to a course
    void addPrerequisite(const string& prereq) {
        prerequisites.push_back(prereq);
    }

    // Print the course info
    void printCourseInfo() const {
        cout << courseNumber << "," << title << "\n";
        if (!prerequisites.empty()) {
            cout << "Prerequisites: ";
            for (const auto& prereq : prerequisites) {
                cout << prereq << " ";
            }
            cout << endl;
        }
        else {
            cout << "No prerequisites." << endl;
        }
    }
};

// Node for the Binary Search Tree
struct TreeNode {
    Course course;
    TreeNode* left;
    TreeNode* right;

    TreeNode(Course c) : course(c), left(nullptr), right(nullptr) {}
};

// Binary Search Tree class
class BST {
private:
    TreeNode* root;

    void insert(TreeNode*& node, const Course& course) {
        if (!node) {
            node = new TreeNode(course);
        }
        else if (course.courseNumber < node->course.courseNumber) {
            insert(node->left, course);
        }
        else {
            insert(node->right, course);
        }
    }

    void inOrderTraversal(TreeNode* node) const {
        if (node) {
            inOrderTraversal(node->left);
            node->course.printCourseInfo();
            inOrderTraversal(node->right);
        }
    }

    TreeNode* search(TreeNode* node, const string& courseNumber) const {
        if (!node || node->course.courseNumber == courseNumber) {
            return node;
        }
        else if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        }
        else {
            return search(node->right, courseNumber);
        }
    }

public:
    BST() : root(nullptr) {}

    // Insert a course into the BST
    void insert(const Course& course) {
        insert(root, course);
    }

    // Print all courses in the BST in alphanumeric order
    void printAllCourses() const {
        inOrderTraversal(root);
    }

    // Find a course in the BST by its course number
    Course* findCourse(const string& courseNumber) const {
        TreeNode* node = search(root, courseNumber);
        if (node) {
            return &node->course;
        }
        return nullptr;
    }
};

// Function to load course data from a file
void loadCoursesFromFile(const string& filename, BST& tree) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string courseNumber, title, prereq;
        getline(ss, courseNumber, ',');
        getline(ss, title, ',');
        Course course(courseNumber, title);

        while (getline(ss, prereq, ',')) {
            course.addPrerequisite(prereq);
        }
        tree.insert(course);
    }
    file.close();
}

int main() {
    BST courseTree;
    string filename;
    int choice;

    while (true) {
        // Menu
        cout << "Welcome to the course planner.\n";
        cout << "1. Load Data Structure.\n";
        cout << "2. Print Course List.\n";
        cout << "3. Print Course.\n";
        cout << "9. Exit\n";
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Enter the course data file name: ";
            cin.ignore();  // Clear the input buffer
            getline(cin, filename);
            loadCoursesFromFile(filename, courseTree);
            break;
        }
        case 2: {
            cout << "All courses in alphanumeric order:\n";
            courseTree.printAllCourses();
            break;
        }
        case 3: {
            cout << "Enter the course number: ";
            string courseNumber;
            cin >> courseNumber;

            Course* course = courseTree.findCourse(courseNumber);
            if (course) {
                course->printCourseInfo();
            }
            else {
                cout << "Course not found!" << endl;
            }
            break;
        }
        case 9: {
            cout << "Thank you for using the course planner!\n";
            return 0;
        }
        default:
            cout << "That is not a valid option.\n";
        }
    }

    return 0;
}