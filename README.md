# fold_cleaner
# 程序简介
模拟扫地机清扫工作状态的QT程序，可以实现手动绘制房间格局、完整房间2D地图清扫演示、路径规划与路径导航等拟真功能。  
代码实现： 
1. 完整量子框架状态机及各子状态切换
2. A*路径规划算法
3. QT动态小程序设计
# 前置程序
QT-Creator5.8及以上、MinGw编译器
# 安装
使用QT-Creator编译启动即可  
**项目设置的 时候选择Mingw作为C++的编译器，否则会产生语法问题**
# 程序界面
![image](https://note.youdao.com/yws/public/resource/3b566b9f39909506c26254343691e8a1/xmlnote/3141A7C389614D59AB88389A8DFA0152/10568)
# 使用说明
## 添加障碍物
在网格地图中点击鼠标即可添加障碍物
![image](https://note.youdao.com/yws/public/resource/3b566b9f39909506c26254343691e8a1/xmlnote/4F57742B25AC44EF9EDF4ED54B7F959B/10570)
**黑色方块代表障碍物**
## 设置起点
输入start(x,y)和end(x,y),或者直接点击“输入起点”按键在地图上点击设置起点，再次点击“设置完成”同样也可完成起点设置。
## 路径规划算法验证  
设置预期终点(设置方式同起点)  
点击calculate即可绘制A*算法规划的最优路径  
![image](https://note.youdao.com/yws/public/resource/3b566b9f39909506c26254343691e8a1/xmlnote/87B2A5B55E06447BAE5552DE91BCE5B4/10574)
**蓝色线条绘制的即为路径规划的路线**
## 完整扫地机模拟
设置好起点和障碍物之后点击弓字清扫，即可开始扫地机的运行。  
点击“暂停”可以暂时停止扫地机运行，暂停后再次点击"暂停“可以恢复扫地机运行。  
清扫过程中可以实时添加障碍物  
按下弓字清扫会清除所有添加障碍物  
![image](https://note.youdao.com/yws/public/resource/3b566b9f39909506c26254343691e8a1/xmlnote/3A06B1E5E9C04B9EA68C3C656FFB054D/10576)
