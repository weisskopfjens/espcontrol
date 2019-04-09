<?php
  // http://serverip/addvalue.php?token=66efff4c945daabbcc271b47d456db&value=
  // set correct timezone !!!
  date_default_timezone_set('Europe/Berlin');
  // check token for client authentication
  /* you can change the token here */
  $tok1 = "66efff4c945daabbcc271b47d456db"; // md5(192.168.1.1)
  $token = htmlspecialchars($_GET["token"]);
  if($token!=$tok1) {
	exit(1);
  }
  $now = time();
  $filename = "logs/sensor-".date("Ym", $now).".csv";
  $value = date("Y.m.d H:i", $now).",".htmlspecialchars($_GET["value"])."\n";
  if(!file_exists($filename)) {
    touch($filename);
    file_put_contents($filename, "Time,Temperature,Humity,Pressure\n", FILE_APPEND);
  }
  file_put_contents($filename, $value, FILE_APPEND);
?>
