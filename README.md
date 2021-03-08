# 校园导航系统

计算机学院 2019 级数据结构课程设计

## 依赖

- GoogleTest
- CMake

## 编译运行

1. **准备** 安装 GNU 工具链(g++、make、cmake)并在项目根目录下 `mkdir build && cd build` 文件夹
2. **编译** 在 `build` 目录下运行 `cmake .. && make` (如果不是第一次编译，cmake 之后最好执行一遍 `make clean` )
3. **运行** 在 `build` 下运行 `./CampusNavigation`
4. **测试** 在 `build` 下运行 `make test`

## 目录说明

- build/: 编译目标(cmake&&make)
- ext/: 外部依赖库
- src/: 源代码
