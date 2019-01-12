<?php
	// First start a session. This should be right at the top of your login page.
include('inc/header.inc');


include('inc/db_config.inc');


$login = preg_replace("/[^a-zA-Z0-9]+/", "", $_POST['login']);
$pass = preg_replace("/[^a-zA-Z0-9]+/", "", $_POST['pass']);
$login_submit = preg_replace("/[^0-9]+/", "", $_POST['login-submit']);



	// Check to see if this run of the script was caused by our login submit button being clicked.
	if (isset($login_submit)) {


		// Also check that our email address and password were passed along. If not, jump
		// down to our error message about providing both pieces of information.
		if (isset($login) && isset($pass) && $pass<>'') {
			//$login = $_POST['login'];
			//$pass = $_POST['pass'];


			// Connect to the database and select the user based on their provided email address.
			// Be sure to retrieve their password and any other information you want to save for the user session.
                        $stmt = $dbh->prepare("SELECT * FROM personel WHERE login=:login1");
                        $stmt -> bindParam(':login1', $login);
                        $stmt -> execute();


                        //$stmt->debugDumpParams();


                        $login_array = $stmt->fetchAll();


			// If the user record was found, compare the password on record to the one provided hashed as necessary.
			// If successful, now set up session variables for the user and store a flag to say they are authorized.
			// These values follow the user around the site and will be tested on each page.
			if (($login_array !== false) && ($stmt->rowCount() > 0)) {
				if ($login_array[0]['password'] == hash('sha256', $pass)) {

					// is_auth is important here because we will test this to make sure they can view other pages
					// that are needing credentials.
					$_SESSION['is_auth'] = true;
					$_SESSION['permission'] = $login_array[0]['permission'];
					$_SESSION['limit_site'] = $login_array[0]['limit_site'];
					$_SESSION['timezone'] = $login_array[0]['timezone'];
					$_SESSION['user_id'] = $login_array[0]['id_personel'];
					$_SESSION['name'] = $login_array[0]['name'];
					$_SESSION['login'] = $login_array[0]['login'];
					$_SESSION['location_id_limit'] = $login_array[0]['location_id_limit'];
					$_SESSION['limit_data_months'] = $login_array[0]['limit_data_months'];

                                        if(count(explode(',',$_SESSION['location_id_limit']))==1 and $_SESSION['location_id_limit']<>0)
                                           $_SESSION['personel_location'] = $_SESSION['location_id_limit'];



                                        $stmt = $dbh->prepare("INSERT INTO `logs` (`personel_id`,`ip`,`type`,`txt`)
                                                                           VALUES (
                                                                                  '".$_SESSION['user_id']."',
                                                                                  '".$_SERVER['REMOTE_ADDR']."',
                                                                                  'login',
                                                                                  '".$_SESSION['name']."|".$_SESSION['screen_width'] . "x" . $_SESSION['screen_height']."|".$_SERVER['HTTP_USER_AGENT']."'
                                                                                  )");
                                        $stmt -> execute();

					// Once the sessions variables have been set, redirect them to the landing page / home page.
                                        if($_SESSION['limit_site']<>"")
                                            header('location: '.$_SESSION['limit_site']);
                                        else
                                            header('location: home.php');
					exit;
				}
				else {
					$error = "Invalid login or password. Please try again.";
				}
			}
			else {
				$error = "Invalid login or password. Please try again.";
			}
		}
		else {
			$error = "Please enter an login and password to login.";
		}
	}
?>

<div class="container">
     <div class="form-container">
     <center>
<?php
echo $error.'<br><br><br><a href="index.php" class="pure-button"><i class="fa fa-arrow-left fa-lg"></i> Back</a>';

?>
</center>
</div>
</div>