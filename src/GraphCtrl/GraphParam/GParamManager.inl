/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParamManager.inl
@Time: 2021/6/11 6:59 下午
@Desc:
***************************/

#ifndef CGRAPH_GPARAMMANAGER_INL
#define CGRAPH_GPARAMMANAGER_INL

CGRAPH_NAMESPACE_BEGIN

template<typename T,
        c_enable_if_t<std::is_base_of<GParam, T>::value, int>>
CStatus GParamManager::create(const std::string& key, CBool backtrace) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_LOCK_GUARD lock(this->mutex_);
    auto result = params_map_.find(key);
    if (result != params_map_.end()) {
        /* 如果是重复创建，则返回ok；非重复创建（类型不同）则返回err */
        auto param = result->second;
        return (typeid(*param).name() == typeid(T).name()) ? CStatus() : CStatus("create param duplicate");
    }

    T* ptr = CGRAPH_SAFE_MALLOC_COBJECT(T);

    std::cout << "=======================" << std::endl;

    std::cout << "sizeof(*ptr) " << sizeof(*ptr)
              << " | addr " << ptr
              << std::endl;
    std::cout << "sizeof(backtrace_enable_) " << sizeof(ptr->backtrace_enable_)
              << " | addr " << &(ptr->backtrace_enable_)
              << " | val "<< ptr->backtrace_enable_
              << std::endl;
    std::cout << "sizeof(key_) " <<sizeof(ptr->key_)
              << " | addr " << &(ptr->key_)
              << std::endl;
    std::cout << "sizeof(backtrace_) " << sizeof(ptr->backtrace_) << std::endl;
    std::cout << "sizeof(backtrace_lock_) " << sizeof(ptr->backtrace_lock_) << std::endl;

    std::cout << "=======================" << std::endl;

    ((GParamPtr)ptr)->backtrace_enable_ = backtrace;
    std::cout << "backtrace_enable_ " << ((GParamPtr)ptr)->backtrace_enable_
              << " | backtrace " << backtrace
              << std::endl;
    ((GParamPtr)ptr)->key_ = key;
    std::cout << "key_ " << ((GParamPtr)ptr)->key_
              << " | key " << key
              << std::endl;

    std::cout << "=======================" << std::endl;

    std::cout << "sizeof(*ptr) " << sizeof(*ptr)
              << " | addr " << ptr
              << std::endl;
    std::cout << "sizeof(backtrace_enable_) " << sizeof(ptr->backtrace_enable_)
              << " | addr " << &(ptr->backtrace_enable_)
              << " | val "<< ptr->backtrace_enable_
              << std::endl;
    std::cout << "sizeof(key_) " <<sizeof(ptr->key_)
              << " | addr " << &(ptr->key_)
              << " | val " << ptr->key_ 
              << std::endl;
    std::cout << "sizeof(backtrace_) " << sizeof(ptr->backtrace_) << std::endl;
    std::cout << "sizeof(backtrace_lock_) " <<sizeof(ptr->backtrace_lock_) << std::endl;

    params_map_.insert(std::make_pair(key, ptr));
   
    std::cout << "=======================" << std::endl;

    auto p = params_map_.find(key);
    if (p != params_map_.end()) {
        std::cout << "sizeof(*(result->second)) " << sizeof(*(p->second))
                  << " | addr " << p->second
                  << std::endl;
        std::cout << "sizeof(backtrace_enable_) " << sizeof(p->second->backtrace_enable_)
                << " | addr " << &(p->second->backtrace_enable_)
                << " | val "<< p->second->backtrace_enable_
                << std::endl;
        std::cout << "sizeof(key_) " <<sizeof(p->second->key_)
                << " | addr " << &(p->second->key_)
                << " | val " << p->second->key_ 
                << std::endl;
        std::cout << "sizeof(backtrace_) " << sizeof(p->second->backtrace_) << std::endl;
        std::cout << "sizeof(backtrace_lock_) " <<sizeof(p->second->backtrace_lock_) << std::endl;
    }
    std::cout << "=======================" << std::endl;
    CGRAPH_FUNCTION_END
}


template<typename T,
        c_enable_if_t<std::is_base_of<GParam, T>::value, int>>
T* GParamManager::get(const std::string& key) {
    auto result = params_map_.find(key);
    if (result == params_map_.end()) {
        return nullptr;
    }

    return dynamic_cast<T *>(result->second);
}

CGRAPH_NAMESPACE_END

#endif // CGRAPH_GPARAMMANAGER_INL