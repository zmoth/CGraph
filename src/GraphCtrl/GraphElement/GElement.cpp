/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: GElement.cpp
@Time: 2021/6/1 10:13 下午
@Desc: 
***************************/

#include "GElement.h"

CGRAPH_NAMESPACE_BEGIN

/******** public ********/
/* 获取name信息 */
const std::string& GElement::getName() const {
    return this->name_;
}


/* 获取session信息（全局唯一） */
const std::string& GElement::getSession() const {
    return this->session_;
}


/******** protected ********/
GElement::GElement() {
    this->session_ = CGRAPH_GENERATE_SESSION();
}


GElement::~GElement() {
    CGRAPH_DELETE_PTR(aspect_manager_);
}


GElement::GElement(const GElement& element) {
    this->done_ = element.done_;
    this->is_init_ = element.is_init_;
    this->run_before_ = element.run_before_;
    this->dependence_ = element.dependence_;
    this->left_depend_.store(element.left_depend_);
    this->linkable_ = element.linkable_;
    this->session_ = element.session_;
    this->loop_ = element.loop_;
    this->name_ = element.name_;

    if (element.aspect_manager_) {
        this->aspect_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GAspectManager)
        this->aspect_manager_->aspect_arr_ = element.aspect_manager_->aspect_arr_;
        this->aspect_manager_->setName(element.aspect_manager_->getName());
    }
}


GElement& GElement::operator=(const GElement& element) {
    if (this == &element) {
        return *this;
    }

    this->done_ = element.done_;
    this->is_init_ = element.is_init_;
    this->run_before_ = element.run_before_;
    this->dependence_ = element.dependence_;
    this->left_depend_.store(element.left_depend_);
    this->linkable_ = element.linkable_;
    this->session_ = element.session_;
    this->loop_ = element.loop_;
    this->name_ = element.name_;

    if (element.aspect_manager_) {
        this->aspect_manager_ = CGRAPH_SAFE_MALLOC_COBJECT(GAspectManager)
        this->aspect_manager_->aspect_arr_ = element.aspect_manager_->aspect_arr_;
        this->aspect_manager_->setName(element.aspect_manager_->getName());
    }

    return *this;
}


CStatus GElement::beforeRun() {
    CGRAPH_FUNCTION_BEGIN
    this->done_ = false;
    this->left_depend_ = (int)dependence_.size();

    CGRAPH_FUNCTION_END
}


CStatus GElement::afterRun() {
    CGRAPH_FUNCTION_BEGIN

    for (auto& element : this->run_before_) {
        element->left_depend_--;
    }
    this->done_ = true;

    CGRAPH_FUNCTION_END
}


GElementPtr GElement::setName(const std::string& name) {
    if (!name.empty()) {
        this->name_ = name;
    } else {
        this->name_ = this->session_;
    }

    // 设置name信息的时候，顺便给 aspect_manager_ 一起设置了
    if (aspect_manager_) {
        aspect_manager_->setName(name);
    }
    return this;
}


GElement* GElement::setLoop(int loop) {
    this->loop_ = (loop >= 0) ? loop : 0;
    return this;
}


bool GElement::isRunnable() const {
    return 0 >= this->left_depend_ && !this->done_;
}


bool GElement::isLinkable() const {
    return this->linkable_;
}


CStatus GElement::process(bool isMock) {
    CGRAPH_NO_SUPPORT
}


CStatus GElement::addDependElements(const GElementPtrSet& dependElements) {
    CGRAPH_FUNCTION_BEGIN

    for (GElementPtr cur: dependElements) {
        // 如果传入的信息中，有nullptr，则所有的信息均不参与计算
        CGRAPH_ASSERT_NOT_NULL(cur);
    }

    for (GElementPtr cur: dependElements) {
        if (this == cur) {
            continue;
        }

        cur->run_before_.insert(this);
        this->dependence_.insert(cur);
    }

    this->left_depend_ = (int)this->dependence_.size();

    CGRAPH_FUNCTION_END
}


CStatus GElement::setElementInfo(const GElementPtrSet& dependElements,
                                 const std::string& name,
                                 int loop,
                                 GParamManagerPtr paramManager) {
    CGRAPH_FUNCTION_BEGIN
    CGRAPH_ASSERT_INIT(false)

    this->setName(name)->setLoop(loop);
    param_manager_ = paramManager;
    status = this->addDependElements(dependElements);
    CGRAPH_FUNCTION_END
}


CStatus GElement::doAspect(const GAspectType& aspectType, const CStatus& curStatus) {
    CGRAPH_FUNCTION_BEGIN

    // 如果切面管理类为空，或者未添加切面，直接返回
    if (this->aspect_manager_
        && 0 != this->aspect_manager_->getSize()) {
        status = aspect_manager_->reflect(aspectType, curStatus);
    }

    CGRAPH_FUNCTION_END
}

CGRAPH_NAMESPACE_END
