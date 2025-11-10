#include "GamePlay/UI/Scores.h"

// Khởi tạo danh sách rỗng
void initList(List &l) {
    l.head = nullptr;
    l.tail = nullptr;
}

// Tạo node mới
Node* createNode(int point, Time t, Date d) {
    Node* p = new Node;
    p->point = point;
    p->t = t;
    p->d = d;
    p->next = nullptr;
    return p;
}

// Thêm node vào cuối danh sách
void addTail(List &l, Node* p) {
    if (!l.head) {
        l.head = l.tail = p;
    } else {
        l.tail->next = p;
        l.tail = p;
    }
}

// Đọc file và lưu vào danh sách
void readFile(const std::string &filename, List &l) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Không thể mở file: " << filename << std::endl;
        return;
    }

    int point, hour, minute, day, month, year;
    char ch1, ch2, ch3; // để đọc các ký tự phân cách : và /

    while (fin >> point >> hour >> ch1 >> minute >> day >> ch2 >> month >> ch3 >> year) {
        Time t = {hour, minute};
        Date d = {day, month, year};
        Node* p = createNode(point, t, d);
        addTail(l, p);
    }

    fin.close();
}

// In danh sách
void printList(const List &l) {
    Node* p = l.head;
    std::cout << "Point\tTime\t\tDate\n";
    while (p) {
        std::cout << p->point << "\t"
                  << (p->t.hour < 10 ? "0" : "") << p->t.hour << ":"
                  << (p->t.minute < 10 ? "0" : "") << p->t.minute << "\t"
                  << (p->d.day < 10 ? "0" : "") << p->d.day << "/"
                  << (p->d.month < 10 ? "0" : "") << p->d.month << "/"
                  << p->d.year
                  << std::endl;
        p = p->next;
    }
}

