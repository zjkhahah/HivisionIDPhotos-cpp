<div align="center">
<h1>HivisionlDPhoto-cpp</h1>

​	

[![release](https://img.shields.io/badge/release-black)](https://github.com/zjkhahah/HivisionIDPhotos-cpp/releases/tag/file)
[![issue](https://img.shields.io/badge/issue-black)](https://github.com/zjkhahah/HivisionIDPhotos-cpp/issues)
[![stars](https://img.shields.io/badge/stars-green)](https://github.com/zjkhahah/HivisionIDPhotos-cpp/stargazers)
[![forks](https://img.shields.io/badge/forks-blue)](https://github.com/zjkhahah/HivisionIDPhotos-cpp/forks)

English / [中文](README.md) 


 </div>




# 目录


- [Project Introduction](# Project-Introduction)
- [preparation](#preparation)
- [Weight file download](#Weight-file-download)
- [Compile source code](#Compile-source-code)
- [How to use](#How-to-use)
- [Reference project](#Reference-project)
- [Contact Us](#Contact-Us)

<br>





# Project Introduction

​	**HivisionIDPhoto aims to develop a practical and systematic algorithm for intelligent production of ID photos. HivisionIDPhoto_cpp is a refactoring of HivisionIDPhoto C++, aimed at utilizing the computing resources of edge devices to achieve local deployment on embedded devices and mobile devices。**

**HivisionIDPhoto_cpp can achieve it。**

​	**1.Offline mobile deployment. Deploy and run ARM devices。**

​	**2.Lightweight cutout**

​	**3.Generate different standard ID photos based on different size specifications**

​	**4.Six inch layout photo**

​	**5.Beauty Filter**

​	**6.andriod apk（waitting）**

<br>





# preparation


The compressed programs for Windows and Linux versions are available in [release](https://github.com/zjkhahah/HivisionIDPhotos_cpp/releases/tag/file )After decompression, the execution file and dependent environment need to be placed in the root directory of HivisinIDPhotos_cpp, and the weight files need to be placed in the model folder

<br>





# Weight file download

Save to th`model`directory of the project

modnet_photographic_portrait_matting.mnn [download](https://github.com/zjkhahah/HivisionIDPhotos-cpp/releases/tag/v1.0/modnet_photographic_portrait_matting.mnn)

hivision_modnet.mnn [download](https://github.com/zjkhahah/HivisionIDPhotos-cpp/releases/tag/v1.0/mnn_hivision_modnet.mnn)

symbol_10_320_20L_5scales_v2_deploy.mnn[download](https://github.com/zjkhahah/HivisionIDPhotos-cpp/releases/tag/v1.0/symbol_10_320_20L_5scales_v2_deploy.mnn)

symbol_10_320_20L_8scales_v2_deploy.mnn[download](https://github.com/zjkhahah/HivisionIDPhotos-cpp/releases/tag/v1.0/symbol_10_320_20L_8scales_v2_deploy.mnn)

<br>





# Compile source code
## 	**1. clone**

```
https://github.com/zjkhahah/HivisionIDPhotos_cpp.git
cd  HivisionIDPhotos_cpp
```



## 	2.Compile Platform

​		**其中MNN版本为2.9.0**

​		**opencv版本为4.7.0**

### **1.windows**

- Environmental requirements

​		Microsoft Visual Studio >= 2017

​		cmake >= 3.13

​		powershell

​		Ninja

Compile the MNN library and place the compiled. a static library file in the lib folder

Compile MNN Windows tutorial as[主库编译 — MNN-Doc 2.1.1 documentation]([Windows (yuque.com)](https://www.yuque.com/mnn/en/build_windows))

- Compile OpenCV Windows version. a static library files and place them in the lib folder编译代码

Find vcvars64.bat (x64 native tool command prompt for VS 2017) in the settings and click to open the virtual environment for compiling x64 architecture programs in VS

```
cd  HivisionIDPhotos_cpp
mkdir build && cd build
cmake .. -G Ninja  #Command line builds cmake .. -G Ninja    apibuilds cmake .. -G Ninja -DCOMPILE_LIBRARY=ON
ninja
```

### 	**2.arm64**

Taking Linaro toolchain as an example. Firstly, starting from [Linaro](https://releases.linaro.org/components/toolchain/binaries/latest-7/)Select the appropriate toolchain based on the host and cross compilation target device in the webpage。 gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabi.tar.xz Download and unzip to any directory

​	Compile the arm64 version of OpenCV and MNN

​	Compile MNN ARM version tutorial as[here](https://mnn-docs.readthedocs.io/en/latest/compile/engine.html) 

Put the compiled static OpenCV and MNN files into the lib folder

```
cd  HivisionIDPhotos_cpp
mkdir build && cd build
cmake .. \
-DCMAKE_SYSTEM_NAME=Host system, such as Linux \
-DCMAKE_SYSTEM_VERSION=1 \
-DCMAKE_SYSTEM_PROCESSOR=Cross compile the information of the target processor. For example, aarch64 \
-DCMAKE_C_COMPILER=Path of C compiler in cross compiler \
-DCMAKE_CXX_COMPILER=Path of C++compiler in cross compiler
make -j8
```
### <br>	**3.Android Compile API**

1. Compile MNN Android tutorial as[Android (yuque.com)](https://www.yuque.com/mnn/en/build_android)
2. Download OpenCV Android version[download](https://github.com/opencv/opencv/releases/download/4.7.0/opencv-4.7.0-android-sdk.zip)

Put the compiled static OpenCV and MNN files into the lib folder

````
cd  HivisionIDPhotos_cpp
mkdir build && cd build

cmake ..     -DCMAKE_TOOLCHAIN_FILE=#CMakePath to toolchain file     -DCMAKE_ANDROID_NDK=#Android NDK（Native Development Kit）installation path    -DANDROID_PLATFORM=android-21     -DANDROID_NATIVE_API_LEVEL=21     -DANDROID_ABI=arm64-v8a     -DANDROID_NATIVE_API_LEVEL=21 -DCOMPILE_LIBRARY=ON -DCOMPILE_ANDROID=ON

make -j8



````







# How to use

Core instructions：
* `-i`：Enter photo path
* `-o`：Output photo path
* `-k`：Output photo size
* `-f`：Facial model selection
* ...

More instructions can be obtained through ` HivisionIDPhotos_cpp.exe -- help ` view

##  1.open the program
Taking the Windows system as an example, use the tool PowerShell to jump to the root directory： 

` cd D:\HivisionIDPhotos-cpp`


## 2.ID photo production
Input a photo and output a standard ID photo png 

`./HivisionIDPhotos_cpp.exe -i demo/images/test.jpg  -o 1  -r 255  -g 0  -b 0 -h 413 -w 295 `

## 3.Portrait cutout
Enter 1 photo to obtain 1 4-channel transparent image png

## 4.Generate a six inch layout photo
Input a photo to obtain a six inch layout photo png 

`./HivisionIDPhotos_cpp.exe -i demo/images/test.jpg  -o 1  -r 255  -g 0  -b 0 -h 413 -w 295 -l 1`

<br>



# API using

Using SpringBoot+JNA to call DLL file generation API (Linux environment calls. so file)
Taking the human_mating method in HivisionIDphotos as an example

## 1.Create a SpringBoot project and add dependencies in pom.xml


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
## 2.Put into dll/so file

Place the DLL file that needs to be called in src \ main \ resources \ win32-x86-64 \

## 3.Build classes that require parameter passing

We need to know the specific parameter passing of the method here, and write it according to the JNA mapping rules. The following three classes are written based on the parameter passing structure of the function in HivisionIDphotos.dll.
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

## 4.Write the corresponding Library interface

A Library interface corresponds to a DLL, and the internal abstract methods correspond one-to-one with the functions in the DLL. The first parameter of Native.loadLibrary() is the DLL file path without the. dll suffix.
HivisionIDphotosLibrary.java

```
public interface HivisionIDphotosLibrary extends Library {
    HivisionIDphotosLibrary INSTANCE = Native.loadLibrary("src\\main\\resources\\win32-x86-64\\HivisionIDphotos", HivisionIDphotosLibrary.class);
    void human_mating(Hivision_java_params hivision_java_params);
    int ID_photo(Hivision_java_params hivision_java_params,int out_size_kb,boolean layout_phot);}
```

## 5.Write Controller class

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




#  Contact Us

If you have any questions, please raise [issue]( https://github.com/zjkhahah/HivisionIDPhotos-cpp/issues)Or send an email to 24021211792@stu.xidian.edu.cn
