import os
while (1 > 0):
    st = input()
    if (st == ""):
        break
    pid = (st.split())[0]
    daemon = (st.split())[1]
    if (daemon != "Jps"):
        os.system("kill -9 " + pid)
