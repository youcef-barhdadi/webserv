
#!/usr/bin/python

# Import modules for CGI handling 
import cgi, cgitb 
import time;
# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')
date  = form.getvalue('date')
sex  = form.getvalue('sex')
time.sleep(182.4)

print ("Content-type:text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello - Second CGI Program</title>")
print ("</head>")
print ("<body>")
print ("<h2>Hello %s %s</h2>" % (first_name, last_name))
print ("<h2>date is  [%s] </h2>" % (date))
print ("<h2>sex is  [%s] </h2>" % (sex))
print ("</body>")
print ("</html>")