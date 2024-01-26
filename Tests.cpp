void RunTests(){
    {
        CircularBuffer cb;
        assert(cb.Capacity() == 0);
        assert(cb.Size() == 0);
    }

    {
        int capacity = 5;
        CircularBuffer cb(capacity);
        assert(cb.Capacity() == capacity);
        assert(cb.Size() == 0);
    }

    {
        int capacity = 5;
        char elem = 'A';
        CircularBuffer cb(capacity, elem);
        assert(cb.Capacity() == capacity);
        assert(cb.Size() == capacity);
        for (int i = 0; i < capacity; i++) {
            assert(cb[i] == elem);
        }
    }
    {
        int capacity = 5;
        CircularBuffer cb(capacity);
        cb.PushFront('A');
        assert(cb.Capacity() == capacity);
        assert(cb.Size() == 1);
        assert(cb[0] == 'A');
        cb.PushFront('B');
        assert(cb.Capacity() == capacity);
        assert(cb.Size() == 2);
        assert(cb[0] == 'A');
        assert(cb[1] == 'B');
        cb.PushBack('C');
        assert(cb[0] == 'C');
        assert(cb[1] == 'A');
        assert(cb[2] == 'B');
    }
    {
        int capacity = 5;
        char elem = 'A';
        CircularBuffer cb(capacity, elem);
        cb.PopFront();
        assert(cb.Size() == capacity - 1);
        assert(cb[0] == 'A');
        cb.PopBack();
        assert(cb.Size() == capacity - 2);
        assert(cb[cb.Size() - 1] == 'A');
        cb.Clear();
        assert(cb.Size() == 0);
    }

    {
        int capacity = 5;
        char elem = 'A';
        CircularBuffer cb(capacity, elem);
        cb.Insert(1, 'C');
        assert(cb[1] == 'C');
        assert(cb.Size() == capacity);
        cb.Erase(1, 3);
        assert(cb.Size() == 3);
        assert(cb[0] == 'A');
        assert(cb[1] == 'A');
        assert(cb[2] == 'A');
        cb.PushBack('C');
        assert(cb[0] == 'C');
        cb.Rotate(2);
        assert(cb[0] == 'A');
        assert(cb[1] == 'A');
        assert(cb[2] == 'C');
        assert(cb[3] == 'A');
    }

    {
        int capacity = 5;
        char elem = 'A';
        CircularBuffer cb(capacity, elem);
        cb.SetCapacity(capacity + 3);
        assert(cb.Capacity() == capacity + 3);
        cb.Resize(3);
        assert(cb.Capacity() == capacity + 3);
        assert(cb.Size() == 3);
    }

}

int main() {
    RunTests();
    cout << "All tests passed";
    
    return 0;
}
