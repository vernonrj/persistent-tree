#pragma once

#include<iostream>
#include<memory>
#include<exception>
#include<stdexcept>

template<typename T>
class Option
{
public:
    bool is_some() const { return m_is_some; };
    bool is_none() const { return !m_is_some; };
    Option(std::shared_ptr<T> value) :
        m_value(value),
        m_is_some(true)
    {};
    Option(T *value) :
        m_value(std::shared_ptr<T>(value)),
        m_is_some(true)
    {};
    Option(T value) :
        m_value(std::shared_ptr<T>(new T(value))),
        m_is_some(true)
    {};
    Option() :
        m_value(NULL),
        m_is_some(false)
    {};
    Option<T>& operator=(Option<T> newval)
    {
        m_value = newval.m_value;
        m_is_some = newval.m_is_some;
        return *this;
    }
    Option<T>& operator=(std::shared_ptr<T> newval)
    {
        m_value = newval;
        m_is_some = true;
        return *this;
    }
    Option<T>& operator=(T newval)
    {
        Option<T> newopt(Some(newval));
        return operator=(newopt);
    }
    std::shared_ptr<T> get_ref() const {
        if (is_some()) {
            return m_value;
        } else {
            throw std::logic_error("dereference of NoneType");
        }
    }
    T get_bare() const {
        if (is_some()) {
            return *m_value;
        } else {
            throw std::logic_error("dereference of NoneType");
        }
    }
    T* operator->() const {
        if (is_some()) {
            return m_value.get();
        } else {
            throw std::logic_error("dereference of NoneType");
        }
    }
private:
    std::shared_ptr<T> m_value;
    bool m_is_some;
};



template<typename T>
Option<T> Some(T value) { return Option<T>(value); };

template<typename T>
Option<T> Some(T* value) { return Option<T>(value); };

template<typename T>
Option<T> Some(std::shared_ptr<T> value) { return Option<T>(value); };

template<typename T>
Option<T> None() { return Option<T>(); };



