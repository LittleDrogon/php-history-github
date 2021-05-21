php-history


php-1：生成几个简单的cgi文件

php-2:
1、可以访问数据库mSQL，与httpd（早期版本apache）配合使用

php-3
1、重写了底层


php-4:
1、增加了Zend1引擎
2、加入了superglobal(超全局的概念，即$_GET、$_POST等)
3、引入了命令行界面CLI

php-5:
1、Zend2引擎
2、引入了编译器来提高性能、增加了PDO作为访问数据库的接口
3、默认启用过滤器扩展
4、支持命名空间;使用XMLReader和XMLWriter增强XML支持;支持SOAP ,延迟静态绑定，跳转标签（有限的goto）, 闭包，Native PHP archives。
4、支持Trait、简短数组表达式。移除了register_globals, safe_mode, allow_call_time_pass_reference, session_register(), session_unregister(), magic_quotes以及session_is_registered()。加入了内建的Web服务器。增强了性能，减小内存使用量。
5、支持generators，用于异常处理的finally ，将OpCache（基于 Zend Optimizer+）加入官方发布中。

php-7
1、Zend Engine 3 (性能提升并在Windows上支持 64-bit 整数)，统一的变量语法， 基于抽象语法树编译过程。
2、void返回值类型，类常量，可见性修饰符
3、PCRE2支持等
4、改进OpenSSL、弱引用等

php-8
JIT、数组负索引等
