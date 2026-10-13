# VulkanEngine

基于 Vulkan 的 2D 渲染引擎（进行中）。C++17 + Vulkan 1.0 从零实现，不使用任何引擎框架。

![演示](images/demo.gif)

## 当前进度

- [x] Instance / 验证层 / 物理设备 / 逻辑设备 / 队列族
- [x] 交换链 / RenderPass / 深度缓冲 / Framebuffer / 同步对象
- [x] 窗口缩放重建交换链（含 oldSwapchain 退休）
- [x] 顶点缓冲 + 多顶点属性（position / color）
- [x] 动态 viewport / scissor
- [x] Push Constant 传递每对象变换与颜色
- [x] GameObject / Transform2d 组件
- [ ] Descriptor Set + UBO
- [ ] 相机与 3D 变换
- [ ] 模型加载 / 纹理 / Mipmap

## 架构

![架构](images/architecture.png)

| 模块 | 职责 |
|---|---|
| LveWindow | 窗口、Surface、缩放回调 |
| LveDevice | Instance / 设备 / 队列 / 命令池 + Buffer、Image 工具函数 |
| LveSwapChain | 交换链、RenderPass、深度、Framebuffer、同步对象 |
| LvePipeline | 着色器模块 + 图形管线 |
| LveModel | 顶点数据 → VkBuffer → bind / draw |
| LveGameObject | 场景对象 + Transform2d 组件 |
| FirstApp | 主循环与一帧的编排 |

## 构建

依赖：

- Vulkan SDK 1.4.x（提供 `vulkan-1.lib` 与 `glslc`）
- GLFW 3.4（预编译库）
- GLM（header-only，需放在项目目录下）

步骤：

1. 打开 `VulkanEngine.sln`
2. 配置选择 `Debug | x64`
3. F5 运行（着色器由自定义生成工具自动调用 glslc 编译）

## 已知问题与后续计划

- 交换链 semaphore 复用问题（同步 bug，待修）
- 投影矩阵为静态，窗口比例变化时画面会拉伸（待接入相机）
