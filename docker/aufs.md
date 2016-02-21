首先，我们建上两个目录（水果和蔬菜），并在这两个目录中放上一些文件，水果中有苹果和蕃茄，蔬菜有胡萝卜和蕃茄。

$ tree
.
├── fruits
│   ├── apple
│   └── tomato
└── vegetables
    ├── carrots
    └── tomato
然后，我们输入以下命令：

# 创建一个mount目录
$ mkdir mnt
 
# 把水果目录和蔬菜目录union mount到 ./mnt目录中
$ sudo mount -t aufs -o dirs=./fruits:./vegetables none ./mnt
 
#  查看./mnt目录
$ tree ./mnt
./mnt
├── apple
├── carrots
└── tomato
我们可以看到在./mnt目录下有三个文件，苹果apple、胡萝卜carrots和蕃茄tomato。水果和蔬菜的目录被union到了./mnt目录下了。

我们来修改一下其中的文件内容：

$ echo mnt > ./mnt/apple
$ cat ./mnt/apple
mnt
$ cat ./fruits/apple
mnt
上面的示例，我们可以看到./mnt/apple的内容改了，./fruits/apple的内容也改了。

$ echo mnt_carrots > ./mnt/carrots
$ cat ./vegetables/carrots
 
$ cat ./fruits/carrots
mnt_carrots
上面的示例，我们可以看到，我们修改了./mnt/carrots的文件内容，./vegetables/carrots并没有变化，
反而是./fruits/carrots的目录中出现了carrots文件，其内容是我们在./mnt/carrots里的内容。

也就是说，我们在mount aufs命令中，我们没有指它vegetables和fruits的目录权限，默认上来说，
命令行上第一个（最左边）的目录是可读可写的，后面的全都是只读的。（一般来说，最前面的目录应该是可写的，而后面的都应该是只读的）

所以，如果我们像下面这样指定权限来mount aufs，你就会发现有不一样的效果（记得先把上面./fruits/carrots的文件删除了）：

$ sudo mount -t aufs -o dirs=./fruits=rw:./vegetables=rw none ./mnt
$ echo "mnt_carrots" > ./mnt/carrots
$ cat ./vegetables/carrots
mnt_carrots
 
$ cat ./fruits/carrots
cat: ./fruits/carrots: No such file or directory
现在，在这情况下，如果我们要修改./mnt/tomato这个文件，那么究竟是哪个文件会被改写？

$ echo "mnt_tomato" > ./mnt/tomato
$ cat ./fruits/tomato
mnt_tomato
$ cat ./vegetables/tomato
I am a vegetable
可见，如果有重复的文件名，在mount命令行上，越往前的就优先级越高。

