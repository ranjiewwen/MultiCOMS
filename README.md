## MultiCOMS

**整个软件包含3个主要模块：数据部分、界面显示、相机&显示控制**
- 数据部分：包括网络数据提取，数据处理，数据保存    
 > 通过发指令，硬件开始发送数据，PC端通过UDP接受数据，放入第一级输出缓冲区（process()中完成）；将第一级输出缓冲区和第二级输入缓冲区连接在一起，第二级使用双缓冲，将缓冲区0整帧的数据拷贝到缓冲区1中（process()中完成）；最后通过接口copyArea（）提供给界面显示。对于图像数据保存，彩色图像需要将8位bayer格式数据转为24位RGB数据保存为BMP格式，对于灰度图像，直接保存8位bayer格式数据为BMP格式。
   
   - 界面显示：包括原始采集图像显示，菜单控制相机参数，界面切换   
   >窗口有数据，转换，文件等保护成员，及各种事件处理。菜单栏完成各种控制指令发送，实现对相机和界面的控制，界面切换通过窗口重排实现。
   
   - 控制模块：提供控制接口调整相机参数

### 整个程序流程： 
   - 为每路相机初始化数据处理链路，初始化缓冲区；初始化每路相机数据输入模块(网络，文件)，其输出和数据处理的初级输入端连接起来；为每路相机创建显示界面；为每个界面窗口设置数据，转换，文件；启动所有数据相关的处理流程。主窗口完成初始化，等待窗口事件。

### 现已完成的功能：
  - UDP接受数据，实现界面显示4通道图像；

   - 实现自动曝光，手动曝光，设置数字增益，曝光时间，AG_CG相机参数；
   
   - 实现保存当前4通道图像为bmp格式；
   
### 还需改进的地方： 
   - 如果需要按90、180、270度旋转图像，需增加第三级数据处理单元；
   
   - 对影响图像分辨率的，可以通过一些方法改进图像质量；
   
   - 对相机控制部分还需要改进和完善；
   
   - 界面还需优化；
   
   - 对其他3通道图像质量成像需改进；

- 当前界面实现
![image](https://github.com/ranjiewwen/MultiCOMS/blob/master/%E6%9C%80%E6%96%B0%E7%95%8C%E9%9D%A2.png)

## 附件1：界面显示
 ![image](https://github.com/ranjiewwen/MultiCOMS/blob/master/界面1.png)
 ![image](https://github.com/ranjiewwen/MultiCOMS/blob/master/界面2.png)
## 附件2：保存图像bmp
   ![image](https://github.com/ranjiewwen/MultiCOMS/blob/master/save.bmp)
   ![image](https://github.com/ranjiewwen/MultiCOMS/blob/master/saveToRGB.bmp)
   ![image](https://github.com/ranjiewwen/MultiCOMS/blob/master/other.bmp)
        
##  2015.10--2016.3多CMOS相机上位机——快视系统
- 项目描述：便携式文物鉴定相机

> 
1. 硬件FPGA进行CMOS图像采集，通过网络传输数据，然后在PC上实现4通道CMOS图像显示；
2. 上位机可以通过界面操作完成对相机的控制。

- 项目职责：

> 
1. 制定硬件和PC之间的通信协议；
2. 运用C++编程，通过UDP接收网络数据并提取完整数据帧；
3. 运用Qt编程，使用MVC框架完成图像数据显示，并且完成PC对硬件的控制功能。
  
