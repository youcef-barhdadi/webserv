
#!/usr/bin/python

# Import modules for CGI handling 
import cgi, cgitb, os
import time;
# Create instance of FieldStorage 
form = cgi.FieldStorage() 

# Get data from fields
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')
date  = form.getvalue('date')
sex  = form.getvalue('sex')
cookie = os.environ.get("HTTP_COOKIE")

print ("Content-type:text/html\r\n\r\n")
print ("<html>")
print ("<head>")
print ("<title>Hello - Second CGI Program</title>")
print ("</head>")
print ("<body>")
print ("<h2>Hello %s %s</h2>" % (first_name, last_name))
print ("<h2>date is  [%s] </h2>" % (date))
print ("<h2>sex is  [%s] </h2>" % (sex))
print("Cookie = {}".format(cookie))
print ("</body>")
print ("</html>")