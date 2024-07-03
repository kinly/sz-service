## 相对完整的小游戏

## 需求

### pre
- 开房间 PVP 类型的游戏
- 大区游戏，服务器可扩容更新
- 游戏内有战斗，可以操控也可以纯AI
- 技能、AI可轻松编辑
- 核心代码 C++，lua是否引入暂定

### 技术
- 框架参考： https://klysisle.space/archives/b8b38dba.html
- 网络：
  - http: cinatra (C++20) https://github.com/qicosmos/cinatra
  - tcp: 基于 asio 协程
- 存储：
  - mongo/mysql
  - redis: redis-cplus-cplus https://github.com/sewenew/redis-plus-plus
- 序列化：
  - json: nlohmann/json
  - protobuf
- 配置：
  - apollo-config: C++ client 需要重新封装
- 技能、AI：
  - behaviorTree.cpp (C++17) https://github.com/BehaviorTree/BehaviorTree.CPP

## 规范

### 规范
- 整体使用 vs2022 `tools -> options -> Text Editor -> C/C++ -> code style -> formatting -> default formatting style ` **llvm**
- 小写英文/简写（目录命名规则相同）
- 下划线分割
- 数据变量、常量不带数据类型
- 命名统一，不要一块是 `jinbi`，一块是 `gold` (参考 [名词表](名词表))
- 统一命名空间：
  - 基础库：`util`
  - 基础库二级：`util::timer`

### 函数
- 函数命名规范：**使用下划线命名，均小写**
- 简洁性：
  - 因为封装是按照模块封装的，模块名已经是必然离不开的前缀了，内部不允许再出现模块名：
    - 错误的: bag:bag_config、bag:add_to_bag
    - 正确的：bag:config，bag:add，bag:sub
  - 一个功能尽量仅一个函数
  - 多考虑如果是你使用，需要怎么样的接口
- 参数：
  - 参数命名不再带参数类型: 错误的：`dwId`， 正确的：`id`
  - 不明确使用数值/字符串，或者可能使用数值也可能使用字符串的，统一用字符串
  - 不再有注册结构，为了控制参数长度，考虑先用序列化方式(json/msgpack)传递
  - 也使用序列化参数的方式简化参数，**做好解析后的检查**
  - 定义清楚枚举，紧凑的
  - 参数顺序统一，例如需要传递对象的函数，对象一定是第一个参数，可用的主要对象：`object，player，monster，npc，item，scene`
  - 接口传递的参数相同含义统一命名，例如 `item_id, scene_id`， 具体参考下面的**名词**


### 名词表
- 标记游戏内常用的名词
- 简写适用于名词本身过长/标记足够常用
- 逐步补充

| 描述 | 命名    | 简写 | 示例 |
|:---- |:-----  | :--- | :--- |
| 配置 | config | cfg | cfg_info |
| 配置数据 | meta | meta | meta_id, meta_name |
| 系统 | system | sys | |
| 基础的 | base | base | |
| 数据 | data | | 怪物数据: monster |
| 管理 | manager | mgr | |
| 初始化 | initialize | init | |
| 获取 | get | | 获取名字: name() |
| 设置/更新 | set | set | 设置名字: set_name() |
| 修改 | change | change | |
| 转换 | exchange | exchange | |
| 添加 | add | add | |
| 扣除 | sub | sub | |
| 删除 | remove | rem | |
| 消耗 | consume | consume | |
| 扩展/补充/额外的 | extend/extra | ext/ex | 扩展名: ext_name, 额外定时器:timerex |
| 列表 | list | lst | player_list |
| 消息 | message | msg | |
| 格式 | format | fmt | |
| 时间（带format格式的） | datetime/time | |
| 时间戳 | timestamp | ts | |
| 时间间隔（steady_clock） | tick | tick | |
| 唯一id | uuid | uuid |  |
| 配置id | id | id | item_id, scene_id （简化，统一规范就不需要带meta前缀了） |
| 对象 | object | obj | |
| 类型 | type | type | |
| 玩家 | player | player | |
| 怪物 | monster | mon | |
| npc | npc | npc | |
| 宠物 | pet | pet | |
| 道具 | item | item | |
| 装备 | equipment | equip | |
| 背包 | bag | bag | |
| 数量 | count | cnt | |
| 技能| skill | skill | |
| buff（可叠加的效果） | buff | buff | |
| 状态 | state | | |
| 叠加 | overlaps | overlap | |
| 等级 | level | lv | |
| 属性 | attribute | attr | |
| 称号 | title | title | |
| 邮件 | mail | mail | |
| 任务 | task | task | |
| 聊天 | chat | chat | |
| 成就 | achievement | ach | |
| 活动 | active | active | |
| 副本 | dungeon | dg | |
| 排行榜 | rank | rank | |
| 好友 | friend | fri | |
| 行会 | guild | guild | |
| 组队 | team |team | |
| 场景 | scene | scene | |
| 坐标/点 | point | pt | |
| 视野/周围 | eyeshot | eye | |
| 脚本 | script | sc | |
| 定时器 | timer | | |
| 登录 | login | login | |
| 登出 | logout | logout | |
| 在线 | online | online | |
| 离线 | offline | offline | |
| 复活 | relive | relive | |
| 回调 | callback | cb | |
| 函数 | function | func | |
