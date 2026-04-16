class Node:
    def __init__(self, data, next=None):
        self.data = data
        self.next = next

    def get_data(self):
        return self.data

    def change_data(self, new_data):
        self.data = new_data

    def __str__(self):
        next_data = self.next.data if self.next else None
        return f"data: {self.data}, next: {next_data}"


class LinkedList:
    def __init__(self, head=None):
        self.head = head
        self.length = 1 if head else 0

    def __len__(self):
        return self.length

    def append(self, element):
        new_node = Node(element)

        if self.head is None:
            self.head = new_node
        else:
            curr = self.head
            while curr.next:
                curr = curr.next
            curr.next = new_node

        self.length += 1

    def __str__(self):
        if self.length == 0:
            return "LinkedList[]"

        result = []
        curr = self.head

        while curr:
            next_data = curr.next.data if curr.next else None
            result.append(f"data: {curr.data}, next: {next_data}")
            curr = curr.next

        return f"LinkedList[length = {self.length}, [{'; '.join(result)}]]"

    def pop(self):
        if self.length == 0:
            raise IndexError("LinkedList is empty!")

        if self.length == 1:
            self.head = None
        else:
            curr = self.head
            while curr.next.next:
                curr = curr.next
            curr.next = None

        self.length -= 1

    def change_on_start(self, n, new_data):
        if n <= 0 or n > self.length:
            raise KeyError("Element doesn't exist!")

        curr = self.head
        for _ in range(n - 1):
            curr = curr.next

        curr.data = new_data

    def clear(self):
        self.head = None
        self.length = 0