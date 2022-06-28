# macOS下进行Linux C/C++开发

目标：  

- 基于VSCode作为IDE进行开发;
- 另外，编译构建必须能够

## 方式1：VSCode -> Add Dockerfile to workspace

- 在vscode中cmd+p，输入add dockerfile to workspace并执行，此时会选择基础镜像，如面向c++开发的基础镜像，此时会生成默认的dockerfile。
- 然后在dockerfile选中后点击右键，选择build image，此时就完成镜像构建了，该默认dockerfile默认是一个编译镜像，里面包含了编译构建产物。
- 直接运行上述镜像默认就是运行程序，运行的方式可以在docker explorer里面找到镜像，右键菜单中选择Run，或者命令行执行。

这个镜像只是用来编译构建、测试运行的，还不能满足我们开发阶段的需求，因为开发阶段需要考虑头文件、库的搜索问题。

常规解决思路：

1. 至少要构建一个支持开发的镜像，如c/c++镜像；
2. 启动这个镜像，并将当前工程以volume的形式挂在到容器中，或者在容器中clone下来这个项目。提交代码要注意随时提交；
3. 开发通过vscode remote连接到vscode server进行开发，其实是本地vscode通过ssh连接传输vscode server软件包到容器中并安装启动；
4. 如果开发镜像支持类似WebIDE的方式进行开发，也可以代替3这种方式，只是一些本地vscode的快捷配置等可能不是很好同步。

## 方式2：Docker Desktop -> New Dev environment + Mount local directory / Clone git repository

Docker Desktop的这种实现方式，就是上面提到的2\3这几步的组合，基本满足我们希望实现的目标了。

这种方式也有不足，就是假设后续有人要接手这个项目，或者有人和你协作，你怎么办呢？
我们可以直接提交一个镜像push到registry，他只要能拉代码，又能拉镜像就基本能还原之前的开发环境。

但是我为什么非要push一个镜像上去呢（包括自定义的基础镜像、开发阶段的分享镜像）？
如果不push镜像而docker destop默认的开发镜像调整了或者我希望定制一个统一的怎么办？

Docker Desktop创建新的开发镜像的时候有一种方式，允许指定一个基础镜像，但是这个基础镜像要push到远程registry。代码会被clone到这个
容器内部，我们就通过vscode remote进行开发即可。

尽管vscode鼓励非Linux用户尽量通过这种方式，因为fs操作更快，但是还是有点不方便，因为这数据卷相当于额外浪费一份存储，考虑到之前已经克隆过的情况下。
有没有办法既能自定义基础镜像，又能挂载本地磁盘目录为数据卷的方式来解决呢？可以，请看方式3。

## 方式3: Docker Desktop -> Remote-Containers: Add Development Container Configuration Files

在这里我们可以指定一个配置文件.devcontainer(实际是个目录)，.devcontainer/devcontainer.json中指明了我们要使用哪个镜像作为开发容器的基础镜像。
这里的基础镜像可以用vscode官方提供的，也可以是自己自定义好push到registry的，也可以是Dockerfile需要vscode代为构建本地镜像的，这几种方式均可！

这样不就方便了吗，既不需要额外存储这个基础镜像，还保留了维护基础镜像的必要配置信息，如Dockerfile等。当然了如果基础镜像维护得当，我们引用registry
中的基础镜像即可，没必要每个工程下都放一份镜像Dockerfile信息。

我目前比较喜欢这种方式，因为可以使用自定义基础镜像的同时，没有了额外容器中克隆代码导致的占用磁盘空间问题……不过这样性能会差点。

## 方式3：构建基于WebIDE的开发环境，容器中增加WebIDE并暴露Web端口对外接入

在开发容器中支持WebIDE，github的codespace，以及gitpod就是类似的解决方案，我自己也基于开源的theia WebIDE做过类似的demo。
