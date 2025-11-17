#include "GamePlay/UI/Scores.h"

// Khởi tạo danh sách rỗng
void initList(List &l) {
    l.head = nullptr;
    l.tail = nullptr;
}

// Tạo node mới
Node *createNode(int point, Time t, Date d) {
    Node *p = new Node;
    p->point = point;
    p->t = t;
    p->d = d;
    p->next = nullptr;
    return p;
}

// Thêm node vào cuối danh sách
void addTail(List &l, Node *p) {
    if (!l.head) {
        l.head = l.tail = p;
    } else {
        l.tail->next = p;
        l.tail = p;
    }
}

// Đọc file và lưu vào danh sách
// Đọc file và lưu vào danh sách
void readFile(const std::string &filename, List &l) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Khong the mo file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty())
            continue;

        int point, hour, minute, day, month, year;
        // SỬA: Dùng sscanf để parse "point hour:minute day/month/year" (handle : / no space)
        int parsed = sscanf(line.c_str(), "%d %d:%d %d/%d/%d", &point, &hour, &minute, &day, &month, &year);
        if (parsed == 6) { // Thành công nếu đọc đủ 6 số
            Time t = {hour, minute};
            Date d = {day, month, year};
            Node *p = createNode(point, t, d);
            addTail(l, p);
            std::cout << "Loaded: " << point << " at " << hour << ":" << minute << " " << day << "/" << month << "/"
                      << year << std::endl; // Debug
        } else {
            std::cerr << "Parse fail line: " << line << std::endl; // Debug
        }
    }

    fin.close();
    // Update tail nếu cần (addTail đã handle)
    if (!l.head)
        l.tail = nullptr;
}
// In danh sách
void printList(const List &l) {
    Node *p = l.head;
    std::cout << "Point\tTime\t\tDate\n";
    while (p) {
        std::cout << p->point << "\t" << (p->t.hour < 10 ? "0" : "") << p->t.hour << ":"
                  << (p->t.minute < 10 ? "0" : "") << p->t.minute << "\t" << (p->d.day < 10 ? "0" : "") << p->d.day
                  << "/" << (p->d.month < 10 ? "0" : "") << p->d.month << "/" << p->d.year << std::endl;
        p = p->next;
    }
}

void swapNodeData(Node *a, Node *b) {
    std::swap(a->point, b->point);
    std::swap(a->t, b->t);
    std::swap(a->d, b->d);
}

// Sắp xếp theo điểm tăng dần
void increasingScore(List &l) {
    if (!l.head)
        return;
    for (Node *i = l.head; i->next; i = i->next) {
        for (Node *j = i->next; j; j = j->next) {
            if (i->point > j->point) {
                swapNodeData(i, j);
            }
        }
    }
}

// Sắp xếp theo điểm giảm dần
void decreasingScore(List &l) {
    if (!l.head)
        return;
    for (Node *i = l.head; i->next; i = i->next) {
        for (Node *j = i->next; j; j = j->next) {
            if (i->point < j->point) {
                swapNodeData(i, j);
            }
        }
    }
}

// So sánh thời gian, trả về true nếu t1 trước t2
bool isEarlier(Time t1, Date d1, Time t2, Date d2) {
    if (d1.year != d2.year)
        return d1.year < d2.year;
    if (d1.month != d2.month)
        return d1.month < d2.month;
    if (d1.day != d2.day)
        return d1.day < d2.day;
    if (t1.hour != t2.hour)
        return t1.hour < t2.hour;
    return t1.minute < t2.minute;
}

// Sắp xếp theo thời gian tăng dần
void increasingTime(List &l) {
    if (!l.head)
        return;
    for (Node *i = l.head; i->next; i = i->next) {
        for (Node *j = i->next; j; j = j->next) {
            if (!isEarlier(i->t, i->d, j->t, j->d)) {
                swapNodeData(i, j);
            }
        }
    }
}

// Sắp xếp theo thời gian giảm dần
void decreasingTime(List &l) {
    if (!l.head)
        return;
    for (Node *i = l.head; i->next; i = i->next) {
        for (Node *j = i->next; j; j = j->next) {
            if (isEarlier(i->t, i->d, j->t, j->d)) {
                swapNodeData(i, j);
            }
        }
    }
}

// Hàm tìm kiếm theo điểm số
// Sắp xếp điểm số cần tìm lên đầu danh sách
// Tìm kiếm tuần tự(Linear Search)
bool searchByPoint(List &l, int target) {
    if (!l.head)
        return false; // Danh sách rỗng -> Không tìm thấy

    Node *prev = nullptr;
    Node *current = l.head;
    Node *newHead = nullptr;
    Node *newTail = nullptr;
    
    bool found = false; // Biến cờ để kiểm tra

    while (current) {
        Node *next = current->next;
        if (current->point == target) {
            found = true; // Đã tìm thấy!
            
            // ... (Giữ nguyên logic di chuyển Node lên đầu) ...
            if (!newHead) {
                newHead = newTail = current;
            } else {
                newTail->next = current;
                newTail = current;
            }
            if (prev) {
                prev->next = next;
            } else {
                l.head = next;
            }
            current->next = nullptr;
        } else {
            prev = current;
        }
        current = next;
    }
    
    // ... (Giữ nguyên logic cập nhật tail) ...
    if (newTail) {
        newTail->next = l.head;
        l.head = newHead;
    }
    if (!l.head) {
        l.tail = nullptr;
    } else {
        Node *temp = l.head;
        while (temp->next)
            temp = temp->next;
        l.tail = temp;
    }

    return found; // Trả về kết quả
}

void deleteList(List &l) {
    Node *current = l.head;
    while (current != nullptr) {
        Node *next = current->next;
        delete current;
        current = next;
    }
    l.head = l.tail = nullptr;
}

int countList(const List &l) {
    int count = 0;
    Node *p = l.head;
    while (p) {
        count++;
        p = p->next;
    }
    return count;
}