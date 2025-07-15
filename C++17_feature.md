
## macro 

在C++11和C++14中有更方便的方法：

``` C++
[[carries_dependency]] 让编译期跳过不必要的内存栅栏指令
[[noreturn]] 函数不会返回
[[deprecated]] 函数将弃用的警告

[[noreturn]] void terminate() noexcept;
[[deprecated("use new func instead")]] void func() {}




```

C++17又新增了三个：

``` C++
// [[fallthrough]]，用在switch中提示可以直接落下去，不需要break，让编译期忽略警告

switch (i) {}
    case 1:
        xxx; // warning
    case 2:
        xxx; 
        [[fallthrough]];      // 警告消除
    case 3:
        xxx;
       break;
}


//[[nodiscard]] ：表示修饰的内容不能被忽略，可用于修饰函数，标明返回值一定要被处理

[[nodiscard]] int func();
void F() {
    func(); // warning 没有处理函数返回值
}

//[[maybe_unused]] ：提示编译器修饰的内容可能暂时没有使用，避免产生警告

void func1() {}
[[maybe_unused]] void func2() {} // 警告消除
void func3() {
    int x = 1;
    [[maybe_unused]] int y = 2; // 警告消除
}

```
---

