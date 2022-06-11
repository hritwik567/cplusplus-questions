# C++ Theory Questions

1. Why should we use `std::make_shared` and `std::make_unique` instead of calling `std::shared_ptr<T>(new T(arg))`?  
    There are 2 reasons for this:  
        - First reason is efficiency(only applies to `std::make_shared`), if we call `std::shared_ptr<T>(new T(arg))` then our code will call 2 `new` calls one for the type `T` and other for the *ref_count* in `std::make_shared`.  
        - Second reason is that using `new` might lead to memory leaks, take for example the following code:  
        `f1(std::shared_ptr<T>(new T(args)), f2())`  
        There are 3 function calls in the above line  
            1. new T  
            2. call std::shared_ptr<T>'s constructor  
            3. call f2  
        Now, the call to `f2` and call to `std::shared_ptr<T>'s` constructor can be swapped by a compiler and if f2 raises an exception then the newly created T leaks memory.
