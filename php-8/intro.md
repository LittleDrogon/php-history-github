
php-8
新增git
就跟js的v8引擎jit一样

https://www.liuvv.com/p/ad42ac48.html

由于默认只安装了cig和cli，在Linux上面通常运行php-fpm，因此需要安装php-fpm

https://www.liuvv.com/p/ad42ac48.html

sudo apt install -y pkg-config build-essential autoconf bison re2c libxml2-dev libsqlite3-dev

./configure --prefix=/usr/local/php  --enable-fpm

make && make  install
