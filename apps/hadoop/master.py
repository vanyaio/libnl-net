import os

slave_homes = "/home/ivan/slave-homes"
hadoop_home = "/home/ivan/hadoop"
os.system("rm -rf " + slave_homes);
os.system("mkdir " + slave_homes);

#set_slave_home:
conf_file = open(argv[3], "r");
node_cnt = int(conf_file.readline())
for i in range(node_cnt):
    slave_home_i = slave_homes + "/home" + str(i)
    os.system("mkdir " + slave_home_i)
    os.system("cp -r " +  hadoop_home + "/* " + slave_home_i)
    os.system("rm -rf " + slave_home_i + "/data/datanode/*")
    os.system("rm -rf " + slave_home_i + "/data/namenode/*")
    os.system("rm -rf " + slave_home_i + "/etc/hadoop/hdfs-site.xml");
    os.system ("cp hdfs-site.xml " + slave_home_i + "/etc/hadoop");


#set master daemons:
os.system(hadoop_home + "/bin/hadoop namenode > /dev/null 2> /dev/null &");
os.system(hadoop_home + "/bin/hadoop datanode > /dev/null 2> /dev/null &");
os.system(hadoop_home + "/bin/yarn resourcemanager > /dev/null 2> /dev/null &");
os.system(hadoop_home + "/bin/yarn nodemanager > /dev/null 2> /dev/null &");


#start task:
os.system("hdfs dfsadmin -safemode leave");
os.system("/bin/sh");
