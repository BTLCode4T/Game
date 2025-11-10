#ifndef SCORES_H
#define SCORES_H

#include <iostream>
#include <fstream>
#include <string>

struct Time {
    int hour;
    int minute;
};

struct Date {
    int day;
    int month;
    int year;
};

struct Node {
    int point;   // điểm
    Time t;
    Date d;
    Node* next;
};

struct List {
    Node* head;
    Node* tail;
};

// Khởi tạo danh sách rỗng
void initList(List &l);

// Tạo node mới
Node* createNode(int point, Time t, Date d);

// Thêm node vào cuối danh sách
void addTail(List &l, Node* p);

// Đọc file và lưu vào danh sách
void readFile(const std::string &filename, List &l);

// In danh sách
void printList(const List &l);

#endif
