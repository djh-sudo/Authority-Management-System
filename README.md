# -
## 权限管理模拟系统
* 运行环境 Visual Studio 2019
* md5 模块来自开源代码
* `QT`文件夹为界面版本
* `VS`文件夹为`Dos`窗体版本

## 结构
![](https://github.com/djh-sudo/Authority-Management-System/blob/main/src/1.png)
## 游客模式
|命令|含义|
|:-:|:-:|
|help|显示命令信息|
|log|登录|
|exit|退出程序|
|clc|清屏|
|out|退出登录|
## 游客状态
![](https://github.com/djh-sudo/Authority-Management-System/blob/main/src/guest.png)
## 游客登录,以`root`账户登录,此时再次使用命令`help`,可以看到，`root`的权限要比一般游客多很多。
![](https://github.com/djh-sudo/Authority-Management-System/blob/main/src/root.png)
|命令|含义|
|:-:|:-:|
|show -x|查询自己对某个对象的权限|
|show obj|查询所有对象的对象名以及id|
|grant auth -X of -O to -U|将对象 O 的 X 权限赋予给用户 U|
|grant auth -X of -O to -U with option|将对象 O 的 X 权限赋予给用户 U，允许转授|
|revoke auth -X of -O from -U|将对象 O 的 X 权限从用户 U 回收|
|chain -X of -O|查询对象O的X权限链|
|show auth of obj -O|查询对象 O 的授权情况|
|show auth of user -U|查询用户 U 的授权情况|
|revoke all -X of -O|回收对象 O 的 X 权限|

## 查询某个对象权限并授权
![](https://github.com/djh-sudo/Authority-Management-System/blob/main/src/test.png)
## GUI查看结果
![](https://github.com/djh-sudo/Authority-Management-System/blob/main/src/test2.png)
可以看到`pwn`用户的对对象`obj3`的`read`权限拥有的，但不能转授

## 在GUI交互界面批量修改权限
![](https://github.com/djh-sudo/Authority-Management-System/blob/main/src/test3.png)
这里将四种权限对`obj1`的权限均给了用户`djh`

## 命令行种同样生效
![](https://github.com/djh-sudo/Authority-Management-System/blob/main/src/tset4.png)

# Thanks for watching ...
