#include <iostream>
#include <cstring>

class CustomStack {
public:
    CustomStack() {
        mHead = NULL;
        mCount = 0;
    }

    ~CustomStack() {
        while (!empty()) {
            pop();
        }
    }

    void push(const char* tag) {
        ListNode* newNode = new ListNode;
        newNode->mData = new char[strlen(tag) + 1];
        strcpy(newNode->mData, tag);
        newNode->mNext = mHead;
        mHead = newNode;
        mCount++;
    }

    void pop() {
        if (mHead != NULL) {
            ListNode* temp = mHead;
            mHead = mHead->mNext;
            delete[] temp->mData;
            delete temp;
            mCount--;
        }
    }

    char* top() {
        return mHead->mData;
    }

    size_t size() {
        return mCount;
    }

    bool empty() {
        return mHead == NULL;
    }

private:
    size_t mCount;

protected:
    ListNode* mHead;
};

int main() {
    char input[3001];
    if (!cin.getline(input, 3001)) {
        return 0;
    }

    CustomStack stack;
    int len = strlen(input);

    for (int i = 0; i < len; i++) {
        if (input[i] == '<') {
            i++;
            bool closing = false;
            if (input[i] == '/') {
                closing = true;
                i++;
            }

            char tag[3001];
            int k = 0;
            while (input[i] != '>' && input[i] != '\0') {
                tag[k++] = input[i++];
            }
            tag[k] = '\0';

            if (strcmp(tag, "br") == 0 || strcmp(tag, "hr") == 0) {
                continue;
            }

            if (closing) {
                if (stack.empty() || strcmp(stack.top(), tag) != 0) {
                    cout << "wrong" << endl;
                    return 0;
                }
                stack.pop();
            } else {
                stack.push(tag);
            }
        }
    }

    if (stack.empty()) {
        cout << "correct" << endl;
    } else {
        cout << "wrong" << endl;
    }

    return 0;
}