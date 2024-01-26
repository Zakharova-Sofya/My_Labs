#pragma once 
namespace CB {
    typedef char value_type;

    class CircularBuffer {
    private:
        value_type* buffer;       // указатель на динамически выделенный массив элементов буфера
        int capacity;             // текущая ёмкость буфера
        int size;                 // количество элементов в буфере
        int frontIndex;           // индекс первого элемента в буфере
        int backIndex;            // индекс последнего элемента в буфере

    public:
        CircularBuffer();

        ~CircularBuffer();

        CircularBuffer(const CircularBuffer& cb);

        explicit CircularBuffer(int capacity);

        CircularBuffer(int capacity, const value_type& elem);

        value_type& operator[](int i);

        const value_type& operator[](int i) const;

        value_type& at(int i);

        const value_type& at(int i) const;

        value_type& Front();

        value_type& Back();

        const value_type& Front() const;


        const value_type& Back() const;

        value_type* Linearize();

        bool IsLinearized() const;

        void Rotate(int new_begin);

        int Size() const;

        bool Empty() const;

        bool Full() const;

        int Reserve() const;

        int Capacity() const;

        void SetCapacity(int new_capacity);

        void Resize(int new_size, const value_type& item = value_type());

        CircularBuffer& operator=(const CircularBuffer& cb);

        void Swap(CircularBuffer& cb);

        void PushFront(const value_type& item = value_type());

        void PushBack(const value_type& item = value_type());

        void PopBack();

        void PopFront();

        void Insert(int pos, const value_type& item = value_type());

        void Erase(int first, int last);

        void Clear();
    };

    bool operator==(const CircularBuffer& a, const CircularBuffer& b);

    bool operator!=(const CircularBuffer& a, const CircularBuffer& b);
    }
}
