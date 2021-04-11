# 校园导航系统

计算机学院 2019 级数据结构课程设计

## 依赖

- GoogleTest
- CMake

## 编译运行

1. **准备** 安装 GNU 工具链(g++、make、cmake)并在项目根目录下 `mkdir build && cd build` 文件夹
2. **编译** 在 `build` 目录下运行 `cmake .. && make all` (cmake 后追加 `-DCMAKE_BUILD_TYPE=Debug` 以开启 debug)
3. **运行** 在 `build` 下运行 `src/CampusNavigation_run`
4. **测试** 在 `build` 下运行 `test/CampusNavigation_test`

## 目录说明

- build/: 编译目标(cmake&&make)
- ext/: 外部依赖库
- src/: 源代码

## 数据文件格式说明

**model_hub** 为了方便跨平台，我们使用文本格式(csv)来读写模型仓库信息。模型仓库存档的格式要求如下：

- 满足csv文件的基本格式要求
- 以行划分，每行存储一个model或者一条path
- 对于一个模型行的格式形如 `[id],[type],[name(optional)],[more specific options]`
- 对于一个path/transport,格式形如 `[id],[type],[name(optional)],[id1],[id2],[distance],[more specific options]`
- 目前所有的connection只能出现在model之后

下面是一个示例

```csv
1,building,教学一
2,building,教学楼二
3,path,,1,2,32
```
