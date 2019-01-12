<?php
header("Cache-Control: no-cache, no-store, must-revalidate"); // HTTP 1.1.
header("Pragma: no-cache"); // HTTP 1.0.
header("Expires: 0"); // Proxies.

$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');



if(isset($_SESSION['screen_width']) AND isset($_SESSION['screen_height'])){
    echo '';
} else if(isset($_REQUEST['width']) AND isset($_REQUEST['height'])) {
    $_SESSION['screen_width'] = $_REQUEST['width'];
    $_SESSION['screen_height'] = $_REQUEST['height'];
    header('Location: ' . $_SERVER['PHP_SELF']);
} else {
    echo '<script type="text/javascript">window.location = "' . $_SERVER['PHP_SELF'] . '?width="+screen.width+"&height="+screen.height;</script>';
}


?>

<div class="container">
     <div class="form-container">

<form method="post" action="login_submit.php" class="pure-form pure-form-stacked">
 <fieldset>
 <legend><center><img class="pure-img" src="inc/favicon-114x114.png" alt="" width="80" height="80"></center></legend>

        <?php
            if (isset($error)) {
                echo "<div class='errormsg'>$error</div>";
            }
?>

            <label for="login">Username:</label>
            <input type="text" name="login" id="login" placeholder="Your login" maxlength="50" autofocus>
            <label for="pass">Password:</label>
            <input type="password" name="pass" id="pass" placeholder="Password" maxlength="50">
        <center>
            <button type="submit" class="pure-button pure-button-primary" name="login-submit" id="login-submit" value="Login" >Sign in</button>
        </center>

 </fieldset>
</form>


       </div>
</div>
</body>
</html>