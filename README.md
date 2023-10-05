# 基于跳表的KV存储引擎

## 1. 项目介绍

本项目基于跳表，实现了支持整型、浮点型以及字符串的KV存储引擎，包含插入数据、删除数据、查询数据、数据展示、数据落盘以及从文件加载数据等操作。同时，该引擎使用读写锁保证了多线程读写时数据的正确性。

提供了用户交互界面，有如下命令：

- insert: `insert key value`，将新的键值对加到跳表中。
- delete: `delete key`， 删除某个键值对。
- search: `search key`，查找某个键对应的值。
- show: `show`，显示所有键值对。
- size: `size`，显示当前保存的键值对数量。
- loadfile: `loadfile filename`，从文件中加载数据。
- dumpfile: `dumpfile filename`，将当前存储的数据落盘。
