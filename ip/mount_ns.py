import os
import sys

ns_name = str(sys.argv[1])
pid = str(sys.argv[2])
print(pid)
os.system("touch /var/run/netns/" + ns_name)
os.system("mount -o ro,bind " + "/proc/" + pid + "/ns/net " + "/var/run/netns/" + ns_name)
