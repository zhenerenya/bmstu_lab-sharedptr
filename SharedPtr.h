#pragma once
#include <atomic>

template <typename T>
class SharedPtr {
public:
    //пустой указатель
    SharedPtr() : _ptr(nullptr), _count(nullptr) {};

    //создать указатель на ptr
    SharedPtr(T* ptr) : _ptr(ptr), _count(new std::atomic_uint(1)) {};

    //копирование 
    SharedPtr(const SharedPtr& r) : _ptr(r._ptr),
        _count(r._count) {
        ++(*_count);
    };

    //перемещение
    SharedPtr(SharedPtr&& r) : _ptr(r._ptr),
        _count(r._count) {
        r._ptr = nullptr;
        r._count = nullptr;
    };

    //деструктор
    ~SharedPtr() {
        if (_count) {
            if (*_count == 1) {
                _ptr = nullptr;
                delete _ptr;
                delete _count;
            }
            else
                --(*_count);
        }
    };
    
    //оператор копирования
    auto operator=(const SharedPtr& r)->SharedPtr& {
        if (this != &r)
        {
            reset();
            _ptr = r._ptr;
            _count = r._count;
            ++(*_count);
        }
        return *this;
    }

    //оператор перемещения
    auto operator=(SharedPtr&& r)->SharedPtr& {
        std::swap(_ptr, r._ptr);
        return *this;
    }

    // проверяет, указывает ли указатель на объект
    operator bool() const {
        return _count != nullptr;
    }

    //возвращает ссылку на объект
    auto operator*() const->T& {
        return *_ptr;
    }
    
    //возвращает указатель на объект
    auto operator->() const->T* {
        return _ptr;
    }

    //возыращает указатель на объект
    auto get()->T* {
        return _ptr;
    }

    //очистка указателя
    void reset() {
        //если есть хоть один объект
        if (_count) {
            //если объект один удаляем весь указатель
            if (*_count == 1) {
                delete _ptr;
                delete _count;
            }
            //если указателей больше, то количество всех указателей уменьшаем на один
            else {
                --(*_count);
            }
        }
        //данный указатель очищаем
        _ptr = nullptr;
        _count = nullptr;
    }

    //очистка указателя при ссылке на объект
    void reset(T* ptr) {
        if (_count) {
            if (*_count == 1) {
                delete _count;
                delete _ptr;
            }
            else
                --(*_count);
        }
        _ptr = ptr;
        _count = new std::atomic_uint(1);
    }

    //обмениваем значения указателей
    void swap(SharedPtr& r) {
        auto ptr = r._ptr;
        auto count = r._count;
        r._ptr = _ptr;
        r._count = _count;
        _ptr = ptr;
        _count = count;

        delete ptr;
        delete count;
    }

    // возвращает количество объектов SharedPtr, которые ссылаются на тот же управляемый объект
    auto use_count() const->size_t {
        return _count;
    }

private:
    T* _ptr;
    std::atomic_uint* _count;
};