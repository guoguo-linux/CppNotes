[toc]



## 一、C++语法与应用

### 1、值传递与引用传递

​	1.1 **值传递与引用传递的使用时机**
​		值传递，传递的是值的拷贝，传递以后与原来的变量就不相关了。
​		引用传递，实际上传递的原对象的地址，对引用变量操作就等于操作原对象。
​		如果只是想传递对象的信息，而不是对其进行修改，在数据量较小的情况下可以使用值传递，
​		但当数据较大(结构体/类)时，还是建议使用引用传递，而且定义为常量引用const doule &value,
​		以防对常量引用变量进行修改。

​	1.2 **临时变量、引用参数和const**
​		如果函数调用的参数不是左值或与相应的const引用参数类型不匹配，则C++将创建正确的匿名变量(临时变量)
​		，将函数调用的参数的值传递给该匿名变量，并让参数来引用该变量。其行为类似于值传递。

​	1.3 **尽可能使用const三个理由**

		- 可以避免无意中修改数据
		- 能够处理const和非const实参，否则只能接受非const数据
		- 能使函数能够正确生成并使用临时变量

### 2、C++中的包装器(Wrapper)

​	包装器，也称为适配器，这些对象用于给其他编程接口提供更一致或更合适的接口。
​	c++11中提供了多种包装器, 模板bind, mem_fn, reference_wrapper 以及function

 1. **std::function**
    一个std::function类型对象实例可包装以下可调用元素类型等。

    - [函数](#)
    - 函数指针
    - 类成员函数指针
    - 任意类型的函数对象(例:定义了operator()操作符重载的类型)

    std::function对象可被拷贝和转移，并且可以使用指定的调用特征来直接调用目标元素。
    当std::function对象未包裹任何实际的可调用元素，调用该std::function对象将抛出std::bad_function_call异常，
    所以要在调用函数对象之前，使函数对象内有可调用的元素。

    【使用】

    ```c++
    #include <iostream>
    #include <functional>
    using namespace std;
    
    int subtract(int m, int n) {
        return (m - n);
    }
    
    template <class T>
    T g_sub(T m, T n) {
        return (m - n);
    }
    
    auto g_Lambda = [](int m, int n) {
        return (m - n);
    };
    
    struct Sub {
        int operator()(int m, int n) {
            return (m - n);
        }
    };
    
    template  <class T>
    struct SubTemp {
        T operator()(T m, T n) {
            return (m - n);
        }
    };
    
    class SubOper {
      public:
        static int stSub(int m, int n) {
            return (m - n);
        }
    
        template <class T>
        static T tempStSub(T m, T n) {
            return (m - n);
        }
    
        double result(double m, double n) {
            return (m - n);
        }
    
        double constResult(double m, double n) const {
            return (m - n);
        }
    };
    
    int main()
    {
        cout << "Usage Of Function!" << endl;
        // 旧式写法
        typedef int(*pFunc)(int, int);
        pFunc oldFunc = subtract;
        cout << "Test old style::" << (*oldFunc)(0,1) << endl;
    
        // [0] 包装函数指针对象
        std::function<int(int, int)> from_pFunc = oldFunc;
        cout << "Test 0::" << from_pFunc(0,0) << endl;
    
        // [1]包装普通函数
        std::function<int(int, int)> newFunc = subtract;
        cout << "Test 1::" << newFunc(1,0) << endl;
    
        // [2]包装模板函数
        std::function<int(int, int)> tempFunc = g_sub<int>;
        cout << "Test 2::" << tempFunc(2,0) << endl;
    
        // [3]包装Lambda函数
        std::function<int(int, int)> lambdaFunc = g_Lambda;
        cout << "Test 3::" << lambdaFunc(3,0) << endl;
    
        // [4]包装仿函数
        std::function<int(int, int)> objFunc = Sub();
        cout << "Test 4::" << objFunc(4,0) << endl;
    
        // [5]包装模板函数对象
        std::function<int(int, int)> tempFuncObj = SubTemp<int>();
        cout << "Test 5::" << tempFuncObj(5,0) << endl;
    
        // [6]类静态函数
        std::function<int(int, int)> stFunc = &SubOper::stSub;
        cout << "Test 6::" << stFunc(6,0) << endl;
    
        // [7]类静态模板函数
        std::function<int(int, int)> tempStFunc = &SubOper::tempStSub<int>;
        cout << "Test 7::" << tempStFunc(7, 0) << endl;
    
        // [8]类普通函数（普通函数绑定需要依赖类对象）
        SubOper subOper;
    
        // [8.1] 使用bind，将类对象地址绑定上
        std::function<double(double, double)> resultFunc = std::bind(&SubOper::result, &subOper, placeholders::_1, placeholders::_2);
        cout << "Test 8.1::" << resultFunc(8.2, 0.1) << endl;
    
        // [8.2] 不使用bind
        std::function<double(SubOper &, double, double)> resultFunc2 = &SubOper::result;
        cout << "Test 8.2::" << resultFunc2(subOper, 8.3, 0.1) << endl;
    
        // [8.3] const
        std::function<double(SubOper &, double, double)> const_resultFunc2 = &SubOper::constResult;
        cout << "Test 8.3::" << const_resultFunc2(subOper, 8.4, 0.1) << endl;
    
        // [8.4] 常量对象
        std::function<double(const SubOper &, double, double)> const_resultFunc3 = &SubOper::constResult;
        cout << "Test 8.4::" << const_resultFunc3(subOper, 8.4, 0.1) << endl;
    
        return 0;
    }
    
    ```

    **应用**

    - 解耦

      场景1：

    


```c++
#include <iostream>
#include <functional>
#include <string>
int main()
{
    std::cout << "Usage Of Function!" << std::endl;
class TestA {
public:
    //通过暴露该接口在外部设置函数对象的具体功能
    void setDestoryHandle(std::function<bool(const std::string&)> setHandle) {
        mDestoryHandler = setHandle;
    }
    bool doDestoryByName(std::string name) {
        return mDestoryHandler(name);
    }

private:
    //定义一个函数对象成员变量
    std::function<bool(const std::string &)> mDestoryHandler;

};

class TestB {
public:
    bool destory(const std::string & name) {
        std::cout << "Test:: Call TestB destroy | name:" << name <<std::endl;
        return true;
    }
};

TestB objB;
TestA objA;

//通过调用B的成员函数设置A的函数对象功能，实现了mDestoryHandler的解耦
objA.setDestoryHandle([&](const std::string &name)->bool{
    return objB.destory(name);
});
objA.doDestoryByName("test0");

//也可以直接在lambda内直接实现其功能
objA.setDestoryHandle([&](const std::string &name)->bool{
    std::cout << "Test:: Register By User | name:" << name <<std::endl;
    return true;
});
objA.doDestoryByName("test1");

return 0;
}

/*测试结果*/
Usage Of Function!
Test:: Call TestB destroy  | name:test0
Test:: Register Cb By User | name:test1
```

场景2：
    
将数据产生与数据发送进行解耦

```c++
#include <iostream>
#include <functional>
#include <string>
#include <vector>

//Defined in media.hpp
class Media {
public:
    void sendData(std::function<void(const char* data, uint64_t dataLen)> dataFun) {
        std::vector<char> vecData('a',10);
        dataFun(vecData.data(), vecData.size());
    }
};

//Defined in network.hpp
class NetWork {
public:
    void sendByNetwork(const char* data, uint64_t dataLen){
        std::cout << "network sending..." << std::endl;
    }
};

int main()
{
    std::cout << "Usage Of Function!" << std::endl;
    Media media;
    NetWork network;

    media.sendData([&](const char* data, uint64_t dataLen){
        network.sendByNetwork(data,dataLen);
    });

    return 0;
}

/* 测试结果
 * Usage Of Function!
 * network sending...
 */
```

```
    

#### 3、类成员变量的初始化(),{},以及列表初始化

4. file_worker 开机初始化部分代码应该怎么写
```

场景3:
利用function注册实现多层级数据传递

```c++
/* media.h */
class Media {
   public:
    void sourceData(std::function<void(char* data, int len)> fun) {
        std::vector<char> msg('a', 10);
        fun(msg.data(), msg.size());
    }
};
```

```c++
/* mediaWorker.h */
#include "media.h"
class MediaWorker {
   public:
    MediaWorker(std::function<void(char* data, int len)> func)
        : mFunction(func), mThMedia([this]() { this->loop(); }) {}

    void loop() {
        while (true) {
            //1. 可以直接将mediaWorker上层的回调注册进来
            mMedia.sourceData(mFunction);

            //2. 如果要使用回调函数传递的数据，可以再包一个lambda，取出想要的数据
            mMedia.sourceData([this](char* data, int len){
                if(len > 100) {
                    //do something
                    std::cerr << "data len too long\n";
                }else{
                    mFunction(data, len);
                }
            });
        }
    }

   private:
    Media mMedia;
    std::function<void(char* data, int len)> mFunction;
    std::thread mThMedia;
};
```

```c++
#include "mediaWorker.h"
class Application {
   public:
	//将数据传递到最外层
    MediaWorker mMediaWorker{[](char * data, int len){
        //do something 
    }};
};
```

对于Media类，它通过sourceData接口产生数据。如果想将数据通过网络传递出去的话，最直白的办法就是在该接口中手写套接字通过网络发送，这样一来就将数据与网络耦合在了一起，最好将数据接口分离开来。通过function函数对象，将数据层层传递。可以用lambda对象实例化function，可以使用注册进来的function对象来实例化。





### 3、RAII

RAII（**R**esource **A**cquisition **I**s **I**nitialization）资源获取即初始化，使用局部对象来管理资源。
这里的资源指的是从系统获取，使用完需要还给系统的东西，如果使用后不释放，会导致系统资源减少。
常见的有堆内存，句柄资源，锁资源，数据库连接等。

RAII的四个过程：

​      a.设计一个类封装资源

​      b.在构造函数中初始化

​      c.在析构函数中执行销毁操作

​      d.使用时声明一个该对象的类

#### RAII的应用

- 基础封装
  先基本封装一下POSIX标准的互斥锁(https://zhuanlan.zhihu.com/p/34660259)
  再通过RAII继续封装

```c++
class MutexLock{
    public:
    	explicit MutexLock(Mutex *mu):mMutex(mu){
            this->mMutex->Lock();
        }
    	~MutexLock() { this->mMutex->Unlock();}
    private:
    Mutex *const mMutex;
    //No copy allowed
    MutexLock(const MutexLock&);
    MutexLock& operator=(const MutexLock&);
};
```

​	锁资源在类构造时获取，在析构时自动释放。

- 通过智能指针管理

  智能指针默认情况下使用delete释放它所指向的对象，我们可以重载一个智能指针中默认的删除器来自定义析构行为。

  ```c++
  class Resource {
      Resource() { /*获取资源*/ }
      ~Resource() { /*资源释放*/ }
  };
  ```

  例子: RAII封装海思音频帧对象
  音频帧的获取`` ``

2、RAII与多线程

2、智能指针返回引用与裸指针

使用RAII需要注意一下几个方面:

- **对拷贝的限制**

  Q:为什么要限制拷贝？
  A:对于一些资源的RAII类，复制行为本身是不合理的，如果对系统mutex类进行了拷贝将增加管理的风险

  ```c++
  //可以将拷贝构造函数和=重载声明为private，在类外就无法调用拷贝了
  class NoCopyResource {
    private:
    	NoCopyResource(const NoCopyResource& rhs);
      NoCopyResource& operate=(const NoCopyResource& rhs);
  };
  
  //在C++11中可以使用delete禁用
  class NoCopyResource {
    public:
    	NoCopyResource(const NoCopyResource& rhs) = delete;
      NoCopyResource& operate=(const NoCopyResource& rhs) = delete;
  };
  ```

  

- **深度拷贝，复制底层资源**
  Q:拷贝RAII合理吗？
  A:只要是需要且合理，都可以进行资源的复制。此时选择复制RAII类，是为了确保不需要该资源的副本可以被RAII正确释放。
  Q:什么情况需要深拷贝？
  A:对象中存在需要RAII类管理的资源时，基本都需要深度拷贝。因为浅拷贝会造成两个对象中都有指向同一个源的指针，如果其中一个析构时释放了资源，另一个变成为了野指针。

  ```c++
  //使用一个自定义string类做例子，资源类流程都类似
  
  //资源类
  class String{
  public:
      static String* MakeString(char* str) {
          
      }
      ~String() {
          
      }
  private:
      //无法通过构造函数来直接获取资源
      String(char* str) {};
      static char* mStr;
  };
  
  //管理类
  class StringManager {
  public:
      String* getString(char* str) {
          //RAII
          
      }
      ~StringManager() {}
      StringManager(const StringManager& rhs) {}
      StringManager& operator=(const StringManager& rhs) {}
  private:
      String* mRes;
  };
  ```



- **完整的RAII实现**

  

### 4、类型强转 - 丢弃const声明

```c++
const_cast用来丢弃变量的const声明，但不能改变变量所指向的对象的const属性。即：const_cast用于原本非const的对象；如果用于原本const的对象，结果不可预知（C++语言未对此种情况进行规定）
```

### 5、线程

thread::join()  -- 在调用join()方法的主线程A中阻塞等待join的线程B退出。线程创建后默认是可结合的(joinable)，当线程函数B自己退出或exit时，其所占用的资源不会被释放，只有调用join()返回后， B线程所占有的资源
才会被释放。
**thread::detach():** 从 thread 对象分离执行的线程，允许执行独立地持续。一旦线程退出，则释放所有分配的资源。调用 `detach` 后， *this 不再占有任何线程。
**多线程调用类方法操作同一个变量时要对该方法进行保护**
直接使用std::mutex的错误示范:

```c++
class A {
  public:
    void setSomething(std::string addr) {
       mMutex.lock();
       mSomething = addr;
       mMutex.unlock();
    }
    std::string getSomething() {
       mMutex.lock();
       return mSomething;   //直接返回了，没有释放锁，导致死锁
       mMutex.unlock();
    }
  private:
    std::string mSomething;
    std::mutex mMutex;
};
```

使用unique_lock自动管理锁的生命周期	

```c++
class A {
  public:
    void setSomething(std::string addr) {
       std::unique_lock<std::mutex> uniqueLock(mMutex);
       mSomething = addr;
    }
    std::string getSomething() {
       std::unique_lock<std::mutex> uniqueLock(mMutex);
       return mSomething;  
    }
  private:
    std::string mSomething;
    std::mutex mMutex;
};

```

### 6、完美转发

完美转发：指函数模板可以将自己的参数"完美"地转发给内部调用的其他函数。“完美”，指不仅能准确地转发参数的值，还能保证转发参数的左右值属性不变。
示例:

```c++
#include <iostream>

template <typename T>
void print(T &t) {
    std::cout << "lvalue" << std::endl;
}

template <typename T>
void print(T &&t) {
    std::cout << "rValue" << std::endl;
}

template <typename T>
void testForward(T &&v) {
    print(v);
    print(std::forward<T>(v));
    print(std::move(v));
}

int main() {
    testForward(1);
    std::cout << "==========" << std::endl;
    int x = 1;
    testForward(x);

    return 0;
}
```

输出:

```c++
lvalue
rValue
rValue
==========
lvalue
lvalue
rValue
```

分析：

传参为1时
``print(v);``传入的1虽然是右值，但是结果函数传参后分配了内存空间变成了左值。
``print(std::forward<T>(v));``使用std::forward方法，会保持其右值属性，故会调用参数为右值引用的模板函数``print(std::move(v));`` move就将传参强制转为右值。
同理传参为左值变量x时，输出如上。

**注意**

> C++11中，通常情况下右值引用形式的参数只能接收右值，不能接收左值。但对于函数模板中使用右值引用语法定义的参数来说，它不再遵守这一规定，既可以接收右值，也可以接收左值（此时的右值引用又被称为“万能引用”）。

推荐阅读:
**C++ std::move and std::forward**  http://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html

### 7、重载operator()

*参考:https://zhuanlan.zhihu.com/p/75353199*
在c++类中重载()，可以实现函数对象，该类就有类似于函数的功能。
operator()有两种常见的重载用法: Callable 和 索引
**Callable**
callable即可调用对象，包括函数指针，重载operator()的对象以及可隐式转换为两者的对象。
重载operator()的对象，也称Functor(函子)。
Functor行为类似函数,C++中的仿函数是通过在类中重载()运算符实现，使你可以像使用函数一样来创建类的对象
**1、基础使用方法**

```c++
#include <iostream>
class FuncClass{
    public:
    	void operator()(const string& str) const {
            std::cout << str << std::endl;
        }
};

int main() {
    FuncClass funClass;
    funClass("print ...");
    return 0;
}
```

**2、作为回调函数**

```c++
#include <iostream>
#include <memory>

// for_each:对于C数组中的每一个元素都进行一个处理
//在Func f 对象中做具体的处理
template <typename T, typename Func>
void for_each(T* begin, T* end, const Func& f) {
    while (begin != end) f(*begin++);
}

//打印一个变量
template <typename T>
void print(const T& x) {
    std::cout << x << " ";
}

//重载operator()的对象
template <typename T>
struct Print {
    void operator()(const T& x) const { std::cout << x << " "; }
};

int main() {
    int arr[5] = {1, 2, 3, 4, 5};

    for_each(arr, arr + 5, print<int>);

    for_each(arr, arr + 5, Print<int>());

    return 0;
}
```

也可以定义一个lambda，这个lambda函数本质上也是一个匿名Functor，C++标准中称之为闭包类型Closure。

```c++
for_each(arr, arr + 5, [](auto&& x) { std::cout << x << " "; });
```

### 8、队列

#### 	1.std::queue

​	std::queue类，实现队列的功能——FIFO，在容器尾部推入元素，从首端弹出元素。
​	实现大概如下:

```c++
template <typename T, typename Container = std::deque<T>>
class queue {
    T& front();
    void push( const T &elem );
    void pop();
    bool empty() const; // avoid undefined behaviour before calling front() 
};
```

其中，front()用来获取头部元素，而pop()使队列头部元素出列。但是为什么不在pop()出列的时候返回元素。如下:

```c++
template <typename T, typename Container = std::deque<T>>
class queue {
  T pop();  
};
```

原因有二:
1) 保证异常安全

```c++
template<class T>
class queue {
    T* elements;
    std::size_t top_position;
    // stuff here
    T pop()
    {
        auto x = elements[top_position];
        // TODO: call destructor for elements[top_position] here
        --top_position;  // alter queue state here
        return x;        // calls T(const T&) which may throw
    }
};
```

```c++
queue<Widget> q;
//....
Widget value = q.pop();
```

在执行赋值的时候，Widget的拷贝构造函数``Widget(const Widget& )``被调用，当``--top_postition``状态已改变，元素在队列内被删除，若此时发生异常，没有拷贝成功，队列顶部的元素就丢失了。如果将出队pop和返回队首元素分离开，先执行front()，将元素先拷贝，再将队首元素删除就可以避免。
2）效率低
	贴一段官方的解释:

> One might wonder why `pop()` returns `void`, instead of `value_type`. That is, why must one use `top()` and `pop()` to examine and remove the top element, instead of combining the two in a  single member function? In fact, there is a good reason for this design. If `pop()` returned the top element, it would have to return by value rather than by  reference: return by reference would create a dangling pointer. Return  by value, however, is inefficient: it involves at least one redundant  copy constructor call. Since it is impossible for `pop()` to return a value in such a way as to be both efficient and correct, it is more sensible for it to return no value at all and to require clients  to use `top()` to inspect the value at the top of the stack

​	意思是，如果返回值，就会创建一个没人使用的副本，而且在接受该值时Widget value必然发生一次实例化。返回引	用则会产生一个空悬指针。

#### 2. 实现ThreadQueue阻塞队列

该阻塞队列将有以下几点要求:

- 可以支持多生产者多消费者并发访问	
- 队列为空时，元素出列会被阻塞
- 元素出列时保证异常安全性

```c++
#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

template <typename T>
class BlockQueue {
   public:
    T pop() {
        std::unique_lock<std::mutex> lock(mMutex);
        mCondition.wait(lock, [this]() { return !mQueue.empty(); });
        auto value = std::move(mQueue.front());
        mQueue.pop();
        return value;
    }

    void pop(T& item) {
        std::unique_lock<std::mutex> lock(mMutex);
        mCondition.wait(lock, [this]() { return !mQueue.empty(); });
        item = std::move(mQueue.front());
        mQueue.pop();
    }

    bool tryPop(T& item) {
        std::unique_lock<std::mutex> lock(mMutex);
        if (mQueue.empty()) {
            return false;
        }
        item = std::move(mQueue.front());
        mQueue.pop();
        return true;
    }
	//empty,size should not modify the queue, so const
    bool empty() const noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        return mQueue.empty();
    }

    size_t size() const noexcept {
        std::unique_lock<std::mutex> lock(mMutex);
        return mQueue.size();
    }

    void push(const T& item) {
        std::unique_lock<std::mutex> lock(mMutex);
        mQueue.push(item);
        lock.unlock();  // unlock before notificiation to minimize mutex
                        // contention
        mCondition.notify_one();  // notify one waiting thread
    }

    void push(T&& item) {
        std::unique_lock<std::mutex> lock(mMutex);
        mQueue.push(std::move(item));
        lock.unlock();
        mCondition.notify_one();
    }

    BlockQueue() = default;
    BlockQueue(const BlockQueue&) = delete;             //disable copying
    BlockQueue& operator=(const BlockQueue&) = delete;  //disable assignment
   private:
    std::queue<T> mQueue;
    std::mutex mMutex;
    std::condition_variable mCondition;
};

#endif
```

1. pop()从队列取出元素时，先给互斥量上锁，然后条件变量wait检测是否满足谓词条件``！mQueue.empty()`` ,满足的话即队列不为空，则取出数据，否则暂时释放锁，阻塞线程，直到条件满足才会被唤醒。

2. push()向队列填入数据，push完之后会主动唤醒阻塞的pop()操作。

3. empty，size成员函数都不会修改queue，所以将其声明为const常成员函数，表明不会修改成员变量，使其函数意义更清晰，增加可读性。

4. 在``push()``的唤醒操作前会主动释放锁，是为了最小化互斥量的争用。在``pop()``中锁的释放由``wait()``和``unique_lock``的生命周期决定。

5. 对于上面的同步队列类还有很多扩展的地方，比如pop()可以增加超时机制，可以队列限制队列的大小，超过大小时阻塞``push()``操作。

6. ``push()``方法重载了左值引用与右值两个版本，以适应传入的左值与右值，不使用值传递是为了避免无效的拷贝。
   当然，还可以使用函数模板与完美转发来代替两次重载。

   ```c++
       template <typename Message>
       void push(Message&& mesg) {
           std::unique_lock<std::mutex> lock(mMutex);
           mQueue.push(std::forward<Message>(mesg));
           lock.unlock();
           mCondition.notify_one();
       }
   ```

   

多生产者消费者场景

   ```c++
#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "blockQueue.h"

BlockQueue<std::string> queue;
std::mutex printMutex;

void producer(int productNum, int waitMilliseconds) {
    for (int i = 0; i < productNum; ++i) {
        std::string message{"Message-"};
        message.append(std::to_string(i));
        queue.push(message);
        std::cout << "push data" << message << std::endl;
        std::this_thread::sleep_for(
            std::chrono::milliseconds(waitMilliseconds));
    }
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << "producer push message ok!\n";
}

void consumer(int consumerID) {
    while (true) {
        std::string message;
        queue.pop(message);
        {
            std::lock_guard<std::mutex> lock(printMutex);
            std::cout << "consumer-" << consumerID << " receive:" << message
                      << std::endl;
        }
    }
}

int main() {
    std::thread thProducer1{producer, 10, 1};
    std::thread thProducer2{producer, 10, 1};
    std::thread thConsumer1{consumer, 1};
    std::thread thConsumer2{consumer, 2};

    thProducer1.join();
    thProducer2.join();
    thConsumer1.join();
    thConsumer2.join();

    return 0;
}
   ```



> 参考:
>
> 1. http://senlinzhan.github.io/2015/08/24/C-%E5%AE%9E%E7%8E%B0%E9%98%BB%E5%A1%9E%E9%98%9F%E5%88%97/
> 2. https://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/

​	

###    9、条件变量

​	在C++11中，可以使用条件变量实现多个线程间的同步操作。
条件变量是利用线程间共享的全局变量进行同步的一种机制，主要有两个动作:

- 一个线程因等待“条件变量的条件成立"而挂起 (wait / wait_for / wait_until)
- 另一个线程使"条件成立"，给出信号，从而唤醒被等待的线程

条件变量一般和一个互斥量同时使用，使用时需要先给互斥量上锁，然后条件变量检测是否满足条件，如果不满足条件就暂时释放锁，然后阻塞线程，直到条件满足，线程才会被唤醒。一般使用互斥锁std::mutex，且使用std::unique_lock管理锁。

#### 1. **成员函数说明**

1) wait()成员函数

```c++
void wait(std::unique<std::mutex>& lock);
//Predicate 谓词函数，可以是普通函数或是lambda表达式
template<class Predicate>
void wait(std::unique<std::mutex>& lock, Predicate pred);
```

以上两种都是被``notify_one()``或 ``notify_broadcast()``唤醒，但是第二种方式先得满足Predicate的条件，再wait，否则继续解锁互斥量，使线程处于阻塞或等待。等价于:

```c++
while(!Pred()) {
    wait(lock);
}
```

2）wait_for() 成员函数

```c++
template< class Rep, class Period >
std::cv_status wait_for( std::unique_lock<std::mutex>& lock,
                         const std::chrono::duration<Rep, Period>& rel_time);
                         
template< class Rep, class Period, class Predicate >
bool wait_for( std::unique_lock<std::mutex>& lock,
               const std::chrono::duration<Rep, Period>& rel_time,
               Predicate pred);
```

wait_for 导致当前线程阻塞直至条件变量被**通知**，或**虚假唤醒发生**，或者**超时返回**。
返回值说明：
		若经过 rel_time 所指定的关联时限则为 std::cv_status::timeout ，否则为 std::cv_status::no_timeout 。
		若经过 rel_time 时限后谓词 pred 仍求值为 false 则为 false ，否则为 true 。
以上两个wait函数都会在阻塞时调用unique_lock的unlock()自动释放锁，以便其他线程有机会获得锁。

3) notify_all / notify_one

```c++
void notify_one() noexcept;
若任何线程在 *this 上等待，则调用 notify_one 会解阻塞(唤醒)等待线程之一
void notify_all() noexcept;
若任何线程在 *this 上等待，则解阻塞（唤醒)全部等待线程
```

#### 2.**虚假唤醒 **

> 用于多线程竞争条件下，具体含义去参考官方文档。我说一下形象的意思就是你妈同时通知你爸和你去做饭，做饭只需要一个人做即可， 当你去准备做饭时，你爸已经在做饭了，所以你就相当于被虚假唤醒了。

在正常情况下，wait类型函数返回有两种情况: 唤醒返回，超时返回
实际情况下，因为操作系统的原因，wait类型在不满足条件时也会返回，导致虚假唤醒，因此一般都是用带谓词的wait函数，wait(xxx, Predicate pred);

```c++
while(!Pred()) {
    wait(lock);
}
```

若不存在虚假唤醒，代码如下:

```c++
if(不满足xxx条件){
    //没有虚假唤醒，wait函数可以一直等待，直到被唤醒或者超时，没有问题。
    //但实际中却存在虚假唤醒，导致假设不成立，wait不会继续等待，跳出if语句，
    //提前执行其他代码，流程异常
    wait();   
}
//其他代码
...
```

正确使用方式·，使用while语句解决:

```c++
while (!(xxx条件)) {
    //虚假唤醒发生，由于while循环，再次检查条件是否满足，
    //否则继续等待，解决虚假唤醒
    wait();  
}
//其他代码
....
```

#### 3.使用条件变量

使用条件变量解决生产者消费者问题，问题描述如下:
生产者生成一定量的数据放到缓冲区，重复此过程。同时消费者也在缓冲区消耗这些数据。
问题关键: 要保证生产者不会在缓存区满时加入数据，消费者也不会缓冲区空时消耗数据。
所以必须让生产者在缓冲区满时休眠，等到下次消费者消耗缓冲区数据的时候，生产者才能被唤醒，开始向缓冲区添加数据。也可以让消费者在缓冲区空时进入休眠，等到生产者向缓冲区添加数据后，再唤醒消费者。

```c++
#include <unistd.h>

#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mutex;
std::condition_variable cv;

// buffer
std::deque<int> dataDeque;
const int maxNum = 30;
int nextIndex = 0;

const int producerThreadNum = 3;
const int consumerThreadNum = 3;

void producerThread(int threadId) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        //加锁
        std::unique_lock<std::mutex> uniqueLock(mutex);
        //当队列未满时，继续添加数据
        cv.wait(uniqueLock, []() { return dataDeque.size() <= maxNum; });
        nextIndex++;
        dataDeque.push_back(nextIndex);
        std::cout << "producer thread: " << threadId << "  data: " << nextIndex;
        std::cout << "  queue size: " << dataDeque.size() << std::endl;

        //向缓冲区放入数据后，唤醒消费者
        cv.notify_all();
        //自动释放锁
    }
}

void consumerThread(int threadId) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(550));
        //加锁
        std::unique_lock<std::mutex> uniqueLock(mutex);
        //当队列非空时，取走数据
        cv.wait(uniqueLock, []() { return !dataDeque.empty(); });
        nextIndex--;
        int data = dataDeque.front();
        dataDeque.pop_front();
        std::cout << "consumer thread: " << threadId << "  data: " << nextIndex;
        std::cout << "  queue size: " << dataDeque.size() << std::endl;

        //消费者从缓冲区取走数据后唤醒生产者
        cv.notify_all();
        //自动释放锁
    }
}

int main() {
    std::thread arrProducerThread[producerThreadNum];
    std::thread arrConsumerThread[consumerThreadNum];

    for (int i = 0; i < producerThreadNum; i++) {
        arrProducerThread[i] = std::thread(producerThread, i);
    }
    for (int i = 0; i < consumerThreadNum; i++) {
        arrConsumerThread[i] = std::thread(consumerThread, i);
    }
    for (int i = 0; i < producerThreadNum; i++) {
        arrProducerThread[i].join();
    }
    for (int i = 0; i < consumerThreadNum; i++) {
        arrConsumerThread[i].join();
    }
    return 0;
}
//reference:https://blog.csdn.net/c_base_jin/article/details/89741247
```





### 9、海思程序框架

有以下几个对象:

```c++
1.MediaWorker  
//负责系统及子模块初始化，并产生音视频帧数据,通过接口发送音频、视频的裸流到不同模块
2.Publisher
//通过发布-订阅模式实现网络发送接口，将接口以回调的形式注册到各个模块实现数据发送
//发送数据有:音视频裸流、Ts流、系统信息、文件状态、Ts状态、Rtmp状态
3.FileWorker
//实现接受Ts保存为Mp4文件的功能，并监测上报文件保存状态
4.TsWorker
//该模块接收MediaWorker产生的裸流，生成Ts流发送给订阅者及FileWorker,且将转Ts的错误信息发布给订阅者
5.RtmpWorker
//接受MediaWorker产生的裸流，在内部发送给Rtmp的URL,且发布Rtmp推流过程中的错误信息给订阅者
6.SystemInfoCollector
//系统信息收集器，主要定时收集电量、内存、编码属性、ISP等信息，并发布给订阅者
7.MessageController
//通过请求-应答模式，接受指令，控制各个模块的行为
8.FdkaacEncoder
//第三方的aac编码库，对MediaWorker的音频裸流进行多声道编码
```

框架如下:

![framework(1)](./pic/framework(1).png)





###  end 

### 10、写文件相关的若干问题

现在有一个需求：每秒钟向文件中写入一个整型数，每次递增1，且要覆盖之前的数。
初步的代码如下:

```c++
int main() {
    std::ofstream file;
    file.open("test.dat", std::ios::binary);
    if (!file.is_open()) {
        std::cout << "open file failed!\n";
        return -1;
    }
    int count = 0;
    while (true) {
        file.write((char*)&count, sizeof(int));
        file.flush();
        count++;
        sleep(1);
    }
    file.close();
    return 0;
}
```

运行后cat一下，文件里似乎没有写入任何东西。
改进了一下代码:
版本一:

```c++
int main() {
    std::ofstream file;
    file.open("test.dat",std::ios::binary);
	if(!file.is_open()) {
        std::cout << "open file failed!\n";
		return -1;
    }
    int count = 0;
	while(true) {
        file.seekp(std::ios::beg);  //write in the begin
		file << count;
		file.flush();
		count++;
		sleep(1);
    }
    file.close();
    return 0;
}
```

版本二:

```c++
int main() {
    std::ofstream file;
    file.open("test.dat",std::ios::binary);
	if(!file.is_open()) {
        std::cout << "open file failed!\n";
		return -1;
    }
    int count = 0;
	while(true) {
        std::string strNum = std::to_string(count);
        file.seekp(std::ios::beg);  //write in the begin
        file.write(strNum.c_str(), strNum.size());
		file.flush();
		count++;
		sleep(1);
    }
    file.close();
    return 0;
}
```

在最开始的错误代码运行后的test.dat文件里并非没有东西，使用vscode的Hex Editor打开可以看到
![charCoder](/home/tianyu/doc/LearnCpp/C++Notes/pic/charCoder.jpg)

文件中写入的依次的count的二进制值，在文件打开显示的时候，会按照二进制值对应的ASCII码的字符进行显示。比如count = 48 时，二进制值为00110000，以十六进制打开就是对应的30, 十进制对应的是48, 按照ASCII表，对应的字符是0，后面的依次类推。ASCII表如下:
![ASCII](/home/tianyu/doc/LearnCpp/C++Notes/pic/ASCII.png)

在版本一与版本二中，">>"和“to_string”都对count进行了格式化，就如何printf一样格式化输出，格式化后写入文件的就是原内容的ASCII值，这样打开文件，就可以直接显示原来的内容。比如count=0，格式化后实现写入文件的是48(00110000)。
如何在每次写入时覆盖掉之前的内容，可以使用std::fstream::feekp(), 重新定位文件指针。
std::ios::beg, std::ios::end;

### 11、实例化对象时使用()、{}的区别





## 二、C++核心准则

- 坚持使用const（检查成员函数是否修改对象；检查函数是否修改通过指针或引用传递的参数）[P.1]()

- 编译时检查优于执行时检查，无法在编译时检查的，应可以在执行时检查 [P.5-6]()

- 使用标准库优化，封装**混乱**、**低层次**、**冗长**的代码(如涉及 复杂指针、自己管理生命周期、类型转换等) [P.11]()

- 避免非常量的全局变量

  > 如果想使用全局数据(命名空间)



## 三、LearnCpp

### 16.6 容器类

生活中，容器随处可见，主要的作用有两个:组织和储存。
同理，容器类是用于保存和组织另外一种类型的类。c++中有很多容器类，vector，map，queue等。
比较常用的就是vector数组类，相比于内置类型的数组，数组容器类在增删元素时能动态调整大小，在传递给函数时记住它们的大小，并进行边界检查。  这使得数组容器类比普通数组更方便，安全。
我们看一下一个定义良好的容器类具有哪些功能:

- 可以通过构造函数创建一个空容器
- 能在容器中插入或移除对象
- 能知道当前容器中的对象数量
- 能清空容器中的所有对象
- 能对容器中的对象进行访问
- 能对元素进行排序(可选)

有时，有的容器会忽略部分功能，例如数组类会忽略掉操作比较慢的插入删除操作。

#### 容器的类型

容器类通常有两种不同的种类。  
**值容器**是存储它们所持有的对象副本的组合（因此负责创建和销毁这些副本）。  ？？？
**引用容器**是存储指向其他对象的指针或引用的聚合（因此不负责创建或销毁这些对象） ？？？

通常，容器只保存一种类型的数据，整型数组就只能存放整型数据。如果同时想在一个容器保存多种类型数据，可以借助模板来实现。

#### 实现一个数组容器类

以上面的功能作参考，从头实现一个整型数组类。这个容器类会是一个值容器，保存元素的副本。类似于``std::vector<int>``。
首先创建头文件

```c++
#ifndef INTARRAY_H
#define INTARRAY_H
 
class IntArray{
};
 
#endif
```



### 8.14 — 函数模板实例化

上一节介绍了函数模板，这一节将重点介绍如何使用模板

#### 使用函数模板

函数模板实际上并不是函数——它们的代码不是直接编译或执行的。相反，函数模板只有一项工作：生成函数；
要使用我们的 max<T> 函数模板，我们可以使用以下语法进行函数调用：

```c++
max<actual_type>(arg1,arg2);  //actual_type is some type, like int or double
```

这看起来很像一个普通的函数调用——主要的区别是在尖括号中添加了类型，它指定了将用来代替模板类型 T 的实际类型。
看一下下面的例子:

```c++
#include <iostream>
 
template <typename T>
T max(T x, T y){
    return (x > y) ? x : y;
}
 
int main(){
    std::cout << max<int>(1, 2) << '\n'; // 实例化->调用max<int>(int, int)
    return 0;
}
```

当编译器执行到``max<int>(1,2)``这一行时，会发现该函数的定义并不存在，最后编译器会找到匹配的函数模板并创建出一个函数。这个创建的过程就叫做函数模板实例化。当实例化在函数调用的时候发生时就叫做隐式实例化。实例化的函数叫做实例或模板函数。
实例化的过程很简单:编译器实质上复制了模板，然后将我们指定的类型(int)替代了模板类型。
在函数首次实例化后，再次调用该函数就无需实例化(多次调用模板函数，仅在第一次实例化)。如下例:

```c++
#include <iostream>
 
template <typename T>
T max(T x, T y){
    return (x > y) ? x : y;
}
 
int main(){
    std::cout << max<int>(1, 2) << '\n'; // 实例化并调用模板函数 max<int>(int, int)
    std::cout << max<int>(4, 3) << '\n'; // 调用已经实例化的模板函数max<int>(int, int)
    std::cout << max<double>(1, 2) << '\n'; // 实例化并调用 max<double>(double, double)
    return 0;
}
```

实例化后，等效于以下:

```c++
#include <iostream>
 
template <typename T> //函数模板的声明(无需再定义)
T max(T x, T y);

//实例化后生成一下两个模板函数(重载两个函数)
template<>  // the generated function max<int>(int, int)
int max<int>(int x, int y) {
    return (x > y) ? x : y;
}
 
template<> // the generated function max<double>(double, double)
double max<double>(double x, double y) {
    return (x > y) ? x : y;
}
 
int main(){
    std::cout << max<int>(1, 2) << '\n'; // 实例化并调用模板函数 max<int>(int, int)
    std::cout << max<int>(4, 3) << '\n'; // 调用已经实例化的模板函数max<int>(int, int)
    std::cout << max<double>(1, 2) << '\n'; // 实例化并调用 max<double>(double, double)
    return 0;
}
```

注意：在实例化max<double>的时候，由于指定了参数类型为double，即使传入的实参为int，也会被隐式转换成double。
实例化后就相当于编译器为我们手动重载了一系列模板函数。

#### 模板参数推导

在大多数情况下，用于实例化的实际类型与函数参数的类型是相匹配的。

```c++
std::cout << max<int>(1,2)<<'\n';  //指定调用max<int>
```

在上面例子中，我们指定要用int替代T，但同时我们也使用int参数调用该函数。
通常情况下，当实参类型与我们想要匹配的模板参数类型相匹配的情况下，我们不需要指定实际类型。相反，可以使用模板类型推导让编译器根据实参自动推导出匹配的模板参数类型来实例化。
上面的代码就可以如下:

```c++
std::cout << max<>(1, 2) << '\n';
std::cout << max(1, 2) << '\n';
```

这里有两种写法，一个带空的尖括号，一个不带。两者有什么区别？
带空尖括号的调用时，编译器在确定调用哪个重载函数时，只会考虑max<int>模板函数重载。
对于不带尖括号，编译器将同时考虑max<int>函数模板重载和max非模板函数重载。
示例代码:

```c++
#include <iostream>
 
template <typename T>
T max(T x, T y){
    return (x > y) ? x : y;
}
 
int max(int x, int y){
    return (x > y) ? x : y;
}
 
int main(){
    std::cout << max<int>(1, 2) << '\n'; // 选择模板函数 max<int>
    std::cout << max<>(1, 2) << '\n'; // 推断出max<int>(int, int) (非模板函数就不会考虑)
    std::cout << max(1, 2) << '\n'; // 调用非模板函数 max(int, int)
    return 0;
}
```

对于``max(1,2)``这种与普通函数调用相同的语法，是调用函数模板时的首选语法，以后默认使用这种写法。

#### 具有非模板参数的函数模板

直接看例子:

```c++
template <typename T>
int someFcn (T x, double y){
    return 5;
}

int main(){
    someFcn(1, 3.4); // matches someFcn(int, double)
    someFcn(1, 3.4f); // matches someFcn(int, double) --float型转为double
    someFcn(1.2, 3.4); // matches someFcn(double, double)
    someFcn(1.2f, 3.4); // matches someFcn(float, double)
    someFcn(1.2f, 3.4f); // matches someFcn(float, double) --float型转为double
    return 0;
}
```



#### 实例化函数不一定被编译

看一下以下代码:

```c++
#include <iostream>
#include <string>
 
template <typename T>
T addOne(T x) {
    return x + 1;
}
 
int main(){
    std::cout << addOne(1) << '\n';
    
    std::string hello { "Hello, world!" };
    std::cout << addOne(hello) << '\n';
    return 0;
}
```

经编译器处理后等效于:

```c++
#include <iostream>

template <typename T>
T addOne(T x);

template<>
int addOne<int>(int x) {
    return x+1;
}

template<>
std::string addOne<std::string>(std::string x){
    return x + 1;
}

int main(){
    std::cout << addOne(1) << '\n';   // calls addOne<int>(int)
    std::cout << addOne(2.3) << '\n'; // calls addOne<double>(double)
    return 0;
}
```

报错，operator+ 无法匹配string与int相加，未实现该操作符重载。

#### 小结:

有模板类型可以被任何类型替换，故模板类型也叫泛型类型。使用模板编程就叫泛型编程。泛型编程可以让我们更专注于算法的逻辑信息和数据结构设计，而不用太关心类型信息。
对于模板的一些缺点:
编译器会为每一个具有唯一参数类型的函数调用创建一个函数，虽然写模板的时候代码很紧凑，但是编译时会扩展成大量代码，进而导致代码膨胀和编译时间变慢。
最令人抓狂的是它往往会产生一大堆可读性差的错误信息。

使用建议:
先创建普通函数，如果发现需要为不同函数重载时，再将其转换为函数模板。

### 8.15 —  具有多模板类型的函数模板

回顾一下之前的比较两数较大值的代码，加入一行

```c++
#include <iostream>
 
template <typename T>
T max(T x, T y){
    return (x > y) ? x : y;
}
 
int main(){
    std::cout << max(2, 3.5) << '\n';  // compile error
    return 0;
}
```

```shell
compare-1.cpp: In function ‘int main()’:
compare-1.cpp:9:28: error: no matching function for call to ‘max(int, double)’
    9 |     std::cout << max(2, 3.5) << '\n';
      |                            ^
compare-1.cpp:4:3: note: candidate: ‘template<class T> T max(T, T)’
    4 | T max(T x, T y) {
      |   ^~~
compare-1.cpp:4:3: note:   template argument deduction/substitution failed:
compare-1.cpp:9:28: note:   deduced conflicting types for parameter ‘T’ (‘int’ and ‘double’)
    9 |     std::cout << max(2, 3.5) << '\n';
      |                            ^

```

提示找不到匹配的 函数调用，首先不存在非模板匹配，对于函数模板，编译器推导出了max(int, double)的类型参数，但是T只能表示一种类型，所以提示找不到模板匹配。
看看如何解决，

1. 使用 static_cast 将参数转换为匹配类型

   ```c++
   #include <iostream>
    
   template <typename T>
   T max(T x, T y){
       return (x > y) ? x : y;
   }
    
   int main(){
       std::cout << max(static_cast<double>(2), 3.5) << '\n'; 
       // convert our int to a double so we can call max(double, double)
       return 0;
   }
   ```

2. 创建具有具体类型的函数

   ```c++
   #include <iostream>
    
   double max(double x, double y){
       return (x > y) ? x : y;
   }
    
   int main(){
       std::cout << max(2, 3.5) << '\n'; 
       // the int argument will be converted to a double
       return 0;
   }
   ```

3. 指定模板参数类型，避免类型推导

   ```c++
   #include <iostream>
    
   template <typename T>
   T max(T x, T y){
       return (x > y) ? x : y;
   }
    
   int main()
   {
       std::cout << max<double>(2, 3.5) << '\n'; 
       // we've provided actual type double, so the compiler won't use template argument deduction
       return 0;
   }
   ```

4. 多模板类型参数

   ```c++
   #include <iostream>
    
   template <typename T, typename U> // use two template type parameters named T and U
   T max(T x, U y){
       return (x > y) ? x : y;  // 此处会发生类型窄缩转换
   }
    
   int main()
   {
       std::cout << max(2, 3.5) << '\n';
       return 0;
   }
   ```

   编译器实例化出 ``max<int, double>(int, double)``,编译通过。
   但是还有一个问题，经过条件运算符将返回一个double，但是模板定义返回T(T会被解析为int)，所以double返回时会窄缩为int，可能会丢失数据。无论返回值类型为T还是U都不能解决问题。
   是否可以让编译器从return语句中推断出返回类型 --- 使用 auto

   ```c++
   template <typename T, typename U> // use two template type parameters named T and U
   auto max(T x, U y){
       return (x > y) ? x : y;  // 此处会发生类型窄缩转换
   }
    
   int main()
   {
       std::cout << max(2, 3.5) << '\n';
       return 0;
   }
   ```

### 19.1 — 类模板

回顾16.1中的整型数组容器类，使用模板使数组能存放任意类型

#### 示例

```c++
#ifndef ARRAY_H
#define ARRAY_H
 
#include <cassert>
 
template <class T>
class Array{
private:
    int m_length{};
    T *m_data{};
 
public:
    Array(int length){
        assert(length > 0);
        m_data = new T[length]{};
        m_length = length;
    }
 
    Array(const Array&) = delete;
    Array& operator=(const Array&) = delete;
 
    ~Array(){
        delete[] m_data;
    }
 
    void erase(){
        delete[] m_data;
        // We need to make sure we set m_data to 0 here, otherwise it will
        // be left pointing at deallocated memory!
        m_data = nullptr;
        m_length = 0;
    }
 
    T& operator[](int index) {
        assert(index >= 0 && index < m_length);
        return m_data[index];
    }
 
    // templated getLength() function defined below
    int getLength() const; 
};
 
// member functions defined outside the class need their own template declaration
template <class T>
int Array<T>::getLength() const // note class name is Array<T>, not Array
{
  return m_length;
}
 
#endif
```

测试:

```c++
#include <iostream>
#include "array.h"

int main() {
    Array<int> intArray(10);
    Array<double> doubleArray(10);
    for(int count = 0; count < intArray.getLength(); ++count) {
        intArray[count] = count;
        doubleArray[count]=count+0.5;
    }
    for (int count=intArray.getLength() - 1 ; count >= 0; --count)
		std::cout << intArray[count] << '\t' << doubleArray[count] << '\n';
    return 0;
}
```

#### 模板类分离

对于非模板类，一般将类定义放在头文件中，将成员函数定义放在类似命名的代码源文件中。编译的的时候，类的源文件就会被编译为一个单独的文件。对于类模板却不可以这么写，改写一下上面的例子:
``Array.h``

```c++
#ifndef ARRAY_H
#define ARRAY_H
 
template <class T>
class Array{
public:
    ...
    int getLength() const; 
};
 
#endif
```

``Array.cpp``

```c++
#include "Array.h"
 
template <class T>
int Array<T>::getLength() const { // note class name is Array<T>, not Array
  return m_length;
}
```

再次测试编译后，出现编译错误：

```shell
main.cpp:(.text+0x4d): undefined reference to `Array<int>::getLength() const'
/usr/bin/ld: main.cpp:(.text+0xab): undefined reference to `Array<int>::getLength() const'
```

提示``Array<int>::getLength() const``未定义，为什么会未定义?我们来分析一下:
首先C++是单独编译文件的，当main.cpp中包含头文件Array.h时，模板类的定义就被复制到了main.cpp中。当编译器发现我们需要Array<int>和Array<double>两个模板实例，就会实例化出两个模板类，并将其编译为main.cpp的一部分。在单独编译Array.cpp时，同样包含Array.h, 但是在Array.cpp中并不会实例化出具体类型的类，它也不知道我们需要Array<int>和Array<double>,因此该模板函数永远不会被实例化。所以，在链接的时候，提示找不到``Array<int>::getLength() const``的定义。
如何解决:

1. 最简单也是比较通用的就是将所有模板类的代码都放在头文件中，当然这样做也有个缺点，当在多处包含该头文件时，最后会得到该模板类的多个本地副本，然后增加了编译和链接的时间。
2. 三文件方法，将模板类定义在头文件中，类模板成员函数定义在源文件，然后添加第三个文件，在其中包含所需要的所有实例化类。详见:``https://www.learncpp.com/cpp-tutorial/template-classes/``

### 19.2 — 模板非类型参数

在前面的内容中，我们学习了如何使用模板类型参数来创建类型无关的函数和类。  模板类型参数是一种占位符类型，用于替代作为参数传入的类型。但是，模板类型参数并不是唯一可用的模板参数类型。  模板类和函数可以使用另一种称为非类型参数的模板参数。

#### 非类型模板参数

非类型模板参数可以是下列类型:

- 整型
- 枚举类型
- 指向类对象的指针或者引用
- 函数指针或函数引用
- 类成员函数指针或引用
- 空指针 std::nullprr
- 浮点类型(C++20)

看一个例子，我们创建一个Static数组类，类型参数控制静态数组的数据类型，整型非类型参数控制静态数组的大小。

```c++
#include <iostream>

template <typename T, int size>
class StaticArray{
    private:
    T mArray[size] {};
    public:
    T* getArray();
	//重载[]运算符
    T& operator[](int index){
        return mArray[index];
    }
};
//类模板成员函数类外定义
template<typename T, int size>
T* StaticArray<T, size>::getArray() {
    return mArray;
}

int main() {
    StaticArray<int, 5> intArray;
    for(int count{0}; count < 5; ++count) {
        intArray[count]  = count;
        std::cout << intArray[count] << std::endl;
    }

    StaticArray<double, 5> doubleArray;
    for(int count{0}; count < 5; ++count) {
        doubleArray[count] = 0.1*count + 2.2;
        std::cout << doubleArray[count] << std::endl;
    }
}
```

由于实例化时由常量size初始化数组，所以不必动态分配，由constexpr (非类型模板参数)静态分配

### 19.3 函数模板特化



可以看到上面模板类的定义

## 四、推荐

1. Learn C++  https://www.learncpp.com/
2.  http://bajamircea.github.io
3. https://stackoverflow.com/tags