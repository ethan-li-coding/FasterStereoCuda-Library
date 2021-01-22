# FasterStereoCuda-Library

# 简介

这是一个基于CUDA加速的快速立体匹配库，它的核心是SemiglobalMatching（SGM）算法，它不仅在时间效率上要远远优于基于CPU的常规SGM，而且占用明显更少的内存，这意味着它不仅可以在较低分辨率（百万级）图像上达到实时的帧率，且完全具备处理千万级甚至更高量级图像的能力。

你可以拉取本测试工程并在自己的数据上体验它的改进效果，也可以在右侧下载已经打包好的压缩包，直接在本地运行控制台程序，或者在你的工程里通过动态库的方式调用它。

# 环境

Windows 10<br>
Visual Studio 2019<br>
CUDA v11.0<br>
Opencv3.2(下载地址：[https://download.csdn.net/download/rs_lys/13193887](https://download.csdn.net/download/rs_lys/13193887))

# 控制台调用方式

>../FasterStereoConsole.exe ../Data/Cone/left.png Data/Cone/right.png ../Data/Cone/option.xml<br>

>把../换成你的路径。option.xml是算法参数文件，在Data/文件夹中，附有两类参数文件option.xml和option2.xml，分别对应视差空间和深度空间的参数，二者用其一即可。不同的数据，需要对应修改option.xml文件的参数值。

><b>关于视差范围有特殊要求：必须满足64x2<sup>n</sup>，如64、128、256、512。</b>

><b>使用过程中有任何问题，特别是算法BUG，请及时和我交流，感谢！</b>

# 重要说明

算法库为试用版，试用期1个月，若要续期，联系我！

微信：EthanYs6
邮箱：ethan.li.whu@gmail.com

# 一些案例图片
## 概览
| 数据 | Cone | Kitti | Building |
| ------ | ------ | ------ | ------ |
| 帧率 | 219.5 | 95.9 | 4.3 |
| 显存(M) | 277 | 397 | 6880 |

案例数据下载地址：[https://download.csdn.net/download/rs_lys/13074343](https://download.csdn.net/download/rs_lys/13074343)<br>
测试平台：NVIDIA GTX1080

## 数据1：Cone（450*375）
<div align=center>
<img src="https://github.com/ethan-li-coding/FasterStereoCuda-Library/blob/master/Data/diagram/Cone.png">
</div>

## 数据2：Kitti（1242*375）
<div align=center>
<img src="https://github.com/ethan-li-coding/FasterStereoCuda-Library/blob/master/Data/diagram/Kitti.png">
</div>

## 数据3：Building（4800*3409）
<div align=center>
<img src="https://github.com/ethan-li-coding/FasterStereoCuda-Library/blob/master/Data/diagram/Building.png">
</div>

## Github图片不显示的解决办法
修改hosts

C:\Windows\System32\drivers\etc\hosts

在文件末尾添加：
```cpp
# GitHub Start
192.30.253.119      gist.github.com
151.101.184.133     assets-cdn.github.com
151.101.184.133     raw.githubusercontent.com
151.101.184.133     gist.githubusercontent.com
151.101.184.133     cloud.githubusercontent.com
151.101.184.133     camo.githubusercontent.com
151.101.184.133     avatars0.githubusercontent.com
151.101.184.133     avatars1.githubusercontent.com
151.101.184.133     avatars2.githubusercontent.com
151.101.184.133     avatars3.githubusercontent.com
151.101.184.133     avatars4.githubusercontent.com
151.101.184.133     avatars5.githubusercontent.com
151.101.184.133     avatars6.githubusercontent.com
151.101.184.133     avatars7.githubusercontent.com
151.101.184.133     avatars8.githubusercontent.com
# GitHub End

```
