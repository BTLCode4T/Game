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

void swapNodeData(Node* a, Node* b) {
    std::swap(a->point, b->point);
    std::swap(a->t, b->t);
    std::swap(a->d, b->d);
}

// Sắp xếp theo điểm tăng dần
void increasingScore(List &l) {
    if (!l.head) return;
    for (Node* i = l.head; i->next; i = i->next) {
        for (Node* j = i->next; j; j = j->next) {
            if (i->point > j->point) {
                swapNodeData(i, j);
            }
        }
    }
}

// Sắp xếp theo điểm giảm dần
void decreasingScore(List &l) {
    if (!l.head) return;
    for (Node* i = l.head; i->next; i = i->next) {
        for (Node* j = i->next; j; j = j->next) {
            if (i->point < j->point) {
                swapNodeData(i, j);
            }
        }
    }
}

// So sánh thời gian, trả về true nếu t1 trước t2
bool isEarlier(Time t1, Date d1, Time t2, Date d2) {
    if (d1.year != d2.year) return d1.year < d2.year;
    if (d1.month != d2.month) return d1.month < d2.month;
    if (d1.day != d2.day) return d1.day < d2.day;
    if (t1.hour != t2.hour) return t1.hour < t2.hour;
    return t1.minute < t2.minute;
}

// Sắp xếp theo thời gian tăng dần
void increasingTime(List &l) {
    if (!l.head) return;
    for (Node* i = l.head; i->next; i = i->next) {
        for (Node* j = i->next; j; j = j->next) {
            if (!isEarlier(i->t, i->d, j->t, j->d)) {
                swapNodeData(i, j);
            }
        }
    }
}

// Sắp xếp theo thời gian giảm dần
void decreasingTime(List &l) {
    if (!l.head) return;
    for (Node* i = l.head; i->next; i = i->next) {
        for (Node* j = i->next; j; j = j->next) {
            if (isEarlier(i->t, i->d, j->t, j->d)) {
                swapNodeData(i, j);
            }
        }
    }
}

void deleteList(List &l) {
    Node* current = l.head;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    l.head = l.tail = nullptr;
}