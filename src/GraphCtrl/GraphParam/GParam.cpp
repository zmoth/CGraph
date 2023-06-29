/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GParam.cpp
@Time: 2023/2/20 22:11
@Desc: 
***************************/

#include "GParam.h"

CGRAPH_NAMESPACE_BEGIN

CStatus GParam::getBacktrace(std::vector<std::string>& trace) {
    CGRAPH_FUNCTION_BEGIN
    if (likely(!backtrace_enable_)) {
        // 非问题排查或信息展示场景，更倾向不开启此功能
        CGRAPH_RETURN_ERROR_STATUS("backtrace no enable.")
    }

    backtrace_lock_.lock();
    backtrace_.getUniqueArray(trace);
    backtrace_lock_.unlock();

    CGRAPH_FUNCTION_END
}


CVoid GParam::addBacktrace(const std::string& trace) {
    std::cout<<__FUNCTION__<<std::endl;
    std::cout<<"sizeof(backtrace_enable_) "<<sizeof(this->backtrace_enable_)<<" val "<<this->backtrace_enable_<<std::endl;
    std::cout<<"sizeof(key_) "<<sizeof(this->key_)<<" val "<< this->key_ <<std::endl;
    std::cout<<"sizeof(backtrace_) "<<sizeof(this->backtrace_)<<std::endl;
    std::cout<<"sizeof(backtrace_lock_) "<<sizeof(this->backtrace_lock_)<<std::endl;

    if (likely(!(this->backtrace_enable_))) {
        // 如果没有开启，直接返回即可
        return;
    }

    // 如果name不为空，则添加name信息。如果name为空，则添加session信息
    backtrace_lock_.lock();
    backtrace_.uniqueAdd(trace);
    backtrace_lock_.unlock();
}


CVoid GParam::cleanBacktrace() {
    if (!backtrace_enable_) {
        return;
    }

    backtrace_lock_.lock();
    backtrace_.clear();
    backtrace_lock_.unlock();
}


const std::string& GParam::getKey() const {
    return key_;
}


CStatus GParam::setup() {
    CGRAPH_EMPTY_FUNCTION
}


CVoid GParam::reset(const CStatus& curStatus) {
}

CGRAPH_NAMESPACE_END