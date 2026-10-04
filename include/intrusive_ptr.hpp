#pragma once

#include <utility>

// declaration

template<class T>
class intrusive_ptr
{
public:
    using element_type = T;

    constexpr intrusive_ptr() noexcept = default;
    intrusive_ptr(T* p, bool add_ref = true);

    intrusive_ptr(const intrusive_ptr& other);
    intrusive_ptr(intrusive_ptr&& other) noexcept;

    ~intrusive_ptr();

    intrusive_ptr& operator=(const intrusive_ptr& other);
    intrusive_ptr& operator=(intrusive_ptr&& other) noexcept;

    void reset(T* p = nullptr);

    T& operator*() const noexcept;
    T* operator->() const noexcept;
    T* get() const noexcept;
    T* detach() noexcept;

    explicit operator bool() const noexcept;

    void swap(intrusive_ptr& other) noexcept;

private:
    T* ptr_ = nullptr;
};

// definition

template <class T>
intrusive_ptr<T>::intrusive_ptr(T *p, bool add_ref)
    : ptr_(p)
{
    if (ptr_ && add_ref)
        intrusive_ptr_add_ref(ptr_);
}

template <class T>
intrusive_ptr<T>::intrusive_ptr(const intrusive_ptr &other)
    : ptr_(other.ptr_)
{
    if (ptr_)
        intrusive_ptr_add_ref(ptr_);
}

template <class T>
intrusive_ptr<T>::intrusive_ptr(intrusive_ptr &&other) noexcept
    : ptr_(other.ptr_)
{
    other.ptr_ = nullptr;
}

template <class T>
intrusive_ptr<T>::~intrusive_ptr()
{
    if (ptr_)
        intrusive_ptr_release(ptr_);
}

template <class T>
intrusive_ptr<T> &intrusive_ptr<T>::operator=(const intrusive_ptr &other)
{
    intrusive_ptr temp(other);
    swap(temp);
    return *this;
}

template <class T>
intrusive_ptr<T> &intrusive_ptr<T>::operator=(intrusive_ptr &&other) noexcept
{
    if (this != &other)
    {
        reset();
        ptr_ = other.ptr_;
        other.ptr_ = nullptr;
    }

    return *this;
}

template <class T>
void intrusive_ptr<T>::reset(T *p)
{
    intrusive_ptr temp(p);
    swap(temp);
}
template <class T>
T *intrusive_ptr<T>::get() const noexcept
{
    return ptr_;
}

template <class T>
T *intrusive_ptr<T>::detach() noexcept
{
    T* result = ptr_;
    ptr_ = nullptr;
    return result;
}

template <class T> T &intrusive_ptr<T>::operator*() const noexcept
{
    return *ptr_;
}

template <class T> T *intrusive_ptr<T>::operator->() const noexcept
{
    return ptr_;
}

template <class T> intrusive_ptr<T>::operator bool() const noexcept
{
    return ptr_ != nullptr;
}

template <class T> void intrusive_ptr<T>::swap(intrusive_ptr &other) noexcept
{
    std::swap(ptr_, other.ptr_);
}

template <class T, class U> intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const & p)
{
    return static_cast<T *>(p.get());
}

template<class T, class U> intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const & p)
{
    return const_cast<T *>(p.get());
}

template<class T, class U> intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const & p)
{
    return dynamic_cast<T *>(p.get());
}