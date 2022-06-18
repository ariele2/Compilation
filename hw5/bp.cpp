#include "bp.hpp"
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

bool replace(string &str, const string &from, const string &to, BranchLabelIndex index);

Buff::Buff() : buffer(), globalDefs() {}

Buff &Buff::instance() {
    static Buff inst; //only instance
    return inst;
}

std::string Buff::genLabel(const string &extension) {
    std::stringstream label;
    label << "label";
    label << buffer.size();
    label << extension;
    std::string ret(label.str());
    label << ":";
    emit("  " + label.str());
    return ret;
}

size_t Buff::emit(const string &s) {
    // returns "address" to jump to
    buffer.push_back(s);
    return buffer.size() - 1;
}

void Buff::bpatch(const vector<pair<int, BranchLabelIndex>> &address_list, const std::string &label) {
    for (const auto &i : address_list) {
        int address = i.first;
        BranchLabelIndex labelIndex = i.second;
        replace(buffer[address], "@", "%" + label, labelIndex);
    }
}

void Buff::printCodeBuffer() {
    for (const auto &it : buffer) {
        cout << it << endl;
    }
}

vector<pair<int, BranchLabelIndex>> Buff::makelist(pair<int, BranchLabelIndex> item) {
    vector<pair<int, BranchLabelIndex>> newList;
    newList.push_back(item);
    return newList;
}

vector<pair<int, BranchLabelIndex>>
Buff::merge(const vector<pair<int, BranchLabelIndex>> &l1, const vector<pair<int, BranchLabelIndex>> &l2) {
    vector<pair<int, BranchLabelIndex>> newList(l1.begin(), l1.end());
    newList.insert(newList.end(), l2.begin(), l2.end());
    return newList;
}

// ******** Methods to handle the global section ********** //
void Buff::emitGlobal(const std::string &dataLine) {
    globalDefs.push_back(dataLine);
}

void Buff::printGlobalBuffer() {
    for (const auto &globalDef : globalDefs) {
        cout << globalDef << endl;
    }
}

// ******** Helper Methods ********** //
bool replace(string &str, const string &from, const string &to, const BranchLabelIndex index) {
    size_t pos;
    if (index == SECOND) {
        pos = str.find_last_of(from);
    } else { //FIRST
        pos = str.find_first_of(from);
    }
    if (pos == string::npos)
        return false;
    str.replace(pos, from.length(), to);
    return true;
}

