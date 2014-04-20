#pragma once

#include<iostream>
#include<memory>
#include<exception>
#include<stdexcept>

/**
 * @brief Nullable Option type
 *
 * This Option type can be Some or None. It can be checked for Some/None,
 * and dereferenced. If None is dereferenced, an exception is thrown.
 * In a perfect world, this would be enforced at compile-time. It is not.
 */
template<typename T>
class Option
{
public:
    /**
     * @brief return true if there is a value contained in this option
     */
    inline bool is_some() const { return m_is_some; };

    /**
     * @brief return true if there is no value contained in this option
     */
    inline bool is_none() const { return !m_is_some; };

    /**
     * @brief constructor that directly uses a shared pointer
     */
    Option(std::shared_ptr<T> value) :
        m_value(value),
        m_is_some(value != NULL)
    {};

    /**
     * @brief constructor that takes a bare pointer.
     *
     * @param   value
     * the value to be stored in the tree
     *
     * @param   assume_ownership
     * If true, takes ownership of the bare pointer.
     * If false, allocates its own memory and uses the copy constructor
     * of the type to copy into the new container.
     */
    Option(T *value, bool assume_ownership=true) :
        m_value(std::shared_ptr<T>(assume_ownership ? value
                                                    : new T(value))),
        m_is_some(true)
    {};

    /**
     * @brief Default "Some" constructor
     *
     * Creates a new Option containing the value.
     */
    Option(T value) :
        m_value(std::shared_ptr<T>(new T(value))),
        m_is_some(true)
    {};

    /**
     * @brief None
     *
     * Creates a new Option containing no value. The object
     * cannot be dereferenced without throwing an exception
     */
    Option() :
        m_value(NULL),
        m_is_some(false)
    {};

    /**
     * @brief Copy assignment Constructor
     */
    Option<T>& operator=(Option<T> newval)
    {
        m_value = newval.m_value;
        m_is_some = newval.m_is_some;
        return *this;
    }

    /**
     * @brief Copy assignment constructor with a shared pointer
     */
    Option<T>& operator=(std::shared_ptr<T> newval)
    {
        m_value = newval;
        m_is_some = true;
        return *this;
    }

    /**
     * @brief Copy assignment constructor with a value
     */
    Option<T>& operator=(T newval)
    {
        Option<T> newopt(Some(newval));
        return operator=(newopt);
    }

    /**
     * @brief get a reference to the contained shared pointer
     *
     * @note throws an exception if there is no contained value
     */
    std::shared_ptr<T> get_ref() const {
        if (is_some()) {
            return m_value;
        } else {
            throw std::logic_error("dereference of NoneType");
        }
    }

    /**
     * @brief get a reference to the contained shared pointer
     *
     * @note throws an exception if there is no contained value
     */
    T get_bare() const {
        if (is_some()) {
            return *m_value;
        } else {
            throw std::logic_error("dereference of NoneType");
        }
    }

    /**
     * @brief allow the -> method
     *
     * @note throws an exception if there is no contained value
     */
    T* operator->() const {
        if (is_some()) {
            return m_value.get();
        } else {
            throw std::logic_error("dereference of NoneType");
        }
    }
private:

    /// contained value. Will be NULL if is None
    std::shared_ptr<T> m_value;

    /// true if is Some
    bool m_is_some;
};



/**
 * @brief construct a new Some value
 */
template<typename T>
Option<T> Some(T value) { return Option<T>(value); };

/**
 * @brief construct a new Some value
 */
template<typename T>
Option<T> Some(T* value) { return Option<T>(value); };

/**
 * @brief construct a new Some value
 */
template<typename T>
Option<T> Some(std::shared_ptr<T> value) { return Option<T>(value); };

/**
 * @brief construct a new None value
 */
template<typename T>
Option<T> None() { return Option<T>(); };



