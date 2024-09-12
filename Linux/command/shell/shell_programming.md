# shell编程

# shell指命令执行器，shell编程指的是linux为用户提供的一种自动化的命令执行方式
# 因而一般谈及shell，既指命令执行器（命令执行环境），又指shell编程语言

# shell编程的基本操作：
    1 shell文件：我们已经知道linux下的任何文件都有三个权限，rwx，rw指读写权限，x指可执行权限，一个shell文件，可以是任何一个可执行文件
        （1）使用touch创建出的文件的默认权限是664，由于shell文件必须具备可执行权限，因而我们可以通过：
            chmod u+x 文件名
        将一个普通文件转变为shell文件
        （2）但是要注意，此时的这个可执行文件的属性还是-，但是颜色由白色变成了绿色
    2 如何执行一个shell文件？
（1）source
（2）bash
（3）路径法（绝对路径或者相对路径皆可）：不光是shell文件，也可以执行二进制格式的文件比如elf，source只能执行shell脚本，bash只能执行bash脚本。
    3 指定Shell解释器:
        #!/bin/bash
        注意：
            （1）该行必须有，否则不能执行命令
            （2）该行必须放在shell文件的第一行

# shell编程的基本语法：
    1 数据：
        几个基本问题：
            A 如何获取一个数据？
                ${数据名}
            B 在写一般的编程语言时，空格并无影响，比如使用cpp定义一个整形变量：
                int i = 1；
              但是，shell不能有空格！
            C 七个特殊变量：以 $为首部，0（文件名或者函数名） n（第几个参数） #（参数个数，不包括文件名，这一点和cpp不一样） *（以字符串形式显示所有参数） @（以列表形式显示所有参数） ?（上一个命令的退出状态或返回值） $
            D 命令替换：通过反引号和$()可以获得某个命令的echo，echo `ls`或echo $(ls)
            E 位置参数：命令行和函数都有位置参数，函数内部听函数的，使用$n获取
            F 两个重要区别：
                a @和*有区别
                b 单引号和双引号是有一定区别的，单引没有占位符功能
        （1）数字
        （2）字符串：不区分单双引号
        （3）数组：
            A 声明定义初始化和C一样
            B 访问修改和C一样
            C 数组长度的获取：
                ${#数组名[@]}
                ${#数组名[*]}
            D 追加元素：
                数组名+=(元素值)
                数组名+=(${数组名[@]} 元素值)
            E 删除元素：
                unset 数组名[下标]
        （4）哈系表：
            A 声明一个哈系表：
                declare -A 哈系表名
            B 访问修改和C一样
            C 长度同上
            D 添加：
                哈系表名[key] = value
            E 删除同上
        关于数组和哈系表的重要问题：
            名[@] / 名[*] ： 这个写法表示整个数组，这种写法的一个重要用法是在遍历时，它可以放在in后面
            但是在遍历时，in后面写成：
                ${名[@]}
                ${!名[@]}
            是不一样的：
                第一种获取的迭代因子是值
                第二种获取到的是索引
    2 控制和运算：整体结构和任何其他编程语言都一样，顺序选择循环，也有continue和break
        第一部分：结构
        （1）选择：
            if condition1; then
                1true
            elif condition2; then
                1false 2true
            else
                12false
            fi
        (2)循环：
            A
                for each in ${名[@]} / ${!名[@]}   // each是迭代因子
                do
                    代码段
                done
            B
                while condition   // condition == true，do
                do
                    代码段
                done
            C
                until condition   // condition == false，do
                do
                    代码段
                done
            D case in esac
#!/bin/bash
printf "Input integer number: "
read num
case $num in
    1)
        echo "Monday"
        ;;
    2)
        echo "Tuesday"
        ;;
    3)
        echo "Wednesday"
        ;;
    4)
        echo "Thursday"
        ;;
    5)
        echo "Friday"
        ;;
    6)
        echo "Saturday"
        ;;
    7)
        echo "Sunday"
        ;;
    *)
        echo "error"
esac
        E select in （死循环）
#!/bin/bash
echo "What is your favourite OS?"
select name in "Linux" "Windows" "Mac OS" "UNIX" "Android"
do
    case $name in
        "Linux")
            echo "Linux是一个类UNIX操作系统，它开源免费，运行在各种服务器设备和嵌入式设备。"
            break
            ;;
        "Windows")
            echo "Windows是微软开发的个人电脑操作系统，它是闭源收费的。"
            break
            ;;
        "Mac OS")
            echo "Mac OS是苹果公司基于UNIX开发的一款图形界面操作系统，只能运行与苹果提供的硬件之上。"
            break
            ;;
        "UNIX")
            echo "UNIX是操作系统的开山鼻祖，现在已经逐渐退出历史舞台，只应用在特殊场合。"
            break
            ;;
        "Android")
            echo "Android是由Google开发的手机操作系统，目前已经占据了70%的市场份额。"
            break
            ;;
        *)
            echo "输入错误，请重新输入"
    esac
done
        第二部分：运算符
    3 函数：fuction可写可不写，如果不写，声明的时候必须加上（），不管写不写，调用的时候不能加（），如果写了，那么小括号可以省略，建议加上
        （1）声明：
        [function] 函数名[()] {
            代码段
        }
        （2）调用：
        函数名 [参数1 参数2 参数3......]
         (3)参数：没有形参表，使用$n接收
        （4）返回值：理论上函数没有返回值，return在shell函数里的作用是返回一个0～255的状态码，0表示函数执行成功，非0是各种各样的错误，但是我们可以通过全局变量（类似与cpp的传出参数）或者命令替换这样的方式得到返回值（函数本身也是一个命令，可以用反引号或者$()获得echo）
    4 内置命令：
        常见的有：echo read declare alias exit let expr bc
