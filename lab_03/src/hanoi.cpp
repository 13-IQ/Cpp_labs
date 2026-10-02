#include <iostream>

struct Node {
    int disk;
    char from;
    char to;
    Node* prev;
    Node* next;
    Node(int d, char f, char t) : disk(d), from(f), to(t), prev(nullptr), next(nullptr) {}
};

void append(Node*& head, Node*& tail, int disk, char from, char to) {
    Node* newNode = new Node(disk, from, to);
    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
}

void printList(Node* head) {
    Node* cur = head;
    int step = 1;
    while (cur) {
        std::cout << step << ": Переместить диск " << cur->disk
                  << " с " << cur->from << " на " << cur->to << std::endl;
        cur = cur->next;
        step++;
    }
}

void clearList(Node*& head, Node*& tail) {
    Node* cur = head;
    while (cur) {
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    head = tail = nullptr;
}

void hanoi(int n, char from, char to, char aux, Node*& head, Node*& tail) {
    if (n == 1) {
        append(head, tail, 1, from, to);
        return;
    }
    hanoi(n - 1, from, aux, to, head, tail);
    append(head, tail, n, from, to);
    hanoi(n - 1, aux, to, from, head, tail);
}

int main() {
    Node* head = nullptr;
    Node* tail = nullptr;
    hanoi(8, 'A', 'C', 'B', head, tail);
    printList(head);
    clearList(head, tail);
    return 0;
}
