<?php session_start(); /* Starts the session */

	/* Check Login form submitted */
	if(isset($_POST['Submit'])){
		/* 
		 * Chanche the login credentials here
		 * (username1,password1,username2,password2)
		 * Define username and associated password array
		 *
		 */
		$logins = array('username1' => 'password1','username2' => 'password2');

		/* Check and assign submitted Username and Password to new variable */
		$Username = isset($_POST['Username']) ? $_POST['Username'] : '';
		$Password = isset($_POST['Password']) ? $_POST['Password'] : '';

		/* Check Username and Password existence in defined array */
		if (isset($logins[$Username]) && $logins[$Username] == $Password){
			/* Success: Set session variables and redirect to Protected page  */
			$_SESSION['UserData']['Username']=$logins[$Username];
			header("location:index.php");
			exit;
		} else {
			/*Unsuccessful attempt: Set error message */
			$msg="<span style='color:red'>Invalid Login Details</span>";
		}
	}
?>

<!DOCTYPE html>
<html>
<title>Login</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="w3.css">
<body onload="document.getElementById('id01').style.display='block'">

<div class="w3-container" >
  <div id="id01" class="w3-modal">
    <div class="w3-modal-content w3-card-4 w3-animate-zoom" style="max-width:600px">

			<?php if(isset($msg)){?>
      <div class="w3-center"><br>
        <div class="w3-panel w3-pale-red w3-leftbar w3-border-red">
 				<p><?php echo $msg;?></p>
				</div>
      </div>
			<?php } ?>

      <form class="w3-container" action="/logger/login.php" method="post">
        <div class="w3-section">
          <label><b>Username</b></label>
          <input class="w3-input w3-border w3-margin-bottom" type="text" placeholder="Enter Username" name="Username" required>
          <label><b>Password</b></label>
          <input class="w3-input w3-border" type="password" placeholder="Enter Password" name="Password" required>
          <button class="w3-button w3-block w3-green w3-section w3-padding" name="Submit" type="submit">Login</button>
        </div>
      </form>

      <div class="w3-container w3-border-top w3-padding-16 w3-light-grey">
      </div>

    </div>
  </div>
</div>

</body>
</html>
