#  【NO.156】 设计模式之工厂设计模式

# 1.开发环境

- IDEA版本： 2022.1.4
- JDK版本：17.0.3

 

# 2.模式由来

## 2.1 自定义MailSender类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102526533-1287688327.png)

## 2.2 自定义Computer类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102544456-933689781.png)

## 2.3 分析图

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213103216889-954434698.png)

## 2.4 案例分析

- 由于Computer类和MailSender类之间的耦合度比较高，而且创建对象的代码太固定了，若希望使用短信发送的功能需要修改Computer类，这种修改违背了开闭原则，为了解决该问题，就可以将创建对象的工作移交出去，而工厂设计模式就是一种创建对象的设计模式，而且可以在创建对象时不对外暴露具体的创建逻辑。

 

# 3 普通工厂方法模式

## 3.1 基本概念

- 普通工厂方法模式就是建立一个工厂类，通过生产方法的参数来进行具体实例的创建。

## 3.2 自定义Sender接口

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102635389-1815819930.png)

## 3.3 修改MailSender类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102649414-57587530.png)

## 3.4 自定义SmsSender类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102719414-1997499446.png)

## 3.5 自定义SendFactory类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102732353-849206262.png)

## 3.6 修改Computer类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102746458-1732378018.png)

## 3.7 分析图

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213103217100-2038713447.png)

## 3.8 案例分析

- 在普通工厂方法模式中，若传递的字符串出错，则不能正确创建对象，而且可能出现空指针异常，所以容错率不高。

 

# 4.多个工厂方法模式

## 4.1 基本概念

- 多个工厂方法模式就是通过多个不同的生产方法对实现同一接口的不同实现类分别进行对象的创建。

## 4.2 修改SendFactory类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102823586-931852715.png)

## 4.3 修改Computer类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102836547-837935901.png)

## 4.4 分析图

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213103217099-1967070422.png)

## 4.5 案例分析

- 在多个工厂方法模式中，为了能够正确创建对象，需要先创建工厂类的对象才能调用工厂类中的生产方法。

 

# 5.静态工厂方法模式

## 5.1 基本概念

- 静态工厂方法模式就是将多个不同的生产方法加上static关键字提升为类层级，此时只需要通过类名.的方式就可以进行方法的调用，从而进行不同实例的创建。

## 5.2 修改SendFactory类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102912371-2099806645.png)

## 5.3 修改Computer类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102931067-656536113.png)

## 5.4 分析图

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213103217104-1842082073.png)

## 5.5 案例分析

- 静态工厂方法模式中类的创建依赖工厂类，如果想要扩展程序来生产新的产品，就必须对工厂类的代码进行修改，这就违背了开闭原则。

 

# 6.抽象工厂模式

## 6.1 基本概念

- 抽象工厂模式就是将多个不同的生产方法放在不同的工厂类中，让多个工厂类实现同一个接口，此时只需要通过不同的工厂类就可以进行不同实例的创建。

## 6.2 自定义SendFactory接口

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213102956517-1621069227.png)

## 6.3 自定义MailSendFactory类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213103008325-189179913.png)

## 6.4 自定义SmsSendFactory类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213103019013-41445781.png)

## 6.5 修改Computer类

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213103030995-591961697.png)

## 6.6 分析图

![img](https://img2023.cnblogs.com/blog/3038036/202212/3038036-20221213103217101-1976135503.png)

## 6.7 案例分析

- 现在想要拓展程序生产新的产品，就只需要增加新的工厂类即可，不用违背开闭原则，同时每个工厂类对应一个产品，符合单一职责的原则。

原文作者：[格子衫007](https://home.cnblogs.com/u/gezishan007/)

原文链接：https://www.cnblogs.com/gezishan007/p/16977899.html