# 插入排序动画

#### 介绍
瞎搞

#### 软件架构
排序和动画分为两个线程，中间通过"发送消息"实现两个线程联系，初学，如有不妥欢迎+qq1790226429交流
完成于2021/12/03
上传于2022/01/16

2023/3/18复盘。。

最近在移动gitee的仓库到github
我知道中间用了Sleep来傻乎乎地等待消息，只能说当时不知道。线程也是用windows的api创建的而不是std::thread。显然std::thread+std::mutex才是比较好的。
只能说吃了经验少的亏，空有想法但没人指点是这样的。

