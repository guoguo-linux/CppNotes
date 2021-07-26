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

![framework(1)](/home/tianyu/Downloads/framework(1).png)





###  end 





## 二、C++核心准则

- 坚持使用const（检查成员函数是否修改对象；检查函数是否修改通过指针或引用传递的参数）[P.1]()

- 编译时检查优于执行时检查，无法在编译时检查的，应可以在执行时检查 [P.5-6]()

- 使用标准库优化，封装**混乱**、**低层次**、**冗长**的代码(如涉及 复杂指针、自己管理生命周期、类型转换等) [P.11]()

- 避免非常量的全局变量

  > 如果想使用全局数据(命名空间)