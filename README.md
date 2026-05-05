# -NKU2026C-DesktopPet
# 智能桌面宠物 (Smart Desktop Pet)

一个基于 Qt6 的桌面宠物程序，集成了角色切换、音乐播放、实时天气查询、待办事项管理、陪伴模式等丰富功能，为你的桌面增添一份生动与陪伴。

## ✨ 主要功能

- **角色切换**：支持多个宠物角色（猫、狗、兔等），每个角色拥有不同的静态图片（常态/单击/双击/长按）和背景音乐。
- **智能去白底**：仅消除图像外部白色背景，保留宠物内部白色，支持静态图和 GIF 动图。
- **音乐循环播放**：每个角色独立背景音乐，播放完毕自动重播。
- **真实天气查询**：通过 OpenWeatherMap API 获取实时天气（温度、湿度、天气描述），支持全球任意城市，自动保存最近查询城市。
- **待办事项管理**：添加/删除待办，截止时间临近变色（红色），过期自动删除。
- **陪伴模式**：一键切换至特殊角色（GIF 动图），隐藏菜单和待办区域，提供沉浸式陪伴体验。
- **界面定制**：无边框透明窗口，可拖拽；右上角按钮（关闭/最小化/菜单隐藏/陪伴模式）；左侧信息面板（时间/天气/待办）。
- **状态持久化**：音量、最近城市、待办列表等设置自动保存，下次启动恢复。

## 🖼️ 预览

（此处可插入程序截图或 GIF 动图）

## 🛠️ 技术栈

- **开发框架**：Qt 6.8.3 (Core, Widgets, Multimedia, Network)
- **编程语言**：C++17
- **构建工具**：CMake 3.16+ / Ninja
- **第三方服务**：OpenWeatherMap API（需自行申请 API Key）

## 📂 项目结构
DesktopPet/
├── CMakeLists.txt # CMake 构建配置
├── main.cpp # 程序入口
├── desktop_pet.h/cpp # 主窗口类（核心逻辑）
├── pet_menu.h/cpp # 右侧菜单栏类
├── tododialog.h/cpp # 待办添加对话框
├── resources.qrc # Qt 资源文件（可选）
└── README.md # 本文件


## 🔧 编译与运行

### 环境要求

- Qt 6.8.3 及以上版本（已测试 MSVC2022 和 MinGW）
- CMake 3.16+
- 支持 C++17 的编译器（MSVC、GCC、Clang）

### 步骤

1. **克隆或下载源码**到本地纯英文路径（避免中文路径导致构建错误）。

2. **获取 OpenWeatherMap API Key**（可选，若不配置则天气功能不可用）：
   - 访问 [OpenWeatherMap](https://openweathermap.org/) 注册账号。
   - 登录后，在 “My API Keys” 页面复制你的 API Key。
   - 在 `desktop_pet.cpp` 中找到 `m_apiKey = "YOUR_OPENWEATHERMAP_API_KEY";` 并替换。

3. **准备资源文件**：
   - 将角色图片和音乐放入指定目录（代码中使用绝对路径示例，你也可以修改为资源系统路径）。
   - 推荐使用 Qt 资源文件（.qrc）管理资源，便于发布。

4. **使用 Qt Creator 打开项目**：
   - 选择对应的 Kit（例如 Desktop Qt 6.8.3 MSVC2022 64bit）。
   - 点击 “构建” → “运行 CMake” → “构建项目”。

5. **命令行构建**（可选）：
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.8.3/msvc2022_64"  # 替换为你的Qt路径
   cmake --build . --config Release

   运行程序：./DesktopPet.exe（Windows）或 ./DesktopPet（Linux/macOS）。

🎮 使用说明
拖拽窗口：鼠标左键按住窗口任意空白区可拖动。

角色切换：右侧菜单选择 Cat/Dog/Rabbit。

天气查询：选择或输入城市名，点击 “Go” 按钮。

待办添加：点击 “Add Todo”，填写名称和截止时间。

陪伴模式：点击右上角星形按钮 🌟 进入/退出。

其他：单击/双击/长按宠物会显示不同表情气泡和动态图片切换。

📝 自定义扩展
添加新角色：在 loadRoles() 中新增 RoleData 项，指定图片和音乐路径，并设置 isGif 标志。

修改 UI 配色：调整 setupUI() 中的样式表（QSS）。

更换天气 API：修改 requestWeatherForCity() 和 onWeatherReplyFinished() 中的 URL 和解析逻辑。

🐛 已知问题
若 GIF 动图帧数较多，去白底可能轻微影响性能（200x200 图片无感）。

首次获取天气可能需要等待几秒（网络延迟）。

📄 许可证
本项目仅供学习交流使用，未经许可不得用于商业目的。

👤 作者
姓名：Andy Wen

课程：C++ 大作业

年份：2026
