// json_manager.cpp

#include "../../include/Core/GameLoop/json.h"
#include <iostream>
#include <fstream>

// ================================
// ĐỊNH NGHĨA CÁC HÀM
// ================================

json LoadJSON(const string& filename) {
    ifstream f(filename);
    json data;
    if (!f.is_open()) {
        cerr << "[!] File khong ton tai. Khoi tao cau truc.\n";
        data[ROOT_KEY] = json::object();
        return data;
    }
    try {
        f >> data;
        if (!data.contains(ROOT_KEY))
            data[ROOT_KEY] = json::object();
    } catch (...) {
        cerr << "[!] Loi khi doc JSON, tao moi.\n";
        data[ROOT_KEY] = json::object();
    }
    return data;
}

void SaveJSON(const json& data, const string& filename) {
    ofstream o(filename);
    if (!o.is_open()) {
        cerr << "[!] Khong the ghi file " << filename << endl;
        return;
    }
    o << data.dump(4);
    cout << "[+] Da luu vao file " << filename << endl;
}

void SaveFullData(const json& full_data, const string& filename) {
    ofstream o(filename);
    if (!o.is_open()) {
        cerr << "[!] Khong the ghi file " << filename << endl;
        return;
    }
    o << full_data.dump(4);
    cout << "[+] Da luu toan bo du lieu Save Game vao file " << filename << endl;
}

void CreateBranch(json& data, const string& parent_name, const string& new_branch) {
    json& root = data[ROOT_KEY];
    if (parent_name.empty()) {
        root[new_branch] = json::object();
        return;
    }
    if (root.contains(parent_name) && root[parent_name].is_object()) {
        root[parent_name][new_branch] = json::object();
    } else {
        cerr << "[!] Thu muc cha '" << parent_name << "' khong ton tai.\n";
    }
}

void AddOrUpdateObject(json& data, const string& parent_branch, const string& object_name, const json& properties) {
    json& root = data[ROOT_KEY];

    if (!root.contains(parent_branch) || !root[parent_branch].is_object()) {
        cerr << "[!] Loi: Nhanh cha '" << parent_branch << "' khong ton tai hoac khong hop le.\n";
        return;
    }

    json& parent = root[parent_branch];

    if (parent.contains(object_name)) {
        for (auto& [key, value] : properties.items()) {
            parent[object_name][key] = value;
        }
        cout << "[~] Da cap nhat doi tuong '" << object_name << "' trong '" << parent_branch << "'.\n";
    } else {
        parent[object_name] = properties;
        cout << "[+] Da them moi '" << object_name << "' trong '" << parent_branch << "'.\n";
    }
}

void DisplayJSON(const json& data, const string& branch_name) {
    if (!data.contains(ROOT_KEY)) {
        cerr << "[!] Khong co du lieu de hien thi.\n";
        return;
    }

    const json& root = data[ROOT_KEY];

    if (branch_name.empty()) {
        cout << "\n===== TOAN BO DU LIEU JSON =====\n";
        cout << root.dump(4) << endl;
    } else if (root.contains(branch_name)) {
        cout << "\n--- Nhanh '" << branch_name << "' ---\n";
        cout << root[branch_name].dump(4) << endl;
    } else {
        cerr << "[!] Khong tim thay nhanh '" << branch_name << "'.\n";
    }
}