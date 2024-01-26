#include<CircularBuffer.h>
#include <cassert>
#include <algorithm>

using namespace CB {
    typedef char value_type;

    class CircularBuffer {
    private:
        value_type* buffer;       // указатель на динамически выделенный массив элементов буфера
        int capacity;             // текущая ёмкость буфера
        int size;                 // количество элементов в буфере
        int frontIndex;           // индекс первого элемента в буфере
        int backIndex;            // индекс последнего элемента в буфере

    public:
        CircularBuffer() : buffer(nullptr), capacity(0), size(0), frontIndex(0), backIndex(-1) {}

        ~CircularBuffer() {
            delete[] buffer;
        }

        CircularBuffer(const CircularBuffer& cb) : capacity(cb.capacity), size(cb.size), frontIndex(cb.frontIndex), backIndex(cb.backIndex) {
            buffer = new value_type[capacity];
            std::copy(cb.buffer, cb.buffer + capacity, buffer);
        }

        explicit CircularBuffer(int capacity) : buffer(nullptr), capacity(capacity), size(0), frontIndex(0), backIndex(-1) {
            buffer = new value_type[capacity];
        }

        CircularBuffer(int capacity, const value_type& elem) : buffer(nullptr), capacity(capacity), size(capacity), frontIndex(0), backIndex(size - 1) {
            buffer = new value_type[capacity];
            std::fill(buffer, buffer + capacity, elem);
        }

        value_type& operator[](int i) {
            assert(i >= 0 && i < size);
            return buffer[(frontIndex + i) % capacity];
        }

        const value_type& operator[](int i) const {
            assert(i >= 0 && i < size);
            return buffer[(frontIndex + i) % capacity];
        }

        value_type& at(int i) {
            assert(i >= 0 && i < size);
            return buffer[(frontIndex + i) % capacity];
        }

        const value_type& at(int i) const {
            assert(i >= 0 && i < size);
            return buffer[(frontIndex + i) % capacity];
        }

        value_type& Front() {
            assert(size > 0);
            return buffer[frontIndex];
        }

        value_type& Back() {
            assert(size > 0);
            return buffer[backIndex];
        }

        const value_type& Front() const {
            assert(size > 0);
            return buffer[frontIndex];
        }


        const value_type& Back() const {
            assert(size > 0);
            return buffer[backIndex];
        }

        value_type* Linearize() {
            if (Empty()) {
                return nullptr;
            }
            if (frontIndex <= backIndex) {
                return buffer + frontIndex;
            }
            else {
                value_type* linearBuffer = new value_type[size];
                std::copy(buffer + frontIndex, buffer + frontIndex + size, linearBuffer);
                return linearBuffer;
            }
        }

        bool IsLinearized() const {
            return frontIndex <= backIndex;
        }

        void Rotate(int new_begin) {
             assert(new_begin >= 0 && new_begin < size);
            if (new_begin == 0) {
            return;
            }
            int num_elements = size - new_begin;
            int* temp_buffer = new int[num_elements];
            for (int i = 0; i < num_elements; i++) {
                temp_buffer[i] = buffer[(new_begin + i) % size];
            }
            for (int i = 0; i < new_begin; i++) {
                buffer[(i + capacity - new_begin) % size] = buffer[i];
            }
            for (int i = 0; i < num_elements; i++) {
                buffer[(i + new_begin) % size] = temp_buffer[i];
            }
            delete[] temp_buffer;
            frontIndex = (frontIndex + new_begin) % size;
            backIndex = (backIndex + new_begin) % size;
        }

        int Size() const {
            return size;
        }

        bool Empty() const {
            return size == 0;
        }

        bool Full() const {
            return size == capacity;
        }

        int Reserve() const {
            return capacity - size;
        }

        int Capacity() const {
            return capacity;
        }

        void SetCapacity(int new_capacity) {
            assert(new_capacity >= 0);
            if (new_capacity < size) {
                Resize(new_capacity);
            }
            else {
                value_type* newBuffer = new value_type[new_capacity];
                if (!Empty()) {
                    if (frontIndex <= backIndex) {
                        std::copy(buffer + frontIndex, buffer + frontIndex + size, newBuffer);
                    }
                    else {
                        std::copy(buffer + frontIndex, buffer + capacity, newBuffer);
                        std::copy(buffer, buffer + (backIndex + 1), newBuffer + (capacity - frontIndex));
                    }
                }
                delete[] buffer;
                buffer = newBuffer;
                capacity = new_capacity;
                frontIndex = 0;
                backIndex = size - 1;
            }
        }

        void Resize(int new_size, const value_type& item = value_type()) {
            assert(new_size >= 0);
            if (new_size == size) {
                return;
            }
            if (new_size < size) {
                if (frontIndex <= backIndex) {
                    std::fill(buffer + frontIndex + new_size, buffer + backIndex + 1, item);
                }
                else {
                    if (new_size <= capacity - frontIndex) {
                        std::fill(buffer + frontIndex + new_size, buffer + capacity, item);
                    }
                    if (new_size > capacity - frontIndex || new_size < size) {
                        std::fill(buffer, buffer + (backIndex + 1), item);
                    }
                }
                size = new_size;
                backIndex = (frontIndex + size - 1) % capacity;
            }
            else {
                int num_new_elements = new_size - size;
                int available_space = Reserve();
                int num_elements_to_fill = std::min(num_new_elements, available_space);
                if (frontIndex <= backIndex) {
                    std::fill_n(buffer + backIndex + 1, num_elements_to_fill, item);
                }
                else {
                    if (num_elements_to_fill <= capacity - backIndex - 1) {
                        std::fill_n(buffer + backIndex + 1, num_elements_to_fill, item);
                    }
                    else {
                        std::fill_n(buffer + backIndex + 1, capacity - backIndex - 1, item);
                        std::fill_n(buffer, num_elements_to_fill - (capacity - backIndex - 1), item);
                    }
                }
                size += num_elements_to_fill;
                backIndex = (frontIndex + size - 1) % capacity;
            }
        }

        CircularBuffer& operator=(const CircularBuffer& cb) {
            if (this != &cb) {
                CircularBuffer temp(cb);
                Swap(temp);
            }
            return *this;
        }

        void Swap(CircularBuffer& cb) {
            std::swap(buffer, cb.buffer);
            std::swap(capacity, cb.capacity);
            std::swap(size, cb.size);
            std::swap(frontIndex, cb.frontIndex);
            std::swap(backIndex, cb.backIndex);
        }

        void PushFront(const value_type& item = value_type()) {
            if (Full()) {
                PopBack();
            }
            backIndex = (backIndex + 1) % capacity;
            buffer[backIndex] = item;
            if (Empty()) {
                frontIndex = backIndex;
            }
            size++;
        }


        void PushBack(const value_type& item = value_type()) {
            if (Full()) {
                PopFront();
            }
            frontIndex = (frontIndex - 1 + capacity) % capacity;
            buffer[frontIndex] = item;
            if (Empty()) {
                backIndex = frontIndex;
            }
            size++;
        }


        void PopBack() {
            assert(size > 0);
            if (size == 1) {
                frontIndex = 0;
                backIndex = -1;
            }
            else {
                backIndex = (backIndex - 1 + capacity) % capacity;
            }
            size--;
        }


        void PopFront() {
            assert(size > 0);
            if (size == 1) {
                frontIndex = 0;
                backIndex = -1;
            }
            else {
                frontIndex = (frontIndex + 1) % capacity;
            }
            size--;
        }

        void Insert(int pos, const value_type& item = value_type()) {
            assert(pos >= 0 && pos <= size);
            if (pos == 0) {
                PushBack(item);
            }
            else if (pos == size) {
                PushFront(item);
            }
            else {
                if (Full()) {
                    PopFront();
                }
                frontIndex = (frontIndex - 1 + capacity) % capacity;
                for (int i = 0; i < pos; i++) {
                    buffer[(frontIndex + i) % capacity] = buffer[(frontIndex + i + 1) % capacity];
                }
                buffer[pos] = item;
                size++;
            }
        }

        void Erase(int first, int last) {
            assert(first >= 0 && first < size);
            assert(last >= first && last <= size);
            if (first == last) {
                return;
            }
            if (first == 0) {
                if (last == size) {
                    Clear();
                }
                else {
                    if (frontIndex <= backIndex) {
                        frontIndex = (frontIndex + last) % capacity;
                    }
                    else {
                        frontIndex = (frontIndex + last) % capacity;
                        backIndex = (frontIndex + size - 1) % capacity;
                    }
                    size -= last;
                }
            }
            else {
                if (last == size) {
                    backIndex = (frontIndex + first - 1) % capacity;
                    size = first;
                }
                else {
                    int shiftIndex = (frontIndex + last) % capacity;
                    for (int i = 0; i < size - last; i++) {
                        buffer[(frontIndex + first + i) % capacity] = buffer[(shiftIndex + i) % capacity];
                    }
                    backIndex = (frontIndex + size - last - 1) % capacity;
                    size -= (last - first);
                }
            }
        }

        void Clear() {
            frontIndex = 0;
            backIndex = -1;
            size = 0;
        }
    };

    bool operator==(const CircularBuffer& a, const CircularBuffer& b) {
        if (a.Size() != b.Size()) {
            return false;
        }
        for (int i = 0; i < a.Size(); i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const CircularBuffer& a, const CircularBuffer& b) {
        return !(a == b);
    }
}
