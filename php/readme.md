# Logging server
Simple server script for logging sensor data in csv files.
Integrated, simple user authentification.

### Setup:
Securitytoken can change in the addvalue.php file.  
Change the login credentials in the login.php file.  
(username1,password1,username2,password2)  
Define username and associated password array

### How to use:
http://serverip/addvalue.php?token={TOKEN}&value=  
Replace {TOKEN} with your securitytoken
