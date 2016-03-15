# MultiCOMS

整个软件包含3个主要模块：数据部分、界面显示、相机&显示控制。

   （1）数据部分：包括网络数据提取，数据处理，数据保存
通过发指令，硬件开始发送数据，PC端通过UDP接受数据，放入第一级输出缓冲区（process()中完成）；将第一级输出缓冲区和第二级输入缓冲区连接在一起，第二级使用双缓冲，将缓冲区0整帧的数据拷贝到缓冲区1中（process()中完成）；最后通过接口copyArea（）提供给界面显示。
对于图像数据保存，彩色图像需要将8位bayer格式数据转为24位RGB数据保存为BMP格式，对于灰度图像，直接保存8位bayer格式数据为BMP格式。
   （2）界面显示：包括原始采集图像显示，菜单控制相机参数，界面切换
    窗口有数据，转换，文件等保护成员，及各种事件处理。菜单栏完成各种控制指令发送，实现对相机和界面的控制，界面切换通过窗口重排实现。
   （3）控制模块：提供控制接口调整相机参数

整个程序流程：
    为每路相机初始化数据处理链路，初始化缓冲区；初始化每路相机数据输入模块(网络，文件)，其输出和数据处理的初级输入端连接起来；为每路相机创建显示界面；为每个界面窗口设置数据，转换，文件；启动所有数据相关的处理流程。主窗口完成初始化，等待窗口事件。

现已完成的功能：
   （1）UDP接受数据，实现界面显示4通道图像；
   （2）实现自动曝光，手动曝光，设置数字增益，曝光时间，AG_CG相机参数；
   （3）实现保存当前4通道图像为bmp格式；
还需改进的地方：
   （1）如果需要按90、180、270度旋转图像，需增加第三级数据处理单元；
   （2）对影响图像分辨率的，可以通过一些方法改进图像质量；
   （3）对相机控制部分还需要改进和完善；
   （4）界面还需优化；
   （5）对其他3通道图像质量成像需改进；

附件1：界面显示
 ![image](https://github.com/ranjiewwen/MultiCOMS/blob/master/界面1.png)
 ![image](https://github.com/ranjiewwen/MultiCOMS/blob/master/界面2.png)
附件2：保存图像bmp
 ![image](https://github.com/ranjiewwen/MultiCOMS/blob/master/save.bmp)
  ![image](https://github.com/ranjiewwen/MultiCOMS/blob/master/saveToRGB.bmp)
   ![image](https://github.com/ranjiewwen/MultiCOMS/blob/master/other.bmp)
        
      
