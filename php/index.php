<?php session_start(); /* Starts the session */
if(!isset($_SESSION['UserData']['Username'])){
	header("location:login.php");
	exit;
}
?>

<!DOCTYPE html>
<html>
<title>W3.CSS</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="https://www.w3schools.com/w3css/4/w3.css">
	<body>
	  <div class="w3-container">
		 <div class="w3-ul w3-card-4">
			<ul style="width:50%"><br>
				<?php
					$directory = './logs/';
					$scanned_directory = str_replace("sensor-","", array_diff(scandir($directory,1), array('..', '.', '.htaccess')));
					$scanned_directory = str_replace(".csv","", $scanned_directory);
					foreach ($scanned_directory as $file){
						$filename="sensor-$file.csv";
						echo "<li><a href=\"$directory$filename\">$file</a></br></li>";
					}
				?>
			</ul>
			<hr/>
		</div>
		</div>
	<script>
	</script>
	</body>
</html>
