#pragma once
#include <atomic>

template <typename T>
class SharedPtr {
public:
    //������ ���������
    SharedPtr() : _ptr(nullptr), _count(nullptr) {};

    //������� ��������� �� ptr
    SharedPtr(T* ptr) : _ptr(ptr), _count(new std::atomic_uint(1)) {};

    //����������� 
    SharedPtr(const SharedPtr& r) : _ptr(r._ptr),
        _count(r._count) {
        ++(*_count);
    };

    //�����������
    SharedPtr(SharedPtr&& r) : _ptr(r._ptr),
        _count(r._count) {
        r._ptr = nullptr;
        r._count = nullptr;
    };

    //����������
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
    
    //�������� �����������
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

    //�������� �����������
    auto operator=(SharedPtr&& r)->SharedPtr& {
        std::swap(_ptr, r._ptr);
        return *this;
    }

    // ���������, ��������� �� ��������� �� ������
    operator bool() const {
        return _count != nullptr;
    }

    //���������� ������ �� ������
    auto operator*() const->T& {
        return *_ptr;
    }
    
    //���������� ��������� �� ������
    auto operator->() const->T* {
        return _ptr;
    }

    //���������� ��������� �� ������
    auto get()->T* {
        return _ptr;
    }

    //������� ���������
    void reset() {
        //���� ���� ���� ���� ������
        if (_count) {
            //���� ������ ���� ������� ���� ���������
            if (*_count == 1) {
                delete _ptr;
                delete _count;
            }
            //���� ���������� ������, �� ���������� ���� ���������� ��������� �� ����
            else {
                --(*_count);
            }
        }
        //������ ��������� �������
        _ptr = nullptr;
        _count = nullptr;
    }

    //������� ��������� ��� ������ �� ������
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

    //���������� �������� ����������
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

    // ���������� ���������� �������� SharedPtr, ������� ��������� �� ��� �� ����������� ������
    auto use_count() const->size_t {
        return _count;
    }

private:
    T* _ptr;
    std::atomic_uint* _count;
};