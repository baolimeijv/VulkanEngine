# VulkanEngine — 基于 Vulkan 的 3D 渲染引擎（进行中）

C++17 + Vulkan 从零实现，不使用任何引擎框架。

## 当前进度

- [x] Instance / 验证层 / 物理设备 / 逻辑设备 / 队列族
- [x] 交换链 / RenderPass / 深度缓冲 / Framebuffer / 同步对象
- [x] 窗口缩放重建交换链（含 oldSwapchain 退休）
- [x] 顶点缓冲 + **索引缓冲** + 多顶点属性
- [x] 动态 viewport / scissor
- [x] Push Constant 传递每物体变换与颜色
- [x] **3D 变换**（TransformComponent + mat4）
- [x] **相机**（透视投影，P×V×M 上传）+ 键盘相机控制
- [x] **模型加载**（tinyobjloader + 顶点去重）
- [x] **渲染器分层**：`LveRender`（帧生命周期）+ `SimpleRenderSystem`（渲染系统）
- [ ] Descriptor Set + UBO（进行中）
- [ ] 纹理 / Mipmap（计划）

## 架构

| 模块 | 职责 |
|---|---|
| LveWindow | 窗口、Surface、缩放回调 |
| LveDevice | Instance / 设备 / 队列 / 命令池 + Buffer、Image 工具函数 |
| LveSwapChain | 交换链、RenderPass、深度、Framebuffer、同步对象 |
| LveRender | 帧生命周期：beginFrame / endFrame、命令缓冲、当前帧与图像索引 |
| LvePipeline | 着色器模块 + 图形管线（动态 viewport / scissor）|
| SimpleRenderSystem | 渲染系统：持有自己的 pipeline，遍历 GameObject 提交 push constant |
| LveModel | 顶点 / 索引数据 → VkBuffer → bind / draw |
| LveGameObject | 场景对象 + TransformComponent（3D）|
| LveCamera | 透视投影矩阵与视图矩阵 |
| KeyboardMovementController | WASD + 鼠标控制相机 |
| FirstApp | 主循环与一帧的编排 |

## 关键实现

**1. 帧生命周期独立成 LveRender，而不是塞在 App 里**

命令缓冲按 frame-in-flight 索引，但交换链图像数量与 frame 数量并不相等（常见 2 vs 3），
所以「当前帧」和「当前图像」必须是两个独立索引。这件事收在 LveRender 之后，
App 只负责 beginFrame → 提交渲染 → endFrame，不必关心同步对象索引和 viewport 设置。

**2. 交换链重建必须退休旧链（oldSwapchain）**

重建时把旧交换链交给 `VkSwapchainCreateInfoKHR::oldSwapchain`，而不是直接销毁重造。
好处是新旧链的绑定原子转移、可以复用旧链的部分资源，且规范禁止同一个原生窗口同时存在
两个未退休的交换链。代价是不能立即销毁旧链，要等它在飞行中结束。

**3. 现在用 Push Constant，下一步换 UBO**

相机矩阵目前每帧每物体都随 push constant 上传（80 字节），物体少时最简单直接。
但 push constant 有 128 字节上限，且相机矩阵对每个物体都相同、重复上传是浪费。
下一步引入 Descriptor Set + UBO，把「每帧全局数据（相机 / 光源）」与「每物体数据」分开。

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
- Descriptor Set + UBO → 纹理 → 光照
