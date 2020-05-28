# 项目前期准备

## 前期调研

mesh应用的使用场景：建模，套用数学方法

* 科学研究：进行仿真实验
* 工程设计：建立模型，进行数学运算

目标人群：有一定知识水平以及开发能力的用户

可以解决的问题：

* 结构分析
* 热传导
* 流体流动
* 质量传输
* 电磁势

## 项目愿景

提供开发mesh应用需要的最基本的接口，用户通过调用接口，可以开发出自己需要的mesh应用，用于完成特定实验。我们需要做的类似于设计一个函数库。

我们解决的问题：mesh应用开发的问题

受众：需要进行mesh应用开发的人

希望我们产品具有的特性：

* 使用简单：
* 意义明确：保证用户能清晰地表达出自己的设计需求
* 功能丰富：能够进行多种类别的实验

## 产品特性

1. 读取msh文件：支持文本模式*和二进制模式*
2. 写msh文件：支持分布式存储和连续存储
3. 遍历运算：支持顺序和并行（并行计算关系到划分的实现）
4. *可视化结果：构造出图像？（动态？色彩？）*

## 需求规格说明

### Usecase Diagram

![image-20200525142301001](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20200525142301001.png)

### use case

#### use case 1 编译用户应用代码

范围：最顶层的输入输出

级别：用户目标

主要参与者：用户

涉众及关注点：

* 用户：清晰地描述出自己的设计需求，高效地获得运算结果。使用场景要大，能够用来开发多种应用。
* msh文件：希望被正确读取，以及处理结束后正确生成。或许还需要生成对应的可视化结果。

前置条件：用户写出地程序语法正确

后置条件（成功保证）：按照需求生成新的msh文件，输出可视化的结果

主成功场景（基本流程）：

* 用户调用我们提供的接口，按照我们规定的语法，写出了一个语法正确的代码
* 将代码复制到我们提供的编译环境下，点击编译（一个.c文件，写好了相关makefile）
* 编译完成后，运行生成的可执行文件，输入要操作的文件的路径名
* 读出路径对应的文件，按照用户的代码逻辑进行处理
* 输出可视化的计算结果（数据/图片/文件）

拓展（替代流程）：

* 用户输入语法错误：系统编译器会自动报错
* 文件无法正常打开，写好相关的处理程序
* 文件处理出错，写好相关处理程序

#### use case 2 代码编写

![image-20200525210829445](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20200525210829445.png)

范围：用户使用

级别：程序运行

主要参与者：用户

涉众：

* 用户：需要能清晰地表达自己的需求

* 用户程序：高效，可以的话支持并行。稳定，对正常出现的报错有一定处理能力，不会导致程序崩溃。

前置条件：接口完备，且有提供清晰的使用说明

后置条件：无

主成功场景：

* 读取需要的文件内容
* 对mesh模型的各个元素进行相应的设置（声明一个相关数据结构，进行参数设置）
* 对文件对应的mesh模型进行partition
* 遍历element进行运算（按照需要确定遍历规则，可实现并行计算）
* 输出可视化结果（数据/图像/文件）

拓展：

* 读取文件出错，立即停止
* 无法指定partition：
  * 方法1：在其他mesh应用上划分好，记住对应编号，再来这边操作
  * 方法2：提供生成图像的函数，生成图像来观察

#### Domain Model

![image-20200525211124192](C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20200525211124192.png)

### 功能模型

#### 读文件

Mode：不同格式的文件，同种格式的也有二进制模式和文本模式

Path：文件路径

MeshStruct：存放Mesh文件的结构体

<img src="C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20200525204400488.png" alt="image-20200525204400488" style="zoom:50%;" />

#### 划分

Mode：模式，根据目前的了解，划分是有多种类型的

Num：划分成多少份

partition list：还未确定划分的具体实现，待讨论



<img src="C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20200525203058075.png" alt="image-20200525203058075" style="zoom:50%;" />

#### 参数设置

把参数单独放到一个结构体里，可使用性更强

ArguStruct：专门存放参数的结构体

Mode：返回的结构体可以有多种类型（点，线，面，……）

<img src="C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20200525204000419.png" alt="image-20200525204000419" style="zoom:50%;" />

#### 遍历

Express：为需要套用的公式，可以为他专门设计一个结构体，以及专门的公式识别程序

*ArguStruct：需要使用到的参数，可能每个元素有多个 

​	规则->string->Express->指导如何使用arguStruct里的数据  "a[n] = a1[n-1] + a1[n+1]"

Mode：指遍历模式

Result：根据不同模式，返回不同的结果

这个函数可以通过不同的需要重定义多个，返回不同类型的结果

<img src="C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20200525204715285.png" alt="image-20200525204715285" style="zoom:50%;" />

#### 写文件

<img src="C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20200525205011809.png" alt="image-20200525205011809" style="zoom:50%;" />

#### 生成可视化图像

<img src="C:\Users\Lenovo\AppData\Roaming\Typora\typora-user-images\image-20200525205122097.png" alt="image-20200525205122097" style="zoom:50%;" />

## 细节

关于Express类：

StringToStruct：把字符串描述的公式转换成Express

StructToString：与上一项相反

设计思想：为了方便用户输入，我们需要实现，用户只需要以字符串的形式交给我们公式，我们就可以把他应用到参数上，计算出用户需要的结果。因此用Express来专门执行公式解析的功能。