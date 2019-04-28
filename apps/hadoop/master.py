import os
import sys

os.system("jps | python3 clean_hadoop_daemons.py")


hadoop_home = "/home/ivan/hadoop"
'''
slave_homes = "/home/ivan/slave-homes"
os.system("rm -rf " + slave_homes);
os.system("mkdir " + slave_homes);
'''

#set_slave_home:
'''
conf_file = open(sys.argv[3], "r");
node_cnt = int(conf_file.readline())
for i in range(node_cnt):
    slave_home_i = slave_homes + "/home" + str(i)
    os.system("mkdir " + slave_home_i)
    os.system("cp -r " +  hadoop_home + "/* " + slave_home_i)
    os.system("rm -rf " + slave_home_i + "/data/datanode/*")
    os.system("rm -rf " + slave_home_i + "/data/namenode/*")
    os.system("rm -rf " + slave_home_i + "/etc/hadoop/hdfs-site.xml");
    os.system('echo "<configuration><property><name>dfs.datanode.data.dir</name><value>' + slave_home_i + '/data/datanode</value></property></configuration>" > hdfs-site.xml' )
    os.system ("cp hdfs-site.xml " + slave_home_i + "/etc/hadoop");
'''

#set master daemons:
master_log = "master_log"
os.system(hadoop_home + "/bin/hadoop namenode " + "&>" + master_log + "_nn &");
os.system(hadoop_home + "/bin/hadoop datanode " + "&> " + master_log + "_dn &");
os.system(hadoop_home + "/bin/yarn resourcemanager " + "&> " + master_log + "_rm &");
os.system(hadoop_home + "/bin/yarn nodemanager " + "&> " + master_log + "_nm &");
os.system("hdfs dfsadmin -safemode leave")

#start task:
os.system("/bin/bash")
#os.system("hdfs dfsadmin -safemode leave")
