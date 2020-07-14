#pragma once

//
// Basis is https://shaharmike.com/cpp/naive-std-function/
//
// It uses a few bytes on the heap for functor object.
// Copy assignment just copies the pointer to the functor object and does not create a new copy! The last one has to cleanup.
// Do not forget to call cleanup() once you do not need the function object (functor) anymore to free the memory.
// No move semantics and/or smart pointers are used.

template <typename T>
class function;

template <typename ReturnValue, typename... Args>
class function<ReturnValue(Args...)>
{
  public:
    function()
    {
        //printf("function()\n");
        callable_ = nullptr;
    }

    ~function()
    {
        //printf("~function()\n");
    }

    template<typename T>
    function(T t)
    {
        //printf("function(T)\n");
        callable_ = new CallableT<T>(t);
    }

    template <typename T>
    function& operator=(T t)
    {
        //printf("operator=(T)\n");
        callable_ = new CallableT<T>(t);
        return *this;
    }

    ReturnValue operator()(Args... args) const
    {
        //assert(callable_);
        if (callable_ == nullptr)
        {
            while (true);
        }
        return callable_->Invoke(args...);
    }

    void cleanup()
    {
        delete callable_;
        callable_ = nullptr;
    }

  private:
    class ICallable
    {
    public:
        virtual ~ICallable() = default;
        virtual ReturnValue Invoke(Args...) = 0;
    };

    template <typename T>
    class CallableT : public ICallable
    {
      public:
        CallableT(const T& t)
            : t_(t) {
        }

        ~CallableT() override = default;

        ReturnValue Invoke(Args... args) override
        {
            return t_(args...);
        }

      private:
        T t_;
    };

    ICallable* callable_;
};
