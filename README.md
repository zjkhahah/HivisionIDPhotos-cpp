<div align="center">
<h1>HivisionlDPhoto-cpp</h1>
​	





[English](README-EN.md) / 中文

[![release](https://img.shields.io/badge/release-black)](https://github.com/zjkhahah/HivisionIDPhotos-cpp/releases/tag/file)
[![issue](https://img.shields.io/badge/issue-black)](https://github.com/zjkhahah/HivisionIDPhotos-cpp/issues)
[![stars](https://img.shields.io/badge/stars-green)](https://github.com/zjkhahah/HivisionIDPhotos-cpp/stargazers)
[![forks](https://img.shields.io/badge/forks-blue)](https://github.com/zjkhahah/HivisionIDPhotos-cpp/forks)




 </div>




# 目录


- [项目简介](#项目简介)
- [准备工作](#准备工作)
- [权重文件下载](#权重文件下载)
- [编译源码](#源码编译)
- [如何使用](#使用)
- [引用项目](#引用项目)
- [联系我们](#联系我们)

<br>





# 项目简介

​	**HivisionIDPhoto 旨在开发一种实用、系统性的证件照智能制作算法。HivisionIDPhoto_cpp是对HivisionIDPhoto C++的重构，目的利用边缘设备的计算资源，以实现嵌入式设备和手机端本地部署。**

**HivisionIDPhoto_cpp可以做到。**

​	**1.离线手机端部署。arm设备部署运行。**

​	**2.轻量级抠图**

​	**3.根据不同尺寸规格生成不同的标准证件照**

​	**4.六寸排版照**

​	**5.美颜**

​	**6.安卓apk（waitting）**

<br>





# 准备工作


windows版和Linux版程序压缩放在[release](https://github.com/zjkhahah/HivisionIDPhotos_cpp/releases/tag/file)中，解压后需将执行文件和依赖环境放入HivisionIDPhotos_cpp根目录下，权重文件放入model文件夹下。

<br>





# 权重文件下载

存到项目的`model`目录下

modnet_photographic_portrait_matting.mnn [下载](https://github.com/zjkhahah/HivisionIDPhotos-cpp/releases/tag/v1.0/modnet_photographic_portrait_matting.mnn)

hivision_modnet.mnn [下载](https://github.com/zjkhahah/HivisionIDPhotos-cpp/releases/tag/v1.0/mnn_hivision_modnet.mnn)

symbol_10_320_20L_5scales_v2_deploy.mnn[下载](https://github.com/zjkhahah/HivisionIDPhotos-cpp/releases/tag/v1.0/symbol_10_320_20L_5scales_v2_deploy.mnn)

symbol_10_320_20L_8scales_v2_deploy.mnn[下载](https://github.com/zjkhahah/HivisionIDPhotos-cpp/releases/tag/v1.0/symbol_10_320_20L_8scales_v2_deploy.mnn)

<br>





# 源码编译
## 	**1. 克隆项目**

```
https://github.com/zjkhahah/HivisionIDPhotos_cpp.git
cd  HivisionIDPhotos_cpp
```



## 	2.编译平台

​		**其中MNN版本为2.9.0**

​		**opencv版本为4.7.0**

### **1.windows**

- 环境要求

​		Microsoft Visual Studio >= 2017

​		cmake >= 3.13

​		powershell

​		Ninja

编译MNN库并把编译后的.a静态库文件放入lib文件夹下

编译MNN windows版教程为[主库编译 — MNN-Doc 2.1.1 documentation](https://mnn-docs.readthedocs.io/en/latest/compile/engine.html)

编译opencv windows版.a静态库文件放入lib文件夹下

- 编译代码

​	在设置中找到vcvars64.bat（适用于 VS 2017 的 x64 本机工具命令提示）并单击，打开VS编译x64架构程序的虚拟环境

```
cd  HivisionIDPhotos_cpp
mkdir build && cd build
cmake .. -G Ninja  #命令行编译cmake .. -G Ninja    api编译 cmake .. -G Ninja -DCOMPILE_LIBRARY=ON
ninja
```

### 	**2.arm64编译**

​	以Linaro工具链为例。首先从[Linaro](https://releases.linaro.org/components/toolchain/binaries/latest-7/)网页中按照宿主机以及交叉编译目标设备来选择合适的工具链。这里我们以`arm-linux-gnueabi`为例，点击网页上的链接，进入[arm-linux-gnueabi](https://releases.linaro.org/components/toolchain/binaries/latest-7/arm-linux-gnueabi/)页面。 按照宿主机类型(这里以X64 Linux为例)选择下载链接, 文件名形如 gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabi.tar.xz 下载后解压到任意目录

​	编译arm64版本的opencv和mnn

​	编译MNN ARM版教程为[点击这里](https://mnn-docs.readthedocs.io/en/latest/compile/engine.html)

编译好的静态opencv文件和mnn文件放入lib文件夹下

```
cd  HivisionIDPhotos_cpp
mkdir build && cd build
cmake .. \
-DCMAKE_SYSTEM_NAME=宿主系统，例如Linux \
-DCMAKE_SYSTEM_VERSION=1 \
-DCMAKE_SYSTEM_PROCESSOR=交叉编译目标处理器的信息。例如aarch64 \
-DCMAKE_C_COMPILER=交叉编译器中C编译器的路径 \
-DCMAKE_CXX_COMPILER=交叉编译器中C++编译器的路径
make -j8
```
### <br>	**3.安卓编译**API

1. 编译MNN 安卓版教程为[[主库编译 — MNN-Doc 2.1.1 documentation (mnn-docs.readthedocs.io)](https://mnn-docs.readthedocs.io/en/latest/compile/engine.html)]
2. 下载opencv安卓版本[链接](https://github.com/opencv/opencv/releases/download/4.7.0/opencv-4.7.0-android-sdk.zip)

编译好的静态opencv文件和mnn文件放入lib文件夹下

````
cd  HivisionIDPhotos_cpp
mkdir build && cd build

cmake ..     -DCMAKE_TOOLCHAIN_FILE=#CMake工具链文件的路径     -DCMAKE_ANDROID_NDK=#Android NDK（Native Development Kit）的安装路径    -DANDROID_PLATFORM=android-21     -DANDROID_NATIVE_API_LEVEL=21     -DANDROID_ABI=arm64-v8a     -DANDROID_NATIVE_API_LEVEL=21 -DCOMPILE_LIBRARY=ON -DCOMPILE_ANDROID=ON

make -j8



````







# 命令行使用

核心指令：
* `-e`: 人像在图片比例
* `-i`：输入照片路径
* `-o`：输出照片路径
* `-k`：输出照片大小
* `-f`：人脸模型选择
* ...

更多指令通过`./HivisionIDPhotos_cpp.exe --help`查看

##  1.打开程序
以windows系统为例，使用工具powershell跳转到根目录下,例： 

` cd D:\HivisionIDPhotos-cpp`


## 2.证件照制作
输入一张照片，输出一张标准证件照png 

`./HivisionIDPhotos_cpp.exe -i demo/images/test.jpg  -o 1  -r 255  -g 0  -b 0 -h 413 -w 295 `

## 3.人像抠图
输入 1 张照片，获得 1张 4 通道透明 png

## 4.生成一张六寸排版照
输入一张照片，得到一张六寸排版照 png 

`./HivisionIDPhotos_cpp.exe -i demo/images/test.jpg  -o 1  -r 255  -g 0  -b 0 -h 413 -w 295 -l 1`

<br>



# API使用

利用SpringBoot+JNA调用dll文件生成API（linux环境调用.so文件）
下面以HivisionIDphotos中的human_mating方法为例

## 1.创建SpringBoot项目并在pom.xml中添加依赖


```
		<dependency>
            <groupId>net.java.dev.jna</groupId>
            <artifactId>jna</artifactId>
            <version>5.12.1</version>
            <scope>compile</scope>
        </dependency>
        <dependency>
            <groupId>org.projectlombok</groupId>
            <artifactId>lombok</artifactId>
            <version>1.18.8</version>
        </dependency>
        <dependency>
            <groupId>com.alibaba</groupId>
            <artifactId>fastjson</artifactId>
            <version>1.2.28</version>
        </dependency>
        <dependency>
            <groupId>commons-beanutils</groupId>
            <artifactId>commons-beanutils</artifactId>
            <version>1.9.2</version>
        </dependency>
        <dependency>
            <groupId>commons-collections</groupId>
            <artifactId>commons-collections</artifactId>
            <version>3.2.1</version>
        </dependency>
        <dependency>
            <groupId>commons-lang</groupId>
            <artifactId>commons-lang</artifactId>
            <version>2.6</version>
        </dependency>
        <dependency>
            <groupId>commons-logging</groupId>
            <artifactId>commons-logging</artifactId>
            <version>1.1.1</version>
        </dependency>
        <dependency>
            <groupId>net.sf.ezmorph</groupId>
            <artifactId>ezmorph</artifactId>
            <version>1.0.6</version>
        </dependency>
```
## 2.放入dll/so文件

将需要调用的dll文件放在src\main\resources\win32-x86-64\中

## 3.构建需要传参的类

这里需要知道方法的具体传参，根据JNA映射规则进行编写，下面三个类根据HivisionIDphotos.dll内函数的传参结构体编写。
Hivision_java_params.java

```
public class Hivision_java_params extends Structure {
    public String model_path;
    public String image_path;
    public String out_path;
    public String face_model_path;
    public int rgb_r, rgb_g, rgb_b;
    public int thread_num, model_scale;
    public Params param = new Params();

    @Override
    protected List<String> getFieldOrder() {
        return Arrays.asList("model_path", "image_path", "out_path", "face_model_path",
                "rgb_r", "rgb_g", "rgb_b", "thread_num", "model_scale", "param");
    }
    public Hivision_java_params() {
        super();
        // 初始化默认值
        this.rgb_r = 255;
        this.rgb_g = 0;
        this.rgb_b = 0;
        this.thread_num = 4;
        this.model_scale = 8;
    }
    }
```

Params.java

```
public class Params extends Structure {
    public int out_image_width, out_image_height;
    public boolean change_bg_only;
    public float head_measure_ratio, head_height_ratio;
    public float[] head_top_range = new float[2];
    public int rgb_r, rgb_g, rgb_b;
    public FaceInfo face_info = new FaceInfo();

    @Override
    protected List<String> getFieldOrder() {
        return Arrays.asList("out_image_width", "out_image_height", "change_bg_only",
                "head_measure_ratio", "head_height_ratio", "head_top_range",
                "rgb_r", "rgb_g", "rgb_b", "face_info");
    }

    public Params() {
        super();
        // 初始化默认值
        out_image_width = 295;
        out_image_height = 413;
        change_bg_only = false;
        head_measure_ratio = 0.2f;
        head_height_ratio = 0.55f;
        head_top_range[0] = 0.12f;
        head_top_range[1] = 0.1f;
        rgb_r = 255;
        rgb_g = 0;
        rgb_b = 0;
    }}
```
FaceInfo.java
```
public class FaceInfo extends Structure {
    public float x1, y1, x2, y2, score, area;
    public float[] landmarks = new float[10];

    @Override
    protected List<String> getFieldOrder() {
        return Arrays.asList("x1", "y1", "x2", "y2", "score", "area", "landmarks");
    }

    public FaceInfo() {
        super();
        // 初始化数组
        Arrays.fill(landmarks, 0.0f);
    }}
```

## 4.编写对应的Library接口

一个Library接口对应一个dll，内抽象方法与dll中的函数一一对应。Native.loadLibrary()第一个参数为dll文件路径，不加.dll后缀。
HivisionIDphotosLibrary.java
```
public interface HivisionIDphotosLibrary extends Library {
    HivisionIDphotosLibrary INSTANCE = Native.loadLibrary("src\\main\\resources\\win32-x86-64\\HivisionIDphotos", HivisionIDphotosLibrary.class);
    void human_mating(Hivision_java_params hivision_java_params);
    int ID_photo(Hivision_java_params hivision_java_params,int out_size_kb,boolean layout_phot);}
```

## 5.编写Controller类

JnaDemoController.java
```
@RestController
@RequestMapping("/JNA")
public class JnaDemoController {
    @PostMapping("/human_mating")
    public String human_mating(@RequestParam("hivision_java_params_json") String hivision_java_params_json){
        try{
            Hivision_java_params hivision_java_params = JSONObject.parseObject(hivision_java_params_json, Hivision_java_params.class);
            HivisionIDphotosLibrary.INSTANCE.human_mating(hivision_java_params);
            return "success";
        }catch (Exception e){
            System.out.println(e.getMessage());
            return "fail";
        }
    }}

```
# 引用项目

1. [MNN](https://github.com/alibaba/MNN):

```
@software{Alibaba_MNN_2024,
    author = {Alibaba},
    title = {{MNN}},
    url = {https://github.com/alibaba/MNN},
    year = {2024},
    publisher = {GitHub}
}
```

2. [HivisionIDPhotos](https://github.com/Zeyi-Lin/HivisionIDPhotos)

```
@software{Zeyi-Lin_HivisionIDPhotos_2024,
    author = {Zeyi-Lin},
    title = {{HivisionIDPhotos}},
    url = {https://github.com/Zeyi-Lin/HivisionIDPhotos},
    year = {2024},
    publisher = {GitHub}
}
```

3. **[LFFD-MNN](https://github.com/SyGoing/LFFD-MNN)**

```

@software{LFFD-MNN_2024,
    author = {SyGoing},
    title = {{LFFD-MNN}},
    url = {https://github.com/SyGoing/LFFD-MNN},
    year = {2024},
    publisher = {GitHub}
}

```

<br>




#  联系我们

如果您有任何问题，请提[issue](https://github.com/zjkhahah/HivisionIDPhotos-cpp/issues)或发邮件至24021211792@stu.xidian.edu.cn
