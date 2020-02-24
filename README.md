# E-Mail 客户端开发说明

## 项目依赖

Qt 版本：5.12.0 或更高。

编译器：MSVC 2015 或更高，本项目在 MinGW 下不稳定。

C++ 版本：C++ 11 以上。

操作系统：Windows 10 或更高，在 Linux 环境下要少量修改套接字连接部分。

## 项目架构

E-Mail 客户端使用 Qt 编写，遵循 UI 与业务逻辑分离的设计原则。

### 主要模块

- 套接字模块：调用 Winsock2 库，使用 Windows 提供的 API 负责套接字连接，该模块屏蔽操作系统 API 细节，向上提供统一的套接字收发服务，为下一步跨系统移植做准备。对应 sock.h 和 sock.cpp。
- SMTP模块：调用套接字模块，用户输入 SMTP 服务器、用户名、密码、邮件内容等，该模块提供发邮件服务。对应 smtp.h 和smtp.cpp。
- POP3模块：调用套接字模块，用户输入 POP3 服务器、用户名和密码等，该模块提供收取邮件服务。 pop3.h 和 pop3.cpp 提供了 POP3 服务提供的基本服务，update.cpp 建立一个独立的线程，并收取所有邮件。
- BASE 64模块：输入一串文本，输出 BASE 64 加密后的结果。 SMTP 登录时调用。对应 base64.h 和 base64.cpp。
- 数据库模块：提供基于 sqlite3 的数据库支持，向上提供插入、查询等数据库服务。对应 database.h 和 database.cpp 。
- 邮件阅读模块：调用 POP3 模块，能返回包含邮件头和邮件体的字符串，该模块分析这些字符串，输出收发件人、邮件主题和邮件正文等。对应 readletter.h 和 readletter.cpp。
- GUI ：图形用户界面模块。mainwindow.h 和 mainwindow.cpp 对应于主窗口，用 QStackWidget 切换不同的界面。

### 文件组织

项目目录
│  base64.cpp	-- base64 编码  
│  base64.h	  
│  database.h	-- 数据库  
│  database.cpp	  
│  email.pro	-- 项目文件  
│  email.pro.user	-- 项目文件配置  
│  image.qrc	-- 图标资源  
│  main.cpp	-- 程序入口  
│  mainwindow.cpp	-- 主窗口  
│  mainwindow.h	  
│  mainwindow.ui	-- 主窗口gui  
│  pop3.cpp	-- POP3服务  
│  pop3.h	  
│  readLetter.cpp	-- 解析 pop3 读信时返回的字符串，分析出标题、正文等  
│  readLetter.h  
│  smtp.cpp	-- smtp 服务  
│  smtp.h  
│  sock.cpp	-- 套接字服务  
│  sock.h  
│  update.cpp 	-- 调用pop3服务，获取邮箱里所有的信  
│  update.h  
│  
└─image	-- 图片  
        back.png  
        background.jpg  
        close.png  
        delete.png  
        draft.png  
        mail.png  
        next.png  
        refresh.png  
        reply.png  
        send.png  
        toWrite.png  
        trash.png  
        user.png     

## 开发规范细节

1. 中文字符串以“.”结束，否则 MSVC 可能编译出错。
2. 程序运行时会显示控制台，会显示调试信息。
3. 邮箱里的信少一点，这样收信快一点。