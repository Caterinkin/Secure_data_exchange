#include <iostream>
#include <mutex>
#include <thread>

class Data
{
private:
    int value;
    std::mutex mtx;

public:
    Data(int val = 0) : value(val)
    {
    }

    int getValue() const
    {
        return value;
    }

    void setValue(int val)
    {
        value = val;
    }

    void lock()
    {
        mtx.lock();
    }

    void unlock()
    {
        mtx.unlock();
    }

    std::mutex& getMutex()
    {
        return mtx;
    }
};

// Вариант 1: с использованием std::lock
void swap_with_lock(Data& a, Data& b)
{
    std::lock(a.getMutex(), b.getMutex());
    std::lock_guard<std::mutex> lock_a(a.getMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> lock_b(b.getMutex(), std::adopt_lock);

    int temp = a.getValue();
    a.setValue(b.getValue());
    b.setValue(temp);
}

// Вариант 2: с использованием std::scoped_lock (C++17)
void swap_with_scoped_lock(Data& a, Data& b)
{
    std::scoped_lock lock(a.getMutex(), b.getMutex());

    int temp = a.getValue();
    a.setValue(b.getValue());
    b.setValue(temp);
}

// Вариант 3: с использованием std::unique_lock
void swap_with_unique_lock(Data& a, Data& b)
{
    std::unique_lock<std::mutex> lock_a(a.getMutex(), std::defer_lock);
    std::unique_lock<std::mutex> lock_b(b.getMutex(), std::defer_lock);
    std::lock(lock_a, lock_b);

    int temp = a.getValue();
    a.setValue(b.getValue());
    b.setValue(temp);
}

int main()
{
    Data d1(10);
    Data d2(20);

    std::cout << "Before swap: d1 = " << d1.getValue() << ", d2 = " << d2.getValue() << std::endl;

    // Тестируем разные варианты swap
    swap_with_lock(d1, d2);
    std::cout << "After swap_with_lock: d1 = " << d1.getValue() << ", d2 = " << d2.getValue() << std::endl;

    swap_with_scoped_lock(d1, d2);
    std::cout << "After swap_with_scoped_lock: d1 = " << d1.getValue() << ", d2 = " << d2.getValue() << std::endl;

    swap_with_unique_lock(d1, d2);
    std::cout << "After swap_with_unique_lock: d1 = " << d1.getValue() << ", d2 = " << d2.getValue() << std::endl;

    return 0;
}