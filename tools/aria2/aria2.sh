#编辑aria2文件
vi /etc/init.d/aria2
#添加以下代码
DAEMON=/root/aria2/bin/aria2c
     
case "$1" in
    start)
        echo "Starting aria2c"
        nohup $DAEMON -c > /root/.aria2/aria2.log 2>&1 &
        ;;
    stop)
        echo "Shutting down aria2c"
        killall aria2c &>/dev/null
        ;;
    restart)
        killall aria2c &>/dev/null
        sleep 3
        nohup $DAEMON -c > /root/.aria2/aria2.log 2>&1 &
        ;;
    *)
        echo "Usage: /etc/init.d/aria2 {start|stop|restart}"
        exit 1
        ;;
esac
exit

