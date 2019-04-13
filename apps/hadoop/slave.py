import os

slave_homes = "/home/ivan/slave-homes"
hadoop_home = "/home/ivan/hadoop"
num = argv[]
slave_home = slave_homes + "/home" + str(num)

os.environ("JAVA_HOME") = "/usr/lib/jvm/java-11-oracle"
os.environ("HADOOP_INSTALL") = slave_home
os.environ("HADOOP_HOME") = slave_home
os.environ("HADOOP_PREFIX") = slave_home
os.environ("PATH") = "/home/ivan/bin:/home/ivan/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/usr/lib/jvm/java-11-oracle/bin:/usr/lib/jvm/java-11-oracle/db/bin:/usr/lib/jvm/java-11-oracle/bin:/home/ivan/slave-home/bin:/home/ivan/slave-home/sbin"
os.environ("HADOOP_MAPRED_HOME") = slave_home
os.environ("HADOOP_COMMON_HOME") = slave_home
os.environ("HADOOP_HDFS_HOME") = slave_home
os.environ("YARN_HOME") = slave_home
os.environ("HADOOP_COMMON_LIB_NATIVE_DIR") = slave_home + "/lib/native"

system(hadoop_home + "/bin/hadoop datanode  > /dev/null 2> /dev/null &");
system(hadoop_home + "/bin/yarn nodemanager  > /dev/null 2> /dev/null &");
