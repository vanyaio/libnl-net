import os
import sys

ns_name = sys.argv[1]
pid = int(sys.argv[2])

os.system("touch /var/run/netns/" + ns_name)
os.system("mount --bind " + "/proc/" + pid + "/ns/net " + "/var/run/netns/" + ns_name)
