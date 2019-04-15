import os
import sys

slave_homes = "/home/ivan/slave-homes"
hadoop_home = "/home/ivan/hadoop"
num = sys.argv[4]
slave_home = slave_homes + "/home" + str(num)

os.environ["HADOOP_INSTALL"] = slave_home
os.environ["JAVA_HOME"] = "/usr/lib/jvm/java-11-oracle"
os.environ["HADOOP_HOME"] = slave_home
os.environ["HADOOP_PREFIX"] = slave_home
os.environ["PATH"] = "/home/ivan/bin:/home/ivan/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/usr/lib/jvm/java-11-oracle/bin:/usr/lib/jvm/java-11-oracle/db/bin:/usr/lib/jvm/java-11-oracle/bin:/home/ivan/slave-home/bin:/home/ivan/slave-home/sbin"
os.environ["HADOOP_MAPRED_HOME"] = slave_home
os.environ["HADOOP_COMMON_HOME"] = slave_home
os.environ["HADOOP_HDFS_HOME"] = slave_home
os.environ["YARN_HOME"] = slave_home
os.environ["HADOOP_COMMON_LIB_NATIVE_DIR"] = slave_home + "/lib/native"


is_master_set = "is_master_set" + num
os.system("touch " + is_master_set)
while True:
    os.system("jps > " + is_master_set)
    file = open(is_master_set, "r")
    arr = [line.split() for line in file]
    daemons = 0
    for elem in arr:
        if (elem[1] != '--'):
            daemons += 1
    if (daemons >= 4):
        break
os.system("rm " + is_master_set)

slave_log = "slove_log" + str(num)
os.system(hadoop_home + "/bin/hadoop datanode " + "&>" + slave_log + "_dn &")
os.system(hadoop_home + "/bin/yarn nodemanager " + "&>" + slave_log + "_nn &")
